#ifndef REPUBLIC_BANK_LEDGER_H
#define REPUBLIC_BANK_LEDGER_H

#include <cmath>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace republic_bank {

struct Config {
    double annualInterestRate = 0.06;
    double maxDebtPerEnterprise = 100000.0;
    double creditLimit = 50000.0;
    double cashBufferKeep = 1000.0;
    int    maxMissedMonths = 3;
    int    autoOverdraftEnabled = 0;
    int    solvencyStreakRequired = 0;
    // Runtime-derived multiplier for the three nominal limits above.  The
    // plugin sets this to live CPI / reference CPI; because resource_pricing
    // reports CPI in the active currency, this includes both price indexation
    // and RUB/USD conversion.  It is not a user-configured exchange rate.
    double currencyScale = 1.0;
};

// Native ui_panels sliders span 0..100.  Spread the bank's 0..25% policy
// range over that full span: four slider units are one percentage point.
// The native -/+ buttons honour that step, but dragging can produce any
// integer.  Snap drag results back to the same grid so the whole-percent
// caption, saved rate and interest calculation can never disagree.
inline unsigned BankRateToSlider(double annualRate) {
    if (!std::isfinite(annualRate) || annualRate <= 0.0) return 0;
    if (annualRate >= 0.25) return 100;
    return (unsigned)std::lround(annualRate * 400.0);
}

inline unsigned BankRateSnapSlider(unsigned value) {
    if (value > 100) value = 100;
    // Round to the nearest four-unit increment; exact half steps round up.
    return ((value + 2) / 4) * 4;
}

inline double BankRateFromSlider(unsigned value) {
    return (double)BankRateSnapSlider(value) / 400.0;
}

// Also migrate rates written by v1.1 after an off-grid drag (for example,
// slider 15 meant 3.75% while the caption said 4%).  Normalising on load
// preserves the user's visible whole-percent choice.
inline double NormalizeBankRate(double annualRate) {
    return BankRateFromSlider(BankRateToSlider(annualRate));
}

// Configured bank amounts are reference-RUB values. A CPI of 2.15 RUB/day
// therefore gives scale 1; 4.30 gives 2 (inflation), 1.075 gives 0.5
// (deflation). D34: an absent or invalid basket answers 0.0 = UNAVAILABLE -
// there is no neutral fallback scale.
inline double BankEconomicScale(double cpiDailyBasket,
                                double referenceCpiDailyBasket) {
    if (!std::isfinite(cpiDailyBasket) || cpiDailyBasket <= 0.0 ||
        !std::isfinite(referenceCpiDailyBasket) ||
        referenceCpiDailyBasket <= 0.0)
        return 0.0;
    const double scale = cpiDailyBasket / referenceCpiDailyBasket;
    return (std::isfinite(scale) && scale > 0.0) ? scale : 0.0;
}

struct LoanKey {
    std::string objectName;
    int qx = 0;
    int qz = 0;
    bool operator<(const LoanKey& o) const {
        if (objectName != o.objectName) return objectName < o.objectName;
        if (qx != o.qx) return qx < o.qx;
        return qz < o.qz;
    }
    bool operator==(const LoanKey& o) const {
        return objectName == o.objectName && qx == o.qx && qz == o.qz;
    }
};

struct Loan {
    LoanKey key;
    double principal = 0.0;
    int    missedMonths = 0;
    int    inDefault = 0;
};

// A "ptr:<address>" key is session-only: the address changes when the world
// reloads, so no debt may be opened against one and serialization skips it.
inline bool IsSessionOnlyLoanKey(const LoanKey& k) {
    return k.objectName.rfind("ptr:", 0) == 0;
}

struct Settlement {
    double treasury_delta = 0.0;
    double enterprise_delta = 0.0;
    double net_outstanding_principal_delta = 0.0;
    double interest_transfer = 0.0;
    double principal_sweep = 0.0;
    double interest = 0.0;
    double principal = 0.0;
    int    missedPayments = 0;
    int    missedMonths = 0;
    int    inDefault = 0;
    int    defaulted = 0;

    // camelCase mirrors
    double treasuryDelta = 0.0;
    double enterpriseDelta = 0.0;
    double netOutstandingPrincipalDelta = 0.0;
    double interestTransfer = 0.0;
    double principalSweep = 0.0;
};

class LoanBook {
public:
    Config config;
    std::map<LoanKey, Loan> loans;

    double getAnnualInterestRate() const { return config.annualInterestRate; }

    void setAnnualInterestRate(double r) {
        if (r < 0.0) r = 0.0;
        if (r > 0.25) r = 0.25;
        config.annualInterestRate = r;
    }

    static LoanKey makeKey(const std::string& name, double x, double z) {
        LoanKey k;
        k.objectName = name;
        k.qx = (int)std::lround(x * 100.0);
        k.qz = (int)std::lround(z * 100.0);
        return k;
    }

