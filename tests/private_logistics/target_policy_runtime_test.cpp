// Host execution of the extracted production DAILY reconciliation body.
// Services/append are stubs: this is not native dispatch/UI/save-load proof.
#include <cassert>
#include <cstdint>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <deque>
#include <limits>
#include <string>
#include <vector>
#include "auto_target_policy.h"
#include "site_permissions.h"
using BYTE = unsigned char;
constexpr unsigned AUTO_ENTITY_MAX=256, AUTO_MEMO_MAX=1024;
constexpr unsigned AUTO_DO_DIRTY=0xD50, AUTO_DO_UI_TIMER=0xFC0;
constexpr unsigned AUTO_TARGET_ENTRY_SIZE=0x18;
constexpr unsigned TSM_BLD_MIN_READABLE=0x100, TSM_BLD_DEMOLISHED=0x10;
struct Entry { alignas(8) BYTE bytes[0x18]{}; alignas(8) BYTE cfg[0x198]{}; };
struct Building {
    alignas(8) BYTE native[0x1100]{};
    uint64_t key=0; int type=5; unsigned caps=3, permission=0;
    bool privateSite=true, readable=true;
    std::vector<void*> tasks;
};
struct AutoEntity { void* building; uint64_t key; int type; int hasStorage; unsigned char isStateStorage, statePermission, targetPresent; };
struct AutoMemo { uint64_t officeKey, targetKey; unsigned nextOffice; unsigned char state; };
struct StateSite { void* building; uint64_t key; unsigned char permission; };
using AutoStateStorageSite = StateSite;
constexpr int AUTO_STORAGE_BUILDING_TYPE=5;
static AutoEntity g_autoEntities[AUTO_ENTITY_MAX];
static StateSite g_autoStateStorageSites[64];
static unsigned g_autoStateStorageCount=0, g_autoMemoCount=0, g_autoLogLines=0;
static AutoMemo g_autoMemo[AUTO_MEMO_MAX];
static int g_autoTargetsEnabled=1, g_autoLastDay=-1, g_autoTargetLogLimit=10000;
static int g_autoResetActiveMemoAfterLoad=0;
static bool g_autoTargetsDryRun=false;
static std::deque<Building> buildings;
static std::deque<Entry> entries;
static std::vector<Building*> world, publicSites;
static std::vector<std::string> logs;
static void* failedTarget=nullptr;
static unsigned appendCalls=0;
static size_t Count() { return world.size(); }
static void* ByIndex(size_t i) { return world[i]; }
static int Private(void* b) { return static_cast<Building*>(b)->privateSite; }
struct Enterprise { size_t (*getEnterpriseCount)(); void* (*getEnterpriseByIndex)(size_t); int (*isBuildingPrivate)(void*); };
static Enterprise service{Count,ByIndex,Private}, *g_enterprise=&service;
static int InterlockedExchange(int* p,int v) { const int old=*p; *p=v; return old; }
static bool ReadablePtr(const void* p,size_t) { return p!=nullptr; }
static bool TsmIsValidGamePtr(void* p) { return p!=nullptr; }
static bool TsmIsDemolished(void*) { return false; }
static void Logf(const char* fmt,...) {
    char line[1024]; va_list a; va_start(a,fmt); vsnprintf(line,sizeof(line),fmt,a); va_end(a); logs.emplace_back(line);
}
static bool AutoBuildingKey(void* b,uint64_t* k,int* t) {
    auto* x=static_cast<Building*>(b); *k=x->key; if(t)*t=x->type; return true;
}
static unsigned AutoBuildingCapabilities(void* b) { return static_cast<Building*>(b)->caps; }
static bool AutoReadBuildingType(void* b,int* t) { *t=static_cast<Building*>(b)->type; return true; }
static unsigned AutoStatePermissionGet(uint64_t k) {
    for(auto* b:publicSites) if(b->key==k)return b->permission;
    return 0;
}
static void AutoProbeStateInfrastructure(int,unsigned) {
    g_autoStateStorageCount=0;
    for(auto* b:publicSites) g_autoStateStorageSites[g_autoStateStorageCount++]={b,b->key,static_cast<unsigned char>(b->permission)};
}
static unsigned indexedCount=0;
static bool AutoBuildPointerIndex(unsigned n) { indexedCount=n; return true; }
static bool AutoBuildOfficeMemoIndex() {
    for(unsigned i=0;i<g_autoMemoCount;++i) {
        g_autoMemo[i].nextOffice=0xFFFFFFFFu;
        for(unsigned j=i+1;j<g_autoMemoCount;++j)
            if(g_autoMemo[j].officeKey==g_autoMemo[i].officeKey && g_autoMemo[j].state==1) {
                g_autoMemo[i].nextOffice=j; break;
            }
    }
    return true;
}
static int AutoOfficeMemoHeadIndex(uint64_t o) {
    for(unsigned i=0;i<g_autoMemoCount;++i) if(g_autoMemo[i].officeKey==o && g_autoMemo[i].state==1)return static_cast<int>(i);
    return -1;
}
static int AutoPointerIndex(void* b) {
    for(unsigned i=0;i<indexedCount;++i) if(g_autoEntities[i].building==b)return static_cast<int>(i);
    return -1;
}
static int AutoMemoFind(uint64_t o,uint64_t t) {
    for(unsigned i=0;i<g_autoMemoCount;++i) if(g_autoMemo[i].officeKey==o && g_autoMemo[i].targetKey==t)return static_cast<int>(i);
    return -1;
}
static int AutoMemoInsert(uint64_t o,uint64_t t) {
    const auto n=g_autoMemoCount++; g_autoMemo[n]={o,t,0xFFFFFFFFu,1}; return static_cast<int>(n);
}
static bool AutoReadTargetVector(void* b,void*** begin,size_t* n) {
    auto* o=static_cast<Building*>(b); if(!o->readable)return false;
    *begin=o->tasks.empty()?nullptr:o->tasks.data(); *n=o->tasks.size(); return true;
}
static bool AutoSetTargetDirections(void* p,unsigned d) {
    auto* e=static_cast<BYTE*>(p); void* cfg=nullptr; memcpy(&cfg,e+16,8);
    const bool a=AutoApplyEntryActivation(e,d);
    const bool b=AutoApplyTaskDirections(cfg,d,10,30); return a||b;
}
static void AutoLogNativeTarget(void*,int,unsigned,bool) {}
static void AutoLogOfficePlanState(void*,uint64_t,int,const char*,bool) {}
static Entry& MakeEntry(Building* t,unsigned d) {
    entries.emplace_back(); auto& e=entries.back(); void* cfg=e.cfg;
    memcpy(e.bytes+8,&t,8); memcpy(e.bytes+16,&cfg,8);
    AutoSetTargetDirections(e.bytes,d); return e;
}
static bool AutoAppendNativeTarget(void* b,void* t,unsigned d) {
    ++appendCalls; auto* o=static_cast<Building*>(b);
    if(t==failedTarget || !AutoCanAppendTarget(o->type,o->tasks.size()))return false;
    o->tasks.push_back(MakeEntry(static_cast<Building*>(t),d).bytes); return true;
}
#include "production_daily.inc"
// Test only the extracted pre-allocation guard, not native function bindings.
static BYTE image[4]{};
static BYTE* g_exeBase=image;
static size_t g_exeSize=sizeof(image);
constexpr unsigned AUTO_RVA_TASKCFG_CTOR=1, AUTO_RVA_VECTOR_PUSH_BACK=1;
#include "production_append_guard.inc"
static void Reset() {
    world.clear(); publicSites.clear(); buildings.clear(); entries.clear(); logs.clear();
    memset(g_autoEntities,0,sizeof(g_autoEntities)); memset(g_autoMemo,0,sizeof(g_autoMemo));
    g_autoMemoCount=0; g_autoStateStorageCount=0; indexedCount=0; appendCalls=0;
    g_autoTargetsDryRun=false; g_autoLastDay=-1; g_autoResetActiveMemoAfterLoad=0; failedTarget=nullptr;
}
static Building* Add(int type=5,unsigned caps=3,bool priv=true) {
    buildings.emplace_back(); auto* b=&buildings.back(); b->key=buildings.size();
    b->type=type; b->caps=caps; b->privateSite=priv;
    if(priv)world.push_back(b); else publicSites.push_back(b); return b;
}
static void Manual(Building* o,Building* t) { o->tasks.push_back(MakeEntry(t,t->caps).bytes); }
static void Tick(int day=1) { AutoTargetsOnDay(day,day-1); }
static unsigned Notification(Building* o) { return o->native[AUTO_DO_DIRTY]; }
static float Timer(Building* o) { float f; memcpy(&f,o->native+AUTO_DO_UI_TIMER,4); return f; }
static void SetTimer(Building* o,float f) { memcpy(o->native+AUTO_DO_UI_TIMER,&f,4); }
static void Limits() {
    for(int type:{43,52,0,5,999}) for(size_t n=0;n<120;++n) for(size_t p=0;p<120;++p) {
        const size_t limit=type==43?20:type==52?5:0;
        assert(AutoCanAppendTarget(type,n,p)==(n+p<limit));
    }
    const size_t m=(std::numeric_limits<size_t>::max)();
    assert(!AutoCanAppendTarget(43,m,1)); assert(!AutoCanAppendTarget(43,1,m));
    assert(!AutoCanAppendTarget(52,m,m)); assert(!AutoCanAppendTarget(-1,0));
}
static void SixthAndDedup() {
    Reset(); auto* o=Add(43,0); for(int i=0;i<5;++i)Manual(o,Add());
    auto* sixth=Add(); auto original=o->tasks; Tick();
    assert(o->tasks.size()==6 && appendCalls==1 && Notification(o)==1 && Timer(o)==3.0f);
    for(size_t i=0;i<5;++i)assert(o->tasks[i]==original[i]);
    void* target=nullptr; memcpy(&target,static_cast<BYTE*>(o->tasks.back())+8,8); assert(target==sixth);
    o->native[AUTO_DO_DIRTY]=0; SetTimer(o,1.75f); Tick(2);
    assert(o->tasks.size()==6 && appendCalls==1 && Notification(o)==0 && Timer(o)==1.75f);
    // Simulated native reconstruction: same targets, new entry/config addresses.
    std::vector<void*> rebuilt;
    for(void* p:o->tasks) { void* t=nullptr; memcpy(&t,static_cast<BYTE*>(p)+8,8); rebuilt.push_back(MakeEntry(static_cast<Building*>(t),3).bytes); }
    o->tasks=rebuilt; g_autoResetActiveMemoAfterLoad=1; Tick(0);
    assert(o->tasks.size()==6 && appendCalls==1);
    // Removed generated target is recreated, not vetoed; existing rows retained.
    o->tasks.pop_back(); Tick(3); assert(o->tasks.size()==6 && appendCalls==2);
}
static void TotalBoundsAndDryRun() {
    Reset(); auto* o=Add(43,0); for(int i=0;i<70;++i)Add(); Tick();
    assert(o->tasks.size()==20 && appendCalls==20); Tick(2);
    assert(o->tasks.size()==20 && appendCalls==20);
    for(int n:{19,20,21,49,101}) {
        Reset(); o=Add(43,0); for(int i=0;i<n;++i)Manual(o,Add());
        auto before=o->tasks; Add(); Tick();
        assert(o->tasks.size()==static_cast<size_t>(n==19?20:n));
        for(size_t i=0;i<before.size();++i)assert(o->tasks[i]==before[i]);
    }
    Reset(); o=Add(43,0); for(int i=0;i<5;++i)Manual(o,Add());
    for(int i=0;i<60;++i) { Add(); }
    auto before=o->tasks; g_autoTargetsDryRun=true; Tick();
    assert(o->tasks==before && appendCalls==0 && Notification(o)==0 && Timer(o)==0.0f);
    bool planned=false; for(const auto& l:logs) if(l.find("type=43 existing=5 planned=15 added=0")!=std::string::npos)planned=true;
    assert(planned);
}
static void RailAndInvalid() {
    Reset(); auto* o=Add(52,0); for(int i=0;i<6;++i)Add(); Tick(); assert(o->tasks.size()==5);
    Reset(); o=Add(52,0); for(int i=0;i<8;++i)Manual(o,Add());
    auto before=o->tasks; Add(); Tick(); assert(o->tasks==before && appendCalls==0);
    Reset(); o=Add(43,0); o->readable=false; Add(); Tick();
    assert(o->tasks.empty() && appendCalls==0 && Notification(o)==0);
    Reset(); o=Add(999,0); Add(); Tick(); assert(o->tasks.empty() && appendCalls==0);
}
static void PermissionsAndCustoms() {
    Reset(); auto* o=Add(43,0); for(int i=0;i<5;++i)Manual(o,Add());
    auto* state=Add(5,3,false); Tick(); assert(o->tasks.size()==5);
    state->permission=1; Tick(2); assert(o->tasks.size()==6);
    auto* e=static_cast<BYTE*>(o->tasks.back()); void* cfg=nullptr; memcpy(&cfg,e+16,8);
    assert(e[0]==1 && static_cast<BYTE*>(cfg)[0]==1 && static_cast<BYTE*>(cfg)[0xC8]==0);
    state->permission=0; Tick(3); assert(o->tasks.size()==6 && e[0]==0);
    // A newly discovered private retail consumer is delivery-only.
    // Public discovery is type-5 storage only; do not fabricate public retail.
    auto* consumer=Add(3,2); Tick(4);
    e=static_cast<BYTE*>(o->tasks.back()); memcpy(&cfg,e+16,8);
    void* observed=nullptr; memcpy(&observed,e+8,8); assert(observed==consumer);
    assert(static_cast<BYTE*>(cfg)[0]==0 && static_cast<BYTE*>(cfg)[0xC8]==1);
    // Customs entry/config bytes are preserved through reconciliation.
    auto* customs=Add(20,0); auto& ce=MakeEntry(customs,0); memset(ce.cfg,0xA5,sizeof(ce.cfg));
    ce.bytes[0]=0x72; o->tasks.push_back(ce.bytes);
    BYTE copy[0x18+0x198]; memcpy(copy,ce.bytes,0x18); memcpy(copy+0x18,ce.cfg,0x198);
    Tick(5); assert(memcmp(copy,ce.bytes,0x18)==0 && memcmp(copy+0x18,ce.cfg,0x198)==0);
}
static void FailedAppendDoesNotSpendBudget() {
    Reset(); auto* o=Add(43,0); for(int i=0;i<19;++i)Manual(o,Add());
    Add(); failedTarget=Add(); Tick(); assert(o->tasks.size()==20 && appendCalls==2);
    // The failed candidate is still omitted at the full total count next day.
    Tick(2); assert(o->tasks.size()==20 && appendCalls==2);
}
static void LiveAppendPreflight() {
    Reset(); auto* o=Add(43,0); for(int i=0;i<19;++i)Manual(o,Add());
    assert(AutoAppendPrefix(o,nullptr,3));
    // A previously admissible planning snapshot must not override live count.
    Manual(o,Add()); assert(!AutoAppendPrefix(o,nullptr,3));
    o->tasks.clear(); o->type=52;
    for(int i=0;i<5;++i) { Manual(o,Add()); }
    assert(!AutoAppendPrefix(o,nullptr,3));
    o->tasks.clear(); o->type=999; assert(!AutoAppendPrefix(o,nullptr,3));
    o->type=43; o->readable=false; assert(!AutoAppendPrefix(o,nullptr,3));
    o->readable=true; g_exeSize=0; assert(!AutoAppendPrefix(o,nullptr,3));
    g_exeSize=sizeof(image); g_exeBase=nullptr; assert(!AutoAppendPrefix(o,nullptr,3));
    g_exeBase=image;
    std::puts("extracted live append preflight PASS");
}
int main() {
    LiveAppendPreflight(); Limits(); SixthAndDedup(); TotalBoundsAndDryRun(); RailAndInvalid();
    PermissionsAndCustoms(); FailedAppendDoesNotSpendBudget();
    std::puts("6 extracted daily-reconciliation groups PASS (host stubs, not native gameplay)");
}
