#include "tsm_mock_host.h"
#include "../../src/game_addresses.h"
#include "../../src/game_world.h"

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>

static int failures = 0;
#define CHECK(cond) do { if (!(cond)) { std::printf("FAIL: %s (%s:%d)\n", #cond, __FILE__, __LINE__); ++failures; } } while(0)

using wrsre::mock::MockTsmEnvironment;

int main() {
    MockTsmEnvironment env;
    TsmHost* h = env.host();
    TsmBind(h);
    CHECK(h != nullptr);
    CHECK(h->apiVersion == TSM_API_VERSION);
    CHECK(h->structSize == sizeof(TsmHost));

    // Service registration/version isolation
    static int dummyA = 1;
    static int dummyB = 2;
    CHECK(h->provide("svc", 1, &dummyA) == 1);
    CHECK(h->consume("svc", 1) == &dummyA);
    CHECK(h->consume("svc", 2) == nullptr); // version mismatch
    CHECK(h->provide("svc", 1, &dummyB) == 0); // duplicate version refused
    CHECK(h->provide("svc", 2, &dummyB) == 1);
    CHECK(h->consume("svc", 2) == &dummyB);

    // Config
    env.setConfigInt("t.ini", "s", "k", 42);
    env.setConfigString("t.ini", "s", "str", "hello");
    CHECK(h->configInt("t.ini", "s", "k", 0) == 42);
    CHECK(h->configInt("t.ini", "s", "missing", 7) == 7);
    char buf[64];
    h->configString("t.ini", "s", "str", buf, sizeof(buf), "");
    CHECK(std::strcmp(buf, "hello") == 0);

    // readablePtr
    void* p = std::malloc(16);
    env.trackRange(p, 16);
    CHECK(h->readablePtr(p, 16) == 1);
    CHECK(h->readablePtr((void*)0x1, 4) == 0);
    CHECK(h->readablePtr((void*)0xDEADBEEF, 4) == 0);
    std::free(p);

    // PE / addresses
    CHECK(g_tsmAddr.exeVerified == 1);
    CHECK(h->exeBase != nullptr);

    // TsmGameObject accessors
    void* go = env.gameObject();
    CHECK(go != nullptr);
    // treasury offsets readable/writable
    std::memset(go, 0, 0x600);
    *reinterpret_cast<long long*>((char*)go + TSM_GO_TREASURY_USD) = 12345;
    *reinterpret_cast<long long*>((char*)go + TSM_GO_TREASURY_RUB) = 67890;
    CHECK(*reinterpret_cast<long long*>((char*)go + TSM_GO_TREASURY_USD) == 12345);
    CHECK(*reinterpret_cast<long long*>((char*)go + TSM_GO_TREASURY_RUB) == 67890);

    // Building / Person allocators
    void* b = env.allocateBuilding();
    CHECK(b != nullptr);
    CHECK(TsmLooksLikeBuilding(b));
    void* person = env.allocatePerson();
    CHECK(person != nullptr);
    CHECK(h->readablePtr(person, TSM_PERSON_SIZE) == 1);

    // 365-day clock stepping
    env.clock().daysPerMonth = 30;
    int startYear = env.clock().year;
    for (int i = 0; i < 365; ++i) env.stepDay();
    CHECK(env.clock().year == startYear + 1);
    CHECK(env.clock().day == 0);
    // month/quarter valid
    CHECK(env.clock().month >= 1 && env.clock().month <= 12);
    CHECK(env.clock().quarter >= 1 && env.clock().quarter <= 4);

    // tick stepping
    env.stepTick();
    CHECK(env.clock().tick == 1);

    // persistence writeAtomic/readFile via temp dir
    const char* data = "hello";
    CHECK(env.writeAtomic(env.tempDir(), "test.txt", data, 5) == 1);
    std::size_t len = 0;
    void* rd = env.readFile(env.tempDir(), "test.txt", &len);
    CHECK(rd != nullptr && len == 5 && std::memcmp(rd, data, 5) == 0);
    std::free(rd);

    if (failures) {
        std::printf("run_mock_host_tests: %d failures\n", failures);
        return 1;
    }
    std::printf("run_mock_host_tests: OK\n");
    return 0;
}
