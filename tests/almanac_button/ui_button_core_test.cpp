// g++ -std=c++17 -Wall -Wextra -Werror -pedantic -Isrc
//   tests/almanac_button/ui_button_core_test.cpp -o build/ui_button_core_test
// Pure shared policy only: no claim about native Collision or modal capture.
#include "ui_button_core.h"
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <initializer_list>

#define CHECK(x) do { if (!(x)) { std::fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #x); std::exit(1); } } while (0)
using tsm_ui::ButtonRect;
using tsm_ui::ButtonPress;

static void Geometry()
{
    for (float dpi : {0.75f, 1.0f, 1.25f, 2.0f}) {
        const ButtonRect r = ButtonRect{130.25f * dpi, 45.5f * dpi,
                                        64.0f * dpi, 64.0f * dpi}.PixelAligned();
        CHECK(r.Valid());
        CHECK(r.CenterX() - r.width / 2 == r.x);
        CHECK(r.CenterX() + r.width / 2 == r.x + r.width);
        CHECK(r.CenterY() - r.height / 2 == r.y);
        CHECK(r.CenterY() + r.height / 2 == r.y + r.height);
        CHECK(r.x == std::trunc(r.x));
        CHECK(r.y == std::trunc(r.y));
        CHECK(r.width == std::trunc(r.width));
        CHECK(r.height == std::trunc(r.height));
    }
    CHECK(!(ButtonRect{0, 0, 0, 64}).Valid());
    CHECK(!(ButtonRect{0, 0, 64, -1}).Valid());
    const float nan = std::numeric_limits<float>::quiet_NaN();
    const float inf = std::numeric_limits<float>::infinity();
    CHECK(!(ButtonRect{nan, 0, 64, 64}).Valid());
    CHECK(!(ButtonRect{0, inf, 64, 64}).Valid());
    CHECK(!(ButtonRect{0, 0, inf, 64}).Valid());
    CHECK(!(ButtonRect{0, 0, 64, nan}).Valid());
    CHECK(!(ButtonRect{0, 0, 0.25f, 0.25f}).PixelAligned().Valid());
    const ButtonRect negative = ButtonRect{-3.7f, -2.2f, 64, 64}.PixelAligned();
    CHECK(negative.x == -3 && negative.y == -2);
    CHECK(negative.x + negative.width == 60);
}

static void Presses()
{
    ButtonPress p;
    CHECK(!p.Update(false, false, true, true));
    CHECK(p.Update(true, false, true, true)); // physical edge
    CHECK(!p.Update(true, true, true, true)); // delayed latch
    CHECK(!p.Update(true, false, true, true));
    CHECK(!p.Update(false, true, true, true)); // stale latch on release
    CHECK(!p.Update(false, false, true, true));
    CHECK(p.Update(true, true, true, true)); // next press
    CHECK(!p.Update(true, true, true, true));
    CHECK(!p.Update(false, false, false, true));
    CHECK(!p.Update(true, false, false, true)); // outside hit area
    CHECK(!p.Update(true, false, true, true)); // entering while held isn't an edge
    CHECK(!p.Update(false, false, true, true));
    CHECK(!p.Update(true, true, true, false)); // inactive screen
    CHECK(!p.Update(false, false, true, true));
    CHECK(p.Update(false, true, true, true)); // engine-only press signal
    CHECK(!p.Update(false, true, true, true));
}

// Exhaust all four-pass input sequences against the pre-refactor widget rule.
static void ExistingPolicyParity()
{
    for (unsigned sequence = 0; sequence < 65536; ++sequence) {
        ButtonPress p;
        bool wasDown = false, consumed = false;
        for (unsigned frame = 0; frame < 4; ++frame) {
            const unsigned bits = (sequence >> (frame * 4)) & 15;
            const bool down = (bits & 1) != 0, latch = (bits & 2) != 0;
            const bool hover = (bits & 4) != 0, live = (bits & 8) != 0;
            const bool edge = down && !wasDown;
            wasDown = down;
            if (!down && !latch) consumed = false;
            const bool expected = hover && live && !consumed && (latch || edge);
            if (expected) consumed = true;
            CHECK(p.Update(down, latch, hover, live) == expected);
        }
    }
    ButtonPress widget, launcher;
    CHECK(widget.Update(true, true, true, true));
    CHECK(launcher.Update(true, true, true, true)); // separate owners, no state bleed
    CHECK(!widget.Update(true, true, true, true));
}

int main()
{
    Geometry();
    Presses();
    ExistingPolicyParity();
    std::puts("ui button core: all tests passed");
}
