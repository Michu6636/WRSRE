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
static bool g_fleetFault=false,g_fleetDiscoveryComplete=true,g_fleetCoverageWarned=false;
static bool g_fleetStarted=false;
static ULONGLONG g_fleetPriceMs=0,g_fleetCoverageLogMs=0;
static bool g_fleetPriceCached=false;
static PanelEndpointPrice g_fleetPrice{}; // Copied quote only; no native pointer retained.
// Optional dry accounting sinks; selected-panel events never feed this ledger.
static void (*g_fleetCostEvent)(const fleet::a::Event&,const PanelEndpointPrice&,bool)=nullptr;
static void (*g_fleetCostSample)(const fleet::a::Candidate&,bool)=nullptr;
static void FleetRefreshPrice(ULONGLONG now) {
    if(!g_fleetPriceCached || now<g_fleetPriceMs || now-g_fleetPriceMs>=30000) {
        g_fleetPrice=DiagnosticFuelPrice();g_fleetPriceMs=now;g_fleetPriceCached=true;
    }
}
static PanelEndpointPrice FleetApproximatePrice(double decrease,unsigned pairs) {
    PanelEndpointPrice q=g_fleetPrice;q.available=false;q.amount=0;
    q.reason="no_compared_samples";if(!pairs) return q;
    q.reason="invalid_decrease";if(!std::isfinite(decrease) || decrease<0) return q;
    q.reason=g_fleetPrice.reason;if(!g_fleetPrice.available) return q;
    q.reason="invalid_product";
    const double amount=decrease*double(q.perTonne);
    if(!std::isfinite(amount) || amount<0) return q;
    q.amount=amount;q.available=true;q.reason="approximate_positive_drops";
    return q;
}
static bool FleetRead(uintptr_t address,void* out,size_t size,void*) {
    __try { return PanelRead(address,out,size,nullptr); }
    __except(EXCEPTION_EXECUTE_HANDLER) { return false; }
}
static void FleetEvents(const fleet::a::Result& result) {
    for(unsigned i=0;i<result.count;++i) {
        const auto& e=result.events[i];const auto& c=e.candidate;
        const auto endpoint=fleet::a::Estimate(c,e.boundary);
        const auto price=FleetApproximatePrice(c.sampledDecrease,c.sampledPairs);
        if(g_fleetCostEvent) g_fleetCostEvent(e,price,g_fleetDiscoveryComplete);
        Logf("[FLEET ASSIGNMENT] epoch=%llu root=%p home=%p id=%llu event=%s reason=%s "
             "original_target_candidate=%p original_target_rows=%u original_target_resolved=0 "
             "first_sample=%u boundary_sample=%u first_ms=%llu boundary_ms=%llu "
             "native_start_valid=%d native_start=%.9g native_end_valid=%d native_end=%.9g "
             "unknown_start=%d observation_gap=%d "
             "role_uncertain=%d fuel_uncertain=%d receiver_changed=%d observed_level_rise=%d "
             "net_change_valid=%d net_change_native=%.9g units=unverified id_scope=process_local_vehicle_slot "
             "native_boundary=unverified refill_coverage=unverified exact_burn=0 "
             "sampled_decrease_native=%.9g sampled_increase_native=%.9g compared_pairs=%u "
             "estimate_partial=%d discovery_complete=%d estimated_cost_available=%d estimated_cost=%.9g "
             "estimate_reason=%s estimate_currency=%d price_per_tonne=%.9g "
             "price_basis=current_cached_30s quantity_assumption=native_tonnes_provisional "
             "actual_cost=not_computed settlement=none money_moved=0",
             (unsigned long long)c.epoch,(void*)c.root,(void*)c.home,(unsigned long long)c.id,
             fleet::a::KindName(e.kind),e.reason,(void*)c.originalTarget,c.originalTargetRows,
             c.first.sample,e.boundary.sample,(unsigned long long)c.first.ms,(unsigned long long)e.boundary.ms,
             fleet::a::ValidFuel(c.first.native)?1:0,fleet::a::NativeTotal(c.first.native),
             fleet::a::ValidFuel(e.boundary.native)?1:0,fleet::a::NativeTotal(e.boundary.native),
             c.unknownStart?1:0,c.observationGap?1:0,
             c.roleUncertain?1:0,c.fuelUncertain?1:0,c.receiverChanged?1:0,endpoint.observedRise?1:0,
             endpoint.valid?1:0,endpoint.valid?endpoint.net:0.0,
             c.sampledDecrease,c.sampledIncrease,c.sampledPairs,
             (c.unknownStart || c.observationGap || c.roleUncertain || c.fuelUncertain || c.receiverChanged ||
              c.observedRise || !c.sampledPairs || !g_fleetDiscoveryComplete)?1:0,g_fleetDiscoveryComplete?1:0,
             price.available?1:0,price.amount,price.reason,price.currency,double(price.perTonne));
    }
}
static void FleetBreakAll() {
    for(auto& v:g_fleetVehicles) if(v.root) v.Break();
}
static unsigned FleetCollect(const fleet::p::Memory& memory,bool& complete,bool* stable=nullptr) {
    if(stable) *stable=true;
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
    if(g_enterprise->getEnterpriseCount()!=total) {
        complete=false;if(stable) *stable=false;
    }
    return count;
}
static void FleetVehicleLog(const fleet::Vehicle& v,const char* reason) {
    const auto price=FleetApproximatePrice(v.observedDecrease,v.observedPairs);
    Logf("[FLEET FUEL] epoch=%llu root=%p home=%p sample=%u ms=%llu reason=%s "
         "native_valid=%d native_total=%.9g objects=%u root_metadata=%p root_raw=%.9g root_capacity=%.9g "
         "observed_decrease_raw=%.9g observed_increase_raw=%.9g "
         "discontinuities=%u panel_required=0 units=unverified refill_coverage=unverified "
         "compared_pairs=%u estimate_partial=%d discovery_complete=%d "
         "estimated_cost_available=%d estimated_cost=%.9g estimate_reason=%s estimate_currency=%d "
         "price_per_tonne=%.9g price_basis=current_cached_30s quantity_assumption=native_tonnes_provisional "
         "estimate_scope=vehicle_observed_not_bill exact_burn=0 "
         "actual_cost=not_computed settlement=none money_moved=0",
         (unsigned long long)v.previousEpoch,(void*)v.root,(void*)v.home,v.sample,
         (unsigned long long)v.lastMs,reason,v.previousValid?1:0,
         fleet::a::NativeTotal(v.previous),v.previous.count,(void*)v.previous.metadata[0],
         (double)v.previous.raw[0],(double)v.previous.capacity[0],
         v.observedDecrease,v.observedIncrease,v.discontinuities,v.observedPairs,
         (v.discontinuities || v.observedIncrease>0 || !v.observedPairs || !g_fleetDiscoveryComplete)?1:0,
         g_fleetDiscoveryComplete?1:0,
         price.available?1:0,price.amount,price.reason,price.currency,double(price.perTonne));
}
static void FleetTickBody() {
    const ULONGLONG now=GetTickCount64();
    if(g_fleetLastMs && now>=g_fleetLastMs && now-g_fleetLastMs<1000) return;
    const LONG epoch=InterlockedCompareExchange(&g_panelEpoch,0,0);
    if(epoch!=g_fleetEpoch) {
        for(auto& v:g_fleetVehicles) if(v.root) {
            FleetVehicleLog(v,"load_epoch_changed");FleetEvents(v.Forget("load_epoch_changed"));
        }
        g_fleetEpoch=epoch;g_fleetPriceCached=false;g_fleetCoverageWarned=false;
    }
    // Elapsed wall time (including an ordinary pause) is not an identity break.
    if(g_fleetLastMs && now<g_fleetLastMs) FleetBreakAll();
    g_fleetLastMs=now;
    FleetRefreshPrice(now);
    const fleet::p::Memory memory{FleetRead,nullptr};
    bool complete=true,stable=true;
    const unsigned count=FleetCollect(memory,complete,&stable);
    if(epoch!=InterlockedCompareExchange(&g_panelEpoch,0,0)) {FleetBreakAll();return;}
    // Partial discovery does not invalidate independently revalidated visible roots.
    // Known duplicates and absent identities are still excluded/retired below.
    g_fleetDiscoveryComplete=complete;
    if(!complete && (!g_fleetCoverageWarned || now<g_fleetCoverageLogMs ||
                     now-g_fleetCoverageLogMs>=30000)) {
        Logf("[FLEET COVERAGE] complete=0 reason=invalid_duplicate_changed_or_capacity "
             "max_enterprises=32 max_vehicles=64 no_missing_vehicle_cost_inferred=1 money_moved=0");
        g_fleetCoverageLogMs=now;
    }
    g_fleetCoverageWarned=!complete;
    if(!stable) {FleetBreakAll();return;} // A changing roster is not merely a capacity limit.
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
        const bool wasValid=slot->previousValid;
        const auto events=slot->Observe(schedule,fuel,(unsigned long long)epoch,now);
        FleetEvents(events);
        if(g_fleetCostSample && slot->ledger.Active())
            g_fleetCostSample(slot->ledger.Current(),g_fleetDiscoveryComplete);
        if(slot->sample==1 || slot->sample%30==0 || events.count ||
           slot->previousValid!=wasValid) FleetVehicleLog(*slot,"poll");
    }
}
static void FleetObserverTick(void*,float,float) {
    if(g_fleetFault) return;
    __try {FleetTickBody();}
    __except(EXCEPTION_EXECUTE_HANDLER) {
        g_fleetFault=true;FleetBreakAll();
        Logf("[FLEET FUEL] disabled_after_guarded_fault=1 exact_burn=0 money_moved=0");
    }
}
static void FleetObserverStart() {
    if(!g_panelObserverEnabled || !g_scheduleObserverEnabled) return; // Existing diagnostic switches only.
    if(!g_simClock || !g_simClock->registerTick || !g_persistenceHandle ||
       !g_enterprise || g_enterprise->structSize<offsetof(TsmEnterpriseBaseApi,getEnterpriseByIndex)+
           sizeof(g_enterprise->getEnterpriseByIndex) || !g_enterprise->getEnterpriseCount ||
       !g_enterprise->getEnterpriseByIndex || !PanelTargetMatches() || !ScheduleTargetMatches()) {
        Logf("[FLEET FUEL] unavailable: requires verified target, tick, load signal and private register");
        return;
    }
    if(!g_simClock->registerTick(FleetObserverTick,nullptr)) {
        Logf("[FLEET FUEL] tick_registration_failed=1 money_moved=0");return;
    }
    g_fleetStarted=true;
    Logf("[FLEET FUEL] started panel_required=0 source=private_enterprise_road_office_fleets "
         "interval_ms=1000 max_enterprises=32 max_vehicles=64 max_fleet=256 new_hooks=0 "
         "coverage=sampled_not_continuous units=unverified refill_coverage=unverified "
         "exact_burn=0 actual_cost=not_computed settlement=none money_moved=0");
}
#endif
