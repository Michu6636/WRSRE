// Real public API layout and production estimator; host shim, NOT a game/DLL test.
#define PANEL_RUNTIME_TEST_NO_MAIN
#include "panel_fuel_runtime_test.cpp"
#include <limits>
namespace a=private_logistics_assignment;
namespace sch=private_logistics_schedule;
namespace p=private_logistics_panel;
static TsmResourcePricingApi prices{};
static TsmCurrencyApi currencies{};
static float unitPrice=200;
static bool resourcePresent=true;
static int lookupCalls=0,priceCalls=0,activeCalls=0,lastCurrency=-1;
static int activeCurrency=TSM_CURRENCY_USD,resourceRecord=0;
static void* FindFuel(const char* name) {
    ++lookupCalls;CHECK(std::strcmp(name,gFuelResource)==0);
    return resourcePresent ? &resourceRecord : nullptr;
}
static float PriceFuel(void* resource,int currency) {
    ++priceCalls;CHECK(resource==&resourceRecord);lastCurrency=currency;return unitPrice;
}
static int ActiveCurrency() {++activeCalls;return activeCurrency;}
static const char* CurrencySymbol() {return "$";}
static void ResetPrices() {
    prices={};prices.structSize=sizeof(prices);prices.getByName=FindFuel;prices.getPrice=PriceFuel;
    currencies={};currencies.structSize=sizeof(currencies);
    currencies.getActiveCurrency=ActiveCurrency;currencies.getCurrencySymbol=CurrencySymbol;
    g_pricing=&prices;g_currency=nullptr;unitPrice=200;resourcePresent=true;
    lookupCalls=priceCalls=activeCalls=0;lastCurrency=-1;activeCurrency=TSM_CURRENCY_USD;
}
static void pricing_live_resource_and_currency();
static void pricing_unavailable_interfaces();
static void pricing_invalid_numbers_and_hypotheses();
static void native_copy_rejects_missing_and_preserves_signed();
static void runtime_closing_boundary_prices_native_not_ui();
static void runtime_quiet_rise_suppresses_hypothesis();
int main() {
    // This harness intentionally exercises only deferred estimation, not hook installation.
    (void)&PanelObserverStart;(void)&PanelObserverLoading;
#define RUN(name) name();std::puts("PASS " #name)
    RUN(pricing_live_resource_and_currency);
    RUN(pricing_unavailable_interfaces);
    RUN(pricing_invalid_numbers_and_hypotheses);
    RUN(native_copy_rejects_missing_and_preserves_signed);
    RUN(runtime_closing_boundary_prices_native_not_ui);
    RUN(runtime_quiet_rise_suppresses_hypothesis);
#undef RUN
    std::puts("6 endpoint runtime scenarios PASS (real API layout, mocked services; no game proof)");
    return 0;
}
static a::Event Closed() {a::Event event{};event.kind=a::Kind::Closed;return event;}
static a::Endpoint Falling() {a::Endpoint e{};e.valid=true;e.net=-.25;return e;}
static void pricing_unavailable_interfaces() {
    for(unsigned mode=0;mode<5;++mode) {
        ResetPrices();
        if(mode==0) g_pricing=nullptr;
        if(mode==1) prices.structSize=offsetof(TsmResourcePricingApi,getPrice);
        if(mode==2) prices.getByName=nullptr;
        if(mode==3) prices.getPrice=nullptr;
        if(mode==4) resourcePresent=false;
        auto q=PanelHypotheticalPrice(Closed(),Falling());
        CHECK(!q.available && q.amount==0 && priceCalls==0 && lookupCalls==(mode==4 ? 1 : 0));
    }
}
static void pricing_invalid_numbers_and_hypotheses() {
    for(float price:{0.f,-1.f,std::numeric_limits<float>::infinity(),std::numeric_limits<float>::quiet_NaN()}) {
        ResetPrices();unitPrice=price;auto q=PanelHypotheticalPrice(Closed(),Falling());
        CHECK(!q.available && q.amount==0 && priceCalls==1);
    }
    for(unsigned mode=0;mode<6;++mode) {
        ResetPrices();auto e=Falling();auto event=Closed();
        if(mode==0) e.valid=false;
        if(mode==1) e.observedRise=true;
        if(mode==2) e.net=0;
        if(mode==3) e.net=.1;
        if(mode==4) e.net=std::numeric_limits<double>::quiet_NaN();
        if(mode==5) event.kind=a::Kind::Abandoned;
        CHECK(!PanelHypotheticalPrice(event,e).available && lookupCalls==0 && priceCalls==0);
    }
    ResetPrices();auto e=Falling();e.net=-std::numeric_limits<double>::max();
    auto q=PanelHypotheticalPrice(Closed(),e);
    CHECK(!q.available && priceCalls==1 && std::strcmp(q.reason,"invalid_product")==0);
}
static p::Snapshot NativeSnapshot() {
    p::Snapshot s{};s.status=p::Status::Complete;s.root=0x40000;s.count=1;
    auto& o=s.objects[0];o.pointer=s.root;o.metadata=0x50000;o.quantityRead=true;
    o.raw=1;o.capacity=2;s.aggregateValid=true;s.litresCandidate=1000;return s;
}
static void native_copy_rejects_missing_and_preserves_signed() {
    auto s=NativeSnapshot();s.objects[0].raw=-.1f;auto n=PanelNativeFuel(s);
    CHECK(a::ValidFuel(n) && n.raw[0]==-.1f && n.raw[0]!=s.litresCandidate);
    for(unsigned mode=0;mode<5;++mode) {
        s=NativeSnapshot();
        if(mode==0) s.objects[0].quantityRead=false;
        if(mode==1) s.objects[0].pointer++;
        if(mode==2) s.status=p::Status::Unreadable;
        if(mode==3) s.objects[0].capacity=std::numeric_limits<float>::quiet_NaN();
        if(mode==4) s.count=p::MaxObjects+1;
        CHECK(!a::ValidFuel(PanelNativeFuel(s)));
    }
}
struct LogFixture {
    sch::History history;
    PanelRecord record{};
    LogFixture() {
        ResetPrices();logs.clear();g_scheduleLedger=a::Ledger{};
        record.epoch=1;record.snapshot=NativeSnapshot();record.schedule.status=sch::Status::Complete;
        Step();record.schedule.count=3;
        record.schedule.rows[0]={0x10000,true,1,0,1,1};
        record.schedule.rows[1]={0x20000,true,0,1,1,1};
        record.schedule.rows[2]={0x30000,true,0,0,1,1};Step();
    }
    void Step() {
        const auto previous=history.LastValidValues();++record.sequence;record.milliseconds+=1000;
        auto d=history.Observe(record.schedule,1,record.snapshot.root,0x30000,record.sequence,record.milliseconds);
        auto changes=d.endpointCompared ? sch::ClassifyChanges(previous,record.schedule) : sch::FieldChanges{};
        ScheduleAssignmentLog(record,d,changes,0x30000);
    }
    void Level(float raw) {record.snapshot.objects[0].raw=raw;record.snapshot.litresCandidate=raw*1000;Step();}
    void Close(float raw) {record.schedule.count=0;Level(raw);}
};
static void runtime_closing_boundary_prices_native_not_ui() {
    LogFixture f;f.Level(.75f);CHECK(priceCalls==0);f.Close(.5f);
    CHECK(priceCalls==1 && lookupCalls==1 && lastCurrency==TSM_CURRENCY_RUB);
    const auto& line=logs.back();
    CHECK(line.find("event=observed_close")!=std::string::npos);
    CHECK(line.find("net_change_valid=1 net_change_native=-0.5")!=std::string::npos);
    CHECK(line.find("hypothetical_cost_if_no_refill_available=1 hypothetical_cost_if_no_refill=100 ")!=std::string::npos);
    CHECK(line.find("actual_cost=not_computed settlement=none billing=off")!=std::string::npos);
    std::puts(line.c_str()); // Archive actual emitted output, not a reconstructed example.
}
static void runtime_quiet_rise_suppresses_hypothesis() {
    LogFixture f;f.Level(.5f);f.Level(.75f);f.Close(.25f);
    CHECK(priceCalls==0 && lookupCalls==0);
    const auto& line=logs.back();
    CHECK(line.find("net_change_valid=1 net_change_native=-0.75")!=std::string::npos);
    CHECK(line.find("observed_level_rise=1")!=std::string::npos);
    CHECK(line.find("hypothetical_cost_reason=observed_level_rise")!=std::string::npos);
    std::puts(line.c_str());
}
static void pricing_live_resource_and_currency() {
    ResetPrices();auto q=PanelHypotheticalPrice(Closed(),Falling());
    CHECK(q.available && q.amount==50 && q.perTonne==200 && q.currency==TSM_CURRENCY_RUB);
    CHECK(lookupCalls==1 && priceCalls==1 && activeCalls==0 && lastCurrency==TSM_CURRENCY_RUB);
    g_currency=&currencies;q=PanelHypotheticalPrice(Closed(),Falling());
    CHECK(q.available && q.amount==50 && q.currency==TSM_CURRENCY_USD && activeCalls==1);
    currencies.structSize=offsetof(TsmCurrencyApi,getCurrencySymbol);
    q=PanelHypotheticalPrice(Closed(),Falling());
    CHECK(q.available && q.currency==TSM_CURRENCY_RUB && activeCalls==1);
    currencies.structSize=0;q=PanelHypotheticalPrice(Closed(),Falling());
    CHECK(q.available && q.currency==TSM_CURRENCY_RUB && activeCalls==1);
    prices.structSize=offsetof(TsmResourcePricingApi,getPrice)+sizeof(prices.getPrice);
    CHECK(PanelHypotheticalPrice(Closed(),Falling()).available);
}
