// almanac_button_geometry.h - pure row logic for the almanac_button prototype.
//
// No Windows headers. Everything the plugin decides about the Economy & Trade
// row is in this file so it can be tested on Linux (tests/almanac_button/).
// The plugin feeds it the tile position the native handler stored, and the
// game-object fields it read, and acts only on the Decision it returns.
//
// Row model (from the native normal-row handler, FUN_140373EA0; see
// export/140373ea0_1403829fd_disasm.asm lines 0x140374114..0x1403743ed):
//   origin = scale * (A + B)     first tile X   (A = 45, B = 85)
//   pitch  = scale * P           X advance and tile width (P = 64)
//   tile k (0-based, visual order) is drawn at X = origin + k * pitch.
// A tile whose visible slot is skipped advances nothing, so with any skip flag
// set the visible count is not known here and the prototype draws nothing.
//
// What a pass means here, and what it does NOT mean:
//   - A "pass" is a candidate run of row-site calls: a tile at the origin,
//     then tiles at origin + k * pitch, all with the same row Y, count and
//     gate values. The plugin also filters on the call's return address and
//     `this`.
//   - This logic does NOT prove that a pass is one native invocation, or that
//     it belongs to one frame. An X sequence can recur, and a pass can be
//     interrupted and resumed in ways the tracker only sees as a new origin.
//     It only prevents drawing when the observed calls are inconsistent.
//
// The only case drawn is the one observed in the read-only snapshot and
// supported by the export without a skip: count == 8, normal row
// (game+0xC7D4 == -1), trade handler selected (byte game+0x10398 != 0),
// game+0x578 == 0 and game+0x5E0 != 0. The ninth slot is origin + 8 * pitch.
//
// Status: the ninth position is a HYPOTHESIS in the handler's scaled units.
// Its mapping to screen pixels, its clipping, and its visibility are not
// established by this file.

#ifndef ALMANAC_BUTTON_GEOMETRY_H
#define ALMANAC_BUTTON_GEOMETRY_H

#include <cmath>
#include <cstdint>

namespace almanac_button {

struct RowConstants {
    float scale;   // DPI scalar, read at runtime (0x992088)
    float originA; // expected 45 (0x90AA0C)
    float originB; // expected 85 (0x90AB14)
    float pitch;   // expected 64 (0x90AAA0)
};

struct RowGate {
    int32_t count;    // (end - begin) / 0x50 of the trade subvector (handler: span * magic >> 69)
    uint8_t selector; // game+0x10398, ONE BYTE. The game tests it as a byte:
                      //   CMP byte ptr [R14+0x10398],R15B  at 0x14031EDE6
                      //   (JZ at 0x14031EDED skips the row call when equal).
                      // Non-zero means the row handler is dispatched.
    int32_t c7d4;     // game+0xC7D4, dword, must be -1 for the normal row
    int32_t skip578;  // game+0x578, dword, must be 0 (mapped indices 3 and 4 not skipped)
    int32_t skip5e0;  // game+0x5E0, dword, must be non-zero (mapped index 6 not skipped)
};

enum class Action { None, DrawNinth };

struct Decision {
    Action      action;
    float       x;      // ninth slot X in handler units (valid when DrawNinth)
    float       y;      // row Y in handler units (valid when DrawNinth)
    const char* reason; // static string: why, for logging
};

// Tolerance for comparing accumulated float positions, in handler units.
constexpr float kPositionTolerance = 0.05f;

// Largest subvector count accepted. The normal row has 8.
constexpr int kMaxCount = 64;

inline bool FiniteF(float v) { return std::isfinite(v); }

inline bool ConstantsValid(const RowConstants& c)
{
    return FiniteF(c.scale) && c.scale > 0.25f && c.scale < 4.0f &&
           FiniteF(c.originA) && FiniteF(c.originB) && FiniteF(c.pitch) &&
           c.pitch > 0.0f;
}

// True when every gate field that the pass depends on is the same.
inline bool SameGate(const RowGate& a, const RowGate& b)
{
    return a.count == b.count && a.selector == b.selector && a.c7d4 == b.c7d4 &&
           a.skip578 == b.skip578 && a.skip5e0 == b.skip5e0;
}

// True when every geometry constant is the same. The values are read from
// game memory, not computed, so exact equality is the intended test: any
// change, however small, means the pass was built on different geometry.
inline bool SameConstants(const RowConstants& a, const RowConstants& b)
{
    return a.scale == b.scale && a.originA == b.originA &&
           a.originB == b.originB && a.pitch == b.pitch;
}

// Tracks one candidate row pass at a time. Call OnRowTile for every row-site
// Draw, in call order, with the tile position the handler stored just before
// the call. Call Reset() whenever the plugin cannot read the row state, so no
// partial pass survives an unreadable frame.
class RowTracker {
public:
    void Reset()
    {
        next_ = -1;
        passCount_ = 0;
        passY_ = 0.0f;
        passGate_ = RowGate{};
        passC_ = RowConstants{};
    }

