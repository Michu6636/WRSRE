// Host-only serialized controller/session wiring. No scheduling or native hooks.
#pragma once
#include "almanac_overlay_controller.h"
#include "almanac_finance_session.h"

namespace almanac {
enum class ExecutorStep { Idle, Completed, Rejected, AcknowledgementFailed };
// Caller establishes live context and provider lifetime at a verified safe phase.
// No concurrent context change, close, reload or provider operation is permitted.
// This is not a renderer callback and provides no exception/transaction rollback.
inline ExecutorStep ExecuteFinanceStep(OverlayController& controller,
                                       FinanceSession& session, OverlayContext live,
                                       const TsmCurrencyApi* currency,
                                       const TsmTaxesApi* taxes) {
    OverlayApplyCommand command{};
    if (!controller.Take(command)) return ExecutorStep::Idle;
    const auto result = session.Execute(command, live, currency, taxes);
    if (result.dispatch == SessionDispatch::Ran)
        return controller.Complete(command.request, result.finance)
            ? ExecutorStep::Completed : ExecutorStep::AcknowledgementFailed;
    return controller.Reject(command.request, result.dispatch)
        ? ExecutorStep::Rejected : ExecutorStep::AcknowledgementFailed;
}
} // namespace almanac
