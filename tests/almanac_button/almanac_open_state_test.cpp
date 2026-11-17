// Host test for plugins/almanac_button/almanac_open_state.h.
#include "almanac_open_state.h"
#include "almanac_api.h"

#include <cstdio>
#include <cstdlib>

using namespace almanac;

#define CHECK(x) do { if (!(x)) { std::fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #x); std::exit(1); } } while (0)

static void checkInvariant(const OpenRequestPolicy& p)
{
    CHECK(p.hasPendingRequest == p.IsPending());
}

int main()
{
    // default construction
    {
        OpenRequestPolicy p;
        CHECK(p.IsClosed());
        CHECK(!p.hasPendingRequest);
        checkInvariant(p);
    }
    // closed + activation -> pending request
    {
        OpenRequestPolicy p;
        CHECK(p.IsClosed());
        CHECK(p.Activate() == true);
        CHECK(p.IsPending());
        CHECK(p.hasPendingRequest);
        checkInvariant(p);
    }
    // repeated activations while pending -> no duplicate
    {
        OpenRequestPolicy p;
        CHECK(p.Activate() == true);
        CHECK(p.Activate() == false);
        CHECK(p.Activate() == false);
        CHECK(p.IsPending());
        CHECK(p.hasPendingRequest);
        checkInvariant(p);
    }
    // successful ack -> open
    {
        OpenRequestPolicy p;
        CHECK(p.Activate() == true);
        p.AcknowledgeOpened();
        CHECK(p.IsOpen());
        CHECK(!p.hasPendingRequest);
        checkInvariant(p);
    }
    // failed ack -> retryable closed
    {
        OpenRequestPolicy p;
        CHECK(p.Activate() == true);
        p.AcknowledgeFailed();
        CHECK(p.IsClosed());
        CHECK(!p.hasPendingRequest);
        checkInvariant(p);
        // can retry
        CHECK(p.Activate() == true);
        checkInvariant(p);
    }
    // activation while open -> no duplicate
    {
        OpenRequestPolicy p;
        (void)p.Activate();
        p.AcknowledgeOpened();
        CHECK(p.Activate() == false);
        CHECK(p.IsOpen());
        CHECK(!p.hasPendingRequest);
        checkInvariant(p);
    }
    // confirmed close -> closed (only when open)
    {
        OpenRequestPolicy p;
        (void)p.Activate();
        p.AcknowledgeOpened();
        p.ConfirmClosed();
        CHECK(p.IsClosed());
        CHECK(!p.hasPendingRequest);
        checkInvariant(p);
        // reopen works
        CHECK(p.Activate() == true);
        checkInvariant(p);
    }
    // ConfirmClosed from Pending is a no-op (preserves request)
    {
        OpenRequestPolicy p;
        (void)p.Activate();
        p.ConfirmClosed();
        CHECK(p.IsPending());
        CHECK(p.hasPendingRequest);
        checkInvariant(p);
    }
    // ConfirmClosed from Closed is a no-op
    {
        OpenRequestPolicy p;
        p.ConfirmClosed();
        CHECK(p.IsClosed());
        checkInvariant(p);
    }
    // ack without pending is no-op
    {
        OpenRequestPolicy p;
        p.AcknowledgeOpened();
        CHECK(p.IsClosed());
        p.AcknowledgeFailed();
        CHECK(p.IsClosed());
        checkInvariant(p);
    }
    // AcknowledgeOpened from Open is no-op (stays open)
    {
        OpenRequestPolicy p;
        (void)p.Activate();
        p.AcknowledgeOpened();
        p.AcknowledgeOpened();
        CHECK(p.IsOpen());
        checkInvariant(p);
    }
    // AcknowledgeFailed from Open does not close
    {
        OpenRequestPolicy p;
        (void)p.Activate();
        p.AcknowledgeOpened();
        p.AcknowledgeFailed();
        CHECK(p.IsOpen());
        checkInvariant(p);
    }
    // AcknowledgeOpened after failed ack does not reopen
    {
        OpenRequestPolicy p;
        (void)p.Activate();
        p.AcknowledgeFailed();
        p.AcknowledgeOpened();
        CHECK(p.IsClosed());
        checkInvariant(p);
    }
    // multiple open/close cycles
    {
        OpenRequestPolicy p;
        for (int i = 0; i < 3; ++i) {
            CHECK(p.Activate() == true);
            p.AcknowledgeOpened();
            CHECK(p.IsOpen());
            p.ConfirmClosed();
            CHECK(p.IsClosed());
        }
        checkInvariant(p);
    }
    // TsmAlmanacHasApi guard
    {
        TsmAlmanacApi full{};
        full.structSize = sizeof(TsmAlmanacApi);
        full.requestOpen = [](void){};
        full.isOpen = []()->int { return 0; };
        full.isPending = []()->int { return 0; };
        full.acknowledgeOpened = [](void){};
        full.acknowledgeFailed = [](void){};
        full.confirmClosed = [](void){};
        CHECK(TsmAlmanacHasApi(&full) == 1);
        CHECK(TsmAlmanacHasApi(nullptr) == 0);
        CHECK(TSM_ALMANAC_VERSION == TSM_ALMANAC_VERSION_V1);
        TsmAlmanacApi truncated = full;
        truncated.structSize = offsetof(TsmAlmanacApi, confirmClosed);
        CHECK(TsmAlmanacHasApi(&truncated) == 0);
        TsmAlmanacApi boundary = full;
        boundary.structSize = offsetof(TsmAlmanacApi, confirmClosed) + sizeof(full.confirmClosed) - 1;
        CHECK(TsmAlmanacHasApi(&boundary) == 0);
        TsmAlmanacApi missing = full;
        missing.requestOpen = nullptr;
        CHECK(TsmAlmanacHasApi(&missing) == 0);
        TsmAlmanacApi missing2 = full;
        missing2.isOpen = nullptr;
        CHECK(TsmAlmanacHasApi(&missing2) == 0);
        TsmAlmanacApi missing3 = full;
        missing3.isPending = nullptr;
        CHECK(TsmAlmanacHasApi(&missing3) == 0);
        TsmAlmanacApi missing4 = full;
        missing4.acknowledgeOpened = nullptr;
        CHECK(TsmAlmanacHasApi(&missing4) == 0);
        TsmAlmanacApi missing5 = full;
        missing5.acknowledgeFailed = nullptr;
        CHECK(TsmAlmanacHasApi(&missing5) == 0);
        TsmAlmanacApi missing6 = full;
        missing6.confirmClosed = nullptr;
        CHECK(TsmAlmanacHasApi(&missing6) == 0);
    }
    std::printf("almanac open state: all tests passed\n");
    return 0;
}
