#include "almanac_finance_session.h"
#include "almanac_overlay_controller.h"
#include <cstdio>
#include <cstdlib>
#define CHECK(x) do { if (!(x)) { std::fprintf(stderr,"FAIL %d: %s\n",__LINE__,#x); std::exit(1); } } while (0)
using namespace almanac;
static TsmTaxPolicySnapshot policy{};
static int reads = 0, writes = 0, reject = -1;
static int Currency() { return TSM_CURRENCY_USD; }
static int Read(TsmTaxPolicySnapshot* out, unsigned) { ++reads; *out = policy; return 1; }
static int Rate(int kind, float value) {
    ++writes;
    if (kind == reject) return 0;
    policy.configured[kind] = value; return 1;
}
static int Enabled(int kind, int value) { ++writes; policy.enabled[kind] = value; return 1; }
int main() {
    policy.structSize = sizeof(policy);
    policy.configured[0] = .2f; policy.configured[1] = .15f; policy.configured[2] = 100;
    policy.enabled[0] = policy.enabled[1] = policy.enabled[2] = 1;
    TsmTaxesApi taxes{}; taxes.structSize = sizeof(taxes);
    taxes.getPolicySnapshot = Read; taxes.setTaxRate = Rate; taxes.setTaxEnabled = Enabled;
    TsmCurrencyApi currency{}; currency.structSize = sizeof(currency);
    currency.getActiveCurrency = Currency;
    FinanceSession session;
    OverlayController controller;
    CHECK(controller.SetWorld(1, true));
    CHECK(controller.Activate());
    auto generation = controller.BeginOpen();
    CHECK(controller.CompleteOpen(generation, true));
    OverlayContext context{1, generation, 1, true};
    CHECK(controller.Publish(1, generation, 1, BuildFinanceSnapshot(&currency, &taxes)));
    CHECK(session.Load(context, &currency, &taxes) == EditLoadStatus::Loaded);
    auto draft = session.Draft(); draft.value[0] = 25;
    CHECK(controller.Apply(draft, session.Token()) == OverlaySubmit::Accepted);
    OverlayApplyCommand command{};
    CHECK(controller.Take(command));
    auto outcome = session.Execute(command, context, &currency, &taxes);
    CHECK(outcome.dispatch == SessionDispatch::Ran);
    CHECK(outcome.finance.status == EditApplyStatus::Applied && writes == 1);
    CHECK(controller.Complete(command.request, outcome.finance));
    OverlayApplyResult ack{};
    CHECK(controller.Poll(ack));
    CHECK(!session.Active());
    CHECK(session.Execute(command, context, &currency, &taxes).dispatch == SessionDispatch::DuplicateOrOld);
    CHECK(writes == 1);
    ++command.request;
    CHECK(session.Execute(command, context, &currency, &taxes).dispatch == SessionDispatch::NotLoaded);

    // Reload mints a new edit identity even if snapshot revision is unchanged.
    const auto oldToken = session.Token();
    CHECK(session.Load(context, &currency, &taxes) == EditLoadStatus::Loaded);
    CHECK(session.Token() != oldToken);
    ++command.request;
    CHECK(session.Execute(command, context, &currency, &taxes).dispatch == SessionDispatch::Stale);
    CHECK(writes == 1);
    command.editSession = session.Token(); command.draft = session.Draft();
    command.draft.value[0] = -1; ++command.request;
    outcome = session.Execute(command, context, &currency, &taxes);
    CHECK(outcome.finance.status == EditApplyStatus::InvalidDraft && session.Active());
    CHECK(writes == 1);
    command.draft.value[0] = 30; ++command.request;
    policy.configured[0] = .4f;
    outcome = session.Execute(command, context, &currency, &taxes);
    CHECK(outcome.finance.status == EditApplyStatus::Conflict && !session.Active());
    CHECK(writes == 1);

    CHECK(session.Load(context, &currency, &taxes) == EditLoadStatus::Loaded);
    command.editSession = session.Token(); command.draft = session.Draft();
    command.draft.value[0] = 50; command.draft.value[1] = 35;
    reject = 1; ++command.request;
    outcome = session.Execute(command, context, &currency, &taxes);
    CHECK(outcome.finance.status == EditApplyStatus::PartiallyApplied && !session.Active());
    CHECK(writes == 3);
    CHECK(session.Load(context, &currency, &taxes) == EditLoadStatus::Loaded);
    command.editSession = session.Token(); ++command.request;
    auto otherWorld = context; ++otherWorld.world;
    const int beforeReads = reads;
    CHECK(session.Execute(command, otherWorld, &currency, &taxes).dispatch == SessionDispatch::Stale);
    CHECK(reads == beforeReads && writes == 3);
    CHECK(session.Cancel());
    ++command.request;
    CHECK(session.Execute(command, context, &currency, &taxes).dispatch == SessionDispatch::Stale);
    CHECK(reads == beforeReads && writes == 3);
    CHECK(session.Load(context, nullptr, nullptr) != EditLoadStatus::Loaded);
    CHECK(!session.Active() && !session.Token());
    std::puts("finance session tests passed");
}
