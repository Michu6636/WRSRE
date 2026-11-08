// Currency-local treasury and paired transfer implementation; ABI unchanged.
#ifndef WRSRE_CURRENCY_TRANSACTION_TREASURY_H
#define WRSRE_CURRENCY_TRANSACTION_TREASURY_H

static double* TreasuryField(int currency)
{
    if (currency != TSM_CURRENCY_RUB && currency != TSM_CURRENCY_USD) return nullptr;
    BYTE* game = GameObject();
    if (!game) return nullptr;
    return (double*)(game + (currency == TSM_CURRENCY_USD
        ? TREASURY_OFFSET_USD : TREASURY_OFFSET_RUB));
}

static double Api_GetTreasuryBalance(int currency)
{
    CurrencyLock lock;
    return ReadTreasuryNative(TreasuryField(currency));
}

static bool ModifyTreasury(double delta, int currency)
{
    if (!std::isfinite(delta)) return false;
    CurrencyLock lock;
    double* field = TreasuryField(currency);
    return field && CommitNative(nullptr, 0.0f, field, delta);
}

static void Api_ModifyTreasury(double delta, int currency)
{
    (void)ModifyTreasury(delta, currency);
}

static void Api_MoveTreasury(double amount, const char* category)
{
    if (!std::isfinite(amount) || amount == 0.0) return;
    if (ModifyTreasury(amount, g_activeCurrency))
        FireFlow(category ? category : "?", amount, g_activeCurrency);
}

static int Api_GetActiveCurrency(void) { return g_activeCurrency; }
static const char* Api_GetCurrencySymbol(void)
{
    return g_activeCurrency == TSM_CURRENCY_USD ? "USD" : "RUB";
}

static float CitizenStateTransfer(void* person, float amount, bool credit,
                                  bool partial, const char* category)
{
    if (!std::isfinite(amount) || amount <= 0.0f) return 0.0f;
    double* field = TreasuryField(g_activeCurrency);
    if (!field) return 0.0f; // never interpret missing treasury as wallet-only
    const float moved = WalletChange(person, amount, credit, partial, field);
    // Call observers only after BOTH legs commit and after releasing the lock.
    if (moved > 0.0f)
        FireFlow(category ? category : "?", credit ? -(double)moved : (double)moved,
                 g_activeCurrency);
    return moved;
}

static int Api_CitizenToState(void* person, float amount, const char* category)
{
    if (!std::isfinite(amount)) return 0;
    if (amount <= 0.0f) return 1;
    return CitizenStateTransfer(person, amount, false, false, category) == amount;
}

static float Api_CitizenToStatePartial(void* person, float amount, const char* category)
{
    return CitizenStateTransfer(person, amount, false, true, category);
}

static void Api_StateToCitizen(void* person, float amount, const char* category)
{
    (void)CitizenStateTransfer(person, amount, true, false, category);
}
#endif
