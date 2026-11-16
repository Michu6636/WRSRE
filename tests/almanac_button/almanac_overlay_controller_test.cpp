#include "almanac_overlay_controller.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#define CHECK(x) do { if (!(x)) { std::fprintf(stderr,"FAIL %d: %s\n",__LINE__,#x); std::exit(1); } } while (0)
using namespace almanac;
static std::uint64_t Open(OverlayController& c) {
    CHECK(c.Activate());
    CHECK(!c.Activate());
    const auto generation = c.BeginOpen();
    CHECK(generation && !c.BeginOpen());
    CHECK(c.CompleteOpen(generation, true));
    return generation;
}
static void Close(OverlayController& c) {
    c.Close();
    const auto ticket = c.BeginClose();
    CHECK(ticket && !c.BeginClose());
    CHECK(c.CompleteClose(ticket, true));
}
static void CompletedBeforeWorldChange() {
    OverlayController c;
    CHECK(c.SetWorld(1, true));
    const auto generation = Open(c);
    CHECK(c.Publish(1, generation, 1, FinanceSnapshot{}));
    CHECK(c.Apply(FinanceDraft{}) == OverlaySubmit::Accepted);
    OverlayApplyCommand work{};
    CHECK(c.Take(work));
    FinanceApplyResult finance{};
    finance.status = EditApplyStatus::Applied;
    CHECK(c.Complete(work.request, finance));
    CHECK(c.SetWorld(2, false));
    OverlayApplyResult ack{};
    CHECK(c.Poll(ack) && ack.stale && ack.executed);
    CHECK(ack.finance.status == EditApplyStatus::Applied);
    CHECK(!c.Pending() && !c.Result(finance) && !c.Feedback(ack));
}
int main() {
    CompletedBeforeWorldChange();
    OverlayController c;
    FinanceDraft draft{};
    FinanceSnapshot snapshot{};
    FinanceView view{};
    OverlayApplyCommand work{};
    OverlayApplyResult ack{};
    FinanceApplyResult result{};
    result.status = EditApplyStatus::PartiallyApplied;
    CHECK(!c.Activate());
    CHECK(c.Apply(draft) == OverlaySubmit::Unavailable);
    CHECK(!c.SetWorld(0, true));
    CHECK(c.SetWorld(1, true));
    const auto first = Open(c);
    CHECK(!c.View(view));
    CHECK(!c.Publish(2, first, 1, snapshot));
    CHECK(c.Publish(1, first, 1, snapshot));
    CHECK(!c.Publish(1, first, 1, snapshot));
    CHECK(c.View(view));
    CHECK(std::strstr(view.rows[0].value, "Unavailable"));
    draft.value[0] = 15;
    CHECK(c.Apply(draft) == OverlaySubmit::Accepted);
    CHECK(c.Apply(draft) == OverlaySubmit::Busy);
    CHECK(c.Take(work) && work.draft.value[0] == 15);
    CHECK(!c.Take(work));
    CHECK(c.Complete(work.request, result));
    CHECK(c.Pending()); // no implicit timeout or retry while result is undrained
    CHECK(c.Poll(ack) && !c.Pending());
    CHECK(c.Result(result) && result.status == EditApplyStatus::PartiallyApplied);
    CHECK(!c.Poll(ack));

    // Closing queued work cancels it; new panel cannot show old acknowledgement.
    CHECK(c.Apply(draft) == OverlaySubmit::Accepted);
    Close(c);
    const auto second = Open(c);
    CHECK(second != first);
    CHECK(c.Publish(1, second, 2, snapshot));
    CHECK(c.Apply(draft) == OverlaySubmit::Busy);
    CHECK(c.Poll(ack) && !ack.executed);
    CHECK(!c.Result(result));
    CHECK(!c.Publish(1, first, 3, snapshot));

    // Closing executing work cannot undo it; reopening still waits for result.
    CHECK(c.Apply(draft) == OverlaySubmit::Accepted);
    CHECK(c.Take(work));
    Close(c);
    const auto third = Open(c);
    CHECK(c.Publish(1, third, 3, snapshot));
    CHECK(c.Apply(draft) == OverlaySubmit::Busy);
    CHECK(c.Complete(work.request, result));
    CHECK(c.Poll(ack) && ack.executed && ack.stale);
    CHECK(!c.Result(result));
    CHECK(!c.Complete(work.request, result));

    // New revision invalidates queued request, not a hidden automatic retry.
    CHECK(c.Apply(draft) == OverlaySubmit::Accepted);
    CHECK(c.Publish(1, third, 4, snapshot));
    CHECK(!c.Take(work));
    CHECK(c.Poll(ack) && ack.outcome == OverlayOutcome::Stale);

    // World transition while opening: keep ownership until close acknowledged.
    Close(c);
    CHECK(c.Activate());
    const auto opening = c.BeginOpen();
    CHECK(c.SetWorld(2, false));
    CHECK(!c.SetWorld(1, true));
    CHECK(!c.Activate());
    CHECK(c.CompleteOpen(opening, true));
    CHECK(c.Phase() == PanelPhase::CloseRequested);
    const auto ticket = c.BeginClose();
    CHECK(c.CompleteClose(ticket, true));
    CHECK(c.SetWorld(2, true));
    const auto fourth = Open(c);
    CHECK(!c.Publish(1, fourth, 5, snapshot));
    CHECK(c.Publish(2, fourth, 1, snapshot));
    CHECK(c.Apply(draft) == OverlaySubmit::Accepted);
    CHECK(c.SetWorld(2, false));
    CHECK(!c.View(view));
    CHECK(c.Poll(ack) && !ack.executed && ack.stale);
    CHECK(!c.Result(result));
    std::puts("overlay controller tests passed");
}
