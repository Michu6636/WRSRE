// Pure production lifecycle policy; no Windows/game dependency.
#include "almanac_panel_state.h"
#include <cstdio>
#include <cstdlib>
#include <type_traits>

#define CHECK(x) do { if (!(x)) { std::fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #x); std::exit(1); } } while (0)
using almanac::PanelState;
using almanac::PanelPhase;
static_assert(!std::is_copy_constructible<PanelState>::value, "one owner only");
static_assert(!std::is_move_constructible<PanelState>::value, "stable owner only");

static PanelState::Generation Open(PanelState& s)
{
    CHECK(s.RequestOpen());
    const auto id = s.BeginOpen();
    CHECK(id != 0);
    CHECK(s.CompleteOpen(id, true));
    CHECK(s.Phase() == PanelPhase::Open);
    return id;
}

static void NormalAndDuplicateRequests()
{
    PanelState s;
    CHECK(s.Phase() == PanelPhase::Closed);
    CHECK(s.CurrentGeneration() == 0);
    CHECK(!s.RequestClose());
    CHECK(s.BeginOpen() == 0 && s.BeginClose() == 0);
    CHECK(!s.CompleteOpen(0, true));
    CHECK(!s.CompleteClose(0, true));
    CHECK(!s.NotifyDestroyed(0));
    CHECK(s.RequestOpen());
    CHECK(s.Phase() == PanelPhase::OpenRequested);
    for (int i = 0; i < 100; ++i) CHECK(!s.RequestOpen());
    const auto id = s.BeginOpen();
    CHECK(s.Phase() == PanelPhase::Opening);
    CHECK(s.BeginOpen() == 0);
    CHECK(!s.RequestOpen());
    CHECK(!s.NotifyDestroyed(id));
    CHECK(s.CompleteOpen(id, true));
    CHECK(!s.CompleteOpen(id, true));
    CHECK(!s.RequestOpen());
    CHECK(s.RequestClose());
    CHECK(s.Phase() == PanelPhase::CloseRequested);
    CHECK(!s.RequestClose());
    CHECK(!s.RequestOpen());
    const auto ticket = s.BeginClose();
    CHECK(ticket != 0 && s.BeginClose() == 0);
    CHECK(s.Phase() == PanelPhase::Closing);
    CHECK(!s.RequestOpen() && !s.RequestClose());
    CHECK(s.CompleteClose(ticket, true));
    CHECK(s.Phase() == PanelPhase::Closed);
    CHECK(!s.CompleteClose(ticket, true));
    CHECK(!s.NotifyDestroyed(id));
    CHECK(Open(s) > id);
}

static void CancelAndFailure()
{
    PanelState s;
    CHECK(s.RequestOpen());
    const auto cancelled = s.CurrentGeneration();
    CHECK(s.RequestClose());
    CHECK(s.Phase() == PanelPhase::Closed);
    CHECK(s.BeginOpen() == 0);
    CHECK(s.RequestOpen());
    auto id = s.BeginOpen();
    CHECK(id > cancelled);
    CHECK(!s.CompleteOpen(cancelled, true));
    CHECK(s.CompleteOpen(id, false));
    CHECK(s.Phase() == PanelPhase::Closed);
    CHECK(!s.CompleteOpen(id, true));
    CHECK(s.RequestOpen());
    id = s.BeginOpen();
    CHECK(s.RequestClose()); // creation has started: defer, don't lose ownership
    CHECK(!s.RequestClose());
    CHECK(!s.RequestOpen());
    CHECK(s.Phase() == PanelPhase::Opening);
    CHECK(s.CompleteOpen(id, true));
    CHECK(s.Phase() == PanelPhase::CloseRequested);
    CHECK(s.CompleteClose(s.BeginClose(), true));
    CHECK(s.RequestOpen());
    id = s.BeginOpen();
    CHECK(s.RequestClose());
    CHECK(s.CompleteOpen(id, false)); // failed creation needs no destroy
    CHECK(s.Phase() == PanelPhase::Closed);
    Open(s); // deferred-close flag must not leak into the next opening
}

