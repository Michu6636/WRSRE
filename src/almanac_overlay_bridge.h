// Host-only, single-slot Apply transport. No renderer, game ABI or service calls.
#pragma once
#include "almanac_finance_editor.h"
#include <cstdint>
#include <mutex>

namespace almanac {
struct OverlayContext {
    std::uint64_t world = 0, panel = 0, revision = 0;
    bool ready = false;
};
struct OverlayApplyCommand {
    std::uint64_t request = 0;
    OverlayContext context{};
    FinanceDraft draft{};
    std::uint64_t editSession = 0; // minted by the model adapter, not the renderer
};
enum class OverlaySubmit { Accepted, Busy, DuplicateOrOld, Unavailable, Stale, InvalidId };
enum class SessionDispatch { Ran, Busy, InvalidRequest, DuplicateOrOld, Stale, NotLoaded };
enum class OverlayOutcome { Completed, Cancelled, Stale, Rejected };
struct OverlayApplyResult {
    std::uint64_t request = 0;
    OverlayContext context{};
    OverlayOutcome outcome = OverlayOutcome::Cancelled;
    bool executed = false;
    bool stale = false;
    FinanceApplyResult finance{};
    SessionDispatch dispatch = SessionDispatch::NotLoaded;
};

// One process-local owner; safe to marshal between threads. Capacity is ONE,
// including its unconsumed acknowledgement. Never call services under its lock.
// Request IDs must be monotonically increasing for this object's whole lifetime.
class OverlayApplyBridge {
public:
    OverlayApplyBridge() = default;
    OverlayApplyBridge(const OverlayApplyBridge&) = delete;
    OverlayApplyBridge& operator=(const OverlayApplyBridge&) = delete;

    // Called at an externally serialized model boundary, not concurrently with
    // actual service execution. Does not cancel or roll back an executing write.
    void SetContext(OverlayContext next) {
        std::lock_guard<std::mutex> lock(mutex_);
        context_ = next;
        if (phase_ == Phase::Queued && !Matches(command_.context))
            FinishWithoutExecution(OverlayOutcome::Stale);
    }
    OverlaySubmit Submit(const OverlayApplyCommand& command) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!command.request) return OverlaySubmit::InvalidId;
        if (command.request <= highWater_) return OverlaySubmit::DuplicateOrOld;
        if (phase_ != Phase::Empty) return OverlaySubmit::Busy;
        if (!context_.ready) return OverlaySubmit::Unavailable;
        if (!Matches(command.context)) return OverlaySubmit::Stale;
        command_ = command;
        highWater_ = command.request;
        phase_ = Phase::Queued;
        return OverlaySubmit::Accepted;
    }
    // Only the serialized model owner may drain/execute. The output is a copy.
    bool Take(OverlayApplyCommand& out) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (phase_ != Phase::Queued) return false;
        if (!Matches(command_.context)) {
            FinishWithoutExecution(OverlayOutcome::Stale);
            return false;
        }
        phase_ = Phase::Executing;
        out = command_;
        return true;
    }
    // Queued close/cancel gets an acknowledgement. Executing means too late:
    // retain the result, and never represent cancellation as rollback.
    bool Cancel(std::uint64_t request) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (phase_ != Phase::Queued || command_.request != request) return false;
        FinishWithoutExecution(OverlayOutcome::Cancelled);
        return true;
    }
    bool Complete(std::uint64_t request, const FinanceApplyResult& finance) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (phase_ != Phase::Executing || command_.request != request) return false;
        result_ = {};
        result_.request = request;
        result_.context = command_.context;
        result_.outcome = OverlayOutcome::Completed;
        result_.executed = true; // handler ran; NOT a claim that setters succeeded
        result_.stale = !Matches(command_.context);
        result_.finance = finance;
        result_.dispatch = SessionDispatch::Ran;
        phase_ = Phase::Result;
        return true;
    }
    // Retire a taken request that the session adapter refused to execute.
    bool Reject(std::uint64_t request, SessionDispatch reason) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (reason == SessionDispatch::Ran || phase_ != Phase::Executing ||
            command_.request != request) return false;
        FinishWithoutExecution(OverlayOutcome::Rejected);
        result_.dispatch = reason;
        return true;
    }
    bool Consume(OverlayApplyResult& out) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (phase_ != Phase::Result) return false;
        out = result_;
        command_ = {};
        result_ = {};
        phase_ = Phase::Empty;
        return true;
    }
private:
    enum class Phase { Empty, Queued, Executing, Result };
    bool Matches(const OverlayContext& c) const {
        return context_.ready && c.ready && c.world && c.panel &&
            c.world == context_.world && c.panel == context_.panel &&
            c.revision == context_.revision;
    }
    void FinishWithoutExecution(OverlayOutcome outcome) {
        result_ = {};
        result_.request = command_.request;
        result_.context = command_.context;
        result_.outcome = outcome;
        result_.stale = !Matches(command_.context);
        phase_ = Phase::Result;
    }
    std::mutex mutex_;
    OverlayContext context_{};
    OverlayApplyCommand command_{};
    OverlayApplyResult result_{};
    std::uint64_t highWater_ = 0;
    Phase phase_ = Phase::Empty;
};
} // namespace almanac
