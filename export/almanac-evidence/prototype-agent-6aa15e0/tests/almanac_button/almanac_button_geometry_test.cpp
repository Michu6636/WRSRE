// Host test for plugins/almanac_button/almanac_button_geometry.h.
// Build and run on Linux:
//   g++ -std=c++17 -Wall -Wextra -Werror -pedantic -Iplugins/almanac_button tests/almanac_button/almanac_button_geometry_test.cpp -o build/almanac_button_geometry_test
//   build/almanac_button_geometry_test
// Pure logic only. The plugin's Windows glue is not exercised here.

#include "almanac_button_geometry.h"

#include <cmath>
#include <cstdio>
#include <initializer_list>
#include <cstdlib>
#include <cstring>

using namespace almanac_button;

#define CHECK(x) do { if (!(x)) { std::fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #x); std::exit(1); } } while (0)

static const RowConstants kC{0.8f, 45.0f, 85.0f, 64.0f};

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

// Feeds one pass of `n` tiles at the origin, pitch step, in visual order.
// Returns the decision of the last tile.
static Decision RunPass(RowTracker& t, int n, const RowGate& g, const RowConstants& c, int* drawnAt = nullptr)
{
    const float origin = c.scale * (c.originA + c.originB);
    const float pitch  = c.scale * c.pitch;
    Decision last{Action::None, 0.0f, 0.0f, "none"};
    for (int k = 0; k < n; ++k) {
        Decision d = t.OnRowTile(origin + static_cast<float>(k) * pitch, 196.0f, c, g);
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
    CHECK(std::fabs(last.y - 196.0f) < 0.01f);
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
    Decision d = t.OnRowTile(origin + pitch, 196.0f, kC, Good());
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
    Decision d0 = t.OnRowTile(origin, 196.0f, kC, Good());
    CHECK(d0.action == Action::None);
    // Second call lands on a slot that is not origin + pitch: the pass is lost.
    Decision d1 = t.OnRowTile(origin + 3.0f * pitch, 196.0f, kC, Good());
    CHECK(d1.action == Action::None);
    CHECK(t.Next() == -1);
    // The rest of that pass draws nothing.
    int drawnAt = -1;
    for (int k = 2; k < 8; ++k) {
        Decision d = t.OnRowTile(origin + static_cast<float>(k) * pitch, 196.0f, kC, Good());
        if (d.action == Action::DrawNinth) drawnAt = k;
    }
    CHECK(drawnAt == -1);
}

static void TestCountChangeMidPassDrawsNothing()
{
    RowTracker t;
    const float origin = kC.scale * (kC.originA + kC.originB);
    const float pitch  = kC.scale * kC.pitch;
    for (int k = 0; k < 7; ++k) {
        int count = (k < 3) ? 8 : 7;   // count changes after three tiles
        Decision d = t.OnRowTile(origin + static_cast<float>(k) * pitch, 196.0f, kC, Good(count));
        CHECK(d.action == Action::None);
    }
    CHECK(t.Next() == -1);
}

static void TestInvalidInputsDrawNothing()
{
    RowTracker t;
    const float origin = kC.scale * (kC.originA + kC.originB);
    RowConstants nan{0.8f, std::nanf(""), 85.0f, 64.0f};
    CHECK(t.OnRowTile(origin, 196.0f, nan, Good()).action == Action::None);
    RowConstants zeroPitch{0.8f, 45.0f, 85.0f, 0.0f};
    CHECK(t.OnRowTile(origin, 196.0f, zeroPitch, Good()).action == Action::None);
    RowConstants bigScale{5.0f, 45.0f, 85.0f, 64.0f};
    CHECK(t.OnRowTile(origin, 196.0f, bigScale, Good()).action == Action::None);
    CHECK(t.OnRowTile(std::nanf(""), 196.0f, kC, Good()).action == Action::None);
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
    std::printf("almanac_button geometry: all tests passed\n");
    return 0;
}
