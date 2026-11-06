// Portable reconciliation helpers for state_ledger v2.0.
// Pure C++17: no game, Windows, or plugin dependencies.
#ifndef STATE_LEDGER_LEDGER_RECONCILE_H
#define STATE_LEDGER_LEDGER_RECONCILE_H

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <map>
#include <string>
#include <unordered_map>

namespace state_ledger {
namespace reconcile {

inline bool Finite(double x) { return std::isfinite(x); }

struct Stocks {
    double stateTreasury = 0.0;
    double citizenWallets = 0.0;
    double enterpriseCapitalSum = 0.0;
    double investmentPool = 0.0;
    double total() const {
        return stateTreasury + citizenWallets + enterpriseCapitalSum + investmentPool;
    }
};

struct Flows {
    double wages = 0.0;
    double pensions = 0.0;
    double welfare = 0.0;
    double stateStoreSales = 0.0;
    double wholesaleB2B = 0.0;
    double vat = 0.0;
    double profitTax = 0.0;
    double propertyTax = 0.0;
    double other = 0.0;
    double net() const {
        return wages + pensions + welfare + stateStoreSales + wholesaleB2B +
               vat + profitTax + propertyTax + other;
    }
    void clear() { *this = Flows{}; }
};

struct PrivateSummary {
    double sales = 0.0;
    double wages = 0.0;
    double taxes = 0.0;
    double pool = 0.0;
};

inline double SumEnterpriseCapital(const double* values, std::size_t count) {
    double sum = 0.0;
    if (!values) return 0.0;
    for (std::size_t i = 0; i < count; ++i) {
        if (std::isfinite(values[i])) sum += values[i];
    }
    return sum;
}

inline bool TimeRewound(int lastYear, int lastMonth, int year, int month) {
    if (lastYear < 0 || lastMonth < 0) return false;
    return year < lastYear || (year == lastYear && month < lastMonth);
}

inline bool MonthChanged(int lastYear, int lastMonth, int year, int month) {
    if (lastYear < 0 || lastMonth < 0) return false;
    return year != lastYear || month != lastMonth;
}

inline bool IsFirstTick(int lastYear, int lastMonth) {
    return lastYear < 0 || lastMonth < 0;
}

inline double TreasuryUnexplained(double openTreasury, double closeTreasury,
                                  double flowSum) {
    return (closeTreasury - openTreasury) - flowSum;
}

inline double MoneySupplyDelta(const Stocks& openStocks,
                              const Stocks& closeStocks) {
    return closeStocks.total() - openStocks.total();
}

inline double MoneySupplyVelocity(double openTotal, double delta) {
    if (openTotal == 0.0) return 0.0;
    return delta / openTotal;
}

inline double PrivateStateShare(double privateSales, double stateSales) {
    double total = privateSales + stateSales;
    if (total == 0.0) return 0.0;
    return privateSales / total;
}

inline bool DriftExceeds(double unexplained, double tolerance) {
    return std::fabs(unexplained) > tolerance;
}

struct ClosedMonth {
    bool closed = false;
    int year = 0;
    int month = 0;
    Stocks openStocks{};
    Stocks closeStocks{};
    Flows flows{};
    PrivateSummary privateSummary{};
    double treasuryOpen = 0.0;
    double treasuryClose = 0.0;
    double flowSum = 0.0;
    double moneySupplyDelta = 0.0;
    double treasuryUnexplained = 0.0;
};

inline std::string CsvHeader(const char* symbol) {
    const char* sym = (symbol && symbol[0]) ? symbol : "RUB";
    char buf[1024];
    std::snprintf(buf, sizeof(buf),
        "Year,Month,Wages,Pensions,Welfare,StateStoreSales,WholesaleB2B,"
        "VAT,ProfitTax,PropertyTax,Other,NetTreasury,"
        "TreasuryOpen_%s,TreasuryClose_%s,"
        "CitizenWallets,EnterpriseCapitalSum,InvestmentPool,"
        "TotalMoneySupplyOpen,TotalMoneySupplyClose,MoneySupplyDelta,"
        "PrivateSales,PrivateWages,PrivateTaxes,PrivatePool,"
        "TreasuryUnexplained\r\n",
        sym, sym);
    return std::string(buf);
}

inline std::string CsvRow(int year, int month, const Flows& f,
                          double openTreasury, double closeTreasury,
                          const Stocks& openStocks, const Stocks& closeStocks,
                          const PrivateSummary& priv, double treasuryUnexplained) {
    char buf[1024];
    std::snprintf(buf, sizeof(buf),
        "%d,%02d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,"
        "%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n",
        year, month,
        f.wages, f.pensions, f.welfare,
        f.stateStoreSales, f.wholesaleB2B,
        f.vat, f.profitTax, f.propertyTax,
        f.other, f.net(),
        openTreasury, closeTreasury,
        closeStocks.citizenWallets, closeStocks.enterpriseCapitalSum,
        closeStocks.investmentPool,
        openStocks.total(), closeStocks.total(),
        MoneySupplyDelta(openStocks, closeStocks),
        priv.sales, priv.wages, priv.taxes, priv.pool,
        treasuryUnexplained);
    return std::string(buf);
}

} // namespace reconcile

namespace reconcile {

struct LedgerState {
    int lastYear = -1;
    int lastMonth = -1;
    bool firstTick = true;
    Stocks openStocks{};
    double treasuryOpen = 0.0;
    double treasuryLastObserved = 0.0;
    double flowSum = 0.0;
    Flows flows{};
    std::unordered_map<std::string, double> monthFlows;
    std::string currencySymbol = "RUB";
    bool enterpriseAvailable = true;

