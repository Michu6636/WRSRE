// Additional cases exercising production ledger/runtime, not a detached estimate model.
static TsmResourcePricingApi fleetPrices{};
static unsigned fleetLookups=0,fleetPriceCalls=0;
static float fleetUnitPrice=200;
static void* FleetFindPrice(const char* name) {
    ++fleetLookups;CHECK(std::strcmp(name,gFuelResource)==0);return &fleetPrices;
}
static float FleetGetPrice(void* resource,int currency) {
    CHECK(resource==&fleetPrices && currency==TSM_CURRENCY_RUB);
    ++fleetPriceCalls;return fleetUnitPrice;
}
static void SetupFleetPrice() {
    fleetPrices={};fleetPrices.structSize=sizeof(fleetPrices);
    fleetPrices.getByName=FleetFindPrice;fleetPrices.getPrice=FleetGetPrice;
    g_pricing=&fleetPrices;g_currency=nullptr;fleetLookups=fleetPriceCalls=0;fleetUnitPrice=200;
}
static bool Near(double x,double y) {return std::abs(x-y)<1e-6;}
static void assignment_estimate_survives_refill_and_includes_closure() {
    auto v=Vehicle();auto empty=Goods();empty.count=0;
    v.Observe(empty,Fuel(v.root,.9f),1,1000);
    v.Observe(Goods(),Fuel(v.root,.8f),1,2000);
    v.Observe(Goods(),Fuel(v.root,.6f),1,3000);
    v.Observe(Goods(),Fuel(v.root,.9f),1,4000);
    const auto r=v.Observe(empty,Fuel(v.root,.7f),1,5000);
    CHECK(r.count==1 && r.events[0].kind==a::Kind::Closed);
    const auto& c=r.events[0].candidate;
    CHECK(Near(c.sampledDecrease,.4) && Near(c.sampledIncrease,.3) && c.sampledPairs==3);
    CHECK(!c.unknownStart && c.observedRise && c.originalTarget==0x20000);
    CHECK(Near(v.observedDecrease,.5)); // Opening interval remains vehicle-only overhead.
    PrepareRuntime();SetupFleetPrice();FleetRefreshPrice(clockMs);
    const auto q=FleetApproximatePrice(c.sampledDecrease,c.sampledPairs);
    CHECK(q.available && Near(q.amount/200,.4));
}
static void cancelled_assignment_keeps_target_and_excludes_post_close_overhead() {
    auto v=Vehicle();auto office=Goods();office.count=1;office.rows[0]=Goods().rows[2];
    v.Observe(Goods(),Fuel(v.root,.8f),1,1000);
    v.Observe(Goods(),Fuel(v.root,.7f),1,2000);
    const auto r=v.Observe(office,Fuel(v.root,.6f),1,3000);
    CHECK(r.count==1 && r.events[0].candidate.originalTarget==0x20000);
    CHECK(Near(r.events[0].candidate.sampledDecrease,.2));
    v.Observe(office,Fuel(v.root,.4f),1,4000);
    CHECK(!v.ledger.Active() && Near(v.observedDecrease,.4));
    auto next=Goods();next.rows[1].target=0xB0000;
    v.Observe(next,Fuel(v.root,.3f),1,5000);
    CHECK(v.ledger.Current().id==2 && v.ledger.Current().sampledDecrease==0);
    CHECK(v.ledger.Current().originalTarget==0xB0000);
}
static void mixed_object_rise_does_not_mask_another_object_drop() {
    auto v=Vehicle();auto f=Fuel(v.root,.8f);f.count=2;
    f.objects[1]=0x90000;f.metadata[1]=0xA0000;f.raw[1]=.7f;f.capacity[1]=1;
    v.Observe(Goods(),f,1,1000);f.raw[0]=.9f;f.raw[1]=.5f;
    v.Observe(Goods(),f,1,2000);
    CHECK(Near(v.ledger.Current().sampledDecrease,.2));
    CHECK(Near(v.ledger.Current().sampledIncrease,.1));
    CHECK(Near(v.observedDecrease,.2) && Near(v.observedIncrease,.1));
}
static void invalid_schedule_skips_assignment_gap_but_keeps_earlier_drops() {
    auto v=Vehicle();v.Observe(Goods(),Fuel(v.root,.8f),1,1000);
    v.Observe(Goods(),Fuel(v.root,.7f),1,2000);
    v.Observe(sch::Snapshot{},Fuel(v.root,.1f),1,3000);
    v.Observe(Goods(),Fuel(v.root,.05f),1,4000);
    v.Observe(Goods(),Fuel(v.root,.03f),1,5000);
    const auto& c=v.ledger.Current();
    CHECK(c.observationGap && Near(c.sampledDecrease,.12) && c.sampledPairs==2);
    CHECK(Near(v.observedDecrease,.77)); // Valid vehicle-only samples, not allocated to task.
}
static void invalid_fuel_and_changed_receiver_rebase_without_erasing_drops() {
    auto v=Vehicle();v.Observe(Goods(),Fuel(v.root,.8f),1,1000);
    v.Observe(Goods(),Fuel(v.root,.7f),1,2000);
    v.Observe(Goods(),a::NativeFuel{},1,3000);
    v.Observe(Goods(),Fuel(v.root,.2f),1,4000);
    v.Observe(Goods(),Fuel(v.root,.1f),1,5000);
    auto f=Fuel(v.root,.01f);f.metadata[0]=0xB0000;
    v.Observe(Goods(),f,1,6000);f.raw[0]=-.01f;
    v.Observe(Goods(),f,1,7000);
    const auto& c=v.ledger.Current();
    CHECK(c.observationGap && c.receiverChanged && c.fuelUncertain);
    CHECK(Near(c.sampledDecrease,.22) && c.sampledPairs==3);
    CHECK(Near(v.observedDecrease,.22) && v.observedPairs==3);
}
static void replacement_boundary_does_not_charge_new_target_for_old_decreases() {
    auto v=Vehicle();v.Observe(Goods(),Fuel(v.root,.8f),1,1000);
    v.Observe(Goods(),Fuel(v.root,.7f),1,2000);
    auto s=Goods();s.rows[1].target=0xB0000;
    const auto r=v.Observe(s,Fuel(v.root,.6f),1,3000);
    CHECK(r.count==2 && Near(r.events[0].candidate.sampledDecrease,.2));
    CHECK(r.events[0].candidate.originalTarget==0x20000);
    CHECK(r.events[1].candidate.originalTarget==0xB0000);
    CHECK(r.events[1].candidate.sampledDecrease==0 && r.events[1].candidate.sampledPairs==0);
}
static void runtime_caches_one_quote_for_all_vehicles_and_resets_on_load() {
    PrepareRuntime(2);SetupFleetPrice();FleetObserverTick(nullptr,1,1);
    CHECK(fleetPriceCalls==1 && fleetLookups==1);
    for(unsigned i=0;i<29;++i) {clockMs+=1000;FleetObserverTick(nullptr,1,1);}
    CHECK(fleetPriceCalls==1);
    clockMs+=1000;FleetObserverTick(nullptr,1,1);CHECK(fleetPriceCalls==2);
    InterlockedIncrement(&g_panelEpoch);clockMs+=1000;FleetObserverTick(nullptr,1,1);
    CHECK(fleetPriceCalls==3 && g_fleetVehicles[0].observedPairs==0);
}
static void unavailable_price_is_not_a_zero_estimate_and_true_zero_is_available() {
    PrepareRuntime();FleetRefreshPrice(clockMs);
    CHECK(!FleetApproximatePrice(.2,1).available);
    SetupFleetPrice();g_fleetPriceCached=false;FleetRefreshPrice(clockMs);
    CHECK(!FleetApproximatePrice(0,0).available);
    auto q=FleetApproximatePrice(0,1);CHECK(q.available && q.amount==0);
    for(double value:{-1.0,std::numeric_limits<double>::infinity(),std::numeric_limits<double>::quiet_NaN()})
        CHECK(!FleetApproximatePrice(value,1).available);
    CHECK(!FleetApproximatePrice(std::numeric_limits<double>::max(),1).available);
    for(float value:{0.f,-1.f,std::numeric_limits<float>::infinity(),std::numeric_limits<float>::quiet_NaN()}) {
        fleetUnitPrice=value;g_fleetPriceCached=false;FleetRefreshPrice(clockMs);
        CHECK(!FleetApproximatePrice(.2,1).available);
    }
}
static void partial_large_discovery_keeps_visible_vehicle_pairs_and_throttles_warnings() {
    PrepareRuntime();SetupFleetPrice();
    // Many legitimate non-road entries plus our road office; only the global bound is incomplete.
    Put(0x60000+0x318,Ptr(0x61000));Put(0x61000+0x360,52);
    owners.resize(FleetEnterpriseLimit+1,(void*)Ptr(0x60000));
    FleetObserverTick(nullptr,1,1);CHECK(!g_fleetDiscoveryComplete);
    const auto firstLogs=logs.size();
    Put(0x30000+0x5F0,.4f);clockMs+=1000;FleetObserverTick(nullptr,1,1);
    CHECK(g_fleetVehicles[0].observedDecrease>0 && g_fleetVehicles[0].observedPairs==1);
    CHECK(!g_fleetVehicles[0].discontinuities && logs.size()==firstLogs);
    for(unsigned i=0;i<29;++i) {clockMs+=1000;FleetObserverTick(nullptr,1,1);}
    unsigned warnings=0;for(const auto& log:logs)
        if(log.find("[FLEET COVERAGE]")!=std::string::npos) ++warnings;
    CHECK(warnings==2 && fleetPriceCalls==2);
}
static void assignment_logs_label_refill_estimate_without_charging_money() {
    PrepareRuntime();SetupFleetPrice();FleetRefreshPrice(clockMs);
    auto v=Vehicle();auto empty=Goods();empty.count=0;
    v.Observe(empty,Fuel(v.root,.9f),1,1000);
    v.Observe(Goods(),Fuel(v.root,.8f),1,2000);
    v.Observe(Goods(),Fuel(v.root,.6f),1,3000);
    v.Observe(Goods(),Fuel(v.root,.9f),1,4000);
    FleetEvents(v.Observe(empty,Fuel(v.root,.7f),1,5000));
    CHECK(logs.size()==1);const auto& log=logs.back();
    for(const char* text:{"observed_close","compared_pairs=3","estimate_partial=1",
        "estimated_cost_available=1","estimate_reason=approximate_positive_drops",
        "price_basis=current_cached_30s","quantity_assumption=native_tonnes_provisional",
        "exact_burn=0","settlement=none money_moved=0"}) CHECK(log.find(text)!=std::string::npos);
    CHECK(fleetPriceCalls==1 && fleetLookups==1);
}
static void backward_clock_skips_pair_and_rebases() {
    PrepareRuntime();FleetObserverTick(nullptr,1,1);
    clockMs+=1000;Put(0x30000+0x5F0,.4f);FleetObserverTick(nullptr,1,1);
    const double drop=g_fleetVehicles[0].observedDecrease;
    clockMs=1500;Put(0x30000+0x5F0,.1f);FleetObserverTick(nullptr,1,1);
    CHECK(g_fleetVehicles[0].observedDecrease==drop && !g_fleetVehicles[0].previousValid);
    clockMs+=1000;FleetObserverTick(nullptr,1,1);
    clockMs+=1000;Put(0x30000+0x5F0,.05f);FleetObserverTick(nullptr,1,1);
    CHECK(Near(g_fleetVehicles[0].observedDecrease,drop+.05));
}
static void newly_duplicate_or_missing_identity_is_retired_despite_partial_discovery() {
    PrepareRuntime();FleetObserverTick(nullptr,1,1);
    clockMs+=1000;Put(0x30000+0x5F0,.4f);FleetObserverTick(nullptr,1,1);
    CHECK(g_fleetVehicles[0].observedPairs==1);
    owners.push_back(owners[0]);clockMs+=1000;FleetObserverTick(nullptr,1,1);
    CHECK(!g_fleetDiscoveryComplete && !g_fleetVehicles[0].root);
    owners.resize(1);clockMs+=1000;FleetObserverTick(nullptr,1,1);
    CHECK(g_fleetVehicles[0].sample==1 && g_fleetVehicles[0].observedPairs==0);
    owners.clear();clockMs+=1000;FleetObserverTick(nullptr,1,1);
    CHECK(!g_fleetVehicles[0].root);
}
static size_t ChangingEnterpriseCount() {
    static unsigned calls=0;return owners.size()+(++calls%2==0 ? 1 : 0);
}
static void changing_roster_still_breaks_baselines_unlike_capacity_only() {
    PrepareRuntime();FleetObserverTick(nullptr,1,1);
    clockMs+=1000;Put(0x30000+0x5F0,.4f);FleetObserverTick(nullptr,1,1);
    const double drop=g_fleetVehicles[0].observedDecrease;
    enterprises.getEnterpriseCount=ChangingEnterpriseCount;
    clockMs+=1000;Put(0x30000+0x5F0,.1f);FleetObserverTick(nullptr,1,1);
    CHECK(!g_fleetDiscoveryComplete && !g_fleetVehicles[0].previousValid);
    CHECK(g_fleetVehicles[0].sample==2 && g_fleetVehicles[0].observedDecrease==drop);
    enterprises.getEnterpriseCount=EnterpriseCount;
    clockMs+=1000;FleetObserverTick(nullptr,1,1);
    CHECK(g_fleetVehicles[0].observedDecrease==drop);
    clockMs+=1000;Put(0x30000+0x5F0,.05f);FleetObserverTick(nullptr,1,1);
    CHECK(Near(g_fleetVehicles[0].observedDecrease,drop+.05));
}
