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
static bool ReadablePtr(const void* p,std::size_t n) {
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
static void Logf(const char* format,...) {
    char line[2048];va_list args;va_start(args,format);std::vsnprintf(line,sizeof(line),format,args);va_end(args);logs.emplace_back(line);
}
static int g_panelObserverEnabled=1,g_billing=0,g_persistenceHandle=1;
static unsigned installed=0,originalCalls=0;
static void Original(void*,void*) { ++originalCalls; }
static bool InstallInlineHook(void*,void*,void** trampoline,const BYTE*,std::size_t n,const char*) {
    CHECK(n==20);*trampoline=(void*)Original;++installed;return true;
}
using Tick=void(*)(void*,float,float);
static Tick tick=nullptr;
static int RegisterTick(Tick fn,void*) { tick=fn;return 1; }
struct ClockApi { int(*registerTick)(Tick,void*); };
static ClockApi api{RegisterTick};
static ClockApi* g_simClock=&api;
#define __fastcall
#define __try try
#define __except(x) catch(...)
#include "panel_fuel_runtime.h"

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
    for(unsigned i=0;i<17;++i) {clockMs+=1000;PanelCapture(context,window);}
    CHECK(g_panelCount==16 && g_panelDropped==1);
    g_panelQueueLock.busy=true;clockMs+=1000;PanelCapture(context,window);CHECK(g_panelCount==16);
    g_panelQueueLock.busy=false;
    PanelObserverLoading();const auto oldLogs=logs.size();tick(nullptr,1,1);
    CHECK(g_panelCount==0 && g_panelSamples==0 && logs.size()==oldLogs);
    std::puts("PASS runtime_queue_bound_trylock_and_load_discard");
    clockMs+=1000;PanelCapture(context,nullptr);CHECK(g_panelLastMs==0);
    PanelCapture(context,window);CHECK(g_panelCount==1);
    g_panelSamples=239;clockMs+=1000;PanelCapture(context,window);CHECK(g_panelSamples==240);
    clockMs+=1000;PanelCapture(context,window);CHECK(g_panelSamples==240);
    std::puts("PASS runtime_inactive_does_not_starve_capture_and_cap");
    g_exeBase[0x90B07C]=1;CHECK(!PanelTargetMatches());g_exeBase[0x90B07C]=0;
    g_exeBase[0x8F3E98]=0;CHECK(!PanelTargetMatches());
    std::puts("PASS runtime_changed_data_refused");
    std::puts("5 host-shim scenarios PASS (not Windows ABI/SEH or a DLL)");return 0;
}
