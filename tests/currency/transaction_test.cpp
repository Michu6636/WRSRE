// Execute the SAME transaction headers included by currency.cpp. Game memory,
// clock, locks and observer delivery are substituted; no Windows hook claims.
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>
#include "../../src/currency_api.h"

using BYTE = unsigned char;
#define PERSON_SPENT_OFFSET 0x734
#define TREASURY_OFFSET_USD 0x580
#define TREASURY_OFFSET_RUB 0x588
struct Person { alignas(8) BYTE bytes[0x738]{}; } person;
alignas(8) static BYTE game[0x600];
static bool gameAvailable = true, personAvailable = true;
static const void* unreadable = nullptr;
static bool throwRead = false;
static std::recursive_mutex g_lock;
static void EnterCriticalSection(std::recursive_mutex* p) { p->lock(); }
static void LeaveCriticalSection(std::recursive_mutex* p) { p->unlock(); }
static BYTE* GameObject() { return gameAvailable ? game : nullptr; }
static bool ValidPerson(const void* p) { return personAvailable && p == &person; }
static bool ReadablePtr(const void* p, size_t) {
    if (throwRead) throw 1;
    return p && p != unreadable;
}
// C++ catch stand-ins exercise validation and cleanup paths, NOT Windows SEH.
#define __try try
#define __except(...) catch (...)
#define Logf(...) ((void)0)
#include "../../plugins/currency/transaction_native.h"
#undef __try
#undef __except

struct WalletEntry { float balance; int32_t lastSeenAbsDay; };
static std::unordered_map<uint64_t, WalletEntry> g_wallets;
static float g_startingWallet = 10.0f;
static int g_activeCurrency = TSM_CURRENCY_RUB;
static int day = 40;
static int CurrentAbsDay() { return day; }
#include "../../plugins/currency/transaction_wallet.h"
struct Flow { double amount; int currency; float wallet; double treasury; float spent; };
static std::vector<Flow> flows;
static float& Spent() { return *reinterpret_cast<float*>(person.bytes + PERSON_SPENT_OFFSET); }
static double& Treasury(int currency) {
    return *reinterpret_cast<double*>(game + (currency == TSM_CURRENCY_USD
        ? TREASURY_OFFSET_USD : TREASURY_OFFSET_RUB));
}
static void FireFlow(const char*, double amount, int currency) {
    // Observers may re-enter getters and see both committed legs.
    flows.push_back({amount, currency, Api_GetWalletBalance(&person),
                     Treasury(currency), Spent()});
}
#include "../../plugins/currency/transaction_treasury.h"
static uint64_t Id() { return TsmCitizenID(&person); }
static void Reset(float balance = 10.0f) {
    person = Person{};
    uint32_t entity = 17;
    std::memcpy(person.bytes + 8, &entity, sizeof(entity));
    g_wallets.clear(); g_startingWallet = balance;
    gameAvailable = personAvailable = true; unreadable = nullptr; throwRead = false;
    Treasury(0) = 1000; Treasury(1) = 2000;
    g_activeCurrency = 0; flows.clear(); day = 40;
}

static void FullCharge() {
    Reset(5);
    assert(Api_ChargeCitizen(&person, 10) == 0);
    assert(Api_GetWalletBalance(&person) == 5 && Spent() == 0);
    assert(g_wallets.empty());
    assert(Api_ChargeCitizen(&person, 5) == 1);
    assert(Api_GetWalletBalance(&person) == 0 && Spent() == 5);
    day = 80;
    assert(Api_ChargeCitizen(&person, 1) == 0);
    assert(g_wallets.at(Id()).lastSeenAbsDay == 80 && Spent() == 5);
    assert(Api_ChargeCitizenPartial(&person, 1) == 0);
    Reset(5);
    assert(Api_ChargeCitizenPartial(&person, 10) == 5);
    assert(Api_GetWalletBalance(&person) == 0 && Spent() == 5);
    assert(flows.empty());
}

