// Toggle policy checks for the Almanac panel. Plain C++17, no ImGui, no Windows.
#include "../../plugins/almanac_overlay_dev/overlay_toggle.h"

#include <cstdio>

namespace {

int g_failures = 0;

void Check(bool ok, const char* what)
{
    if (!ok) {
        std::printf("FAIL  %s\n", what);
        ++g_failures;
    }
}

void CheckEqual(int actual, int expected, const char* what)
{
    if (actual != expected) {
        std::printf("FAIL  %s (expected %d, got %d)\n", what, expected, actual);
        ++g_failures;
    }
}

// ---------------------------------------------------------------- toggle

void ToggleTests()
{
    {
        almanac::OverlayToggle t;
        t.SetAutoShow(false);
        Check(!t.Visible(), "starts hidden when auto-show is off");

        // A key already held when the panel first runs is not a press: the
        // level has to be seen released once. Otherwise a stuck or held key at
        // load would open the panel by itself.
        CheckEqual(t.Frame(true, true) ? 1 : 0, 0, "held-from-start is not a press");
        CheckEqual(t.Frame(false, true) ? 1 : 0, 0, "release does not show");

        CheckEqual(t.Frame(true, true) ? 1 : 0, 1, "first press shows");
        // Holding the key: the level stays true for many frames.
        for (int frame = 0; frame < 60; ++frame)
            CheckEqual(t.Frame(true, true) ? 1 : 0, 1, "held key stays shown");
        CheckEqual(t.Frame(false, true) ? 1 : 0, 1, "release keeps it shown");
        CheckEqual(t.Frame(true, true) ? 1 : 0, 0, "second press hides");
        CheckEqual(t.Frame(true, true) ? 1 : 0, 0, "held key stays hidden");
        CheckEqual(t.Frame(false, true) ? 1 : 0, 0, "release keeps it hidden");
        CheckEqual(t.Frame(true, true) ? 1 : 0, 1, "third press shows again");
    }

    {   // automatic start, as the shipped INI asks for
        almanac::OverlayToggle t;
        Check(!t.Visible(), "not visible before the first frame");
        CheckEqual(t.Frame(false, true) ? 1 : 0, 1, "auto-shows on first frame");
        CheckEqual(t.Frame(false, true) ? 1 : 0, 1, "stays shown");
        CheckEqual(t.Frame(true, true) ? 1 : 0, 0, "key press hides it");
        CheckEqual(t.Frame(false, true) ? 1 : 0, 0, "stays hidden");
        CheckEqual(t.Frame(true, true) ? 1 : 0, 1, "key press reopens it");
    }

    {   // the first frame is ineligible: nothing may appear
        almanac::OverlayToggle t;
        CheckEqual(t.Frame(false, false) ? 1 : 0, 0, "ineligible frame draws nothing");
        CheckEqual(t.Frame(false, true) ? 1 : 0, 1, "auto-show waits for an eligible frame");
    }

    {   // key held through an alt-tab must not reopen the panel for free
        almanac::OverlayToggle t;
        t.Frame(false, true);        // auto-show
        t.Frame(false, true);        // one more released frame arms the edge
        CheckEqual(t.Frame(true, true) ? 1 : 0, 0, "press hides");
        // alt-tab away while still holding the key
        for (int frame = 0; frame < 10; ++frame)
            CheckEqual(t.Frame(true, false) ? 1 : 0, 0, "ineligible stays hidden");
        // come back still holding: no new press, so no reopen
        CheckEqual(t.Frame(true, true) ? 1 : 0, 0, "held key does not reopen");
        CheckEqual(t.Frame(false, true) ? 1 : 0, 0, "release still hidden");
        CheckEqual(t.Frame(true, true) ? 1 : 0, 1, "fresh press reopens");
    }

    {
        almanac::OverlayToggle t;
        t.Frame(false, true);                 // auto-show consumed
        t.Hide();
        Check(!t.Visible(), "Hide hides");
        CheckEqual(t.Frame(false, true) ? 1 : 0, 0, "Hide does not re-show by itself");
    }

    {   // floating launcher button clicks toggle visibility
        almanac::OverlayToggle t;
        t.SetAutoShow(false);
        CheckEqual(t.Frame(false, true, false) ? 1 : 0, 0, "idle frame stays hidden");
        CheckEqual(t.Frame(false, true, true) ? 1 : 0, 1, "launcher click opens panel");
        CheckEqual(t.Frame(false, true, false) ? 1 : 0, 1, "panel stays open after click");
        CheckEqual(t.Frame(false, true, true) ? 1 : 0, 0, "second launcher click closes panel");
        CheckEqual(t.Frame(false, false, true) ? 1 : 0, 0, "ineligible frame ignores click");
        CheckEqual(t.Frame(false, true, true) ? 1 : 0, 1, "click reopens when eligible");
        CheckEqual(t.Frame(true, true, false) ? 1 : 0, 0, "hotkey closes panel opened by click");
    }
}

} // namespace

int main()
{
    ToggleTests();
    if (g_failures) {
        std::printf("%d toggle check(s) failed\n", g_failures);
        return 1;
    }
    std::printf("toggle policy checks passed\n");
    return 0;
}
