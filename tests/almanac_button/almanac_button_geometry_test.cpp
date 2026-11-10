// Host test for plugins/almanac_button/almanac_button_geometry.h.
// Build and run on Linux:
//   g++ -std=c++17 -Wall -Wextra -Werror -pedantic -Iplugins/almanac_button tests/almanac_button/almanac_button_geometry_test.cpp -o build/almanac_button_geometry_test
//   build/almanac_button_geometry_test
// Pure logic only. The plugin's Windows glue is not exercised here.
// Passing these tests does not prove that a pass is one native invocation or
// one frame; see the header comment.

#include "almanac_button_geometry.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <initializer_list>

using namespace almanac_button;

#define CHECK(x) do { if (!(x)) { std::fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #x); std::exit(1); } } while (0)

static const RowConstants kC{0.8f, 45.0f, 85.0f, 64.0f};
static const float kRowY = 196.0f;

static RowGate Good(int count = 8)
{
    RowGate g{};
    g.count    = count;
    g.selector = 1;
    g.c7d4     = -1;
    g.skip578  = 0;
    g.skip5e0  = 1;
    return g;
}

// Feeds one tile, at its nominal X for slot k, with the given Y and gate.
static Decision Tile(RowTracker& t, int k, float y, const RowGate& g,
                     const RowConstants& c = kC)
{
    const float origin = c.scale * (c.originA + c.originB);
    const float pitch  = c.scale * c.pitch;
    return t.OnRowTile(origin + static_cast<float>(k) * pitch, y, c, g);
}

// Feeds one pass of `n` tiles at the origin, pitch step, in visual order.
// Returns the decision of the last tile.
static Decision RunPass(RowTracker& t, int n, const RowGate& g, const RowConstants& c,
                        int* drawnAt = nullptr)
{
    Decision last{Action::None, 0.0f, 0.0f, "none"};
    for (int k = 0; k < n; ++k) {
        Decision d = Tile(t, k, kRowY, g, c);
        if (d.action == Action::DrawNinth && drawnAt) *drawnAt = k;
        last = d;
    }
    return last;
}

static void TestEightTilePassDrawsNinthOnLastTile()
{
    RowTracker t;
    int drawnAt = -1;
    Decision last = RunPass(t, 8, Good(), kC, &drawnAt);
    CHECK(drawnAt == 7);                          // only the eighth call decides
    CHECK(last.action == Action::DrawNinth);
    CHECK(std::fabs(last.x - 513.6f) < 0.01f);    // scale * (45 + 85 + 8 * 64)
    CHECK(std::fabs(last.y - kRowY) < 0.01f);
    CHECK(t.Next() == -1);
    CHECK(std::fabs(TileWidth(kC) - 51.2f) < 1e-4f);
}

static void TestNinthPositionFollowsScale()
{
    for (float s : {0.5f, 0.8f, 1.0f, 1.25f, 2.0f}) {
        RowConstants c{s, 45.0f, 85.0f, 64.0f};
        RowTracker t;
        Decision d = RunPass(t, 8, Good(), c);
        CHECK(d.action == Action::DrawNinth);
        CHECK(std::fabs(d.x - s * 642.0f) < 0.01f * s + 0.01f);   // s * (130 + 512)
    }
}

static void TestGateFailuresDrawNothing()
{
    struct Case { const char* name; RowGate g; };
    RowGate g;

    g = Good(); g.skip578 = 1;  Case c1{"skip578", g};
    g = Good(); g.skip5e0 = 0;  Case c2{"skip5e0", g};
    g = Good(); g.c7d4 = 0;     Case c3{"c7d4", g};
    g = Good(); g.selector = 0; Case c4{"selector", g};
    g = Good(7);                Case c5{"count7", g};
    g = Good(9);                Case c6{"count9", g};

    for (Case* c : {&c1, &c2, &c3, &c4, &c5, &c6}) {
        RowTracker t;
        int drawnAt = -1;
        Decision last = RunPass(t, c->g.count, c->g, kC, &drawnAt);
        CHECK(drawnAt == -1);
        CHECK(last.action == Action::None);
        CHECK(t.Next() == -1);
    }
}

static void TestStrayStartIsIdleUntilOrigin()
{
    RowTracker t;
    const float origin = kC.scale * (kC.originA + kC.originB);
    const float pitch  = kC.scale * kC.pitch;
    // Enter mid-row: first call is the second slot, not the origin.
    Decision d = t.OnRowTile(origin + pitch, kRowY, kC, Good());
    CHECK(d.action == Action::None);
    CHECK(t.Next() == -1);
    // A full pass afterwards still draws exactly once.
    int drawnAt = -1;
    Decision last = RunPass(t, 8, Good(), kC, &drawnAt);
    CHECK(drawnAt == 7);
    CHECK(last.action == Action::DrawNinth);
}

