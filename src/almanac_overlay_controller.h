// Synthetic integration seam. One serialized owner thread; no game/graphics calls.
#pragma once
#include "almanac_overlay_bridge.h"
#include "almanac_panel_state.h"
#include "almanac_finance_view.h"
#include <limits>

namespace almanac {
class OverlayController {
public:
    PanelPhase Phase() const { return panel_.Phase(); }
    std::uint64_t PanelGeneration() const { return panel_.CurrentGeneration(); }
    // Epochs never recycle. Same epoch readiness changes are permitted; loss
    // of readiness invalidates data even if readiness later returns.
    bool SetWorld(std::uint64_t epoch, bool ready) {
        if (!epoch || epoch < world_) return false;
        const bool invalidate = epoch != world_ || !ready;
        world_ = epoch;
        ready_ = ready;
        if (invalidate) {
            snapshotValid_ = false;
            revision_ = 0;
            resultVisible_ = false;
            Sync(); // mark old work stale BEFORE close can classify it as cancellation
            Close();
        } else Sync();
        return true;
    }
    bool Activate() {
        if (!ready_ || !panel_.RequestOpen()) return false;
        snapshotValid_ = false;
        resultVisible_ = false;
        Sync();
        return true;
    }
    std::uint64_t BeginOpen() { return panel_.BeginOpen(); }
    bool CompleteOpen(std::uint64_t generation, bool ok) {
        const bool accepted = panel_.CompleteOpen(generation, ok);
        Sync();
        return accepted;
    }
    bool Close() {
        if (pending_) bridge_.Cancel(pending_);
        snapshotValid_ = false;
        resultVisible_ = false;
        const bool accepted = panel_.RequestClose();
        Sync();
        return accepted;
    }
    std::uint64_t BeginClose() { return panel_.BeginClose(); }
    bool CompleteClose(std::uint64_t ticket, bool ok) {
        const bool accepted = panel_.CompleteClose(ticket, ok);
        Sync();
        return accepted;
    }
    bool Publish(std::uint64_t epoch, std::uint64_t generation,
                 std::uint64_t revision, const FinanceSnapshot& snapshot) {
        if (!ready_ || panel_.Phase() != PanelPhase::Open || epoch != world_ ||
            generation != panel_.CurrentGeneration() || !revision || revision <= revision_)
            return false;
        snapshot_ = snapshot;
        snapshotValid_ = true;
        revision_ = revision;
        Sync();
        return true;
    }
    bool View(FinanceView& out) const {
        if (!snapshotValid_ || !ready_ || panel_.Phase() != PanelPhase::Open) return false;
        out = BuildFinanceView(snapshot_);
        return true;
    }
    OverlaySubmit Apply(const FinanceDraft& draft, std::uint64_t editSession = 0) {
        if (pending_) return OverlaySubmit::Busy;
        if (!snapshotValid_ || !ready_ || panel_.Phase() != PanelPhase::Open)
            return OverlaySubmit::Unavailable;
        if (lastRequest_ == std::numeric_limits<std::uint64_t>::max())
            return OverlaySubmit::InvalidId;
        OverlayApplyCommand command{lastRequest_ + 1, Context(), draft, editSession};
        const auto status = bridge_.Submit(command);
        if (status == OverlaySubmit::Accepted) {
            pending_ = ++lastRequest_;
            resultVisible_ = false;
        }
        return status;
    }
    // Synthetic model endpoint. In production all these operations and actual
    // execution must share this controller's serialized owner phase.
    bool Take(OverlayApplyCommand& out) { return bridge_.Take(out); }
    bool Complete(std::uint64_t request, const FinanceApplyResult& result) {
        return bridge_.Complete(request, result);
    }
    bool Reject(std::uint64_t request, SessionDispatch reason) {
        return bridge_.Reject(request, reason);
    }
    // Always drain stale acknowledgements to release capacity, but never put
    // them in a new panel. The caller may record `out` as diagnostic history.
    bool Poll(OverlayApplyResult& out) {
        if (!bridge_.Consume(out)) return false;
        if (out.request == pending_) pending_ = 0;
        resultVisible_ = !out.stale &&
            (out.executed || out.outcome == OverlayOutcome::Rejected) && snapshotValid_ &&
            panel_.Phase() == PanelPhase::Open && out.context.world == world_ &&
            out.context.panel == panel_.CurrentGeneration() &&
            out.context.revision == revision_;
        if (resultVisible_) { result_ = out.finance; acknowledgement_ = out; }
        return true;
    }
    bool Result(FinanceApplyResult& out) const {
        if (!resultVisible_ || !acknowledgement_.executed) return false;
        out = result_;
        return true;
    }
    bool Feedback(OverlayApplyResult& out) const {
        if (!resultVisible_) return false;
        out = acknowledgement_;
        return true;
    }
    bool Pending() const { return pending_ != 0; }
private:
    OverlayContext Context() const {
        return {world_, panel_.CurrentGeneration(), revision_,
            ready_ && snapshotValid_ && panel_.Phase() == PanelPhase::Open};
    }
    void Sync() { bridge_.SetContext(Context()); }
    PanelState panel_;
    OverlayApplyBridge bridge_;
    FinanceSnapshot snapshot_{};
    FinanceApplyResult result_{};
    OverlayApplyResult acknowledgement_{};
    std::uint64_t world_ = 0, revision_ = 0, lastRequest_ = 0, pending_ = 0;
    bool ready_ = false, snapshotValid_ = false, resultVisible_ = false;
};
} // namespace almanac