    void reset() {
        lastYear = -1;
        lastMonth = -1;
        firstTick = true;
        openStocks = Stocks{};
        treasuryOpen = 0.0;
        treasuryLastObserved = 0.0;
        flowSum = 0.0;
        flows = Flows{};
        monthFlows.clear();
    }

    void rewind(int year, int month, double treasury, const Stocks& stocks) {
        lastYear = year;
        lastMonth = month;
        firstTick = false;
        openStocks = stocks;
        treasuryOpen = treasury;
        treasuryLastObserved = treasury;
        flowSum = 0.0;
        flows = Flows{};
        monthFlows.clear();
    }

    void recordFlow(const std::string& category, double amount) {
        monthFlows[category] += amount;
        flowSum += amount;
    }

    ClosedMonth observe(int year, int month, double treasury, const Stocks& stocks,
                        const PrivateSummary& priv) {
        ClosedMonth out;
        if (year < 0 || month < 0) return out;

        if (firstTick) {
            firstTick = false;
            lastYear = year;
            lastMonth = month;
            openStocks = stocks;
            treasuryOpen = treasury;
            treasuryLastObserved = treasury;
            return out;
        }

        if (TimeRewound(lastYear, lastMonth, year, month)) {
            rewind(year, month, treasury, stocks);
            return out;
        }

        if (month != lastMonth || year != lastYear) {
            out.closed = true;
            out.year = lastYear;
            out.month = lastMonth;
            out.openStocks = openStocks;
            out.closeStocks = stocks;
            out.treasuryOpen = treasuryOpen;
            out.treasuryClose = treasury;
            out.flowSum = flowSum;
            out.privateSummary = priv;

            Flows f{};
            auto val = [&](const char* k) -> double {
                auto it = monthFlows.find(k);
                return (it == monthFlows.end()) ? 0.0 : it->second;
            };
            f.wages = val("wage");
            f.pensions = val("pension");
            f.welfare = val("welfare");
            f.stateStoreSales = val("state_store_sale");
            f.wholesaleB2B = val("wholesale_b2b");
            f.vat = val("vat");
            f.profitTax = val("profit_tax");
            f.propertyTax = val("property_tax");
            double known = f.wages + f.pensions + f.welfare + f.stateStoreSales +
                           f.wholesaleB2B + f.vat + f.profitTax + f.propertyTax;
            double totalSum = 0.0;
            for (auto& kv : monthFlows) totalSum += kv.second;
            f.other = totalSum - known;
            out.flows = f;
            out.moneySupplyDelta = MoneySupplyDelta(openStocks, stocks);
            out.treasuryUnexplained =
                TreasuryUnexplained(treasuryOpen, treasury, flowSum);

            openStocks = stocks;
            treasuryOpen = treasury;
            treasuryLastObserved = treasury;
            flowSum = 0.0;
            flows = Flows{};
            monthFlows.clear();
            lastYear = year;
            lastMonth = month;
        } else {
            treasuryLastObserved = treasury;
        }
        return out;
    }
};

} // namespace reconcile

// Backward-compatible aliases without the reconcile namespace.
using reconcile::Stocks;
using reconcile::Flows;
using reconcile::PrivateSummary;
using reconcile::ClosedMonth;
using reconcile::LedgerState;

} // namespace state_ledger
#endif
