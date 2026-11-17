// Host-only development visibility policy; no Windows input or graphics calls.
#pragma once
namespace almanac {
// One serialized UI owner. Visibility is NOT resource ownership or readiness.
// The adapter supplies physical F8 state and explicit permission each tick.
class DebugOverlayToggle {
public:
    bool Visible() const { return visible_; }

    // Show automatically on the first eligible foreground frame. Later focus
    // loss still hides/disarms; subsequent reopening remains an explicit F8.
    bool ShowOnFirstAllowedFrame(bool allowed) {
        if (autoShowConsumed_ || !allowed) return false;
        autoShowConsumed_ = true;
        const bool changed = !visible_;
        visible_ = true;
        armed_ = false;
        return changed;
    }

    // Returns true only when visibility changes. Disallowed input hides the
    // panel and disarms until an allowed key-up is observed. Thus holding F8
    // through focus loss, a modal, startup or re-enable cannot reopen it.
    bool Update(bool f8Down, bool allowed) {
        const bool before = visible_;
        if (!allowed) {
            visible_ = false;
            armed_ = false;
        } else if (!f8Down) {
            armed_ = true;
        } else if (armed_) {
            visible_ = !visible_;
            armed_ = false;
        }
        return before != visible_;
    }

    // Hide only: never releases a context, GPU resource or panel generation.
    // Requires a fresh allowed release before another press is accepted.
    void Hide() { visible_ = false; armed_ = false; }
private:
    bool visible_ = false;
    bool armed_ = false;
    bool autoShowConsumed_ = false;
};
} // namespace almanac
