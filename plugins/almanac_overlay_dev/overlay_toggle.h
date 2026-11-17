// overlay_toggle.h - "is the panel shown?" as a pure decision, no OS calls.
//
// The launcher for this panel is a function key. A key sample is a HELD state,
// while the render path runs many frames per second, so sampling the key alone
// would toggle the panel once per frame instead of once per press. This policy
// turns the level into one edge per press, and refuses to accept an edge while
// the game cannot safely be drawn over.
//
// It is separate from the renderer so the rules can be tested without Windows,
// a GPU or the game. It holds no handle, acquires nothing, allocates nothing.
#pragma once

namespace almanac {

class OverlayToggle {
public:
    bool Visible() const { return visible_; }

    // True when the panel should be drawn THIS frame. Call exactly once per
    // frame that is actually being produced, with the current physical key
    // level and whether this frame is eligible to draw.
    //
    // `eligible` false means the game window is not foreground, is minimised,
    // drawing was refused by config, or no frame is being produced. The panel
    // hides while ineligible and needs a fresh press afterwards, so holding the
    // key through an alt-tab does not bring it back for free.
    bool Frame(bool keyDown, bool eligible, bool buttonClicked = false) {
        if (!eligible) {
            visible_ = false;
            armed_   = false;
            return false;
        }

        if (!keyDown) armed_ = true;    // released: the next press is a new edge

        if (autoShow_ && !autoShowDone_) {
            // Show once, unasked, so a first run produces something on screen
            // without anyone having to guess whether the key is bound. The
            // press that may be held right now is deliberately consumed, so
            // the panel does not vanish before it has been seen.
            autoShowDone_ = true;
            visible_      = true;
            armed_        = false;
            return true;
        }

        if (buttonClicked) {
            visible_ = !visible_;
            return visible_;
        }

        if (keyDown && armed_) {
            armed_   = false;           // consume this press
            visible_ = !visible_;
        }
        return visible_;
    }

    void SetAutoShow(bool on) { autoShow_ = on; }
    bool AutoShow() const { return autoShow_; }

    // Used on a draw failure. Never re-shows by itself: auto-show is one-shot
    // and already consumed, so a broken frame cannot loop.
    void Hide() {
        visible_ = false;
        armed_   = false;
    }

private:
    bool visible_      = false;
    bool armed_        = false;
    bool autoShow_     = true;
    bool autoShowDone_ = false;
};

} // namespace almanac
