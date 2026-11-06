#ifndef TSM_REPUBLIC_BANK_API_H
#define TSM_REPUBLIC_BANK_API_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TSM_SERVICE_REPUBLIC_BANK "republic_bank"
#define TSM_REPUBLIC_BANK_VERSION 1u

typedef struct TsmRepublicBankApi {
    unsigned structSize;

    // Checks creditworthiness, debt cap and solvency; advances principal into
    // enterprise capital and debits the state treasury by the approved amount.
    // Returns the approved (possibly zero) amount.
    double (*requestCreditDraw)(void* building, double neededAmount, const char* reason);

    // Debits enterprise capital and credits the state treasury.
    void (*repayPrincipal)(void* building, double amount);

    // Reads outstanding principal, monthly interest and missed payments.
    void (*getEnterpriseDebt)(void* building, double* outPrincipal,
                               double* outMonthlyInterest, int* outMissedPayments);

    double (*getAnnualInterestRate)(void);
    void   (*setAnnualInterestRate)(double rate);

} TsmRepublicBankApi;

#ifdef __cplusplus
}
#endif

#endif // TSM_REPUBLIC_BANK_API_H