    bool isInDefault(const LoanKey& key) const {
        auto it = loans.find(key);
        if (it == loans.end()) return false;
        return it->second.inDefault != 0;
    }

    double requestCreditDraw(const LoanKey& key, double neededAmount, int solvencyStreak) {
        if (IsSessionOnlyLoanKey(key)) return 0.0;
        if (neededAmount < 0.0) neededAmount = 0.0;
        Loan& L = loans[key];
        L.key = key;
        if (L.inDefault) return 0.0;
        if (solvencyStreak < config.solvencyStreakRequired) return 0.0;
        double cap = config.maxDebtPerEnterprise * config.currencyScale;
        double avail = cap - L.principal;
        if (avail < 0.0) avail = 0.0;
        double creditLim = config.creditLimit * config.currencyScale;
        if (neededAmount > creditLim) neededAmount = creditLim;
        if (neededAmount > avail) neededAmount = avail;
        if (neededAmount < 0.0) neededAmount = 0.0;
        L.principal += neededAmount;
        return neededAmount;
    }

    double requestCreditDraw(const LoanKey& key, double neededAmount,
                            const char* /*reason*/, int solvencyStreak) {
        return requestCreditDraw(key, neededAmount, solvencyStreak);
    }

    void repayPrincipal(const LoanKey& key, double amount) {
        if (IsSessionOnlyLoanKey(key)) return;
        auto it = loans.find(key);
        if (it == loans.end()) return;
        if (amount < 0.0) amount = 0.0;
        if (amount > it->second.principal) amount = it->second.principal;
        it->second.principal -= amount;
    }

    bool getEnterpriseDebt(const LoanKey& key, double& principal,
                           double& monthlyInterest, int& missedPayments) const {
        if (IsSessionOnlyLoanKey(key)) {
            principal = 0.0; monthlyInterest = 0.0; missedPayments = 0;
            return false;
        }
        auto it = loans.find(key);
        if (it == loans.end()) {
            principal = 0.0; monthlyInterest = 0.0; missedPayments = 0;
            return false;
        }
        principal = it->second.principal;
        monthlyInterest = principal * (config.annualInterestRate / 12.0);
        missedPayments = it->second.missedMonths;
        return true;
    }

    Settlement monthlySettle(const LoanKey& key, double& enterpriseCapital) {
        Settlement s;
        if (IsSessionOnlyLoanKey(key)) {
            sync(s);
            return s;
        }
        Loan& L = loans[key];
        L.key = key;
        if (L.inDefault) {
            s.inDefault = 1; s.defaulted = 1;
            s.missedPayments = L.missedMonths; s.missedMonths = L.missedMonths;
            s.principal = L.principal;
            sync(s);
            return s;
        }
        double monthlyRate = config.annualInterestRate / 12.0;
        double interest = L.principal * monthlyRate;
        double beforePrincipal = L.principal;
        double sweep = 0.0;
        const double cashBuffer = config.cashBufferKeep * config.currencyScale;
        if (enterpriseCapital > cashBuffer) {
            sweep = enterpriseCapital - cashBuffer;
            if (sweep > L.principal) sweep = L.principal;
            if (sweep < 0.0) sweep = 0.0;
        }
        double total = interest + sweep;
        if (enterpriseCapital + 1e-9 >= total && total >= 0.0) {
            enterpriseCapital -= total;
            L.principal -= sweep;
            if (L.principal < 0.0) L.principal = 0.0;
            L.missedMonths = 0;
            s.treasury_delta = total;
            s.enterprise_delta = -total;
            s.net_outstanding_principal_delta = L.principal - beforePrincipal;
            s.interest_transfer = interest;
            s.principal_sweep = sweep;
            s.interest = interest;
        } else {
            L.principal += interest;
            L.missedMonths++;
            if (L.missedMonths >= config.maxMissedMonths) L.inDefault = 1;
            s.treasury_delta = 0.0;
            s.enterprise_delta = 0.0;
            s.net_outstanding_principal_delta = L.principal - beforePrincipal;
            s.interest_transfer = 0.0;
            s.principal_sweep = 0.0;
            s.interest = interest;
        }
        s.principal = L.principal;
        s.missedPayments = L.missedMonths;
        s.missedMonths = L.missedMonths;
        s.inDefault = L.inDefault;
        s.defaulted = L.inDefault;
        sync(s);
        return s;
    }

    Settlement settleMonth(const LoanKey& key, double& enterpriseCapital) {
        return monthlySettle(key, enterpriseCapital);
    }

