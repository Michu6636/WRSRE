// almanac_open_state.h - pure, host-testable open-request state policy.
//
// This is the seam between a sidebar launcher (which only requests an open)
// and a future real Almanac panel backend (which acknowledges creation).
//
// Rules:
//   - closed + activation  -> one pending open request (Activate returns true)
//   - repeated activations while pending -> no duplicate request (false)
//   - activation while already open -> no duplicate panel (false)
//   - successful panel creation acknowledgement -> open
//   - failed creation acknowledgement -> retryable closed state
//   - confirmed panel close -> closed
//
// Backend call sequence (required):
//   - On successful creation: AcknowledgeOpened().
//   - On failed creation: AcknowledgeFailed() (returns to retryable closed).
//   - On confirmed close of an open panel: ConfirmClosed().
//   - ConfirmClosed is a no-op while Pending; aborting a pending request
//     requires AcknowledgeFailed. Do not call ConfirmClosed to cancel.
//
// Threading: single-threaded ownership. The policy holds plain (non-atomic)
// state and is NOT safe for concurrent access. The launcher's live path uses
// interlocked state; this policy is the host-testable model of that contract.
//
// Invariant: hasPendingRequest == (state == Pending). All transitions keep
// this invariant; callers must not mutate the fields directly.
//
// No Windows headers. No allocation. Safe for host tests.

#ifndef ALMANAC_OPEN_STATE_H
#define ALMANAC_OPEN_STATE_H

#include <cstdint>

namespace almanac {

enum class OpenState : std::uint8_t { Closed = 0, Pending = 1, Open = 2 };

struct OpenRequestPolicy {
    OpenState state = OpenState::Closed;
    bool hasPendingRequest = false;

    // Returns true iff this activation emitted a NEW open request.
    [[nodiscard]] bool Activate()
    {
        if (state == OpenState::Open) return false;     // already open: no duplicate
        if (state == OpenState::Pending) return false;  // pending: no duplicate
        // Closed -> Pending
        state = OpenState::Pending;
        hasPendingRequest = true;
        return true;
    }

    // Panel backend reports successful creation.
    void AcknowledgeOpened()
    {
        if (state == OpenState::Pending) {
            state = OpenState::Open;
            hasPendingRequest = false;
        }
    }

    // Panel backend reports failed creation: retryable closed state.
    void AcknowledgeFailed()
    {
        if (state == OpenState::Pending) {
            state = OpenState::Closed;
            hasPendingRequest = false;
        }
    }

    // Confirmed panel close. Only an actually-open panel closes; a pending
    // request is preserved (use AcknowledgeFailed to cancel/abort it).
    void ConfirmClosed()
    {
        if (state == OpenState::Open) {
            state = OpenState::Closed;
            hasPendingRequest = false;
        }
    }

    bool IsClosed() const { return state == OpenState::Closed; }
    bool IsPending() const { return state == OpenState::Pending; }
    bool IsOpen() const { return state == OpenState::Open; }
};

}  // namespace almanac

#endif  // ALMANAC_OPEN_STATE_H
