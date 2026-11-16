#include "almanac_finance_executor.h"
#include <cstdio>
#include <cstdlib>
#define CHECK(x) do { if (!(x)) { std::fprintf(stderr,"FAIL %d: %s\n",__LINE__,#x); std::exit(1); } } while (0)
using namespace almanac;
static int reads, writes;
static TsmTaxPolicySnapshot policy{};
static int Currency() { return TSM_CURRENCY_USD; }
static int Read(TsmTaxPolicySnapshot* out, unsigned) { ++reads; *out = policy; return 1; }
static int Rate(int kind, float value) { ++writes; policy.configured[kind] = value; return 1; }
int main() {
    policy.structSize = sizeof(policy);
    policy.configured[0] = .2f; policy.configured[1] = .15f; policy.configured[2] = 100;
    policy.enabled[0] = policy.enabled[1] = policy.enabled[2] = 1;
    TsmTaxesApi taxes{}; taxes.structSize = sizeof(taxes);
    taxes.getPolicySnapshot = Read; taxes.setTaxRate = Rate;
    TsmCurrencyApi currency{}; currency.structSize = sizeof(currency);
    currency.getActiveCurrency = Currency;
    OverlayController c; FinanceSession session;
    CHECK(c.SetWorld(1, true) && c.Activate());
    const auto panel = c.BeginOpen(); CHECK(c.CompleteOpen(panel, true));
    OverlayContext live{1, panel, 1, true};
    CHECK(c.Publish(1, panel, 1, BuildFinanceSnapshot(&currency, &taxes)));
    CHECK(session.Load(live, &currency, &taxes) == EditLoadStatus::Loaded);
    auto draft = session.Draft(); draft.value[0] = 25;
    const auto token = session.Token();
    OverlayApplyResult ack{}; FinanceApplyResult finance{};
    const int before = reads;
    CHECK(c.Apply(draft, token + 1) == OverlaySubmit::Accepted);
    CHECK(ExecuteFinanceStep(c, session, live, &currency, &taxes) == ExecutorStep::Rejected);
    CHECK(c.Pending());
    CHECK(c.Poll(ack) && !c.Pending());
    CHECK(ack.outcome == OverlayOutcome::Rejected && !ack.executed);
    CHECK(ack.dispatch == SessionDispatch::Stale);
    CHECK(c.Feedback(ack) && !c.Result(finance));
    CHECK(reads == before && writes == 0);
    CHECK(ExecuteFinanceStep(c, session, live, &currency, &taxes) == ExecutorStep::Idle);

    CHECK(c.Apply(draft, token) == OverlaySubmit::Accepted);
    CHECK(ExecuteFinanceStep(c, session, live, &currency, &taxes) == ExecutorStep::Completed);
    CHECK(c.Poll(ack) && ack.executed && ack.dispatch == SessionDispatch::Ran);
    CHECK(c.Result(finance) && finance.status == EditApplyStatus::Applied && writes == 1);
    // No automatic reload after writes: explicit NotLoaded acknowledgement.
    CHECK(c.Apply(draft, token) == OverlaySubmit::Accepted);
    CHECK(ExecuteFinanceStep(c, session, live, &currency, &taxes) == ExecutorStep::Rejected);
    CHECK(c.Poll(ack) && ack.dispatch == SessionDispatch::NotLoaded);
    CHECK(c.Feedback(ack) && !c.Result(finance) && writes == 1);

    // A new live world cannot execute old context, even if UI wasn't notified yet.
    CHECK(session.Load(live, &currency, &taxes) == EditLoadStatus::Loaded);
    CHECK(c.Apply(draft, session.Token()) == OverlaySubmit::Accepted);
    auto newWorld = live; ++newWorld.world;
    CHECK(ExecuteFinanceStep(c, session, newWorld, &currency, &taxes) == ExecutorStep::Rejected);
    CHECK(c.Poll(ack) && ack.dispatch == SessionDispatch::Stale && !ack.executed);
    CHECK(writes == 1);

    // Wrong/duplicate rejection cannot overwrite an already retired result.
    CHECK(!c.Reject(1, SessionDispatch::Stale));
    CHECK(c.Apply(draft, session.Token()) == OverlaySubmit::Accepted);
    OverlayApplyCommand command{}; CHECK(c.Take(command));
    CHECK(!c.Reject(command.request, SessionDispatch::Ran));
    CHECK(c.Close());
    CHECK(c.Reject(command.request, SessionDispatch::Busy));
    CHECK(c.Poll(ack) && ack.stale && !ack.executed);
    CHECK(!c.Feedback(ack) && !c.Pending());
    CHECK(!c.Reject(command.request, SessionDispatch::Busy));
    std::puts("finance executor tests passed");
}
