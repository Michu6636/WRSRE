#include "almanac_overlay_bridge.h"
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <thread>
#define CHECK(x) do { if (!(x)) { std::fprintf(stderr,"FAIL line %d: %s\n",__LINE__,#x); std::exit(1); } } while (0)
using namespace almanac;
int main() {
    OverlayApplyBridge bridge;
    OverlayContext context{1, 1, 1, true};
    OverlayApplyCommand command{1, context, {}};
    OverlayApplyCommand taken{};
    OverlayApplyResult result{};
    CHECK(bridge.Submit(command) == OverlaySubmit::Unavailable);
    bridge.SetContext(context);
    auto bad = command;
    bad.request = 0;
    CHECK(bridge.Submit(bad) == OverlaySubmit::InvalidId);
    bad = command; bad.context.world = 2;
    CHECK(bridge.Submit(bad) == OverlaySubmit::Stale);
    command.draft.value[0] = 12.5;
    CHECK(bridge.Submit(command) == OverlaySubmit::Accepted);
    command.draft.value[0] = 99;
    CHECK(bridge.Submit(command) == OverlaySubmit::DuplicateOrOld);
    bad = command; bad.request = 2;
    CHECK(bridge.Submit(bad) == OverlaySubmit::Busy);
    CHECK(bridge.Take(taken));
    CHECK(taken.draft.value[0] == 12.5);
    CHECK(!bridge.Take(taken));
    CHECK(!bridge.Cancel(1));
    FinanceApplyResult finance{};
    finance.status = EditApplyStatus::PartiallyApplied;
    finance.accepted = 1; finance.rejected = 1;
    CHECK(!bridge.Complete(2, finance));
    CHECK(bridge.Complete(1, finance));
    CHECK(!bridge.Complete(1, finance));
    CHECK(bridge.Submit(bad) == OverlaySubmit::Busy);
    CHECK(bridge.Consume(result));
    CHECK(result.executed && !result.stale);
    CHECK(result.finance.status == EditApplyStatus::PartiallyApplied);
    CHECK(result.finance.accepted == 1 && result.finance.rejected == 1);
    CHECK(!bridge.Consume(result));
    CHECK(bridge.Submit(command) == OverlaySubmit::DuplicateOrOld);

    // Close before execution: acknowledge cancellation, never run handler.
    command.request = 2;
    CHECK(bridge.Submit(command) == OverlaySubmit::Accepted);
    CHECK(!bridge.Cancel(3));
    CHECK(bridge.Cancel(2));
    CHECK(!bridge.Take(taken));
    CHECK(bridge.Consume(result));
    CHECK(result.outcome == OverlayOutcome::Cancelled && !result.executed);

    // Epoch change invalidates queued requests but retains their result.
    command.request = 3;
    CHECK(bridge.Submit(command) == OverlaySubmit::Accepted);
    ++context.world;
    bridge.SetContext(context);
    CHECK(!bridge.Take(taken));
    CHECK(bridge.Consume(result));
    CHECK(result.outcome == OverlayOutcome::Stale && result.stale && !result.executed);
    CHECK(bridge.Submit(command) == OverlaySubmit::DuplicateOrOld);

    // A late completion is not called a rollback or shown as a fresh result.
    command.request = 4; command.context = context;
    CHECK(bridge.Submit(command) == OverlaySubmit::Accepted);
    CHECK(bridge.Take(taken));
    ++context.panel;
    bridge.SetContext(context);
    CHECK(bridge.Complete(4, finance));
    CHECK(bridge.Consume(result));
    CHECK(result.executed && result.stale && result.context.panel == 1);
    CHECK(!bridge.Complete(4, finance));

    command.request = 5; command.context = context;
    CHECK(bridge.Submit(command) == OverlaySubmit::Accepted);
    ++context.revision;
    bridge.SetContext(context);
    CHECK(bridge.Consume(result) && result.stale);
    command.request = 6; command.context = context;
    CHECK(bridge.Submit(command) == OverlaySubmit::Accepted);
    context.ready = false;
    bridge.SetContext(context);
    CHECK(bridge.Consume(result) && result.stale && !result.executed);
    context.ready = true;
    bridge.SetContext(context);

    // Concurrent duplicate submission admits exactly one handler execution.
    command.request = 7; command.context = context;
    OverlaySubmit a{}, b{};
    std::thread first([&] { a = bridge.Submit(command); });
    std::thread second([&] { b = bridge.Submit(command); });
    first.join(); second.join();
    CHECK((a == OverlaySubmit::Accepted && b == OverlaySubmit::DuplicateOrOld) ||
          (b == OverlaySubmit::Accepted && a == OverlaySubmit::DuplicateOrOld));
    CHECK(bridge.Take(taken));
    CHECK(bridge.Complete(7, finance));
    CHECK(bridge.Consume(result));

    // IDs never wrap: exhausted producer IDs cannot be reused.
    command.request = std::numeric_limits<std::uint64_t>::max();
    CHECK(bridge.Submit(command) == OverlaySubmit::Accepted);
    CHECK(bridge.Cancel(command.request));
    CHECK(bridge.Consume(result));
    command.request = 1;
    CHECK(bridge.Submit(command) == OverlaySubmit::DuplicateOrOld);
    std::puts("overlay bridge tests passed");
}
