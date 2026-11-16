#include "../../prototypes/almanac_overlay/almanac_debug_toggle.h"
#include <cstdio>
#include <cstdlib>
#define CHECK(x) do { if (!(x)) { std::fprintf(stderr, "FAIL %d: %s\n", __LINE__, #x); std::exit(1); } } while (0)
int main() {
    almanac::DebugOverlayToggle toggle;
    CHECK(!toggle.Visible());
    CHECK(!toggle.Update(true, true)); // held at startup: not a fresh press
    CHECK(!toggle.Update(false, true));
    CHECK(toggle.Update(true, true) && toggle.Visible());
    for (int i = 0; i < 100; ++i) {
        CHECK(!toggle.Update(true, true)); // repeat/down samples
        CHECK(toggle.Visible());
    }
    CHECK(!toggle.Update(false, true));
    CHECK(toggle.Visible());
    CHECK(toggle.Update(true, true) && !toggle.Visible());
    CHECK(!toggle.Update(false, true));
    CHECK(toggle.Update(true, true) && toggle.Visible());
    CHECK(toggle.Update(true, false) && !toggle.Visible());
    CHECK(!toggle.Update(true, true)); // focus restored with F8 held
    CHECK(!toggle.Update(false, false)); // release while blocked isn't arming
    CHECK(!toggle.Update(true, true));
    CHECK(!toggle.Visible());
    CHECK(!toggle.Update(false, true));
    CHECK(toggle.Update(true, true) && toggle.Visible());
    toggle.Hide();
    CHECK(!toggle.Visible());
    CHECK(!toggle.Update(true, true));
    toggle.Hide(); // idempotent, including while already hidden
    CHECK(!toggle.Update(false, true));
    CHECK(toggle.Update(true, true) && toggle.Visible());
    CHECK(toggle.Update(false, false) && !toggle.Visible());
    CHECK(!toggle.Update(false, false));
    CHECK(!toggle.Update(false, true));
    CHECK(toggle.Update(true, true) && toggle.Visible());

    almanac::DebugOverlayToggle autoShow;
    CHECK(!autoShow.ShowOnFirstAllowedFrame(false)); // wait for foreground permission
    CHECK(!autoShow.Visible());
    CHECK(autoShow.ShowOnFirstAllowedFrame(true) && autoShow.Visible());
    CHECK(!autoShow.ShowOnFirstAllowedFrame(true)); // show once, not every frame
    CHECK(!autoShow.Update(true, true)); // a held startup key does not close it
    CHECK(autoShow.Visible());
    CHECK(!autoShow.Update(false, true));
    CHECK(autoShow.Update(true, true) && !autoShow.Visible());
    CHECK(!autoShow.Update(false, false)); // focus loss remains fail-closed
    CHECK(!autoShow.ShowOnFirstAllowedFrame(true)); // focus restore does not auto-reopen
    CHECK(!autoShow.Update(true, true)); // held through restore is ignored
    CHECK(!autoShow.Update(false, true));
    CHECK(autoShow.Update(true, true) && autoShow.Visible());
    std::puts("debug visibility toggle tests passed");
}
