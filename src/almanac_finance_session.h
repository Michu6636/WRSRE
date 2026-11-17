// Serialized model-side adapter. Provider pointers are borrowed per operation.
#pragma once
#include "almanac_overlay_bridge.h"
#include <limits>

namespace almanac {
struct SessionResult {
    SessionDispatch dispatch = SessionDispatch::NotLoaded;
    FinanceApplyResult finance{};
};
class FinanceSession {
public:
    FinanceSession() = default;
    FinanceSession(const FinanceSession&) = delete;
    FinanceSession& operator=(const FinanceSession&) = delete;
    std::uint64_t Token() const { return token_; }
    bool Active() const { return editor_.Active(); }
    FinanceDraft Draft() const { return editor_.Draft(); }
    EditLoadStatus Load(OverlayContext context, const TsmCurrencyApi* currency,
                        const TsmTaxesApi* taxes) {
        if (busy_) return EditLoadStatus::Busy;
        Guard guard(busy_);
        editor_.Cancel();
        token_ = 0;
        context_ = {};
        if (!context.ready || !context.world || !context.panel ||
            nextToken_ == std::numeric_limits<std::uint64_t>::max())
            return EditLoadStatus::SnapshotUnavailable;
        const auto status = editor_.Load(currency, taxes);
        if (status == EditLoadStatus::Loaded) {
            context_ = context;
            token_ = ++nextToken_;
        }
        return status;
    }
    // Must accompany close/world/readiness loss on the serialized model owner.
    // Cannot roll back an already executed write or interrupt provider callbacks.
    bool Cancel() {
        if (busy_) return false;
        editor_.Cancel();
        token_ = 0;
        context_ = {};
        return true;
    }
    SessionResult Execute(const OverlayApplyCommand& command, OverlayContext live,
                          const TsmCurrencyApi* currency, const TsmTaxesApi* taxes) {
        SessionResult out;
        if (busy_) { out.dispatch = SessionDispatch::Busy; return out; }
        Guard guard(busy_);
        if (!command.request) { out.dispatch = SessionDispatch::InvalidRequest; return out; }
        if (command.request <= highWater_) {
            out.dispatch = SessionDispatch::DuplicateOrOld; return out;
        }
        // Consume the identity even on rejection: the same request must not
        // become executable after a later reload/context change.
        highWater_ = command.request;
        if (!token_ || command.editSession != token_ ||
            !Same(command.context, context_) || !Same(live, context_)) {
            out.dispatch = SessionDispatch::Stale; return out;
        }
        if (!editor_.Active()) return out;
        for (int i = 0; i < 3; ++i) {
            editor_.SetValue(i, command.draft.value[i]);
            editor_.SetEnabled(i, command.draft.enabled[i]);
        }
        out.dispatch = SessionDispatch::Ran;
        out.finance = editor_.Apply(currency, taxes);
        return out;
    }
private:
    struct Guard {
        bool& flag;
        explicit Guard(bool& f) : flag(f) { flag = true; }
        ~Guard() { flag = false; }
    };
    static bool Same(const OverlayContext& a, const OverlayContext& b) {
        return a.ready && b.ready && a.world == b.world && a.panel == b.panel &&
            a.revision == b.revision;
    }
    FinanceEditor editor_;
    OverlayContext context_{};
    std::uint64_t token_ = 0, nextToken_ = 0, highWater_ = 0;
    bool busy_ = false;
};
} // namespace almanac