static void PairedTransfers() {
    for (int currency : {TSM_CURRENCY_RUB, TSM_CURRENCY_USD}) {
        Reset(5); g_activeCurrency = currency;
        double start = Treasury(currency), other = Treasury(1-currency);
        assert(Api_CitizenToState(&person, 10, "sale") == 0);
        assert(Api_GetWalletBalance(&person) == 5 && Spent() == 0);
        assert(Treasury(currency) == start && flows.empty());
        assert(Api_CitizenToState(&person, 3, "sale") == 1);
        assert(Api_GetWalletBalance(&person) == 2 && Spent() == 3);
        assert(flows.size() == 1 && flows.back().amount == 3);
        assert(flows.back().wallet == 2 && flows.back().treasury == start+3);
        assert(Api_CitizenToStatePartial(&person, 10, "sale") == 2);
        assert(Spent() == 5 && Treasury(currency) == start+5);
        Api_StateToCitizen(&person, 7, "wage");
        assert(Api_GetWalletBalance(&person) == 7 && Spent() == 5);
        assert(Treasury(currency) == start-2 && Treasury(1-currency) == other);
        assert(flows.size() == 3 && flows.back().currency == currency);
        assert(flows.back().amount == -7 && flows.back().wallet == 7);
        assert(Api_GetActiveCurrency() == currency);
        assert(std::strcmp(Api_GetCurrencySymbol(), currency ? "USD" : "RUB") == 0);
    }
}

static void InvalidAmounts() {
    for (float bad : {NAN, INFINITY, -INFINITY}) {
        Reset(5);
        assert(Api_AddWalletBalance(&person, bad) == 0);
        assert(Api_ChargeCitizen(&person, bad) == 0);
        assert(Api_ChargeCitizenPartial(&person, bad) == 0);
        assert(Api_CitizenToState(&person, bad, "bad") == 0);
        assert(Api_CitizenToStatePartial(&person, bad, "bad") == 0);
        Api_StateToCitizen(&person, bad, "bad");
        Api_ModifyTreasury(bad, 0); Api_MoveTreasury(bad, "bad");
        assert(g_wallets.empty() && Api_GetWalletBalance(&person) == 5);
        assert(Treasury(0) == 1000 && Spent() == 0 && flows.empty());
    }
    Reset(5);
    for (float noop : {0.0f, -1.0f}) {
        assert(Api_AddWalletBalance(&person, noop) == 1);
        assert(Api_ChargeCitizen(&person, noop) == 1);
        assert(Api_CitizenToState(&person, noop, "noop") == 1);
        assert(Api_CitizenToStatePartial(&person, noop, "noop") == 0);
        Api_StateToCitizen(&person, noop, "noop");
    }
    assert(g_wallets.empty() && Spent() == 0 && flows.empty());
}

static void UnavailableMemory() {
    for (int failure = 0; failure < 5; ++failure) {
        Reset(5);
        if (failure == 0) gameAvailable = false;
        if (failure == 1) unreadable = &Treasury(0);
        if (failure == 2) personAvailable = false;
        if (failure == 3) unreadable = &Spent();
        if (failure == 4) throwRead = true;
        assert(Api_CitizenToState(&person, 3, "bad") == 0);
        assert(Api_CitizenToStatePartial(&person, 3, "bad") == 0);
        // A bad spent counter should not block a wage, which does not touch it.
        if (failure != 3) Api_StateToCitizen(&person, 3, "bad");
        assert(g_wallets.empty() && Spent() == 0 && Treasury(0) == 1000);
        assert(flows.empty());
        // A subsequent transaction still works; no stranded currency lock.
        personAvailable = gameAvailable = true; unreadable = nullptr; throwRead = false;
        assert(Api_CitizenToState(&person, 1, "ok") == 1);
    }
    Reset(5);
    assert(Api_AddWalletBalance(&person, 1) == 1);
    unreadable = &Treasury(0);
    assert(Api_CitizenToState(&person, 3, "bad") == 0);
    Api_StateToCitizen(&person, 3, "bad");
    assert(Api_GetWalletBalance(&person) == 6 && g_wallets.size() == 1);
    assert(Treasury(0) == 1000 && Spent() == 0 && flows.empty());
    Reset(5);
    assert(Api_ChargeCitizen(nullptr, 1) == 0);
    assert(Api_AddWalletBalance(nullptr, 1) == 0);
    Api_StateToCitizen(nullptr, 3, "bad");
    std::memset(person.bytes + 8, 0, sizeof(uint32_t)); // invalid citizen ID
    Api_StateToCitizen(&person, 3, "bad");
    assert(Treasury(0) == 1000 && g_wallets.empty() && flows.empty());
}

