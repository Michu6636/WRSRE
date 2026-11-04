#include <cassert>
#include <cmath>
#include <cstdio>
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
        assert(near(s.treasury_delta + s.enterprise_delta + s.net_outstanding_principal_delta, s.interest_transfer));
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
        assert(near(s.treasury_delta + s.enterprise_delta + s.net_outstanding_principal_delta, s.interest_transfer));
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

    // (d) USD/RUB scaling
    {
        LoanBook rub;
        rub.config.currencyScale = 1.0;
        LoanKey k; k.objectName = "entE";
        double a1 = rub.requestCreditDraw(k, 1000.0, 1);
        assert(near(a1, 1000.0));

        LoanBook usd;
        usd.config.currencyScale = 90.0; // 1 USD = 90 RUB scale? amount scaled
        LoanKey k2; k2.objectName = "entF";
        // with scale, credit limit and cap scale; request 1000 (active units) -> allowed up to scaled cap
        double a2 = usd.requestCreditDraw(k2, 1000.0, 1);
        assert(a2 > 0.0);
        // debt stored scaled
        double p = 0, mi = 0; int m = 0;
        usd.getEnterpriseDebt(k2, p, mi, m);
        assert(near(p, 1000.0)); // stored in active units
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