static void TestInterruptedPassThenFullPass()
{
    RowTracker t;
    // Five tiles, then the pass stops (handler exited early).
    int drawnAt = -1;
    RunPass(t, 5, Good(), kC, &drawnAt);
    CHECK(drawnAt == -1);
    // A new pass at the origin starts clean and draws once, at its own end.
    drawnAt = -1;
    Decision last = RunPass(t, 8, Good(), kC, &drawnAt);
    CHECK(drawnAt == 7);
    CHECK(last.action == Action::DrawNinth);
}

static void TestOffSequenceLosesThePass()
{
    RowTracker t;
    const float origin = kC.scale * (kC.originA + kC.originB);
    const float pitch  = kC.scale * kC.pitch;
    Decision d0 = t.OnRowTile(origin, kRowY, kC, Good());
    CHECK(d0.action == Action::None);
    // Second call lands on a slot that is not origin + pitch: the pass is lost.
    Decision d1 = t.OnRowTile(origin + 3.0f * pitch, kRowY, kC, Good());
    CHECK(d1.action == Action::None);
    CHECK(t.Next() == -1);
    // The rest of that pass draws nothing.
    int drawnAt = -1;
    for (int k = 2; k < 8; ++k) {
        Decision d = Tile(t, k, kRowY, Good());
        if (d.action == Action::DrawNinth) drawnAt = k;
    }
    CHECK(drawnAt == -1);
}

static void TestCountChangeMidPassDrawsNothing()
{
    RowTracker t;
    for (int k = 0; k < 7; ++k) {
        int count = (k < 3) ? 8 : 7;   // count changes after three tiles
        Decision d = Tile(t, k, kRowY, Good(count));
        CHECK(d.action == Action::None);
    }
    CHECK(t.Next() == -1);
}

static void TestInvalidInputsDrawNothing()
{
    RowTracker t;
    const float origin = kC.scale * (kC.originA + kC.originB);
    RowConstants nan{0.8f, std::nanf(""), 85.0f, 64.0f};
    CHECK(t.OnRowTile(origin, kRowY, nan, Good()).action == Action::None);
    RowConstants zeroPitch{0.8f, 45.0f, 85.0f, 0.0f};
    CHECK(t.OnRowTile(origin, kRowY, zeroPitch, Good()).action == Action::None);
    RowConstants bigScale{5.0f, 45.0f, 85.0f, 64.0f};
    CHECK(t.OnRowTile(origin, kRowY, bigScale, Good()).action == Action::None);
    CHECK(t.OnRowTile(std::nanf(""), kRowY, kC, Good()).action == Action::None);
    CHECK(t.OnRowTile(origin, std::nanf(""), kC, Good()).action == Action::None);
    CHECK(t.Next() == -1);
}

static void TestTwoPassesDrawTwice()
{
    RowTracker t;
    int total = 0;
    for (int p = 0; p < 2; ++p) {
        int drawnAt = -1;
        RunPass(t, 8, Good(), kC, &drawnAt);
        if (drawnAt == 7) ++total;
    }
    CHECK(total == 2);
}

// Row Y changes on an interior tile: the pass is dropped and nothing is drawn.
static void TestRowYChangeMidPassDrawsNothing()
{
    RowTracker t;
    for (int k = 0; k < 8; ++k) {
        float y = (k >= 4) ? kRowY + 1.0f : kRowY;   // Y moves at tile 4
        CHECK(Tile(t, k, y, Good()).action == Action::None);
    }
    CHECK(t.Next() == -1);
}

// Row Y changes only on the decision tile: still nothing is drawn.
static void TestRowYChangeOnLastTileOnlyDrawsNothing()
{
    RowTracker t;
    for (int k = 0; k < 8; ++k) {
        float y = (k == 7) ? kRowY + 0.5f : kRowY;
        CHECK(Tile(t, k, y, Good()).action == Action::None);
    }
    CHECK(t.Next() == -1);
}

// A skip flag set mid-pass drops the pass; flipping it back does not revive it.
static void TestSkip578ChangeMidPassDrawsNothing()
{
    RowTracker t;
    for (int k = 0; k < 8; ++k) {
        RowGate g = Good();
        if (k >= 3) g.skip578 = 1;                    // stays set
        CHECK(Tile(t, k, kRowY, g).action == Action::None);
    }
    CHECK(t.Next() == -1);

    RowTracker u;
    for (int k = 0; k < 8; ++k) {
        RowGate g = Good();
        g.skip578 = (k == 3) ? 1 : 0;                 // set once, then cleared
        CHECK(Tile(u, k, kRowY, g).action == Action::None);
    }
    CHECK(u.Next() == -1);
}

