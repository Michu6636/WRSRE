#include "ledger_reconcile.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

namespace r = state_ledger::reconcile;

#define CHECK(x) do{if(!(x)){std::fprintf(stderr,"FAIL %d: %s\n",__LINE__,#x);std::exit(1);}}while(0)
#define TEST(name) static void name()

TEST(month_rollover_closes_and_resets) {
    r::LedgerState st;
    r::Stocks s0{}; s0.stateTreasury=100; s0.citizenWallets=50; s0.enterpriseCapitalSum=25; s0.investmentPool=10;
    st.firstTick = false; st.lastYear=2026; st.lastMonth=1; st.openStocks=s0; st.treasuryOpen=100;
    st.recordFlow("wage", -10);
    st.recordFlow("vat", 5);
    r::Stocks s1=s0; s1.stateTreasury=95;
    r::PrivateSummary priv{}; priv.sales=200; priv.wages=50; priv.taxes=10; priv.pool=5;
    r::ClosedMonth cm = st.observe(2026, 2, 95, s1, priv);
    CHECK(cm.closed);
    CHECK(cm.year == 2026 && cm.month == 1);
    CHECK(cm.treasuryOpen == 100 && cm.treasuryClose == 95);
    CHECK(cm.flowSum == -5);
    CHECK(std::fabs(cm.treasuryUnexplained - (95-100 - (-5))) < 1e-9);
    CHECK(cm.moneySupplyDelta == (s1.total() - s0.total()));
    CHECK(st.lastMonth == 2);
    CHECK(st.flowSum == 0.0);
}

TEST(save_rewind_resets_baseline) {
    r::LedgerState st;
    r::Stocks s0{}; s0.stateTreasury=100;
    st.firstTick = false; st.lastYear=2026; st.lastMonth=6; st.openStocks=s0; st.treasuryOpen=100;
    st.recordFlow("wage", -10);
    r::Stocks s1{}; s1.stateTreasury=50;
    r::PrivateSummary priv{};
    r::ClosedMonth cm = st.observe(2025, 3, 50, s1, priv);
    CHECK(!cm.closed);
    CHECK(st.lastYear == 2025 && st.lastMonth == 3);
    CHECK(st.treasuryOpen == 50);
    CHECK(st.flowSum == 0.0);
    CHECK(st.monthFlows.empty());
}

TEST(first_tick_no_close) {
    r::LedgerState st;
    r::Stocks s0{}; s0.stateTreasury=10;
    r::PrivateSummary priv{};
    r::ClosedMonth cm = st.observe(2026, 1, 10, s0, priv);
    CHECK(!cm.closed);
    CHECK(st.lastYear == 2026 && st.lastMonth == 1);
}

TEST(usd_rub_header_switch) {
    std::string h1 = r::CsvHeader("USD");
    std::string h2 = r::CsvHeader("RUB");
    CHECK(h1.find("TreasuryOpen_USD") != std::string::npos);
    CHECK(h2.find("TreasuryOpen_RUB") != std::string::npos);
    CHECK(h1.find("TreasuryClose_USD") != std::string::npos);
}

TEST(missing_enterprise_fallback) {
    double caps[3] = {0,0,0};
    CHECK(r::SumEnterpriseCapital(caps, 3) == 0.0);
    r::Stocks s{};
    s.stateTreasury = 10; s.citizenWallets = 5; s.enterpriseCapitalSum = 0; s.investmentPool = 0;
    CHECK(s.total() == 15.0);
    r::Stocks zeroEnt = s;
    CHECK(zeroEnt.enterpriseCapitalSum == 0.0);
}

TEST(drift_detection) {
    double unexp = r::TreasuryUnexplained(100.0, 95.0, -5.0 + 2.0);
    CHECK(r::DriftExceeds(unexp, 1.0));
    double unexp2 = r::TreasuryUnexplained(100.0, 95.0, -5.0);
    CHECK(!r::DriftExceeds(unexp2, 1.0));
}

TEST(money_supply_velocity_and_share) {
    r::Stocks o{}; o.stateTreasury=100; o.citizenWallets=100;
    r::Stocks c{}; c.stateTreasury=110; c.citizenWallets=100;
    double d = r::MoneySupplyDelta(o, c);
    CHECK(std::fabs(d - 10.0) < 1e-9);
    CHECK(std::fabs(r::MoneySupplyVelocity(o.total(), d) - (10.0/200.0)) < 1e-9);
    CHECK(std::fabs(r::PrivateStateShare(30.0, 70.0) - 0.3) < 1e-9);
}

TEST(csv_row_contains_columns) {
    r::Flows f{}; f.wages=-5;
    r::Stocks o{}; o.stateTreasury=100;
    r::Stocks c{}; c.stateTreasury=95; c.citizenWallets=10;
    r::PrivateSummary p{}; p.sales=20;
    std::string row = r::CsvRow(2026,1,f,100,95,o,c,p,0.0);
    CHECK(row.find("2026,01") == 0);
    CHECK(row.size() > 10);
}

int main(int argc, char** argv) {
    // run all TEST functions via registry? The python discovers TEST names and calls binary with name.
    // We need dispatch by argv.
    // Simple: if argc>1, find function by name.
    // Use manual dispatch.
    const char* name = (argc > 1) ? argv[1] : nullptr;
    if (!name) { std::printf("no test name\n"); return 2; }
    // dispatch
    if (std::strcmp(name, "month_rollover_closes_and_resets") == 0) month_rollover_closes_and_resets();
    else if (std::strcmp(name, "save_rewind_resets_baseline") == 0) save_rewind_resets_baseline();
    else if (std::strcmp(name, "first_tick_no_close") == 0) first_tick_no_close();
    else if (std::strcmp(name, "usd_rub_header_switch") == 0) usd_rub_header_switch();
    else if (std::strcmp(name, "missing_enterprise_fallback") == 0) missing_enterprise_fallback();
    else if (std::strcmp(name, "drift_detection") == 0) drift_detection();
    else if (std::strcmp(name, "money_supply_velocity_and_share") == 0) money_supply_velocity_and_share();
    else if (std::strcmp(name, "csv_row_contains_columns") == 0) csv_row_contains_columns();
    else { std::printf("unknown test %s\n", name); return 2; }
    std::printf("PASS %s\n", name);
    return 0;
}
