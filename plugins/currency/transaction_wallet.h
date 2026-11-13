// Currency-local transaction implementation. Included after the native adapter.
#ifndef WRSRE_CURRENCY_TRANSACTION_WALLET_H
#define WRSRE_CURRENCY_TRANSACTION_WALLET_H
#include <new>

class CurrencyLock {
public:
    CurrencyLock() { EnterCriticalSection(&g_lock); }
    ~CurrencyLock() { LeaveCriticalSection(&g_lock); }
    CurrencyLock(const CurrencyLock&) = delete;
    CurrencyLock& operator=(const CurrencyLock&) = delete;
};

static float WalletGet(void* person)
{
    const uint64_t id = ReadPersonId(person);
    if (!id) return 0.0f;
    const int now = CurrentAbsDay();
    CurrencyLock lock;
    auto it = g_wallets.find(id);
    if (it == g_wallets.end()) return g_startingWallet;
    if (now >= 0) it->second.lastSeenAbsDay = now;
    // Do not turn corrupt state into a fresh starting grant.
    return std::isfinite(it->second.balance) && it->second.balance >= 0.0f
        ? it->second.balance : 0.0f;
}

// One lock covers affordability, both money legs, and the spent counter.
// Native commits cannot allocate; prepare a missing map node BEFORE native writes.
// A failed attempt still touches existing wallets for the idle-pruning policy.
static float WalletChange(void* person, float amount, bool credit, bool partial,
                          double* treasury)
{
    if (!std::isfinite(amount) || amount <= 0.0f) return 0.0f;
    const uint64_t id = ReadPersonId(person);
    if (!id) return 0.0f;
    const int now = CurrentAbsDay();
    CurrencyLock lock;
    auto it = g_wallets.find(id);
    const bool missing = it == g_wallets.end();
    if (!missing && now >= 0) it->second.lastSeenAbsDay = now;
    const float balance = missing ? g_startingWallet : it->second.balance;
    if (!std::isfinite(balance) || balance < 0.0f) return 0.0f;
    float moved = amount;
    if (!credit && balance < amount) {
        if (!partial) return 0.0f;
        moved = balance;
    }
    if (moved <= 0.0f) return 0.0f;
    const float next = credit ? balance + moved : balance - moved;
    if (!std::isfinite(next) || next < 0.0f || next == balance) return 0.0f;
    if (missing) {
        WalletEntry entry = { balance, now };
        try { it = g_wallets.emplace(id, entry).first; }
        catch (const std::bad_alloc&) { return 0.0f; }
    }
    if (!CommitNative(person, credit ? 0.0f : moved, treasury,
                      credit ? -(double)moved : (double)moved)) {
        if (missing) g_wallets.erase(it);
        return 0.0f;
    }
    it->second.balance = next;
    return moved;
}

static int WalletAdd(void* person, float amount)
{
    if (!std::isfinite(amount) || !ReadPersonId(person)) return 0;
    if (amount <= 0.0f) return 1;
    return WalletChange(person, amount, true, false, nullptr) == amount;
}

static float WalletTake(void* person, float amount)
{
    return WalletChange(person, amount, false, true, nullptr);
}

static float Api_GetWalletBalance(void* person) { return WalletGet(person); }
static int Api_AddWalletBalance(void* person, float amount) { return WalletAdd(person, amount); }
static int Api_ChargeCitizen(void* person, float amount)
{
    if (!std::isfinite(amount)) return 0;
    if (amount <= 0.0f) return 1; // retain the finite non-positive no-op contract
    return WalletChange(person, amount, false, false, nullptr) == amount;
}
static float Api_ChargeCitizenPartial(void* person, float amount)
{
    return WalletTake(person, amount);
}
#endif