// skip5e0 cleared mid-pass, and cleared only on the decision tile.
static void TestSkip5e0ChangeMidPassDrawsNothing()
{
    RowTracker t;
    for (int k = 0; k < 8; ++k) {
        RowGate g = Good();
        g.skip5e0 = (k == 6) ? 0 : 1;                 // cleared on tile 6, back on 7
        CHECK(Tile(t, k, kRowY, g).action == Action::None);
    }

    RowTracker u;
    for (int k = 0; k < 8; ++k) {
        RowGate g = Good();
        g.skip5e0 = (k == 7) ? 0 : 1;                 // cleared only on the decision tile
        CHECK(Tile(u, k, kRowY, g).action == Action::None);
    }
    CHECK(u.Next() == -1);
}

// Selector byte changed mid-pass, then restored: the pass stays dropped.
static void TestSelectorChangeMidPassDrawsNothing()
{
    RowTracker t;
    for (int k = 0; k < 8; ++k) {
        RowGate g = Good();
        g.selector = (k == 2) ? 0 : 1;
        CHECK(Tile(t, k, kRowY, g).action == Action::None);
    }
    CHECK(t.Next() == -1);
}

static void TestC7d4ChangeMidPassDrawsNothing()
{
    RowTracker t;
    for (int k = 0; k < 8; ++k) {
        RowGate g = Good();
        g.c7d4 = (k == 5) ? 0 : -1;
        CHECK(Tile(t, k, kRowY, g).action == Action::None);
    }
    CHECK(t.Next() == -1);
}

// The gate is captured at the pass's first tile; a later tile that differs
// from that snapshot drops the pass, even if the decision-tile gate is good.
static void TestGateSnapshotComesFromFirstTile()
{
    RowTracker t;
    for (int k = 0; k < 8; ++k) {
        RowGate g = Good();
        g.skip578 = (k == 0) ? 1 : 0;
        CHECK(Tile(t, k, kRowY, g).action == Action::None);
    }
    CHECK(t.Next() == -1);
}

// Reset() in the middle of a pass drops it. The remaining tiles draw nothing.
static void TestResetMidPassDropsPass()
{
    RowTracker t;
    for (int k = 0; k < 4; ++k) {
        CHECK(Tile(t, k, kRowY, Good()).action == Action::None);
    }
    t.Reset();
    CHECK(t.Next() == -1);
    for (int k = 4; k < 8; ++k) {
        CHECK(Tile(t, k, kRowY, Good()).action == Action::None);
    }
    CHECK(t.Next() == -1);
}

// The selector is one byte, as the game reads it. Any non-zero byte dispatches.
static void TestSelectorIsOneByte()
{
    static_assert(sizeof(RowGate::selector) == 1, "selector must be one byte");
    for (int v : {1, 2, 127, 255}) {
        RowGate g = Good();
        g.selector = static_cast<uint8_t>(v);
        RowTracker t;
        int drawnAt = -1;
        Decision last = RunPass(t, 8, g, kC, &drawnAt);
        CHECK(drawnAt == 7);
        CHECK(last.action == Action::DrawNinth);
    }
}

// Count outside 1..kMaxCount is rejected, both at the start and mid-pass.
static void TestCountRangeEnforced()
{
    RowTracker t;
    CHECK(Tile(t, 0, kRowY, Good(0)).action == Action::None);
    CHECK(Tile(t, 0, kRowY, Good(kMaxCount + 1)).action == Action::None);
    CHECK(t.Next() == -1);

    RowTracker u;
    CHECK(Tile(u, 0, kRowY, Good(8)).action == Action::None);
    CHECK(Tile(u, 1, kRowY, Good(kMaxCount + 1)).action == Action::None);
    CHECK(u.Next() == -1);
}

// A failed pass does not poison the next clean pass.
static void TestCleanPassAfterFailedPassDraws()
{
    RowTracker t;
    RowGate bad = Good();
    bad.skip578 = 1;
    for (int k = 0; k < 8; ++k) {
        CHECK(Tile(t, k, kRowY, (k == 3) ? bad : Good()).action == Action::None);
    }
    int drawnAt = -1;
    Decision last = RunPass(t, 8, Good(), kC, &drawnAt);
    CHECK(drawnAt == 7);
    CHECK(last.action == Action::DrawNinth);
}