static void OverflowAndCorruption() {
    Reset(std::numeric_limits<float>::max());
    assert(Api_AddWalletBalance(&person, std::numeric_limits<float>::max()) == 0);
    Api_StateToCitizen(&person, std::numeric_limits<float>::max(), "overflow");
    assert(Treasury(0) == 1000 && g_wallets.empty() && flows.empty());
    // Sub-resolution credits must not debit the treasury without changing wallet.
    Api_StateToCitizen(&person, 1, "rounded_away");
    assert(Treasury(0) == 1000 && g_wallets.empty());
    for (float bad : {NAN, INFINITY, -1.0f}) {
        Reset(); g_wallets.emplace(Id(), WalletEntry{bad, 0});
        assert(Api_GetWalletBalance(&person) == 0);
        assert(Api_ChargeCitizen(&person, 1) == 0);
        assert(Api_AddWalletBalance(&person, 1) == 0);
        Api_StateToCitizen(&person, 1, "bad");
        assert(Treasury(0) == 1000 && flows.empty());
    }
    Reset(std::numeric_limits<float>::max());
    Spent() = std::numeric_limits<float>::max();
    assert(Api_ChargeCitizen(&person, std::numeric_limits<float>::max()) == 0);
    assert(g_wallets.empty() && Spent() == std::numeric_limits<float>::max());
    Reset();
    Spent() = INFINITY;
    assert(Api_CitizenToState(&person, 1, "bad_spent") == 0);
    assert(Api_GetWalletBalance(&person) == 10 && Treasury(0) == 1000);
    for (double bad : {std::numeric_limits<double>::quiet_NaN(),
                       std::numeric_limits<double>::infinity()}) {
        Reset(); Treasury(0) = bad;
        assert(Api_CitizenToState(&person, 1, "bad") == 0);
        Api_StateToCitizen(&person, 1, "bad"); Api_MoveTreasury(1, "bad");
        assert(Api_GetWalletBalance(&person) == 10 && flows.empty());
        assert(Api_GetTreasuryBalance(0) == 0);
    }
    Reset(); Treasury(0) = std::numeric_limits<double>::max();
    Api_MoveTreasury(std::numeric_limits<double>::max(), "overflow");
    assert(Treasury(0) == std::numeric_limits<double>::max() && flows.empty());
}

static void TreasuryEvents() {
    Reset();
    Api_ModifyTreasury(12, 1);
    assert(Treasury(1) == 2012 && flows.empty());
    Api_MoveTreasury(7, nullptr);
    assert(Treasury(0) == 1007 && flows.size() == 1 && flows.back().amount == 7);
    Api_MoveTreasury(0, "noop");
    Api_ModifyTreasury(5, 42);
    assert(Api_GetTreasuryBalance(42) == 0 && Treasury(0) == 1007);
    gameAvailable = false;
    Api_MoveTreasury(8, "missing");
    assert(flows.size() == 1 && Treasury(0) == 1007);
}

static void ConcurrentCharges() {
    Reset(1000);
    int successes[8]{};
    std::vector<std::thread> threads;
    for (int i = 0; i < 8; ++i) threads.emplace_back([&, i] {
        for (int n = 0; n < 200; ++n) successes[i] += Api_ChargeCitizen(&person, 1);
    });
    for (auto& thread : threads) thread.join();
    int total = 0; for (int count : successes) total += count;
    assert(total == 1000 && Api_GetWalletBalance(&person) == 0 && Spent() == 1000);
    assert(g_wallets.size() == 1);
}

int main(int argc, char** argv) {
    assert(argc == 2);
    switch (argv[1][0]) {
    case '1': FullCharge(); break;
    case '2': PairedTransfers(); break;
    case '3': InvalidAmounts(); break;
    case '4': UnavailableMemory(); break;
    case '5': OverflowAndCorruption(); break;
    case '6': TreasuryEvents(); break;
    case '7': ConcurrentCharges(); break;
    default: assert(false);
    }
    std::puts("currency transaction tests: PASS");
}
