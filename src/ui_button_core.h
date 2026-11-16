// Shared, engine-independent button geometry and press policy.
// Coordinates are already DPI-scaled by the caller; no window layout is assumed.
#pragma once
#include <cmath>

namespace tsm_ui {
struct ButtonRect {
    float x, y, width, height;
    bool Valid() const {
        return std::isfinite(x) && std::isfinite(y) &&
               std::isfinite(width) && std::isfinite(height) &&
               width > 0.0f && height > 0.0f &&
               std::isfinite(x + width) && std::isfinite(y + height);
    }
    // Match the engine's integer C3DRECT path before either drawing or testing.
    ButtonRect PixelAligned() const {
        const float left = std::trunc(x), top = std::trunc(y);
        return {left, top, std::trunc(x + width) - left,
                std::trunc(y + height) - top};
    }
    float CenterX() const { return x + width * 0.5f; }
    float CenterY() const { return y + height * 0.5f; }
};

// One instance per interaction owner. Observe every pass, even while not hovered.
// A delayed engine latch cannot generate a second click during the same press.
// Re-arm requires both physical release and the engine latch to be clear.
struct ButtonPress {
    bool wasDown = false;
    bool consumed = false;
    bool Update(bool down, bool latch, bool hover, bool live) {
        const bool edge = down && !wasDown;
        wasDown = down;
        if (!down && !latch) consumed = false;
        if (!live || !hover || consumed || !(edge || latch)) return false;
        consumed = true;
        return true;
    }
};
} // namespace tsm_ui