// An unreadable (non-finite) tile in the middle of a pass drops the pass. The
// tile that would have continued it, at the expected slot, must not draw.
static void TestNonFiniteTileMidPassDropsPass()
{
    RowTracker t;
    for (int k = 0; k < 4; ++k) {
        CHECK(Tile(t, k, kRowY, Good()).action == Action::None);
    }
    const float origin = kC.scale * (kC.originA + kC.originB);
    const float pitch  = kC.scale * kC.pitch;
    CHECK(t.OnRowTile(origin + 4.0f * pitch, std::nanf(""), kC, Good()).action == Action::None);
    CHECK(t.Next() == -1);
    for (int k = 4; k < 8; ++k) {
        CHECK(Tile(t, k, kRowY, Good()).action == Action::None);
    }
    CHECK(t.Next() == -1);
}

// Mixed state: each tile X still follows the pass's geometry (kC), but the
// constants read for the call have changed. Only the constants check can
// reject these. Position alone would accept them.
static Decision TileMixed(RowTracker& t, int k, float y, const RowGate& g,
                          const RowConstants& xGeometry, const RowConstants& readConstants)
{
    const float origin = xGeometry.scale * (xGeometry.originA + xGeometry.originB);
    const float pitch  = xGeometry.scale * xGeometry.pitch;
    return t.OnRowTile(origin + static_cast<float>(k) * pitch, y, readConstants, g);
}

// The scale read changes on the last tile, positions still follow kC.
static void TestScaleReadChangesOnLastTileDrawsNothing()
{
    const RowConstants c2{0.9f, 45.0f, 85.0f, 64.0f};
    RowTracker t;
    for (int k = 0; k < 7; ++k) {
        CHECK(Tile(t, k, kRowY, Good(), kC).action == Action::None);
    }
    CHECK(TileMixed(t, 7, kRowY, Good(), kC, c2).action == Action::None);
    CHECK(t.Next() == -1);
    // A clean pass on the original geometry afterwards still draws once.
    int drawnAt = -1;
    Decision last = RunPass(t, 8, Good(), kC, &drawnAt);
    CHECK(drawnAt == 7);
    CHECK(last.action == Action::DrawNinth);
}

// The scale read changes mid-pass, positions still follow kC.
static void TestScaleReadChangesMidPassDrawsNothing()
{
    const RowConstants c2{0.9f, 45.0f, 85.0f, 64.0f};
    RowTracker t;
    for (int k = 0; k < 8; ++k) {
        const RowConstants& read = (k >= 3) ? c2 : kC;
        CHECK(TileMixed(t, k, kRowY, Good(), kC, read).action == Action::None);
    }
    CHECK(t.Next() == -1);
}

// originB and pitch reads change, positions still follow kC. These are the
// cases where a scale-only comparison would miss the change.
static void TestGeometryReadChangesDrawsNothing()
{
    const RowConstants cB{0.8f, 45.0f, 86.0f, 64.0f};   // originB read changes
    RowTracker t;
    for (int k = 0; k < 8; ++k) {
        const RowConstants& read = (k >= 5) ? cB : kC;
        CHECK(TileMixed(t, k, kRowY, Good(), kC, read).action == Action::None);
    }
    CHECK(t.Next() == -1);

    const RowConstants cP{0.8f, 45.0f, 85.0f, 65.0f};   // pitch read changes on the last tile
    RowTracker u;
    for (int k = 0; k < 7; ++k) {
        CHECK(Tile(u, k, kRowY, Good(), kC).action == Action::None);
    }
    CHECK(TileMixed(u, 7, kRowY, Good(), kC, cP).action == Action::None);
    CHECK(u.Next() == -1);
}

int main()
{
    TestEightTilePassDrawsNinthOnLastTile();
    TestNinthPositionFollowsScale();
    TestGateFailuresDrawNothing();
    TestStrayStartIsIdleUntilOrigin();
    TestInterruptedPassThenFullPass();
    TestOffSequenceLosesThePass();
    TestCountChangeMidPassDrawsNothing();
    TestInvalidInputsDrawNothing();
    TestTwoPassesDrawTwice();
    TestRowYChangeMidPassDrawsNothing();
    TestRowYChangeOnLastTileOnlyDrawsNothing();
    TestSkip578ChangeMidPassDrawsNothing();
    TestSkip5e0ChangeMidPassDrawsNothing();
    TestSelectorChangeMidPassDrawsNothing();
    TestC7d4ChangeMidPassDrawsNothing();
    TestGateSnapshotComesFromFirstTile();
    TestResetMidPassDropsPass();
    TestSelectorIsOneByte();
    TestCountRangeEnforced();
    TestCleanPassAfterFailedPassDraws();
    TestNonFiniteTileMidPassDropsPass();
    TestScaleReadChangesOnLastTileDrawsNothing();
    TestScaleReadChangesMidPassDrawsNothing();
    TestGeometryReadChangesDrawsNothing();
    std::printf("almanac_button geometry: all tests passed\n");
    return 0;
}