static void CloseRetryAndStaleResults()
{
    PanelState s;
    const auto oldPanel = Open(s);
    CHECK(s.RequestClose());
    const auto first = s.BeginClose();
    CHECK(s.CompleteClose(first, false));
    CHECK(s.Phase() == PanelPhase::CloseRequested);
    CHECK(!s.RequestOpen()); // failed close leaves a live panel
    const auto retry = s.BeginClose();
    CHECK(retry > first);
    CHECK(!s.CompleteClose(first, true));
    CHECK(!s.CompleteClose(first, false));
    CHECK(s.Phase() == PanelPhase::Closing);
    CHECK(s.NotifyDestroyed(oldPanel)); // native close wins over in-flight retry
    const auto newerPanel = Open(s);
    CHECK(newerPanel > oldPanel);
    CHECK(!s.NotifyDestroyed(oldPanel));
    CHECK(!s.CompleteOpen(oldPanel, true));
    CHECK(!s.CompleteClose(retry, true));
    CHECK(s.RequestClose());
    const auto newerClose = s.BeginClose();
    CHECK(newerClose > retry);
    CHECK(!s.CompleteClose(retry, true));
    CHECK(s.Phase() == PanelPhase::Closing);
    CHECK(s.CompleteClose(newerClose, true));
    const auto thirdPanel = Open(s);
    CHECK(s.NotifyDestroyed(thirdPanel)); // native close without RequestClose
    const auto fourthPanel = Open(s);
    CHECK(s.RequestClose());
    CHECK(s.NotifyDestroyed(fourthPanel)); // destroyed before BeginClose
    CHECK(s.BeginClose() == 0);
}

// Fake adapter drives the actual production controller, without a fake game ABI.
struct FakeBackend {
    PanelState state;
    unsigned creates = 0, destroys = 0, livePanels = 0;
    PanelState::Generation opening = 0;
    PanelState::CloseTicket closing = 0;
    void Pump() {
        if (const auto id = state.BeginOpen()) {
            CHECK(opening == 0 && livePanels == 0);
            opening = id;
            ++creates;
        }
        if (const auto id = state.BeginClose()) {
            CHECK(closing == 0 && livePanels == 1);
            closing = id;
            ++destroys;
        }
    }
    void FinishCreate(bool ok) {
        CHECK(opening != 0);
        if (ok) ++livePanels;
        CHECK(state.CompleteOpen(opening, ok));
        opening = 0;
    }
    void FinishDestroy() {
        CHECK(closing != 0 && livePanels == 1);
        --livePanels;
        CHECK(state.CompleteClose(closing, true));
        closing = 0;
    }
};

static void AdapterScenario()
{
    FakeBackend b;
    CHECK(b.state.RequestOpen());
    for (int frame = 0; frame < 200; ++frame) {
        b.Pump();
        CHECK(!b.state.RequestOpen());
    }
    CHECK(b.creates == 1 && b.livePanels == 0);
    b.FinishCreate(true);
    CHECK(!b.state.RequestOpen());
    CHECK(b.state.RequestClose());
    for (int frame = 0; frame < 200; ++frame) {
        b.Pump();
        CHECK(!b.state.RequestOpen());
    }
    CHECK(b.destroys == 1 && b.livePanels == 1);
    b.FinishDestroy();
    CHECK(b.state.RequestOpen());
    b.Pump();
    b.FinishCreate(false);
    CHECK(b.state.RequestOpen());
    b.Pump();
    CHECK(b.state.RequestClose());
    b.FinishCreate(true);
    b.Pump();
    b.FinishDestroy();
    CHECK(b.creates == 3 && b.destroys == 2 && b.livePanels == 0);
    CHECK(b.state.Phase() == PanelPhase::Closed);
}

int main()
{
    NormalAndDuplicateRequests();
    CancelAndFailure();
    CloseRetryAndStaleResults();
    AdapterScenario();
    std::puts("almanac panel lifecycle: all tests passed");
}
