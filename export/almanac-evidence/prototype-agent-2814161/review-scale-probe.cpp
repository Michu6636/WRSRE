// Synthetic inputs only: demonstrate changing scale during a candidate pass.
// g++ -std=c++17 -Wall -Wextra -Werror -pedantic -Iplugins/almanac_button review-scale-probe.cpp -o /tmp/almanac-scale-probe
#include "almanac_button_geometry.h"
#include <cstdio>
using namespace almanac_button;
int main() {
    RowConstants c{0.8f, 45, 85, 64};
    RowGate g{8, 1, -1, 0, 1};
    RowTracker tracker;
    Decision decision{};
    for (int i = 0; i < 8; ++i) {
        if (i == 7) c.scale = 1.0f;
        float x = c.scale * (c.originA + c.originB + i * c.pitch);
        decision = tracker.OnRowTile(x, 196.0f, c, g);
    }
    std::printf("scale changed on last tile: decision=%s, new x=%.1f\n",
                decision.action == Action::DrawNinth ? "DrawNinth" : "None", decision.x);
}