    // -1 means idle or lost (waiting for a tile at the row origin).
    int Next() const { return next_; }

    Decision OnRowTile(float tileX, float tileY,
                       const RowConstants& c, const RowGate& g)
    {
        const Decision none{Action::None, 0.0f, 0.0f, "none"};

        if (!ConstantsValid(c)) {
            Reset();
            return Decision{Action::None, 0.0f, 0.0f, "constants invalid"};
        }
        if (!FiniteF(tileX) || !FiniteF(tileY)) {
            Reset();
            return Decision{Action::None, 0.0f, 0.0f, "tile position not finite"};
        }
        if (g.count < 1 || g.count > kMaxCount) {
            Reset();
            return Decision{Action::None, 0.0f, 0.0f, "count out of range"};
        }

        const float origin = c.scale * (c.originA + c.originB);

        if (std::fabs(tileX - origin) <= kPositionTolerance) {
            // A tile at the origin starts a new candidate pass and drops any
            // pass in progress. Constants, row Y and the gate are captured here.
            next_ = 0;
            passCount_ = g.count;
            passY_ = tileY;
            passGate_ = g;
            passC_ = c;
        } else if (next_ < 0) {
            return Decision{Action::None, 0.0f, 0.0f, "not at row origin; idle"};
        }

        // Every tile of a pass must agree with the pass's first tile: the same
        // geometry constants, row Y and gate values. This is checked on every
        // tile, including the last one that decides the draw.
        if (!SameConstants(c, passC_)) {
            Reset();
            return Decision{Action::None, 0.0f, 0.0f, "scale or geometry changed mid-pass; lost"};
        }
        if (std::fabs(tileY - passY_) > kPositionTolerance) {
            Reset();
            return Decision{Action::None, 0.0f, 0.0f, "row Y changed mid-pass; lost"};
        }
        if (!SameGate(g, passGate_)) {
            Reset();
            return Decision{Action::None, 0.0f, 0.0f, "gate changed mid-pass; lost"};
        }

        // Positions use the constants captured at the pass's first tile.
        const float passOrigin = passC_.scale * (passC_.originA + passC_.originB);
        const float pitch      = passC_.scale * passC_.pitch;
        const float expected   = passOrigin + static_cast<float>(next_) * pitch;
        if (std::fabs(tileX - expected) > kPositionTolerance) {
            Reset();
            return Decision{Action::None, 0.0f, 0.0f, "tile off sequence; lost"};
        }

        const int index = next_;
        next_ += 1;
        if (index + 1 < passCount_) {
            return none;   // not the last tile yet
        }

        // Last tile of the pass. The pass is complete either way.
        const int count = passCount_;
        const RowGate gate = passGate_;
        const float rowY = passY_;
        Reset();

        if (count != 8)                return Decision{Action::None, 0.0f, 0.0f, "count is not 8"};
        if (gate.selector == 0)        return Decision{Action::None, 0.0f, 0.0f, "trade handler not selected"};
        if (gate.c7d4 != -1)           return Decision{Action::None, 0.0f, 0.0f, "not the normal row"};
        if (gate.skip578 != 0)         return Decision{Action::None, 0.0f, 0.0f, "skip 578 set: visible count unknown"};
        if (gate.skip5e0 == 0)         return Decision{Action::None, 0.0f, 0.0f, "skip 5E0 clear: visible count unknown"};

        return Decision{Action::DrawNinth,
                        passOrigin + static_cast<float>(count) * pitch,
                        rowY,
                        "ok"};
    }

private:
    int next_ = -1;      // index of the next expected tile in this pass; -1 = idle or lost
    int passCount_ = 0;  // count captured at the pass's first tile
    float passY_ = 0.0f; // row Y captured at the pass's first tile
    RowGate passGate_{}; // gate captured at the pass's first tile
    RowConstants passC_{}; // geometry constants captured at the pass's first tile
};

// Width of the ninth backdrop, the same as the native tile width (scale * P).
inline float TileWidth(const RowConstants& c) { return c.scale * c.pitch; }

}  // namespace almanac_button

#endif  // ALMANAC_BUTTON_GEOMETRY_H
