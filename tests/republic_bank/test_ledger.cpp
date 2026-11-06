#include <cassert>
#include <cmath>
#include <cstdio>
#include <limits>
#include <string>
#include "bank_ledger.h"

using namespace republic_bank;

static bool near(double a, double b, double eps = 1e-6) {
    return std::fabs(a - b) <= eps;
}

int main() {
    // (a) exact accounting identity on paid monthly settlement
    {
        LoanBook book;
        book.config.cashBufferKeep = 0.0;
        LoanKey k; k.objectName = "entA"; k.qx = 1; k.qz = 2;
        // draw 1000
        double approved = book.requestCreditDraw(k, 1000.0, "test", 1);
        assert(near(approved, 1000.0));
        double capital = 5000.0;
        double beforePrincipal = 0;
        double principal = 0, mi = 0; int missed = 0;
        book.getEnterpriseDebt(k, principal, mi, missed);
        beforePrincipal = principal;
        (void)beforePrincipal;
        Settlement s = book.monthlySettle(k, capital);
        // identity
        assert(near(s.treasury_delta + s.enterprise_delta, 0.0));
        assert(near(s.treasury_delta + s.net_outstanding_principal_delta, s.interest));
        // principal decreased by sweep (all principal if capital enough)
        assert(principal >= s.principal);
    }

    // (a) identity on missed payment (capital insufficient)
    {
        LoanBook book;
        book.config.cashBufferKeep = 1000.0;
        book.config.maxMissedMonths = 3;
        LoanKey k; k.objectName = "entB";
        book.requestCreditDraw(k, 1000.0, 1);
        double capital = 0.0; // cannot pay
        Settlement s = book.monthlySettle(k, capital);
        assert(near(s.treasury_delta + s.enterprise_delta, 0.0));
        assert(near(s.treasury_delta + s.net_outstanding_principal_delta, s.interest));
        assert(s.missedPayments == 1);
    }

    // (b) amortization convergence to 0.00
    {
        LoanBook book;
        book.config.cashBufferKeep = 0.0;
        book.config.annualInterestRate = 0.12;
        LoanKey k; k.objectName = "entC";
        book.requestCreditDraw(k, 1000.0, 1);
        double capital = 10000.0;
        for (int i = 0; i < 100; i++) {
            Settlement s = book.monthlySettle(k, capital);
            (void)s;
            double p = 0, mi = 0; int m = 0;
            book.getEnterpriseDebt(k, p, mi, m);
            if (p <= 1e-6) { p = 0.0; break; }
        }
        double p = 0, mi = 0; int m = 0;
        book.getEnterpriseDebt(k, p, mi, m);
        assert(near(p, 0.0, 1e-6));
    }

    // (c) default freeze after max_missed_months
    {
        LoanBook book;
        book.config.maxMissedMonths = 2;
        book.config.cashBufferKeep = 1000.0;
        LoanKey k; k.objectName = "entD";
        book.requestCreditDraw(k, 500.0, 1);
        double capital = 0.0;
        book.monthlySettle(k, capital);
        book.monthlySettle(k, capital);
        assert(book.isInDefault(k));
        // credit draw refused
        double approved = book.requestCreditDraw(k, 100.0, 1);
        assert(near(approved, 0.0));
    }

    // (d) CPI indexation handles inflation, deflation and neutral USD.
    {
        LoanBook rub;
        rub.config.currencyScale = BankEconomicScale(2.15, 2.15);
        LoanKey k; k.objectName = "entE";
        assert(near(rub.config.currencyScale, 1.0));
        assert(near(rub.requestCreditDraw(k, 1000.0, 1), 1000.0));

        LoanBook usd;
        // 2.15 RUB / 50 = 0.043 USD, so the reference-RUB 50,000 draw
        // limit becomes 1,000 USD without a second hardcoded nominal cap.
        usd.config.currencyScale = BankEconomicScale(0.043, 2.15);
        LoanKey k2; k2.objectName = "entF";
        assert(near(usd.config.currencyScale, 0.02));
        assert(near(usd.requestCreditDraw(k2, 2000.0, 1), 1000.0));
        double p = 0, mi = 0; int m = 0;
        usd.getEnterpriseDebt(k2, p, mi, m);
        assert(near(p, 1000.0));

        LoanBook inflated;
        inflated.config.currencyScale = BankEconomicScale(4.30, 2.15);
        LoanKey ki; ki.objectName = "inflated";
        assert(near(inflated.config.currencyScale, 2.0));
        assert(near(inflated.requestCreditDraw(ki, 150000.0, 1), 100000.0));

        LoanBook deflated;
        deflated.config.currencyScale = BankEconomicScale(1.075, 2.15);
        LoanKey kd; kd.objectName = "deflated";
        assert(near(deflated.config.currencyScale, 0.5));
        assert(near(deflated.requestCreditDraw(kd, 30000.0, 1), 25000.0));

        const double inf = std::numeric_limits<double>::infinity();
        assert(near(BankEconomicScale(inf, 2.15, 0.02), 0.02));
        assert(near(BankEconomicScale(2.15, 0.0, 0.02), 0.02));
    }

    // (d2) The retained cash buffer uses the same live scale as credit caps.
    {
        LoanBook book;
        book.config.annualInterestRate = 0.0;
        book.config.cashBufferKeep = 1000.0;
        book.config.currencyScale = 2.0;
        LoanKey k; k.objectName = "buffer";
        assert(near(book.requestCreditDraw(k, 1000.0, 1), 1000.0));
        double capital = 2500.0;
        Settlement s = book.monthlySettle(k, capital);
        assert(near(s.principal_sweep, 500.0)); // retain 2,000, not 1,000
        assert(near(capital, 2000.0));
        assert(near(s.principal, 500.0));
    }

    // (d3) The native 0..100 slider covers 0..25% in 1% increments.
    // Off-grid native drags snap to that same grid, including the exact
    // positions observed in the two-save runtime test.
    {
        assert(BankRateToSlider(0.0) == 0);
        assert(BankRateToSlider(0.06) == 24);
        assert(BankRateToSlider(0.25) == 100);
        assert(BankRateToSlider(1.0) == 100);
        assert(BankRateSnapSlider(0) == 0);
        assert(BankRateSnapSlider(1) == 0);
        assert(BankRateSnapSlider(2) == 4);
        assert(BankRateSnapSlider(15) == 16);
        assert(BankRateSnapSlider(31) == 32);
        assert(BankRateSnapSlider(94) == 96);
        assert(BankRateSnapSlider(98) == 100);
        assert(BankRateSnapSlider(100) == 100);
        assert(BankRateSnapSlider(101) == 100);
        assert(near(BankRateFromSlider(15), 0.04));
        assert(near(BankRateFromSlider(28), 0.07));
        assert(near(BankRateFromSlider(31), 0.08));
        assert(near(BankRateFromSlider(94), 0.24));
        assert(near(BankRateFromSlider(98), 0.25));
        assert(near(BankRateFromSlider(100), 0.25));
        assert(near(BankRateFromSlider(101), 0.25));
        assert(near(BankRateFromSlider(BankRateToSlider(0.06)), 0.06));

        // v1.1 saves could contain these fractional rates beneath an integer
        // caption. Loading through the normaliser preserves the visible rate.
        assert(near(NormalizeBankRate(0.0375), 0.04));
        assert(near(NormalizeBankRate(0.0775), 0.08));
        assert(near(NormalizeBankRate(0.235), 0.24));
        assert(near(NormalizeBankRate(0.245), 0.25));
    }

    // (e) save/load roundtrip integrity
    {
        LoanBook book;
        LoanKey k; k.objectName = "entG"; k.qx = 5; k.qz = 7;
        book.requestCreditDraw(k, 1234.5, 1);
        book.config.annualInterestRate = 0.09;
        std::string s = book.serialize();
        LoanBook book2;
        assert(book2.deserialize(s));
        double p1=0,p2=0,mi1=0,mi2=0; int m1=0,m2=0;
        book.getEnterpriseDebt(k,p1,mi1,m1);
        book2.getEnterpriseDebt(k,p2,mi2,m2);
        assert(near(p1,p2));
        assert(near(book.getAnnualInterestRate(), book2.getAnnualInterestRate()));
        // bytes roundtrip
        auto bytes = book.saveBytes();
        LoanBook book3;
        assert(book3.loadBytes(bytes));
        double p3=0,mi3=0; int m3=0;
        book3.getEnterpriseDebt(k,p3,mi3,m3);
        assert(near(p1,p3));
    }

    printf("ALL TESTS PASSED\n");
    return 0;
}