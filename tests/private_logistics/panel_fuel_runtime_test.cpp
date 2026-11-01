// Native host shim exercises the production queue/control code. NOT Windows ABI,
// SEH, patch execution, real threads, or native game lifecycle validation.
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <string>
#include <cstdarg>
#include "../../src/resource_pricing_api.h"
static const TsmResourcePricingApi* g_pricing=nullptr;
static const TsmCurrencyApi* g_currency=nullptr;
static char gFuelResource[64]="fuel";
#define CHECK(x) do { if (!(x)) { std::fprintf(stderr,"line %d: %s\n",__LINE__,#x); std::exit(1); } } while (0)

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
static std::vector<BYTE> image(0x9D54C4); // backed storage through the global int32
static BYTE* g_exeBase=image.data();
static std::size_t g_exeSize=image.size();
static bool throwOnRead=false;
static uintptr_t throwAtRead=0, denyAtRead=0;
static std::vector<uintptr_t> attemptedReads;
static bool ReadablePtr(const void* p,std::size_t n) {
    attemptedReads.push_back((uintptr_t)p);
    if (denyAtRead && (uintptr_t)p==denyAtRead) return false;
    if(throwOnRead || (throwAtRead && (uintptr_t)p==throwAtRead)) throw 1;
    const auto a=(uintptr_t)p,b=(uintptr_t)image.data(); // physical backing, not injected module base
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
// Compile production runtime BEFORE the reader fixture's global namespace import.
#include "panel_fuel_runtime.h"
#define main ReaderHarnessMain
#include "panel_fuel_observer_test.cpp"
#undef main

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
    InterlockedExchange(&g_panelFault,0);
    PanelObserverLoading();tick(nullptr,1,1);
    g_exeBase[0x8F3E98]=0x25;
    const auto rootAddr=ptr(fixture.root),metaAddr=ptr(fixture.meta);
    const auto globalAddr=ptr(0x9D54C0);
    const auto wasRead=[&](uintptr_t address) {
        for(auto attempted:attemptedReads) if(attempted==address) return true;
        return false;
    };
    const auto fresh=[&]() {
        CHECK(g_panelCount==0);
        const auto samples=g_panelSamples;
        clockMs+=1000;attemptedReads.clear();PanelCapture(context,window);
        CHECK(!g_panelFault && g_panelCount==1 && g_panelSamples==samples+1);
        return g_panelQueue[g_panelHead].snapshot;
    };
    for(std::size_t size:{std::size_t(0),std::size_t(0x9D54BF),
                         std::size_t(0x9D54C0),std::size_t(0x9D54C3)}) {
        g_exeSize=size;const auto s=fresh();
        CHECK(s.aggregateValid && s.root514Read && !s.gateGlobalRead);
        CHECK(!wasRead(globalAddr));tick(nullptr,1,1);
    }
    g_exeSize=image.size();
    std::puts("PASS runtime_diag_short_images_never_read_global");
    for(std::int32_t value:{-7,0,2,42}) {
        std::memcpy((void*)globalAddr,&value,sizeof(value));
        const auto s=fresh();
        CHECK(s.gateGlobalRead && s.gateGlobal==value && wasRead(globalAddr));
        tick(nullptr,1,1);
    }
    std::puts("PASS runtime_diag_exact_size_relocated_signed_global");
    g_exeBase=nullptr;
    {const auto s=fresh();CHECK(s.aggregateValid && !s.gateGlobalRead && !wasRead(globalAddr));}
    tick(nullptr,1,1);g_exeBase=image.data();
    std::puts("PASS runtime_diag_null_image_preserves_capture");
    for(std::uintptr_t base:{UINTPTR_MAX-0x9D54C0+1,UINTPTR_MAX-0x9D54C0-1}) {
        g_exeBase=reinterpret_cast<BYTE*>(base);const auto s=fresh();
        CHECK(s.aggregateValid && !s.gateGlobalRead);
        // No fabricated global address reaches the memory reader.
        for(auto address:attemptedReads)
            CHECK(address>=(uintptr_t)image.data() && address<(uintptr_t)image.data()+image.size());
        tick(nullptr,1,1);
    }
    g_exeBase=image.data();
    std::puts("PASS runtime_diag_wrapping_base_or_read_end_never_read");
    const uintptr_t faults[]={rootAddr+0x514,rootAddr+0x4F0,globalAddr,
                              rootAddr+0x1794,metaAddr+0x8680};
    const char* names[]={"root514","root4f0","global","raw1794","proto8680"};
    for(unsigned i=0;i<5;++i) {
        throwAtRead=faults[i];const auto s=fresh();
        CHECK(wasRead(faults[i]) && s.aggregateValid && s.objects[0].quantityRead);
        CHECK(s.root514Read==(i!=0) && s.root4F0Read==(i!=1));
        CHECK(s.gateGlobalRead==(i!=2) && s.objects[0].raw1794Read==(i!=3));
        CHECK(s.objects[0].proto8680Read==(i!=4));
        // Every later independent diagnostic was actually attempted.
        for(unsigned j=i+1;j<5;++j) CHECK(wasRead(faults[j]));
        throwAtRead=0;tick(nullptr,1,1);
        std::printf("PASS runtime_diag_exception_%s_capture_survives\n",names[i]);
    }
    for(auto denied:{rootAddr+0x5F0,metaAddr+0x8684}) {
        denyAtRead=denied;const auto s=fresh();
        CHECK(wasRead(denied) && !s.aggregateValid && !s.objects[0].quantityRead);
        CHECK(s.objects[0].metadata==metaAddr && s.objects[0].proto8680Read);
        CHECK(wasRead(metaAddr+0x8680) && s.objects[0].raw1794Read);
        denyAtRead=0;tick(nullptr,1,1);
    }
    std::puts("PASS runtime_diag_proto8680_with_unavailable_quantity_or_capacity");
    {
        std::uint8_t flag=0xAB,proto=1;std::uint32_t raw=17;std::int32_t global=-7;
        std::memcpy((void*)(rootAddr+0x514),&flag,sizeof(flag));
        std::memcpy((void*)(metaAddr+0x8680),&proto,sizeof(proto));
        std::memcpy((void*)(rootAddr+0x1794),&raw,sizeof(raw));
        std::memcpy((void*)globalAddr,&global,sizeof(global));
        const auto s=fresh();CHECK(s.root514==0xAB && s.gateGlobal==-7);
        const auto beforeLogs=logs.size();
        flag=0;proto=0;raw=29;global=42; // mutate AFTER enqueue, BEFORE drain
        std::memcpy((void*)(rootAddr+0x514),&flag,sizeof(flag));
        std::memcpy((void*)(metaAddr+0x8680),&proto,sizeof(proto));
        std::memcpy((void*)(rootAddr+0x1794),&raw,sizeof(raw));
        std::memcpy((void*)globalAddr,&global,sizeof(global));
        const auto& queued=g_panelQueue[g_panelHead].snapshot;
        CHECK(queued.root514==0xAB && queued.objects[0].raw1794==17);
        CHECK(queued.objects[0].proto8680==1 && queued.gateGlobal==-7);
        tick(nullptr,1,1);
        CHECK(logs.size()==beforeLogs+2);
        CHECK(logs[beforeLogs].find("root514=171 ")!=std::string::npos);
        CHECK(logs[beforeLogs].find("gate_global=-7 ")!=std::string::npos);
        CHECK(logs[beforeLogs+1].find("raw1794=17 ")!=std::string::npos);
        CHECK(logs[beforeLogs+1].find("proto8680=1")!=std::string::npos);
    }
    std::puts("PASS runtime_diag_queued_values_immutable_after_source_mutation");
    {
        const auto child=Address(0x90000),childMeta=Address(0xA0000);
        put(fixture.root+0x3A0,ptr(fixture.links));put(fixture.root+0x3A8,ptr(fixture.links+8));
        put(fixture.links,ptr(child));put(child+0x1708,ptr(childMeta));
        const float raw=0.1f,capacity=0.2f;
        std::memcpy((void*)ptr(child+0x5F0),&raw,sizeof(raw));
        std::memcpy((void*)ptr(childMeta+0x8684),&capacity,sizeof(capacity));
        for(std::uint8_t flag:{0,1}) {
            const std::uint32_t state=flag ? 0x80000000u : 0u;
            std::memcpy((void*)ptr(childMeta+0x8680),&flag,sizeof(flag));
            std::memcpy((void*)ptr(child+0x1794),&state,sizeof(state));
            const auto s=fresh();CHECK(s.aggregateValid && s.count==2);
            CHECK(s.objects[0].proto8680Read && s.objects[1].proto8680Read);
            CHECK(s.objects[1].proto8680==flag && s.objects[1].raw1794==state);
            CHECK(s.objects[1].raw1794Read);tick(nullptr,1,1);
        }
        throwAtRead=ptr(child+0x1794);const auto s=fresh();
        CHECK(wasRead(throwAtRead) && !s.objects[1].raw1794Read);
        CHECK(s.objects[1].quantityRead && s.objects[1].proto8680Read);
        CHECK(s.objects[0].raw1794Read && s.aggregateValid);
        throwAtRead=0;tick(nullptr,1,1);
        put(fixture.root+0x3A0,0);put(fixture.root+0x3A8,0);
    }
    std::puts("PASS runtime_diag_linked_zero_nonzero_and_independent_fault");
    {
        (void)fresh();const auto oldEpoch=g_panelQueue[g_panelHead].epoch;
        const auto beforeLogs=logs.size();PanelObserverLoading();tick(nullptr,1,1);
        CHECK(g_panelCount==0);
        for(auto i=beforeLogs;i<logs.size();++i)
            CHECK(logs[i].find("[PANEL FUEL]")==std::string::npos);
        (void)fresh();CHECK(g_panelQueue[g_panelHead].epoch!=oldEpoch);
        CHECK(g_panelQueue[g_panelHead].sequence==1);tick(nullptr,1,1);
    }
    std::puts("PASS runtime_diag_load_epoch_discards_stale_values");
    std::puts("13 gate-diagnostic scenarios PASS (host shim, not real SEH)");
    std::puts("8 host-shim scenarios PASS (not Windows ABI/SEH or a DLL)");return 0;
}

#endif
