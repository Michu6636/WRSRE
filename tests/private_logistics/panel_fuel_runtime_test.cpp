// Native host shim exercises the production queue/control code. NOT Windows ABI,
// SEH, patch execution, real threads, or native game lifecycle validation.
#define main ReaderHarnessMain
#include "panel_fuel_observer_test.cpp"
#undef main
#include <cstdarg>

using BYTE = unsigned char;
using LONG = long; // host-shim log ABI; real Windows LONG is 32-bit
using ULONGLONG = unsigned long long;
struct CRITICAL_SECTION { bool busy=false; };
static bool TryEnterCriticalSection(CRITICAL_SECTION* p) { if(p->busy)return false;p->busy=true;return true; }
static void LeaveCriticalSection(CRITICAL_SECTION* p) { CHECK(p->busy);p->busy=false; }
static void InitializeCriticalSection(CRITICAL_SECTION* p) { p->busy=false; }
static LONG InterlockedCompareExchange(volatile LONG* p,LONG value,LONG expected) { LONG old=*p;if(old==expected)*p=value;return old; }
static LONG InterlockedExchange(volatile LONG* p,LONG value) { LONG old=*p;*p=value;return old; }
static LONG InterlockedIncrement(volatile LONG* p) { return ++*p; }
static ULONGLONG clockMs=1000;
static ULONGLONG GetTickCount64() { return clockMs; }
static std::vector<BYTE> image(0x90B080);
static BYTE* g_exeBase=image.data();
static std::size_t g_exeSize=image.size();
static bool throwOnRead=false;
static uintptr_t throwAtRead=0;
static bool ReadablePtr(const void* p,std::size_t n) {
    if(throwOnRead || (throwAtRead && (uintptr_t)p==throwAtRead)) throw 1;
    const auto a=(uintptr_t)p,b=(uintptr_t)g_exeBase;
    return a>=b && a-b<=image.size() && n<=image.size()-(a-b);
}
constexpr unsigned IMAGE_DOS_SIGNATURE=0x5A4D,IMAGE_NT_SIGNATURE=0x4550;
constexpr unsigned IMAGE_FILE_MACHINE_AMD64=0x8664,IMAGE_NT_OPTIONAL_HDR64_MAGIC=0x20B;
struct IMAGE_DOS_HEADER { unsigned e_magic; int e_lfanew; };
struct IMAGE_NT_HEADERS64 {
    unsigned Signature;
    struct { unsigned Machine, TimeDateStamp; } FileHeader;
    struct { unsigned Magic; } OptionalHeader;
};
static std::vector<std::string> logs;
#if defined(__GNUC__) || defined(__clang__)
static void Logf(const char*,...) __attribute__((format(printf,1,2)));
#endif
static void Logf(const char* format,...) {
    char line[2048];va_list args;va_start(args,format);std::vsnprintf(line,sizeof(line),format,args);va_end(args);logs.emplace_back(line);
}
static int g_panelObserverEnabled=1,g_scheduleObserverEnabled=0,g_billing=0,g_persistenceHandle=1;
static unsigned installed=0,originalCalls=0;
static void Original(void*,void*) { ++originalCalls; }
static bool InstallInlineHook(void*,void*,void** trampoline,const BYTE*,std::size_t n,const char*) {
    CHECK(n==20);*trampoline=(void*)Original;++installed;return true;
}
// Mock resource_pricing and currency for header compilation: endpoint estimate
// cost computation in ScheduleAssignmentLog uses these when computing
// hypothetical_cost_if_no_refill. In the real DLL they resolve via service
// consumption in private_logistics.cpp.
struct TsmCurrencyApi { unsigned structSize; };
static int TsmCurrencyActive(const TsmCurrencyApi*) { return 0; }
struct TsmResourcePricingApi {
    unsigned structSize;
    void* (*getByName)(const char* name);
    float (*getPrice)(void* resRecord, int currency);
};
static const TsmResourcePricingApi* g_pricing = nullptr;
static char gFuelResource[64] = "fuel";
static const TsmCurrencyApi* g_currency = nullptr;
using Tick=void(*)(void*,float,float);
static Tick tick=nullptr;
static int RegisterTick(Tick fn,void*) { tick=fn;return 1; }
static bool paused=false;
static int IsPaused() { return paused ? 1 : 0; }
struct ClockApi { int(*registerTick)(Tick,void*); int(*isPaused)(); };
static ClockApi api{RegisterTick,IsPaused};
static ClockApi* g_simClock=&api;
#define __fastcall
#define __try try
#define __except(x) catch(...)
#include "panel_fuel_runtime.h"

