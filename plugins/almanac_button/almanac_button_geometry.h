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
// The only case drawn is the one that was observed in the read-only snapshot
// and that the export supports without a skip: count == 8, normal row
// (game+0xC7D4 == -1), trade handler selected, game+0x578 == 0 and
// game+0x5E0 != 0. In that case the loop draws all eight tiles, so the eighth
// call is the last tile of the row, and the ninth slot is origin + 8 * pitch.
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
    int32_t count;    // (end - begin) / 0x50 of the trade subvector
    int32_t selector; // game+0x10398, trade handler selector (non-zero = row handler)
    int32_t c7d4;     // game+0xC7D4, must be -1 for the normal row
    int32_t skip578;  // game+0x578, must be 0 (mapped indices 3 and 4 not skipped)
    int32_t skip5e0;  // game+0x5E0, must be non-zero (mapped index 6 not skipped)
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

inline bool FiniteF(float v) { return std::isfinite(v); }

inline bool ConstantsValid(const RowConstants& c)
{
    return FiniteF(c.scale) && c.scale > 0.25f && c.scale < 4.0f &&
           FiniteF(c.originA) && FiniteF(c.originB) && FiniteF(c.pitch) &&
           c.pitch > 0.0f;
}

// Tracks one row pass at a time. Call OnRowTile for every row-site Draw, in
// call order, with the tile position the handler stored just before the call.
class RowTracker {
public:
    void Reset()
    {
        next_ = -1;
        passCount_ = 0;
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

        const float origin = c.scale * (c.originA + c.originB);
        const float pitch  = c.scale * c.pitch;

        if (std::fabs(tileX - origin) <= kPositionTolerance) {
            // First tile of a pass.
            next_ = 0;
            passCount_ = g.count;
        } else if (next_ < 0) {
            return Decision{Action::None, 0.0f, 0.0f, "not at row origin; idle"};
        }

        const float expected = origin + static_cast<float>(next_) * pitch;
        if (std::fabs(tileX - expected) > kPositionTolerance) {
            next_ = -1;
            return Decision{Action::None, 0.0f, 0.0f, "tile off sequence; lost"};
        }

        const int index = next_;
        next_ += 1;

        if (g.count != passCount_) {
            next_ = -1;
            return Decision{Action::None, 0.0f, 0.0f, "count changed mid-pass"};
        }
        if (index + 1 < passCount_) {
            return none;   // not the last tile yet
        }

        // Last tile of the pass. The pass is complete either way.
        next_ = -1;

        if (passCount_ != 8)           return Decision{Action::None, 0.0f, 0.0f, "count is not 8"};
        if (g.selector == 0)           return Decision{Action::None, 0.0f, 0.0f, "trade handler not selected"};
        if (g.c7d4 != -1)              return Decision{Action::None, 0.0f, 0.0f, "not the normal row"};
        if (g.skip578 != 0)            return Decision{Action::None, 0.0f, 0.0f, "skip 578 set: visible count unknown"};
        if (g.skip5e0 == 0)            return Decision{Action::None, 0.0f, 0.0f, "skip 5E0 clear: visible count unknown"};

        return Decision{Action::DrawNinth,
                        origin + static_cast<float>(passCount_) * pitch,
                        tileY,
                        "ok"};
    }

private:
    int next_ = -1;     // index of the next expected tile in this pass; -1 = idle or lost
    int passCount_ = 0; // count captured at the first tile of the pass
};

// Width of the ninth backdrop, the same as the native tile width (scale * P).
inline float TileWidth(const RowConstants& c) { return c.scale * c.pitch; }

}  // namespace almanac_button

#endif  // ALMANAC_BUTTON_GEOMETRY_H
