// Production core, codec, native adapter and runtime; host shims, NOT game proof.
#define FLEET_RUNTIME_TEST_NO_MAIN
#include "fleet_fuel_runtime_test.cpp"
#include "../../src/persistence_api.h"
#include <memory>
static CRITICAL_SECTION g_lock;
static void EnterCriticalSection(CRITICAL_SECTION* p) {CHECK(TryEnterCriticalSection(p));}
using DWORD=unsigned long;
constexpr DWORD INVALID_FILE_ATTRIBUTES=0xFFFFFFFF,FILE_ATTRIBUTE_DIRECTORY=16;
constexpr DWORD ERROR_FILE_NOT_FOUND=2,ERROR_PATH_NOT_FOUND=3;
constexpr unsigned MAX_PATH=260;
static int fileState=0,identityReads=0;
static DWORD GetFileAttributesA(const char*) {return fileState==1?0:INVALID_FILE_ATTRIBUTES;}
static DWORD GetLastError() {return fileState==0?ERROR_FILE_NOT_FOUND:5;}
struct TsmBuildingSpan {void** begin=nullptr;void** end=nullptr;size_t count=0;};
static std::vector<void*> world;
static BYTE* TsmGameObject(BYTE* p) {return p;}
static bool TsmGetBuildings(BYTE*,TsmBuildingSpan* out) {
    if(world.empty()) return false;
    *out={world.data(),world.data()+world.size(),world.size()};return true;
}
// World vector must be in readable backing, as in the actual adapter.
static void MakeWorld() {
    world={(void*)Ptr(0x20000),(void*)Ptr(0xB0000)};
}
static float positionX=10,positionZ=20;
static bool AutoReadBuildingType(void*,int* out) {++identityReads;*out=12;return true;}
static bool AutoBuildingPosition(void*,float* x,float* z) {*x=positionX;*z=positionZ;return true;}
static const char* ObjectName(void*) {CHECK(!g_lock.busy);return (const char*)(g_exeBase+0x80000);}
// Allow this shim's vector backing without weakening production ReadablePtr.
static bool CostReadable(const void* p,size_t n) {
    const auto address=(uintptr_t)p,base=(uintptr_t)world.data();
    if(address>=base && address-base<=world.size()*sizeof(void*) && n<=world.size()*sizeof(void*)-(address-base)) return true;
    return ::ReadablePtr(p,n);
}
#define ReadablePtr CostReadable
#include "dry_assignment_native.h"
#undef ReadablePtr
static TsmPersistenceApi persistence{};
static const TsmPersistenceApi* g_persistence=&persistence;
static std::vector<unsigned char> disk;
static unsigned writes=0;static bool failWrite=false,failRead=false,failRegister=false;
static TsmSaveCallback savedCallback=nullptr;
static int RegisterCosts(const char*,TsmSaveCallback cb,void*) {savedCallback=cb;return failRegister?0:1;}
static int WriteCosts(const char*,const char*,const void* p,size_t n) {
    CHECK(!g_lock.busy);if(failWrite) return 0;
    const auto* bytes=(const unsigned char*)p;disk.assign(bytes,bytes+n);fileState=1;++writes;return 1;
}
static void* ReadCosts(const char*,const char*,size_t* n) {
    CHECK(!g_lock.busy);if(failRead || disk.empty()) return nullptr;
    auto* p=std::malloc(disk.size());CHECK(p);std::memcpy(p,disk.data(),disk.size());*n=disk.size();return p;
}
#include "b2b_settlement.h"
#include <unordered_map>
static int g_dryRun=1,g_allowBuyerOverdraft=1,g_counterpartySettlement=1;
static double g_buyerOverdraftLimit=5000,g_b2bTripFuelSum=0,g_b2bStateNetSum=0;
static unsigned g_b2bSettleCount=0,g_skipBuyerInsolvent=0,g_skipTreasury=0;
static std::unordered_map<uintptr_t,double> capitals;
static bool TsmIsDecayed(void*) {return false;}
static bool AutoBuildingKey(void* b,std::uint64_t* key,int*) {
    if(!b || g_lock.busy) return false;
    *key=(std::uint64_t)(uintptr_t)b;
    return true;
}
static bool IsPrivate(void* b) {return b!=nullptr;}
static uintptr_t unfinishedBuilding=0;
static bool AutoBuildingFinished(void* b) {return b!=nullptr && (uintptr_t)b!=unfinishedBuilding;}
static wrsre::b2b::FundsTransferCallbacks MakeTransferCallbacks(unsigned* insolvent,unsigned* treasury) {
    wrsre::b2b::FundsTransferCallbacks cb{};
    cb.isLivePrivate=[](void*,void* b){return b!=nullptr;};
    cb.getCapital=[](void*,void* b){return capitals[(uintptr_t)b];};
    cb.modifyCapital=[](void*,void* b,double d,int){capitals[(uintptr_t)b]+=d;};
    cb.getTreasury=[](void*,int){return 1e12;};
    cb.moveTreasury=[](void*,double,const char*){};
    cb.checkSellerKey=[](void*,void* b,std::uint64_t key){return key==(std::uint64_t)(uintptr_t)b;};
    cb.allowBuyerOverdraft=g_allowBuyerOverdraft!=0;cb.buyerOverdraftLimit=g_buyerOverdraftLimit;cb.minPayment=.01;
    cb.dryRun=g_dryRun!=0 || g_billing==0;cb.skipBuyerInsolvent=insolvent;cb.skipTreasury=treasury;
    return cb;
}
static void TrackCarrier(void*) {}
static void NoteCarrierIncome(void*,double) {}
static void BuildingName(void* b,char* out,size_t n) {
    if(!b) {std::snprintf(out,n,"-");return;}
    std::snprintf(out,n,"%s",ObjectName(b));
}
#include "dry_assignment_runtime.h"
static void PrepareDry() {
    g_fleetCostEvent=nullptr;g_fleetCostSample=nullptr;PrepareRuntime();
    InitializeCriticalSection(&g_lock);g_panelEpoch=2;g_fleetStarted=true;
    g_dryBook.count=0;g_dryBook.next=1;g_dryReady=false;g_dryCapacityWarned=false;
    for(auto& t:g_dryTokens) t=DryToken{};
    for(auto& t:g_assignmentTokens) t=AssignmentToken{};
    for(auto& r:g_assignmentReceipts) r=AssignmentReceipt{};
    g_assignmentReceiptHead=0;g_b2bSettleCount=0;g_b2bTripFuelSum=0;g_b2bStateNetSum=0;
    g_dryRun=1;g_counterpartySettlement=1;capitals.clear();capitals[Ptr(0x20000)]=10000;capitals[Ptr(0x50000)]=0;
    fileState=0;disk.clear();writes=0;failWrite=failRead=failRegister=false;
    identityReads=0;positionX=10;positionZ=20;MakeWorld();
    std::memcpy(g_exeBase+0x80000,"factory_asset",14);enterprises.getBuildingObjectName=ObjectName;
    persistence={};persistence.structSize=sizeof(persistence);persistence.registerHandler=RegisterCosts;
    persistence.writeAtomic=WriteCosts;persistence.readFile=ReadCosts;
    DryCostStart();CHECK(g_fleetCostEvent && g_fleetCostSample && savedCallback);
}
static sch::Snapshot Task() {
    auto s=Goods();for(unsigned i=0;i<s.count;++i) s.rows[i].target=Ptr(s.rows[i].target);return s;
}
static fleet::Vehicle Truck() {auto v=Vehicle(Ptr(0x30000));v.home=Ptr(0x50000);return v;}
static void Feed(fleet::Vehicle& v,const sch::Snapshot& s,float raw,unsigned ms) {
    FleetEvents(v.Observe(s,Fuel(v.root,raw),2,ms));
    if(v.ledger.Active()) g_fleetCostSample(v.ledger.Current(),true);
}
static PanelEndpointPrice Quote200() {PanelEndpointPrice p{};p.available=true;p.perTonne=200;p.currency=0;return p;}
static void ReadyQuote() {g_fleetPrice=Quote200();g_fleetPriceCached=true;g_fleetPriceMs=1000;}
static void frozen_quote_cancellation_identity_and_duplicate_close() {
    PrepareDry();ReadyQuote();auto v=Truck();auto empty=Task();empty.count=0;
    Feed(v,empty,.9f,1000);Feed(v,Task(),.8f,2000);
    const auto key=dry::TargetKey(g_dryBook.rows[0].target);CHECK(key);
    Feed(v,Task(),.6f,3000);Feed(v,Task(),.9f,4000);
    auto office=Task();office.count=1;office.rows[0]=Task().rows[2];
    world.clear();const int reads=identityReads;
    const auto closed=v.Observe(office,Fuel(v.root,.7f),2,5000);
    FleetEvents(closed);CHECK(g_dryBook.count==1 && identityReads==reads);
    const auto frozen=g_dryBook.rows[0];CHECK(frozen.stage==dry::Stage::Closed);
    CHECK(frozen.reason==dry::Reason::OfficeOnly && Near(frozen.decrease,.4));
    CHECK(Near(frozen.amount/200,.4) && frozen.available && dry::TargetKey(frozen.target)==key);
    g_fleetPrice.perTonne=999;FleetEvents(closed);Feed(v,office,.3f,6000);
    CHECK(g_dryBook.rows[0].amount==frozen.amount && g_dryBook.count==1);
    CHECK(logs.back().find("billing=off")!=std::string::npos);
}
static void replacement_separates_original_and_new_target() {
    PrepareDry();ReadyQuote();auto v=Truck();Feed(v,Task(),.8f,1000);Feed(v,Task(),.7f,2000);
    const auto original=dry::TargetKey(g_dryBook.rows[0].target);
    auto next=Task();next.rows[1].target=Ptr(0xB0000);positionX=30;
    Feed(v,next,.6f,3000);
    CHECK(g_dryBook.count==2 && Near(g_dryBook.rows[0].decrease,.2));
    CHECK(g_dryBook.rows[0].reason==dry::Reason::Replaced);
    CHECK(dry::TargetKey(g_dryBook.rows[0].target)==original);
    CHECK(dry::TargetKey(g_dryBook.rows[1].target)!=original && g_dryBook.rows[1].decrease==0);
}
static void unresolved_and_invalid_identities_are_not_guessed() {
    PrepareDry();ReadyQuote();auto v=Truck();world.clear();
    Feed(v,Task(),.8f,1000);MakeWorld();Feed(v,Task(),.7f,2000);
    auto empty=Task();empty.count=0;Feed(v,empty,.6f,3000);
    CHECK(!g_dryBook.rows[0].target.valid && g_dryBook.rows[0].available);
    CHECK(logs[logs.size()-2].find("would_owe_available=0")!=std::string::npos);
    dry::Identity t{};const int reads=identityReads;
    CHECK(!DryIdentitySnapshot(Ptr(0xDEAD),t) && identityReads==reads);
    world.push_back(world[0]);CHECK(!DryIdentitySnapshot(Ptr(0x20000),t));
    MakeWorld();std::memset(g_exeBase+0x80000,'x',128);
    CHECK(!DryIdentitySnapshot(Ptr(0x20000),t));
    CHECK(!dry::MakeIdentity("x",12,std::numeric_limits<float>::quiet_NaN(),0).valid);
}
static void saved_closed_quote_is_stable_and_active_snapshot_is_unpriced() {
    PrepareDry();ReadyQuote();auto v=Truck();Feed(v,Task(),.8f,1000);Feed(v,Task(),.6f,2000);
    savedCallback(TSM_SAVE_WRITING,"save-A",nullptr);
    CHECK(writes==1 && g_dryBook.rows[0].stage==dry::Stage::Open);
    const auto saved=disk;auto loaded=std::make_unique<dry::Book>();
    CHECK(dry::Decode(disk.data(),disk.size(),*loaded));
    CHECK(loaded->rows[0].stage==dry::Stage::Interrupted && !loaded->rows[0].available);
    CHECK(Near(loaded->rows[0].decrease,.2));
    auto empty=Task();empty.count=0;Feed(v,empty,.5f,3000);
    const auto frozen=g_dryBook.rows[0];savedCallback(TSM_SAVE_WRITING,"save-A",nullptr);
    InterlockedIncrement(&g_panelEpoch);savedCallback(TSM_SAVE_LOADING,"save-A",nullptr);
    CHECK(g_dryReady && g_dryBook.count==1 && g_dryBook.rows[0].amount==frozen.amount);
    CHECK(!g_dryTokens[0].root && g_dryBook.next==2);
    disk=saved;savedCallback(TSM_SAVE_LOADING,"save-A",nullptr);
    CHECK(g_dryBook.rows[0].stage==dry::Stage::Interrupted && !g_dryBook.rows[0].available);
    CHECK(g_dryBook.rows[0].target.valid && g_dryBook.Begin({})==1);
}
static void corrupt_unreadable_or_failed_writes_preserve_existing_file() {
    PrepareDry();ReadyQuote();auto v=Truck();Feed(v,Task(),.8f,1000);Feed(v,Task(),.7f,2000);
    savedCallback(TSM_SAVE_WRITING,"A",nullptr);disk.back()^=1;const auto broken=disk;
    InterlockedIncrement(&g_panelEpoch);savedCallback(TSM_SAVE_LOADING,"A",nullptr);
    CHECK(!g_dryReady && g_dryBook.count==0);
    savedCallback(TSM_SAVE_WRITING,"A",nullptr);CHECK(disk==broken && writes==1);
    failRead=true;savedCallback(TSM_SAVE_LOADING,"A",nullptr);CHECK(!g_dryReady);
    fileState=-1;savedCallback(TSM_SAVE_LOADING,"A",nullptr);CHECK(!g_dryReady);
    fileState=0;savedCallback(TSM_SAVE_LOADING,"new-save",nullptr);CHECK(g_dryReady && !g_dryBook.count);
    failWrite=true;savedCallback(TSM_SAVE_WRITING,"new-save",nullptr);CHECK(disk==broken && writes==1);
}
static void old_epoch_events_cannot_cross_saved_worlds_or_billing_gate() {
    PrepareDry();ReadyQuote();auto v=Truck();Feed(v,Task(),.8f,1000);
    const auto c=v.ledger.Current();savedCallback(TSM_SAVE_WRITING,"A",nullptr);
    InterlockedIncrement(&g_panelEpoch);savedCallback(TSM_SAVE_LOADING,"A",nullptr);
    auto e=fleet::a::Event{};e.kind=fleet::a::Kind::Closed;e.candidate=c;e.reason="observed_removed";
    e.candidate.sampledDecrease=.5;e.candidate.sampledPairs=10;
    DryCostEvent(e,Quote200(),true);CHECK(g_dryBook.count==1 && !g_dryBook.rows[0].available);
    g_billing=1;e.candidate.epoch=(unsigned long long)g_panelEpoch;e.kind=fleet::a::Kind::Opened;
    DryCostEvent(e,Quote200(),true);CHECK(g_dryBook.count==1);g_billing=0;
}
static void codec_rejects_malformed_semantic_values_and_never_evicts_records() {
    auto b=std::make_unique<dry::Book>(),copy=std::make_unique<dry::Book>();
    const auto id=dry::MakeIdentity("asset",12,10,20);const auto i=b->Begin(id);
    CHECK(b->Update(i,.25,.4,3,0) && b->Close(i,{true,200,1},dry::Reason::Removed));
    std::vector<unsigned char> bytes(dry::MaxBytes);auto n=dry::Encode(*b,bytes.data(),bytes.size());
    CHECK(n==dry::HeaderBytes+dry::RowBytes && dry::Decode(bytes.data(),n,*copy));
    CHECK(copy->rows[0].amount==50 && copy->rows[0].currency==1);
    CHECK(!dry::Decode(bytes.data(),n-1,*copy));
    const auto validBytes=bytes;
    auto rejects=[&](unsigned offset,std::uint64_t value,unsigned width) {
        bytes=validBytes;dry::Writer field{bytes.data()+offset};field.Int(value,width);
        dry::Writer hash{bytes.data()+20};hash.Int(dry::Checksum(bytes.data(),n),8);
        CHECK(!dry::Decode(bytes.data(),n,*copy)); // Valid checksum, invalid semantics.
    };
    rejects(28+8,1,1); // Persisted Open is forbidden.
    rejects(28+9,0,1); // Closed needs an actual boundary reason.
    rejects(28+10,8,4);rejects(28+14,2,1); // Flags and target boolean.
    rejects(28+19,0x7FC00000,4); // NaN position.
    rejects(28+155,0xBFF0000000000000ull,8); // Negative quantity.
    rejects(28+175,2,1);rejects(28+176,0x7FF0000000000000ull,8);
    rejects(28+184,0,8);rejects(28+192,2,4); // Inconsistent amount/currency.
    rejects(12,1,8);rejects(28,0,8); // Bad next ID or zero record ID.
    bytes=validBytes;bytes[8]=255;CHECK(!dry::Decode(bytes.data(),n,*copy));
    b->rows[0].amount=49;CHECK(!dry::Encode(*b,bytes.data(),bytes.size()));
    b->rows[0].amount=50;b->rows[0].decrease=std::numeric_limits<double>::quiet_NaN();
    CHECK(!dry::Encode(*b,bytes.data(),bytes.size()));
    b->count=0;b->next=1;
    for(unsigned j=0;j<dry::Limit;++j) CHECK(b->Begin(id)==j);
    CHECK(b->Begin(id)==dry::Limit && b->rows[0].id==1);
    CHECK(dry::Encode(*b,bytes.data(),bytes.size())==dry::MaxBytes);
    CHECK(dry::Decode(bytes.data(),dry::MaxBytes,*copy) && copy->count==dry::Limit);
    CHECK(copy->rows[0].id==1 && copy->rows[dry::Limit-1].id==dry::Limit);
}
static void InstallTask(const sch::Snapshot& s) {
    Put(0x30000+0x680,Ptr(0x110000));Put(0x30000+0x688,Ptr(0x110000+s.count*8));
    Put(0x30000+0x638,Ptr(0x120000));
    for(unsigned i=0;i<s.count;++i) {
        Put(0x110000+i*8,s.rows[i].target);const auto cfg=0x120000+i*sch::ConfigStride;
        Put(cfg,s.rows[i].flag00);Put(cfg+4,s.rows[i].value04);
        Put(cfg+0xC8,s.rows[i].flagC8);Put(cfg+0xCC,s.rows[i].valueCC);
    }
}
static void actual_tick_quiet_samples_save_and_long_pause_are_accounted() {
    PrepareDry();SetupFleetPrice();FleetObserverTick(nullptr,1,1);
    InstallTask(Task());clockMs+=1000;Put(0x30000+0x5F0,.5f);FleetObserverTick(nullptr,1,1);
    const int reads=identityReads;
    clockMs+=1000;Put(0x30000+0x5F0,.4f);FleetObserverTick(nullptr,1,1);
    CHECK(g_dryBook.count==1 && Near(g_dryBook.rows[0].decrease,.1) && identityReads==reads);
    savedCallback(TSM_SAVE_WRITING,"A",nullptr);
    auto loaded=std::make_unique<dry::Book>();CHECK(dry::Decode(disk.data(),disk.size(),*loaded));
    CHECK(Near(loaded->rows[0].decrease,.1) && loaded->rows[0].stage==dry::Stage::Interrupted);
    clockMs+=60000;Put(0x30000+0x5F0,.3f);FleetObserverTick(nullptr,1,1);
    auto empty=Task();empty.count=0;InstallTask(empty);
    clockMs+=1000;Put(0x30000+0x5F0,.2f);FleetObserverTick(nullptr,1,1);
    CHECK(g_dryBook.rows[0].stage==dry::Stage::Closed && Near(g_dryBook.rows[0].amount/200,.3));
    CHECK(!g_fleetVehicles[0].discontinuities);
}
static void unavailable_price_is_frozen_unavailable_and_zero_is_real_zero() {
    PrepareDry();auto v=Truck();Feed(v,Task(),.8f,1000);Feed(v,Task(),.7f,2000);
    auto empty=Task();empty.count=0;Feed(v,empty,.6f,3000);
    CHECK(!g_dryBook.rows[0].available && g_dryBook.rows[0].price==0);
    ReadyQuote();Feed(v,Task(),.5f,4000);Feed(v,empty,.5f,5000);
    CHECK(g_dryBook.rows[1].available && g_dryBook.rows[1].amount==0);
    CHECK(!g_dryBook.rows[0].available); // Never retrofit a later quote.
}
static void snapshot_key_is_pointer_independent_and_guarded() {
    PrepareDry();dry::Identity first{},second{};
    CHECK(DryIdentitySnapshot(Ptr(0x20000),first));
    world[0]=(void*)Ptr(0x21000);CHECK(DryIdentitySnapshot(Ptr(0x21000),second));
    CHECK(dry::TargetKey(first)==dry::TargetKey(second));
    positionX+=.01f;CHECK(DryIdentitySnapshot(Ptr(0x21000),second));
    CHECK(dry::TargetKey(first)!=dry::TargetKey(second));
    denyAtRead=Ptr(0x80000);CHECK(!DryIdentitySnapshot(Ptr(0x21000),second));denyAtRead=0;
    throwAtRead=Ptr(0x80000);CHECK(!DryIdentitySnapshot(Ptr(0x21000),second));throwAtRead=0;
}
static void persistence_registration_and_capacity_fail_closed() {
    PrepareDry();g_fleetCostEvent=nullptr;g_fleetCostSample=nullptr;
    persistence.structSize=offsetof(TsmPersistenceApi,readFile);DryCostStart();CHECK(!g_fleetCostEvent);
    persistence.structSize=sizeof(persistence);failRegister=true;DryCostStart();CHECK(!g_dryReady && !g_fleetCostEvent);
    failRegister=false;DryCostStart();CHECK(g_dryReady && g_fleetCostEvent);
    g_dryBook.count=dry::Limit;g_dryBook.next=dry::Limit+1;
    auto v=Truck();Feed(v,Task(),.8f,1000);CHECK(g_dryBook.count==dry::Limit && g_dryCapacityWarned);
    char longDirectory[MAX_PATH+1];std::memset(longDirectory,'x',MAX_PATH);longDirectory[MAX_PATH]=0;
    CHECK(DryFileState(longDirectory,DryCostFile)==-1);
}
static void simultaneous_vehicles_keep_independent_frozen_records() {
    PrepareDry();ReadyQuote();auto x=Truck(),y=Vehicle(Ptr(0x90000));y.home=x.home;
    Feed(x,Task(),.8f,1000);Feed(y,Task(),.6f,1000);
    Feed(x,Task(),.7f,2000);Feed(y,Task(),.4f,2000);
    auto empty=Task();empty.count=0;Feed(x,empty,.6f,3000);
    CHECK(g_dryBook.rows[0].stage==dry::Stage::Closed && g_dryBook.rows[1].stage==dry::Stage::Open);
    Feed(y,empty,.3f,3000);CHECK(Near(g_dryBook.rows[0].amount/200,.2));
    CHECK(Near(g_dryBook.rows[1].amount/200,.3) && g_dryBook.rows[0].id!=g_dryBook.rows[1].id);
}
static void assignment_close_charges_original_buyer_once_only_when_complete() {
    PrepareDry();ReadyQuote();g_billing=1;g_dryRun=0;
    auto v=Truck();auto empty=Task();empty.count=0;
    Feed(v,empty,.9f,1000);Feed(v,Task(),.8f,2000);
    Feed(v,Task(),.6f,3000);
    const auto closed=v.Observe(empty,Fuel(v.root,.5f),2,4000);FleetEvents(closed);
    CHECK(std::abs(capitals[Ptr(0x20000)]-9940)<.001);
    CHECK(std::abs(capitals[Ptr(0x50000)]-60)<.001);
    CHECK(std::abs(g_b2bTripFuelSum-60)<.001 && g_b2bSettleCount==1);
    const auto buyer=capitals[Ptr(0x20000)],carrier=capitals[Ptr(0x50000)];
    FleetEvents(closed); // duplicate event delivery cannot charge twice
    CHECK(capitals[Ptr(0x20000)]==buyer && capitals[Ptr(0x50000)]==carrier);

    PrepareDry();ReadyQuote();g_billing=1;g_dryRun=0;
    v=Truck();auto replacement=Task();
    replacement.rows[0].target=Ptr(0x10001);replacement.rows[1].target=Ptr(0xB0000);
    capitals[Ptr(0xB0000)]=10000;
    Feed(v,empty,.9f,1000);Feed(v,Task(),.8f,2000);Feed(v,Task(),.7f,3000);
    Feed(v,replacement,.6f,4000);Feed(v,empty,.5f,5000);
    // The replacement is a different task: the original unloader pays its own
    // 0.2 decrease (40), and the new unloader separately pays the new task's
    // observed 0.1 decrease (20). Neither pays the other's task.
    CHECK(std::abs(capitals[Ptr(0x20000)]-9960)<.001);
    CHECK(std::abs(capitals[Ptr(0xB0000)]-9980)<.001);
    CHECK(std::abs(capitals[Ptr(0x50000)]-60)<.001);

    PrepareDry();ReadyQuote();g_billing=1;g_dryRun=0;g_counterpartySettlement=0;
    v=Truck();Feed(v,empty,.9f,1000);Feed(v,Task(),.8f,2000);Feed(v,empty,.7f,3000);
    CHECK(capitals[Ptr(0x20000)]==10000 && capitals[Ptr(0x50000)]==0);

    PrepareDry();ReadyQuote();g_billing=1;g_dryRun=0;
    v=Truck();Feed(v,Task(),.8f,1000); // adopted: native start was not observed
    Feed(v,empty,.6f,2000);
    CHECK(capitals[Ptr(0x20000)]==10000 && capitals[Ptr(0x50000)]==0);
    CHECK(g_b2bSettleCount==0);

    PrepareDry();ReadyQuote();g_billing=1;g_dryRun=0;
    v=Truck();auto ambiguous=Task();ambiguous.count=4;
    ambiguous.rows[3]={Ptr(0xB0000),true,0,1,1,1};
    Feed(v,empty,.9f,1000);Feed(v,ambiguous,.8f,2000);Feed(v,empty,.7f,3000);
    CHECK(capitals[Ptr(0x20000)]==10000 && capitals[Ptr(0x50000)]==0);
    CHECK(g_b2bSettleCount==0);

    // Buyer still under construction: the engine refused the unload, so the
    // otherwise complete trip is never charged.
    PrepareDry();ReadyQuote();g_billing=1;g_dryRun=0;unfinishedBuilding=Ptr(0x20000);
    v=Truck();Feed(v,empty,.9f,1000);Feed(v,Task(),.8f,2000);Feed(v,Task(),.6f,3000);
    FleetEvents(v.Observe(empty,Fuel(v.root,.5f),2,4000));
    CHECK(capitals[Ptr(0x20000)]==10000 && capitals[Ptr(0x50000)]==0);
    CHECK(g_b2bSettleCount==0);
    bool logged=false;
    for(const auto& line:logs) logged|=line.find("reason=target_under_construction")!=std::string::npos;
    CHECK(logged);
    unfinishedBuilding=0;
}
int main() {
    FleetRegressionMain();
#define RUN(name) name();std::puts("PASS " #name)
    RUN(frozen_quote_cancellation_identity_and_duplicate_close);
    RUN(replacement_separates_original_and_new_target);
    RUN(unresolved_and_invalid_identities_are_not_guessed);
    RUN(saved_closed_quote_is_stable_and_active_snapshot_is_unpriced);
    RUN(corrupt_unreadable_or_failed_writes_preserve_existing_file);
    RUN(old_epoch_events_cannot_cross_saved_worlds_or_billing_gate);
    RUN(codec_rejects_malformed_semantic_values_and_never_evicts_records);
    RUN(actual_tick_quiet_samples_save_and_long_pause_are_accounted);
    RUN(unavailable_price_is_frozen_unavailable_and_zero_is_real_zero);
    RUN(snapshot_key_is_pointer_independent_and_guarded);
    RUN(persistence_registration_and_capacity_fail_closed);
    RUN(simultaneous_vehicles_keep_independent_frozen_records);
    RUN(assignment_close_charges_original_buyer_once_only_when_complete);
#undef RUN
    std::puts("13 dry-ledger scenarios PASS (production runtime, synthetic memory/services; no Windows/game proof)");
}
