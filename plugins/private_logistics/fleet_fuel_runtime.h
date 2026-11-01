// Simulation-thread diagnostic only. Uses existing services; installs NO hook.
#ifndef PRIVATE_LOGISTICS_FLEET_FUEL_RUNTIME_H
#define PRIVATE_LOGISTICS_FLEET_FUEL_RUNTIME_H
#include "fleet_fuel_observer.h"
namespace fleet=private_logistics_fleet;
static constexpr unsigned FleetVehicleLimit=64,FleetEnterpriseLimit=32;
static fleet::Vehicle g_fleetVehicles[FleetVehicleLimit];
struct FleetRoot { uintptr_t root=0,home=0;bool duplicate=false; };
static FleetRoot g_fleetRoots[FleetVehicleLimit];
static ULONGLONG g_fleetLastMs=0;
static LONG g_fleetEpoch=0;
static bool g_fleetFault=false;
static bool FleetRead(uintptr_t address,void* out,size_t size,void*) {
    __try { return PanelRead(address,out,size,nullptr); }
    __except(EXCEPTION_EXECUTE_HANDLER) { return false; }
}
static void FleetEvents(const fleet::a::Result& result) {
    for(unsigned i=0;i<result.count;++i) {
        const auto& e=result.events[i];const auto& c=e.candidate;
        const auto endpoint=fleet::a::Estimate(c,e.boundary);
        Logf("[FLEET ASSIGNMENT] epoch=%llu root=%p home=%p id=%llu event=%s reason=%s "
             "original_target_candidate=%p original_target_rows=%u original_target_resolved=0 "
             "first_sample=%u boundary_sample=%u first_ms=%llu boundary_ms=%llu "
             "native_start_valid=%d native_start=%.9g native_end_valid=%d native_end=%.9g "
             "unknown_start=%d observation_gap=%d "
             "role_uncertain=%d fuel_uncertain=%d receiver_changed=%d observed_level_rise=%d "
             "net_change_valid=%d net_change_native=%.9g units=unverified id_scope=process_local_vehicle_slot "
             "native_boundary=unverified refill_coverage=unverified exact_burn=0 "
             "actual_cost=not_computed settlement=none billing=off",
             (unsigned long long)c.epoch,(void*)c.root,(void*)c.home,(unsigned long long)c.id,
             fleet::a::KindName(e.kind),e.reason,(void*)c.originalTarget,c.originalTargetRows,
             c.first.sample,e.boundary.sample,(unsigned long long)c.first.ms,(unsigned long long)e.boundary.ms,
             fleet::a::ValidFuel(c.first.native)?1:0,fleet::a::NativeTotal(c.first.native),
             fleet::a::ValidFuel(e.boundary.native)?1:0,fleet::a::NativeTotal(e.boundary.native),
             c.unknownStart?1:0,c.observationGap?1:0,
             c.roleUncertain?1:0,c.fuelUncertain?1:0,c.receiverChanged?1:0,endpoint.observedRise?1:0,
             endpoint.valid?1:0,endpoint.valid?endpoint.net:0.0);
    }
}
static void FleetBreakAll() {
    for(auto& v:g_fleetVehicles) if(v.root) v.Break();
}
static unsigned FleetCollect(const fleet::p::Memory& memory,bool& complete) {
    unsigned count=0;
    const size_t total=g_enterprise->getEnterpriseCount();
    if(total>FleetEnterpriseLimit) complete=false;
    const size_t limit=total>FleetEnterpriseLimit?FleetEnterpriseLimit:total;
    for(size_t i=0;i<limit;++i) {
        void* office=g_enterprise->getEnterpriseByIndex(i);
        fleet::FleetList list{};
        const auto status=fleet::ReadOffice(memory,(uintptr_t)office,list);
        if(status==fleet::ListStatus::NotRoadOffice) continue;
        if(status!=fleet::ListStatus::Complete) {complete=false;continue;}
        for(unsigned k=0;k<list.count;++k) {
            bool duplicate=false;
            for(unsigned j=0;j<count;++j) if(g_fleetRoots[j].root==list.roots[k]) {
                g_fleetRoots[j].duplicate=true;duplicate=true;complete=false;
            }
            if(duplicate) continue; // Never sample an ambiguous root, regardless of owner.
            if(count==FleetVehicleLimit) {complete=false;continue;}
            g_fleetRoots[count++]={list.roots[k],(uintptr_t)office,false};
        }
    }
    if(g_enterprise->getEnterpriseCount()!=total) complete=false;
    return count;
}
static void FleetVehicleLog(const fleet::Vehicle& v,const char* reason) {
    Logf("[FLEET FUEL] epoch=%llu root=%p home=%p sample=%u ms=%llu reason=%s "
         "native_valid=%d native_total=%.9g objects=%u root_metadata=%p root_raw=%.9g root_capacity=%.9g "
         "observed_decrease_raw=%.9g observed_increase_raw=%.9g "
         "discontinuities=%u panel_required=0 units=unverified refill_coverage=unverified "
         "exact_burn=0 actual_cost=not_computed settlement=none billing=off",
         (unsigned long long)v.previousEpoch,(void*)v.root,(void*)v.home,v.sample,
         (unsigned long long)v.lastMs,reason,v.previousValid?1:0,
         fleet::a::NativeTotal(v.previous),v.previous.count,(void*)v.previous.metadata[0],
         (double)v.previous.raw[0],(double)v.previous.capacity[0],
         v.observedDecrease,v.observedIncrease,v.discontinuities);
}
static void FleetTickBody() {
    const ULONGLONG now=GetTickCount64();
    if(g_fleetLastMs && now>=g_fleetLastMs && now-g_fleetLastMs<1000) return;
    const LONG epoch=InterlockedCompareExchange(&g_panelEpoch,0,0);
    if(epoch!=g_fleetEpoch) {
        for(auto& v:g_fleetVehicles) if(v.root) {
            FleetVehicleLog(v,"load_epoch_changed");FleetEvents(v.Forget("load_epoch_changed"));
        }
        g_fleetEpoch=epoch;
    }
    if(g_fleetLastMs && (now<g_fleetLastMs || now-g_fleetLastMs>1500)) FleetBreakAll();
    g_fleetLastMs=now;
    const fleet::p::Memory memory{FleetRead,nullptr};
    bool complete=true;
    const unsigned count=FleetCollect(memory,complete);
    if(epoch!=InterlockedCompareExchange(&g_panelEpoch,0,0)) {FleetBreakAll();return;}
    if(!complete) {
        FleetBreakAll();
        Logf("[FLEET COVERAGE] complete=0 reason=invalid_duplicate_changed_or_capacity "
             "max_enterprises=32 max_vehicles=64 no_missing_vehicle_cost_inferred=1 billing=off");
    }
    // Retire missing identities BEFORE allocating slots. No saved pointer is dereferenced.
    for(auto& v:g_fleetVehicles) if(v.root) {
        v.seen=false;
        for(unsigned i=0;i<count;++i) if(!g_fleetRoots[i].duplicate &&
            v.root==g_fleetRoots[i].root && v.home==g_fleetRoots[i].home) v.seen=true;
        if(!v.seen) {FleetVehicleLog(v,"not_observed");FleetEvents(v.Forget("fleet_identity_not_observed"));}
    }
    for(unsigned i=0;i<count;++i) {
        const auto& r=g_fleetRoots[i];if(r.duplicate) continue;
        fleet::Vehicle* slot=nullptr;
        for(auto& v:g_fleetVehicles) if(v.root==r.root && v.home==r.home) {slot=&v;break;}
        if(!slot) for(auto& v:g_fleetVehicles) if(!v.root) {slot=&v;break;}
        if(!slot) {FleetBreakAll();continue;}
        slot->root=r.root;slot->home=r.home;slot->seen=true;
        const auto fuel=fleet::ReadFuel(memory,r.home,r.root);
        fleet::sch::Snapshot schedule{};
        if(fleet::a::ValidFuel(fuel)) fleet::sch::Capture(memory,r.root,schedule);
        if(epoch!=InterlockedCompareExchange(&g_panelEpoch,0,0)) {FleetBreakAll();return;}
        const double oldIncrease=slot->observedIncrease;
        const auto events=slot->Observe(schedule,fuel,(unsigned long long)epoch,now);
        FleetEvents(events);
        if(slot->sample==1 || slot->sample%10==0 || events.count ||
           slot->observedIncrease!=oldIncrease || !slot->previousValid) FleetVehicleLog(*slot,"poll");
    }
}
static void FleetObserverTick(void*,float,float) {
    if(g_fleetFault || g_billing) return;
    __try {FleetTickBody();}
    __except(EXCEPTION_EXECUTE_HANDLER) {
        g_fleetFault=true;FleetBreakAll();
        Logf("[FLEET FUEL] disabled_after_guarded_fault=1 exact_burn=0 billing=off");
    }
}
static void FleetObserverStart() {
    if(!g_panelObserverEnabled || !g_scheduleObserverEnabled) return; // Existing diagnostic switches only.
    if(g_billing || !g_simClock || !g_simClock->registerTick || !g_persistenceHandle ||
       !g_enterprise || g_enterprise->structSize<offsetof(TsmEnterpriseBaseApi,getEnterpriseByIndex)+
           sizeof(g_enterprise->getEnterpriseByIndex) || !g_enterprise->getEnterpriseCount ||
       !g_enterprise->getEnterpriseByIndex || !PanelTargetMatches() || !ScheduleTargetMatches()) {
        Logf("[FLEET FUEL] unavailable: requires billing=0, verified target, tick, load signal and private register");
        return;
    }
    if(!g_simClock->registerTick(FleetObserverTick,nullptr)) {
        Logf("[FLEET FUEL] tick_registration_failed=1 billing=off");return;
    }
    Logf("[FLEET FUEL] started panel_required=0 source=private_enterprise_road_office_fleets "
         "interval_ms=1000 max_enterprises=32 max_vehicles=64 max_fleet=256 new_hooks=0 "
         "coverage=sampled_not_continuous units=unverified refill_coverage=unverified "
         "exact_burn=0 actual_cost=not_computed settlement=none billing=off");
}
#endif
