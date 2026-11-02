// Production portable reader AND runtime, synthetic memory/public services only.
#define PANEL_RUNTIME_TEST_NO_MAIN
#include "panel_fuel_runtime_test.cpp"
#include "../../src/enterprise_base_api.h"
static TsmEnterpriseBaseApi enterprises{};
static const TsmEnterpriseBaseApi* g_enterprise=&enterprises;
#include "fleet_fuel_runtime.h"
namespace a=private_logistics_assignment;
namespace sch=private_logistics_schedule;
static std::vector<void*> owners;
static size_t EnterpriseCount() {return owners.size();}
static void* EnterpriseAt(size_t i) {return owners.at(i);}
static sch::Snapshot Goods() {
    sch::Snapshot s{};s.status=sch::Status::Complete;s.count=3;
    s.rows[0]={0x10000,true,1,0,1,1};s.rows[1]={0x20000,true,0,1,1,1};
    s.rows[2]={0x50000,true,0,0,1,1};return s;
}
static a::NativeFuel Fuel(Address root,float raw) {
    a::NativeFuel f{};f.count=1;f.valid=true;f.objects[0]=root;f.metadata[0]=0x40000;
    f.raw[0]=raw;f.capacity[0]=1;return f;
}
static fleet::Vehicle Vehicle(Address root=0x30000) {
    fleet::Vehicle v;v.root=root;v.home=0x50000;return v;
}
static void reader_without_window_and_signed_levels() {
    Fixture f;f.Set(f.meta,0x294,1);f.denied.push_back({f.window,0x500});
    fleet::FleetList list;CHECK(fleet::ReadOffice(f.Mem(),f.home,list)==fleet::ListStatus::Complete);
    CHECK(list.count==1 && list.roots[0]==f.root);
    f.Set(f.root,0x5F0,-.03f);const auto fuel=fleet::ReadFuel(f.Mem(),f.home,f.root);
    CHECK(a::ValidFuel(fuel) && fuel.raw[0]==-.03f);
}
static void reader_rejects_bad_fleets() {
    Fixture f;fleet::FleetList list;
    for(Address end:{f.fleet+1,f.fleet-8,f.fleet+(MaxFleet+1)*8}) {
        f.Set(f.home,0xC78,end);CHECK(fleet::ReadOffice(f.Mem(),f.home,list)==fleet::ListStatus::Invalid);
    }
    f.Set(f.home,0xC78,f.fleet+16);f.Set(f.fleet,8,f.root);
    CHECK(fleet::ReadOffice(f.Mem(),f.home,list)==fleet::ListStatus::Invalid);
    f.Set(f.descriptor,0x360,52);CHECK(fleet::ReadOffice(f.Mem(),f.home,list)==fleet::ListStatus::NotRoadOffice);
}
static void reader_requires_metadata_membership_and_whole_quantity() {
    Fixture f;CHECK(!a::ValidFuel(fleet::ReadFuel(f.Mem(),f.home,f.root)));
    f.Set(f.meta,0x294,1);f.Link(.1f,.2f);
    CHECK(fleet::ReadFuel(f.Mem(),f.home,f.root).count==2);
    f.denied.push_back({0x90000+0x5F0,4});CHECK(!a::ValidFuel(fleet::ReadFuel(f.Mem(),f.home,f.root)));
    f.denied.clear();f.Set(f.fleet,0,Address(0xBAD));CHECK(!a::ValidFuel(fleet::ReadFuel(f.Mem(),f.home,f.root)));
}
static void two_vehicles_have_independent_histories() {
    auto x=Vehicle(),y=Vehicle(0x90000);auto empty=Goods();empty.count=0;
    x.Observe(empty,Fuel(x.root,.9f),1,1000);y.Observe(empty,Fuel(y.root,.6f),1,1000);
    CHECK(x.Observe(Goods(),Fuel(x.root,.8f),1,2000).events[0].kind==a::Kind::Opened);
    CHECK(y.Observe(Goods(),Fuel(y.root,.5f),1,2000).events[0].kind==a::Kind::Opened);
    x.Observe(empty,Fuel(x.root,.7f),1,3000);
    CHECK(!x.ledger.Active() && y.ledger.Active() && y.ledger.Current().last.native.raw[0]==.5f);
}
static void refills_do_not_erase_observed_decreases() {
    auto v=Vehicle();v.Observe(Goods(),Fuel(v.root,.8f),1,1000);
    v.Observe(Goods(),Fuel(v.root,.6f),1,2000);const auto decline=v.observedDecrease;
    v.Observe(Goods(),Fuel(v.root,.9f),1,3000);
    CHECK(v.observedDecrease==decline && v.observedIncrease>0 && v.ledger.Current().observedRise);
    CHECK(!a::Estimate(v.ledger.Current(),{4,4000,false,0,Fuel(v.root,.7f)}).valid); // adopted/unknown start
}
static void invalid_read_gap_does_not_join_quantities() {
    auto v=Vehicle();v.Observe(Goods(),Fuel(v.root,.8f),1,1000);
    v.Observe(Goods(),a::NativeFuel{},1,2000);v.Observe(Goods(),Fuel(v.root,.2f),1,3000);
    CHECK(v.observedDecrease==0 && v.ledger.Current().observationGap && v.ledger.Current().fuelUncertain);
}
static void original_target_is_unloader_not_home_and_frozen_on_replacement() {
    auto v=Vehicle();auto s=Goods();auto empty=s;empty.count=0;
    v.Observe(empty,Fuel(v.root,.9f),1,1000);v.Observe(s,Fuel(v.root,.8f),1,2000);
    CHECK(v.ledger.Current().originalTarget==0x20000 && v.ledger.Current().originalTarget!=v.home);
    s.rows[1].valueCC=7;s.index698=1;v.Observe(s,Fuel(v.root,.7f),1,3000);
    CHECK(v.ledger.Current().id==1 && v.ledger.Current().originalTarget==0x20000);
    s.rows[1].target=0xB0000;const auto r=v.Observe(s,Fuel(v.root,.6f),1,4000);
    CHECK(r.count==2 && r.events[0].kind==a::Kind::Closed && r.events[1].kind==a::Kind::Opened);
    CHECK(r.events[0].candidate.originalTarget==0x20000 && r.events[1].candidate.originalTarget==0xB0000);
    CHECK(r.events[0].candidate.home==r.events[1].candidate.home);
}
static void missing_vehicle_preserves_target_but_invalidates_end() {
    auto v=Vehicle();v.Observe(Goods(),Fuel(v.root,.8f),1,1000);
    const auto r=v.Forget("not_observed");CHECK(r.count==1 && r.events[0].kind==a::Kind::Abandoned);
    CHECK(r.events[0].candidate.originalTarget==0x20000 && !a::ValidFuel(r.events[0].boundary.native));
    CHECK(!v.root && !v.ledger.Active() && v.observedDecrease==0);
    v.root=0x90000;v.home=0x50000;v.Observe(Goods(),Fuel(v.root,.2f),1,2000);
    CHECK(v.ledger.Current().id==2 && v.ledger.Current().unknownStart);
}
static void multiple_unload_rows_do_not_invent_a_payer() {
    auto v=Vehicle();auto s=Goods();s.count=4;s.rows[3]={0xB0000,true,0,1,1,1};
    v.Observe(s,Fuel(v.root,.8f),1,1000);
    CHECK(v.ledger.Current().originalTarget==0 && v.ledger.Current().originalTargetRows==2);
}
static void epoch_and_clock_changes_prevent_cross_load_deltas() {
    auto v=Vehicle();v.Observe(Goods(),Fuel(v.root,.8f),1,1000);
    auto r=v.Observe(Goods(),Fuel(v.root,.2f),2,2000);
    CHECK(v.observedDecrease==0 && r.count==2 && r.events[0].candidate.originalTarget==0x20000);
    r=v.Observe(Goods(),Fuel(v.root,.1f),2,1500);
    CHECK(r.count==1 && r.events[0].kind==a::Kind::Abandoned && !v.previousValid);
}
template<class T> static void Put(Address offset,T value) {std::memcpy(g_exeBase+offset,&value,sizeof(value));}
static Address Ptr(Address offset) {return (Address)(g_exeBase+offset);}
static void PrepareRuntime(unsigned n=1) {
    std::memset(g_exeBase,0,image.size());Fixture f;f.Set(f.meta,0x294,1);
    for(const auto& r:f.regions) std::memcpy(g_exeBase+r.address,r.data.data(),r.data.size());
    Put(f.root+0x1708,Ptr(f.meta));Put(f.root+0x4F8,Ptr(f.home));Put(f.home+0x318,Ptr(f.descriptor));
    Put(f.home+0xC70,Ptr(f.fleet));Put(f.home+0xC78,Ptr(f.fleet+n*8));Put(f.fleet,Ptr(f.root));
    if(n==2) {
        Put(f.fleet+8,Ptr(0x90000));Put(0x90000+0x1708,Ptr(0xA0000));Put(0x90000+0x4F8,Ptr(f.home));
        Put(0x90000+0x5F0,.4f);Put(0xA0000+0x8684,.6f);Put(0xA0000+0x294,1);
    }
    owners={(void*)Ptr(f.home)};enterprises={};enterprises.structSize=sizeof(enterprises);
    enterprises.getEnterpriseCount=EnterpriseCount;enterprises.getEnterpriseByIndex=EnterpriseAt;
    for(auto& v:g_fleetVehicles) v=fleet::Vehicle{};
    g_fleetLastMs=0;g_fleetEpoch=0;g_fleetFault=false;g_billing=0;clockMs=1000;logs.clear();
    g_fleetPrice={};g_fleetPriceCached=false;g_fleetPriceMs=g_fleetCoverageLogMs=0;
    g_fleetDiscoveryComplete=true;g_fleetCoverageWarned=false;g_pricing=nullptr;g_currency=nullptr;
    throwOnRead=false;throwAtRead=denyAtRead=0;attemptedReads.clear();
}
static void runtime_samples_two_vehicles_without_rendering() {
    PrepareRuntime(2);FleetObserverTick(nullptr,1,1);
    CHECK(g_fleetVehicles[0].sample==1 && g_fleetVehicles[1].sample==1);
    CHECK(g_fleetVehicles[0].root!=g_fleetVehicles[1].root);
    Put(0x30000+0x5F0,.5f);Put(0x90000+0x5F0,.3f);clockMs+=1000;FleetObserverTick(nullptr,1,1);
    CHECK(g_fleetVehicles[0].observedDecrease>0 && g_fleetVehicles[1].observedDecrease>0);
    CHECK(g_fleetVehicles[0].sample==2 && g_fleetVehicles[1].sample==2);
    for(auto address:attemptedReads) CHECK(address<Ptr(0x10000) || address>=Ptr(0x10500));
    CHECK(logs[0].find("panel_required=0")!=std::string::npos);
}
static void runtime_throttles_but_preserves_long_pause_difference() {
    PrepareRuntime();FleetObserverTick(nullptr,1,1);clockMs+=500;FleetObserverTick(nullptr,1,1);
    CHECK(g_fleetVehicles[0].sample==1);
    clockMs+=125000;Put(0x30000+0x5F0,.1f);FleetObserverTick(nullptr,1,1);
    CHECK(g_fleetVehicles[0].sample==2 && g_fleetVehicles[0].observedDecrease>0);
    CHECK(g_fleetVehicles[0].discontinuities==0 && g_fleetVehicles[0].observedPairs==1);
}
static void runtime_limits_and_duplicate_roots_are_explicit() {
    PrepareRuntime();owners.push_back(owners[0]);bool complete=true;
    const unsigned count=FleetCollect({FleetRead,nullptr},complete);
    CHECK(!complete && count==1 && g_fleetRoots[0].duplicate);
    FleetObserverTick(nullptr,1,1);CHECK(!g_fleetVehicles[0].root);
    owners.assign(FleetEnterpriseLimit+1,owners[0]);complete=true;
    FleetCollect({FleetRead,nullptr},complete);CHECK(!complete);
    PrepareRuntime();Put(0x50000+0xC78,Ptr(0x70000+(FleetVehicleLimit+1)*8));
    // Populate valid distinct fleet roots; collecting is bounded before vehicle reads.
    for(unsigned i=0;i<FleetVehicleLimit+1;++i) Put(0x70000+i*8,Ptr(0x100000+i*0x2000));
    complete=true;CHECK(FleetCollect({FleetRead,nullptr},complete)==FleetVehicleLimit && !complete);
}
static void runtime_load_signal_retires_before_new_reading() {
    PrepareRuntime();FleetObserverTick(nullptr,1,1);Put(0x30000+0x5F0,.1f);
    InterlockedIncrement(&g_panelEpoch);clockMs+=1000;FleetObserverTick(nullptr,1,1);
    CHECK(g_fleetVehicles[0].sample==1 && g_fleetVehicles[0].observedDecrease==0);
}
static void runtime_guarded_reads_and_billing_gate() {
    PrepareRuntime();throwAtRead=Ptr(0x30000+0x5F0);FleetObserverTick(nullptr,1,1);
    CHECK(!g_fleetVehicles[0].previousValid && !g_fleetFault);
    PrepareRuntime();g_billing=1;FleetObserverTick(nullptr,1,1);CHECK(!g_fleetVehicles[0].root);
    g_billing=0;
}
static void runtime_start_uses_existing_tick_and_no_new_hook() {
    PrepareRuntime();g_scheduleObserverEnabled=1;
    auto* dos=(IMAGE_DOS_HEADER*)g_exeBase;dos->e_magic=IMAGE_DOS_SIGNATURE;dos->e_lfanew=0x80;
    auto* nt=(IMAGE_NT_HEADERS64*)(g_exeBase+0x80);nt->Signature=IMAGE_NT_SIGNATURE;
    nt->FileHeader.Machine=IMAGE_FILE_MACHINE_AMD64;nt->FileHeader.TimeDateStamp=0x6A3EB6AD;
    nt->OptionalHeader.Magic=IMAGE_NT_OPTIONAL_HDR64_MAGIC;
    const BYTE scale[]={0,0,0x7A,0x44},format[]={0x25,0,0x2e,0,0x32,0,0x66,0,0x6c,0,0,0};
    std::memcpy(g_exeBase+0x90B07C,scale,sizeof(scale));std::memcpy(g_exeBase+0x8F3E98,format,sizeof(format));
    for(const auto& f:g_scheduleFingerprints) std::memcpy(g_exeBase+f.rva,f.bytes,f.size);
    const auto hooks=installed;tick=nullptr;FleetObserverStart();
    CHECK(tick==FleetObserverTick && installed==hooks);
    CHECK(logs.back().find("started panel_required=0")!=std::string::npos);
    tick=nullptr;g_billing=1;FleetObserverStart();CHECK(!tick);g_billing=0;
    enterprises.structSize=0;FleetObserverStart();CHECK(!tick);
}
#include "fleet_approximation_cases.h"
#ifdef FLEET_RUNTIME_TEST_NO_MAIN
int FleetRegressionMain() {
#else
int main() {
#endif
    (void)&PanelObserverStart;(void)&PanelObserverLoading;
#define RUN(name) name();std::puts("PASS " #name)
    RUN(reader_without_window_and_signed_levels);
    RUN(reader_rejects_bad_fleets);
    RUN(reader_requires_metadata_membership_and_whole_quantity);
    RUN(two_vehicles_have_independent_histories);
    RUN(refills_do_not_erase_observed_decreases);
    RUN(invalid_read_gap_does_not_join_quantities);
    RUN(original_target_is_unloader_not_home_and_frozen_on_replacement);
    RUN(missing_vehicle_preserves_target_but_invalidates_end);
    RUN(multiple_unload_rows_do_not_invent_a_payer);
    RUN(epoch_and_clock_changes_prevent_cross_load_deltas);
    RUN(runtime_samples_two_vehicles_without_rendering);
    RUN(runtime_throttles_but_preserves_long_pause_difference);
    RUN(runtime_limits_and_duplicate_roots_are_explicit);
    RUN(runtime_load_signal_retires_before_new_reading);
    RUN(runtime_guarded_reads_and_billing_gate);
    RUN(runtime_start_uses_existing_tick_and_no_new_hook);
    RUN(assignment_estimate_survives_refill_and_includes_closure);
    RUN(cancelled_assignment_keeps_target_and_excludes_post_close_overhead);
    RUN(mixed_object_rise_does_not_mask_another_object_drop);
    RUN(invalid_schedule_skips_assignment_gap_but_keeps_earlier_drops);
    RUN(invalid_fuel_and_changed_receiver_rebase_without_erasing_drops);
    RUN(replacement_boundary_does_not_charge_new_target_for_old_decreases);
    RUN(runtime_caches_one_quote_for_all_vehicles_and_resets_on_load);
    RUN(unavailable_price_is_not_a_zero_estimate_and_true_zero_is_available);
    RUN(partial_large_discovery_keeps_visible_vehicle_pairs_and_throttles_warnings);
    RUN(assignment_logs_label_refill_estimate_without_charging_money);
    RUN(backward_clock_skips_pair_and_rebases);
    RUN(newly_duplicate_or_missing_identity_is_retired_despite_partial_discovery);
    RUN(changing_roster_still_breaks_baselines_unlike_capacity_only);
#undef RUN
    std::puts("29 fleet scenarios PASS (synthetic memory, host services; no game/DLL proof)");
    return 0;
}
