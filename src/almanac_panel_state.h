// Mod-owned lifecycle policy; no game ABI, allocation, rendering or input capture.
#pragma once
#include <cstdint>
#include <limits>

namespace almanac {
enum class PanelPhase {
    Closed, OpenRequested, Opening, Open, CloseRequested, Closing
};

// Owned and called on ONE UI thread. This is not a cross-DLL service interface.
// Backend adapter contract and usage: tests/almanac_button/PANEL_LIFECYCLE.md.
// Each successful RequestOpen reserves a generation. Keep this object alive
// across close/reopen; never recreate it while backend completions can arrive.
class PanelState {
public:
    PanelState() = default;
    PanelState(const PanelState&) = delete;
    PanelState& operator=(const PanelState&) = delete;
    using Generation = std::uint64_t;
    PanelPhase Phase() const { return phase_; }
    Generation CurrentGeneration() const { return generation_; }

    bool RequestOpen() {
        if (phase_ != PanelPhase::Closed ||
            generation_ == std::numeric_limits<Generation>::max()) return false;
        ++generation_; // Never recycle IDs, even after a cancelled request.
        closeAfterOpen_ = false;
        phase_ = PanelPhase::OpenRequested;
        return true;
    }

    // Drain an open request ONCE, immediately before invoking the backend.
    // Zero means no action. Transition first so synchronous completion is safe.
    Generation BeginOpen() {
        if (phase_ != PanelPhase::OpenRequested) return 0;
        phase_ = PanelPhase::Opening;
        return generation_;
    }

    // Backend owns native resources. A failure means it retained NO live panel.
    bool CompleteOpen(Generation id, bool succeeded) {
        if (!Matches(id) || phase_ != PanelPhase::Opening) return false;
        if (!succeeded) {
            SetClosed();
        } else {
            phase_ = closeAfterOpen_ ? PanelPhase::CloseRequested : PanelPhase::Open;
            closeAfterOpen_ = false;
        }
        return true;
    }

    // Close button, Escape and host teardown can all request the same action.
    // Return true only when a NEW cancellation/close request was accepted.
    bool RequestClose() {
        switch (phase_) {
        case PanelPhase::OpenRequested:
            SetClosed(); // Nothing has been handed to the backend yet.
            return true;
        case PanelPhase::Opening:
            if (closeAfterOpen_) return false;
            closeAfterOpen_ = true; // Do not pretend creation was cancelled.
            return true;
        case PanelPhase::Open:
            phase_ = PanelPhase::CloseRequested;
            return true;
        default:
            return false;
        }
    }

    // Close tickets identify ATTEMPTS, not panel generations: a delayed result
    // from a failed attempt cannot finish a later retry or a newer panel.
    using CloseTicket = std::uint64_t;
    CloseTicket BeginClose() {
        if (phase_ != PanelPhase::CloseRequested ||
            closeTicket_ == std::numeric_limits<CloseTicket>::max()) return 0;
        ++closeTicket_;
        phase_ = PanelPhase::Closing;
        return closeTicket_;
    }

    // Failure means the panel remains live; adapter may retry with backoff.
    // A missing acknowledgement deliberately does not permit another open.
    bool CompleteClose(CloseTicket ticket, bool succeeded) {
        if (ticket == 0 || ticket != closeTicket_ ||
            phase_ != PanelPhase::Closing) return false;
        if (succeeded) SetClosed();
        else phase_ = PanelPhase::CloseRequested;
        return true;
    }

    // For native destruction observed without our own BeginClose (e.g. native
    // window close). Must mean destruction/removal is CONFIRMED, not requested.
    // During Opening, backend must first report creation success/failure.
    bool NotifyDestroyed(Generation id) {
        if (!Matches(id) || (phase_ != PanelPhase::Open &&
            phase_ != PanelPhase::CloseRequested && phase_ != PanelPhase::Closing))
            return false;
        SetClosed();
        return true;
    }

private:
    bool Matches(Generation id) const { return id != 0 && id == generation_; }
    void SetClosed() {
        phase_ = PanelPhase::Closed;
        closeAfterOpen_ = false;
    }
    PanelPhase phase_ = PanelPhase::Closed;
    Generation generation_ = 0;
    CloseTicket closeTicket_ = 0;
    bool closeAfterOpen_ = false;
};
} // namespace almanac
