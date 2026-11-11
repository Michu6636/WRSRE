// Synthetic adversarial inputs for the original row tracker at 6aa15e0.
// These are not claims that these sequences occurred in the game.
// Compile: g++ -std=c++17 -Wall -Wextra -Werror -pedantic -Iplugins/almanac_button review-probe.cpp -o /tmp/almanac-review-probe
#include "almanac_button_geometry.h"
#include <cstdio>
using namespace almanac_button;
int main() {
    RowConstants c{0.8f, 45, 85, 64};
    RowGate g{8, 1, -1, 0, 1};
    RowTracker tracker;
    Decision decision{};
    for (int i = 0; i < 8; ++i)
        decision = tracker.OnRowTile(104.0f + i * 51.2f, i == 7 ? 800.0f : 196.0f, c, g);
    std::printf("mixed Y across row: decision=%s, new y=%.1f\n",
                decision.action == Action::DrawNinth ? "DrawNinth" : "None", decision.y);
    tracker.Reset();
    for (int i = 0; i < 8; ++i) {
        g.skip578 = i == 7 ? 0 : 1;
        decision = tracker.OnRowTile(104.0f + i * 51.2f, 196.0f, c, g);
    }
    std::printf("skip-gate changed during row: decision=%s\n",
                decision.action == Action::DrawNinth ? "DrawNinth" : "None");
}