    double dailyOverdraftRescue(const LoanKey& key, double& enterpriseCapital,
                               int solvencyStreak) {
        if (!config.autoOverdraftEnabled) return 0.0;
        if (IsSessionOnlyLoanKey(key)) return 0.0;
        Loan& L = loans[key];
        L.key = key;
        if (L.inDefault) return 0.0;
        if (solvencyStreak < config.solvencyStreakRequired) return 0.0;
        if (enterpriseCapital >= 0.0) return 0.0;
        double needed = -enterpriseCapital;
        double creditLim = config.creditLimit * config.currencyScale;
        if (needed > creditLim) needed = creditLim;
        if (needed < 0.0) needed = 0.0;
        double approved = requestCreditDraw(key, needed, solvencyStreak);
        if (approved > 0.0) enterpriseCapital += approved;
        return approved;
    }

    std::string serialize() const {
        std::string rows;
        size_t written = 0;
        for (const auto& kv : loans) {
            const Loan& L = kv.second;
            // A pointer key is session-only (its address changes when the
            // world reloads). Persisting it would orphan the debt, so it
            // is deliberately not written.
            if (IsSessionOnlyLoanKey(L.key)) continue;
            rows += "loan|" + L.key.objectName + "|" +
                    std::to_string(L.key.qx) + "|" +
                    std::to_string(L.key.qz) + "|" +
                    std::to_string(L.principal) + "|" +
                    std::to_string(L.missedMonths) + "|" +
                    std::to_string(L.inDefault) + "\n";
            written++;
        }
        std::string out;
        out += "REPUBLIC_BANK_LEDGER v1\n";
        out += "rate=" + std::to_string(config.annualInterestRate) + "\n";
        out += "scale=" + std::to_string(config.currencyScale) + "\n";
        out += "count=" + std::to_string(written) + "\n";
        out += rows;
        return out;
    }

    bool deserialize(const std::string& data) {
        loans.clear();
        std::vector<std::string> lines;
        size_t start = 0;
        while (true) {
            size_t nl = data.find('\n', start);
            if (nl == std::string::npos) {
                if (start < data.size()) lines.push_back(data.substr(start));
                break;
            }
            lines.push_back(data.substr(start, nl - start));
            start = nl + 1;
        }
        for (const auto& line : lines) {
            if (line.rfind("rate=", 0) == 0) {
                try { config.annualInterestRate = std::stod(line.substr(5)); }
                catch (...) {}
            } else if (line.rfind("scale=", 0) == 0) {
                try { config.currencyScale = std::stod(line.substr(6)); }
                catch (...) {}
            } else if (line.rfind("loan|", 0) == 0) {
                std::string rest = line.substr(5);
                std::vector<std::string> parts;
                size_t cur = 0;
                while (true) {
                    size_t bar = rest.find('|', cur);
                    if (bar == std::string::npos) {
                        parts.push_back(rest.substr(cur));
                        break;
                    }
                    parts.push_back(rest.substr(cur, bar - cur));
                    cur = bar + 1;
                }
                if (parts.size() >= 6) {
                    Loan L;
                    L.key.objectName = parts[0];
                    try {
                        L.key.qx = std::stoi(parts[1]);
                        L.key.qz = std::stoi(parts[2]);
                        L.principal = std::stod(parts[3]);
                        L.missedMonths = std::stoi(parts[4]);
                        L.inDefault = std::stoi(parts[5]);
                    } catch (...) {
                        continue;
                    }
                    // Old builds could persist a pointer key (address
                    // changes on reload), and pre-repair builds could
                    // persist a name with no coordinates. Neither can
                    // match a building in this world, so drop them rather
                    // than keep an unreachable loan. A real key always has
                    // a non-zero coordinate (keyForBuilding requires it).
                    if (IsSessionOnlyLoanKey(L.key)) continue;
                    if (L.key.qx == 0 && L.key.qz == 0) continue;
                    loans[L.key] = L;
                }
            }
        }
        return true;
    }

    std::vector<char> saveBytes() const {
        std::string s = serialize();
        return std::vector<char>(s.begin(), s.end());
    }

    bool loadBytes(const std::vector<char>& b) {
        std::string s(b.begin(), b.end());
        return deserialize(s);
    }

    bool loadBytes(const char* data, size_t len) {
        std::string s(data, data + len);
        return deserialize(s);
    }

private:
    static void sync(Settlement& s) {
        s.treasuryDelta = s.treasury_delta;
        s.enterpriseDelta = s.enterprise_delta;
        s.netOutstandingPrincipalDelta = s.net_outstanding_principal_delta;
        s.interestTransfer = s.interest_transfer;
        s.principalSweep = s.principal_sweep;
    }
};

// free function aliases
inline double getAnnualInterestRate(const LoanBook& book) { return book.getAnnualInterestRate(); }
inline void setAnnualInterestRate(LoanBook& book, double r) { book.setAnnualInterestRate(r); }

} // namespace republic_bank

#endif