#ifndef PANEL_RUNTIME_TEST_NO_MAIN
int main() {
    Fixture fixture;
    for(const auto& r:fixture.regions) std::memcpy(g_exeBase+r.address,r.data.data(),r.data.size());
    const auto ptr=[&](Address a){return (Address)(g_exeBase+a);};
    const auto put=[&](Address a,Address v){std::memcpy(g_exeBase+a,&v,sizeof(v));};
    put(fixture.window+0x240,ptr(fixture.root));put(fixture.root+0x1708,ptr(fixture.meta));
    put(fixture.root+0x4F8,ptr(fixture.home));put(fixture.home+0x318,ptr(fixture.descriptor));
    put(fixture.home+0xC70,ptr(fixture.fleet));put(fixture.home+0xC78,ptr(fixture.fleet+8));put(fixture.fleet,ptr(fixture.root));
    auto* dos=(IMAGE_DOS_HEADER*)g_exeBase;dos->e_magic=IMAGE_DOS_SIGNATURE;dos->e_lfanew=0x80;
    auto* nt=(IMAGE_NT_HEADERS64*)(g_exeBase+0x80);nt->Signature=IMAGE_NT_SIGNATURE;
    nt->FileHeader.Machine=IMAGE_FILE_MACHINE_AMD64;nt->FileHeader.TimeDateStamp=0x6A3EB6AD;
    nt->OptionalHeader.Magic=IMAGE_NT_OPTIONAL_HDR64_MAGIC;
    const BYTE prologue[]={0x40,0x55,0x56,0x57,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57,0x48,0x8D,0xAC,0x24,0x50,0xD1,0xFF,0xFF};
    std::memcpy(g_exeBase+0x7D5460,prologue,sizeof(prologue));
    const BYTE scale[]={0,0,0x7A,0x44},format[]={0x25,0,0x2e,0,0x32,0,0x66,0,0x6c,0,0,0};
    std::memcpy(g_exeBase+0x90B07C,scale,sizeof(scale));std::memcpy(g_exeBase+0x8F3E98,format,sizeof(format));
    for(const auto& f:g_scheduleFingerprints) std::memcpy(g_exeBase+f.rva,f.bytes,f.size);
    CHECK(ScheduleTargetMatches());
    g_exeBase[0x7E8438]^=1;CHECK(!ScheduleTargetMatches());g_exeBase[0x7E8438]^=1;
    PanelObserverStart();CHECK(installed==1 && tick && g_panelStarted);
    std::puts("PASS runtime_install_fingerprint");
    void* context=(void*)ptr(fixture.context);void* window=(void*)ptr(fixture.window);
    const auto logged=logs.size();PanelRendererDetour(context,window);
    CHECK(originalCalls==1 && g_panelCount==1 && logs.size()==logged);
    PanelCapture(context,window);CHECK(g_panelCount==1);
    tick(nullptr,1,1);CHECK(g_panelCount==0 && logs.size()==logged+2);
    CHECK(logs[logged].find("litres_candidate=549.54")!=std::string::npos);
    CHECK(logs[logged+1].find("exact_fleet_pointer")!=std::string::npos);
    std::puts("PASS runtime_post_render_throttle_and_deferred_log");
    paused=true;
    const auto pausedLogs=logs.size();
    for(unsigned i=0;i<72;++i) {clockMs+=1000;PanelCapture(context,window);}
    CHECK(g_panelCount==1 && g_panelDropped==0 && g_panelSamples==2);
    CHECK(g_panelQueue[g_panelHead].coalesced==71 && logs.size()==pausedLogs);
    paused=false;clockMs+=1000;PanelCapture(context,window);
    CHECK(g_panelCount==2);tick(nullptr,1,1);
    CHECK(logs[pausedLogs].find("reason=paused_snapshot_coalescing")!=std::string::npos);
    CHECK(logs[pausedLogs].find("discarded_paused_snapshots=71")!=std::string::npos);
    CHECK(logs[pausedLogs+1].find("captured_paused=1")!=std::string::npos);
    tick(nullptr,1,1);CHECK(g_panelCount==0);
    std::puts("PASS runtime_long_pause_coalesces_and_reports_omissions");
    PanelObserverLoading();tick(nullptr,1,1);paused=true;
    clockMs+=1000;PanelCapture(context,window);
    put(fixture.window+0x240,ptr(0x90000)); // another root, no assumed native metadata
    clockMs+=1000;PanelCapture(context,window);
    CHECK(g_panelCount==2 && g_panelQueue[g_panelHead].snapshot.root==ptr(fixture.root));
    CHECK(g_panelQueue[(g_panelHead+1)%16].snapshot.root==ptr(0x90000));
    std::memcpy(g_exeBase+0xB0000,g_exeBase+fixture.window,0x500);
    clockMs+=1000;PanelCapture(context,(void*)ptr(0xB0000));CHECK(g_panelCount==3);
    paused=false;clockMs+=1000;PanelCapture(context,(void*)ptr(0xB0000));CHECK(g_panelCount==4);
    put(fixture.window+0x240,ptr(fixture.root));
    PanelObserverLoading();tick(nullptr,1,1);
    std::puts("PASS runtime_pause_coalescing_never_overwrites_other_root_window_or_unpaused_record");
    for(unsigned i=0;i<17;++i) {clockMs+=1000;PanelCapture(context,window);}
    CHECK(g_panelCount==16 && g_panelDropped==1);
    const auto fullLogs=logs.size();tick(nullptr,1,1);
    CHECK(logs[fullLogs].find("reason=queue_full")!=std::string::npos);
    CHECK(logs[fullLogs].find("skipped_snapshot_captures=1")!=std::string::npos);
    CHECK(logs[fullLogs+1].find("queue_full_total_at_capture=0 queue_full_total_at_drain=1")!=std::string::npos);
    const auto afterGap=logs.size();tick(nullptr,1,1);
    CHECK(logs[afterGap].find("[PANEL FUEL]")!=std::string::npos); // no duplicate gap
    g_panelQueueLock.busy=true;clockMs+=1000;PanelCapture(context,window);
    g_panelQueueLock.busy=false;const auto contentionLogs=logs.size();tick(nullptr,1,1);
    CHECK(logs[contentionLogs].find("reason=render_trylock_contention")!=std::string::npos);
    PanelObserverLoading();const auto oldLogs=logs.size();tick(nullptr,1,1);
    CHECK(g_panelCount==0 && g_panelSamples==0 && logs.size()==oldLogs);
    std::puts("PASS runtime_queue_gap_capture_vs_drain_contention_and_load_discard");
    clockMs+=1000;PanelCapture(context,nullptr);CHECK(g_panelLastMs==0);
    PanelCapture(context,window);CHECK(g_panelCount==1);
    g_panelSamples=239;clockMs+=1000;PanelCapture(context,window);CHECK(g_panelSamples==240);
    clockMs+=1000;PanelCapture(context,window);CHECK(g_panelSamples==240);
    const auto capLogs=logs.size();tick(nullptr,1,1);
    CHECK(logs[capLogs].find("reason=capture_cap cap=240 capture=stopped")!=std::string::npos);
    tick(nullptr,1,1);const auto drainedLogs=logs.size();tick(nullptr,1,1);
    CHECK(logs.size()==drainedLogs);
    std::puts("PASS runtime_inactive_does_not_starve_capture_and_cap");
    PanelObserverLoading();tick(nullptr,1,1);clockMs+=1000;
    throwOnRead=true;PanelCapture(context,window);throwOnRead=false;
    CHECK(g_panelFault && g_panelCount==0);
    const auto faultLogs=logs.size();tick(nullptr,1,1);
    CHECK(logs[faultLogs].find("reason=guarded_read_fault observer=disabled")!=std::string::npos);
    const auto disabledLogs=logs.size();clockMs+=1000;PanelCapture(context,window);tick(nullptr,1,1);
    CHECK(g_panelCount==0 && logs.size()==disabledLogs);
    std::puts("PASS runtime_guarded_fault_disables_and_reports_once");
    g_exeBase[0x90B07C]=1;CHECK(!PanelTargetMatches());g_exeBase[0x90B07C]=0;
    g_exeBase[0x8F3E98]=0;CHECK(!PanelTargetMatches());
    std::puts("PASS runtime_changed_data_refused");

    // Reset fault state and queue caps from earlier tests so diagnostic captures proceed.
    InterlockedExchange(&g_panelFault, 0);
    g_panelSamples = 0;
    g_panelCount = 0;
    g_panelHead = 0;
    g_panelLastMs = 0;
    g_panelDropped = 0;
    g_panelDroppedReported = 0;
    g_panelFirstDropMs = 0;
    g_panelLastDropMs = 0;
    g_panelCapLogged = false;
    g_panelQueueEpoch = 0;

    // ---- Gate diagnostic tests ----
    // Helper: actual address of fixture.root within the module image.
    const auto rootAddr = (uintptr_t)g_exeBase + fixture.root;
    const auto metaAddr = (uintptr_t)g_exeBase + fixture.meta;
    const auto globalAddr = (uintptr_t)g_exeBase + 0x9D54C0;

    // 1. Short image: RVA fits but no room for int32 -> global unavailable, capture still succeeds
    {
        const auto oldSize = g_exeSize;
        g_exeSize = 0x9D54C0;
        PanelCapture(context, window);
        CHECK(!g_panelFault);
        CHECK(g_panelCount == 1);
        auto& rec = g_panelQueue[(g_panelHead + g_panelCount - 1) % 16];
        CHECK(!rec.snapshot.gateGlobalRead);
        tick(nullptr, 1, 1);
        g_exeSize = oldSize;
    }
    std::puts("PASS runtime_diag_short_image_global_unavailable");

    // 2. Short image plus 3 bytes: still not enough for int32 -> global unavailable
    {
        const auto oldSize = g_exeSize;
        g_exeSize = 0x9D54C3;
        PanelCapture(context, window);
        CHECK(!g_panelFault);
        auto& rec = g_panelQueue[(g_panelHead + g_panelCount - 1) % 16];
        CHECK(!rec.snapshot.gateGlobalRead);
        tick(nullptr, 1, 1);
        g_exeSize = oldSize;
    }
    std::puts("PASS runtime_diag_short_image_3bytes_global_unavailable");

    // 3. Bounds logic: image large enough -> globalBoundsValid is true (bounds-only, no PanelRead)
    {
        const auto oldSize = g_exeSize;
        g_exeSize = 0x9D54C4;
        bool gbv = g_exeBase != nullptr &&
            static_cast<uint64_t>(0x9D54C0) <= g_exeSize &&
            sizeof(std::int32_t) <= g_exeSize - 0x9D54C0;
        CHECK(gbv);
        g_exeSize = oldSize;
    }
    std::puts("PASS runtime_diag_exact_size_bounds_valid");

    // 4. Null image base: globalBoundsValid false (bounds-only, no PanelRead)
    {
        const auto oldBase = g_exeBase;
        g_exeBase = nullptr;
        bool gbv = g_exeBase != nullptr &&
            static_cast<uint64_t>(0x9D54C0) <= g_exeSize &&
            sizeof(std::int32_t) <= g_exeSize - 0x9D54C0;
        CHECK(!gbv);
        g_exeBase = oldBase;
    }
    std::puts("PASS runtime_diag_null_image_base_global_unavailable");

    // 5. Exception at root+0x514: per-field guard catches it, capture survives
    {
        const auto oldSize = g_exeSize;
        g_exeSize = 0x9D54C4; // ensure global bounds valid so capture proceeds fully
        const auto beforeFault = g_panelFault;
        throwAtRead = rootAddr + 0x514;
        clockMs += 1000; // advance time so throttle does not block
        PanelCapture(context, window);
        CHECK(g_panelFault == beforeFault);
        CHECK(g_panelCount >= 1);
        auto& rec = g_panelQueue[(g_panelHead + g_panelCount - 1) % 16];
        CHECK(!rec.snapshot.root514Read);   // read attempted, exception caught
        CHECK(rec.snapshot.root4F0Read);     // later read still occurred
        tick(nullptr, 1, 1);
        throwAtRead = 0;
        g_exeSize = oldSize;
    }
    std::puts("PASS runtime_diag_exception_root514_capture_survives");

    // 6. Exception at root+0x4F0: per-field guard catches it, capture survives
    {
        const auto oldSize = g_exeSize;
        g_exeSize = 0x9D54C4;
        const auto beforeFault = g_panelFault;
        throwAtRead = rootAddr + 0x4F0;
        clockMs += 1000; // advance time so throttle does not block
        PanelCapture(context, window);
        CHECK(g_panelFault == beforeFault);
        CHECK(g_panelCount >= 1);
        auto& rec = g_panelQueue[(g_panelHead + g_panelCount - 1) % 16];
        CHECK(rec.snapshot.root514Read);     // earlier read succeeded
        CHECK(!rec.snapshot.root4F0Read);    // read attempted, exception caught
        tick(nullptr, 1, 1);
        throwAtRead = 0;
        g_exeSize = oldSize;
    }
    std::puts("PASS runtime_diag_exception_root4f0_capture_survives");

    // 7. Exception at global RVA: per-field guard catches it, capture survives
    {
        const auto oldSize = g_exeSize;
        g_exeSize = 0x9D54C4; // globalBoundsValid true so global read IS attempted
        const auto beforeFault = g_panelFault;
        throwAtRead = globalAddr;
        clockMs += 1000; // advance time so throttle does not block
        PanelCapture(context, window);
        CHECK(g_panelFault == beforeFault);
        CHECK(g_panelCount >= 1);
        auto& rec = g_panelQueue[(g_panelHead + g_panelCount - 1) % 16];
        CHECK(rec.snapshot.root514Read);     // root read succeeded
        CHECK(!rec.snapshot.gateGlobalRead); // global read attempted, exception caught
        tick(nullptr, 1, 1);
        throwAtRead = 0;
        g_exeSize = oldSize;
    }
    std::puts("PASS runtime_diag_exception_global_capture_survives");

    // 8. Exception at object+0x1794: per-field guard catches it, later reads still occur
    {
        const auto oldSize = g_exeSize;
        g_exeSize = 0x9D54C4;
        const auto beforeFault = g_panelFault;
        throwAtRead = rootAddr + 0x1794;
        clockMs += 1000; // advance time so throttle does not block
        PanelCapture(context, window);
        CHECK(g_panelFault == beforeFault);
        CHECK(g_panelCount >= 1);
        auto& rec = g_panelQueue[(g_panelHead + g_panelCount - 1) % 16];
        CHECK(!rec.snapshot.objects[0].raw1794Read); // read attempted, exception caught
        CHECK(rec.snapshot.objects[0].quantityRead); // mandatory read still happened
        tick(nullptr, 1, 1);
        throwAtRead = 0;
        g_exeSize = oldSize;
    }
    std::puts("PASS runtime_diag_exception_raw1794_capture_survives");

    // 9. Exception at metadata+0x8680: per-field guard catches it, capture survives
    {
        const auto oldSize = g_exeSize;
        g_exeSize = 0x9D54C4;
        const auto beforeFault = g_panelFault;
        throwAtRead = metaAddr + 0x8680;
        clockMs += 1000; // advance time so throttle does not block
        PanelCapture(context, window);
        CHECK(g_panelFault == beforeFault);
        CHECK(g_panelCount >= 1);
        auto& rec = g_panelQueue[(g_panelHead + g_panelCount - 1) % 16];
        CHECK(rec.snapshot.objects[0].quantityRead);   // mandatory read succeeded
        CHECK(!rec.snapshot.objects[0].proto8680Read); // proto8680 read attempted, caught
        tick(nullptr, 1, 1);
        throwAtRead = 0;
        g_exeSize = oldSize;
    }
    std::puts("PASS runtime_diag_exception_proto8680_capture_survives");

    // 10. proto8680 read independently of quantity outcome
    {
        const auto oldSize = g_exeSize;
        g_exeSize = 0x9D54C4;
        clockMs += 1000; // advance time so throttle does not block
        PanelCapture(context, window);
        CHECK(!g_panelFault);
        CHECK(g_panelCount >= 1);
        auto& rec = g_panelQueue[(g_panelHead + g_panelCount - 1) % 16];
        CHECK(rec.snapshot.objects[0].proto8680Read);  // proto8680 read as separate step
        tick(nullptr, 1, 1);
        g_exeSize = oldSize;
    }
    std::puts("PASS runtime_diag_proto8680_with_unavailable_quantity");

    // 11. Queued-value immutability: captured values persist capture→drain
    {
        const auto oldSize = g_exeSize;
        g_exeSize = 0x9D54C4;
        const std::uint8_t known514 = 0xAB;
        std::memcpy(g_exeBase + 0x30514, &known514, sizeof(known514)); // root+0x514, in-bounds
        clockMs += 1000; // advance time so throttle does not block
        const auto beforeLogs = logs.size();
        PanelCapture(context, window);
        tick(nullptr, 1, 1);
        const auto afterLogs = logs.size();
        CHECK(afterLogs > beforeLogs);
        bool fuelLogWithRoot514 = false;
        for (auto i = beforeLogs; i < afterLogs; ++i) {
            if (logs[i].find("[PANEL FUEL]") != std::string::npos &&
                logs[i].find("root514=171") != std::string::npos) { // 0xAB = 171
                fuelLogWithRoot514 = true;
                break;
            }
        }
        CHECK(fuelLogWithRoot514);
        // Restore image
        std::uint8_t zero = 0;
        std::memcpy(g_exeBase + 0x30514, &zero, sizeof(zero));
        g_exeSize = oldSize;
    }
    std::puts("PASS runtime_diag_queued_value_immutability");

    // 12. Load epoch resets queue (load resets preserved)
    {
        clockMs += 1000; // advance time so throttle does not block
        PanelCapture(context, window);
        CHECK(g_panelCount == 1);
        PanelObserverLoading();
        tick(nullptr, 1, 1);
        CHECK(g_panelCount == 0);
    }
    std::puts("PASS runtime_diag_load_epoch_resets_queue");

    std::puts("12 gate-diagnostic scenarios PASS (host shim, not real SEH)");
    std::puts("8 host-shim scenarios PASS (not Windows ABI/SEH or a DLL)");return 0;
}

#endif
