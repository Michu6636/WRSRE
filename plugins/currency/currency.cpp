// currency - the money service.
//
// v4.1: full charges and paired treasury transfers commit atomically under
// the currency lock. Nonfinite/overflowing mutations are refused; labelled
// observers run only after successful native commits. Public ABI is unchanged.
//
// B5 bound (v1.1): g_wallets no longer grows forever. WalletGet is a
// pure read (miss returns the starting wallet, no key); keys are born
// on real money movement. New memory-only lastSeenAbsDay (absolute
// day, never persisted) is touched on every hit/mutation - including a
// broke citizen's taken == 0 attempt, which still proves liveness. A
// monthly sim_clock hook evicts entries idle longer than
// prune_after_days (default 90, 0 = disabled); SaveWallets skips stale
// records so sidecars stay clean with no WALLET_VERSION bump.
//
// See currency_api.h for the contract and the reasoning. This file is
// the implementation: a wallet map keyed by TsmCitizenID, direct access
// to the game's two treasury doubles, a labelled flow observer bus, and
// a persistence subscription that writes the map to a file next to the
// save's stats.ini.

#include "../../src/tesmio_plugin.h"
#include "../../src/currency_api.h"
#include "../../src/persistence_api.h"
#include "../../src/sim_clock_api.h"

#include <stdint.h>
#include <stddef.h>
#include <unordered_map>
#include <vector>
#include <cmath>

#define RVA_GAME_OBJECT     (g_tsmAddr.game_object)
#define TREASURY_OFFSET_USD 0x580
#define TREASURY_OFFSET_RUB 0x588

#define PERSON_SPENT_OFFSET 0x734
#define PERSON_MIN_READABLE 0x738

static int   g_enabled        = 1;
static int   g_probe          = 1;
static float g_startingWallet = 150.0f;
static float g_startingWalletCfg = 150.0f; // as written in the ini
static int   g_startingWalletSet = 0;      // 1 when the ini key was present

// v4: unified global currency. primary_currency selects which treasury the
// labelled operations route to (RUB = game+0x588, USD = game+0x580).
static int g_activeCurrency = TSM_CURRENCY_RUB;

// B5: bound the map. Entries idle longer than g_pruneAfterDays are
// evicted on the month boundary (and skipped at save). lastSeenAbsDay
// is a memory-only GC hint - never persisted, so no save-format bump.
static int   g_pruneAfterDays = 90;

struct WalletEntry {
    float   balance;
    int32_t lastSeenAbsDay;  // absolute day (year*365+day), -1 = unknown
};

static std::unordered_map<uint64_t, WalletEntry> g_wallets;

static const TsmSimClockApi* g_simClock = nullptr;

// Absolute day so the year rollover (day 364 -> 0) never looks like
// 364 days of idleness. Returns -1 before a world is loaded.
static int CurrentAbsDay(void)
{
    if (!g_simClock) return -1;
    int day  = g_simClock->getDay();
    int year = g_simClock->getYear();
    if (day < 0 || year < 0) return -1;
    return year * 365 + day;
}

struct FlowObserver {
    TsmMoneyFlowCallback cb;
    void*                userData;
    int                  active;
};

#define MAX_FLOW_OBSERVERS 16
static FlowObserver g_flowObservers[MAX_FLOW_OBSERVERS];
static int          g_flowObserverCount;

static int RegisterFlowObserver(TsmMoneyFlowCallback cb, void* userData)
{
    if (!cb) return 0;
    if (g_flowObserverCount >= MAX_FLOW_OBSERVERS) {
        Logf("currency  flow observer table full (%d) - refusing", MAX_FLOW_OBSERVERS);
        return 0;
    }
    int i = g_flowObserverCount++;
    g_flowObservers[i].cb       = cb;
    g_flowObservers[i].userData = userData;
    g_flowObservers[i].active   = 1;
    return i + 1;
}

static void FireFlow(const char* category, double signedAmount, int currency)
{
    if (!category) category = "?";
    for (int i = 0; i < g_flowObserverCount; i++) {
        FlowObserver* o = &g_flowObservers[i];
        if (!o->active) continue;
        __try { o->cb(o->userData, category, signedAmount, currency); }
        __except (FaultFilter("currency flow observer", GetExceptionInformation()))
        {
            Logf("currency  a flow observer faulted; disabling it for this session");
            o->active = 0;
        }
    }
}

static inline bool IsValidGamePtr(const void* p)
{
    uintptr_t u = (uintptr_t)p;
    return u >= 0x10000 && u <= 0x00007FFFFFFEFFFFULL;
}

static BYTE* GameObject()
{
    if (!TsmCheckExeBuild(H, "currency") || !g_exeBase || !RVA_GAME_OBJECT)
        return nullptr;
    return g_exeBase + RVA_GAME_OBJECT;
}

static bool ValidPerson(const void* p)
{
    return IsValidGamePtr(p) && ReadablePtr(p, PERSON_MIN_READABLE);
}

#include "transaction_native.h"
#include "transaction_wallet.h"

static void Api_ForEachWallet(TsmWalletVisitor cb, void* userData)
{
    if (!cb) return;

    std::vector<std::pair<uint64_t, float>> snap;
    EnterCriticalSection(&g_lock);
    snap.reserve(g_wallets.size());
    for (const auto& kv : g_wallets) snap.emplace_back(kv.first, kv.second.balance);
    LeaveCriticalSection(&g_lock);

    for (const auto& kv : snap)
        if (!cb(kv.first, kv.second, userData)) break;
}

static double Api_GetTotalWalletBalance(void)
{
    EnterCriticalSection(&g_lock);
    double total = 0.0;
    for (const auto& kv : g_wallets)
        if (std::isfinite(kv.second.balance)) total += (double)kv.second.balance;
    LeaveCriticalSection(&g_lock);
    return total;
}

#include "transaction_treasury.h"

// ---------------------------------------------------------------- persistence

static const TsmPersistenceApi* g_persistence = nullptr;
static int                      g_persHandle  = -1;

// B5: evict entries idle longer than g_pruneAfterDays. Single lock hold,
// iterator-safe erase, no-op before a world loads or when disabled (<=0).
static void PruneWallets(void)
{
    if (g_pruneAfterDays <= 0) return;
    int now = CurrentAbsDay();
    if (now < 0) return;

    size_t before = 0, after = 0;
    EnterCriticalSection(&g_lock);
    before = g_wallets.size();
    for (auto it = g_wallets.begin(); it != g_wallets.end(); ) {
        int seen = it->second.lastSeenAbsDay;
        if (seen >= 0 && now - seen > g_pruneAfterDays)
            it = g_wallets.erase(it);
        else
            ++it;
    }
    after = g_wallets.size();
    LeaveCriticalSection(&g_lock);

    if (g_probe && after != before)
        Logf("currency  pruned %zu stale wallet(s), %zu remain",
             before - after, after);
}

static void OnMonthPrune(void* /*ud*/, int /*day*/, int /*month*/, int /*year*/)
{
    PruneWallets();
}

#define WALLET_MAGIC      "TSMWALLT"
#define WALLET_VERSION    1u
#define WALLET_FILENAME   "tesmio_currency.dat"

#pragma pack(push, 1)
struct WalletSaveHeader {
    char     magic[8];
    uint32_t version;
    uint32_t recordSize;
    uint32_t count;
    uint32_t reserved;
};
struct WalletSaveRecord {
    uint64_t citizenId;
    float    wallet;
};
#pragma pack(pop)

static void SaveWallets(const char* dir)
{
    std::vector<WalletSaveRecord> records;
    EnterCriticalSection(&g_lock);
    // B5 backstop: skip stale entries at pack time (no erase under the
    // save-thread lock - the monthly pass does the erasing). Sidecars
    // on disk still get cleaned up, format unchanged.
    int now = CurrentAbsDay();
    records.reserve(g_wallets.size());
    for (const auto& kv : g_wallets) {
        int seen = kv.second.lastSeenAbsDay;
        if (g_pruneAfterDays > 0 && now >= 0 && seen >= 0 &&
            now - seen > g_pruneAfterDays)
            continue;
        WalletSaveRecord r;
        r.citizenId = kv.first;
        r.wallet    = (kv.second.balance >= 0.0f) ? kv.second.balance : 0.0f;
        records.push_back(r);
    }
    LeaveCriticalSection(&g_lock);

    size_t count = records.size();
    size_t total = sizeof(WalletSaveHeader) + count * sizeof(WalletSaveRecord);
    std::vector<BYTE> buf(total);

    WalletSaveHeader hdr;
    memcpy(hdr.magic, WALLET_MAGIC, 8);
    hdr.version    = WALLET_VERSION;
    hdr.recordSize = (uint32_t)sizeof(WalletSaveRecord);
    hdr.count      = (uint32_t)count;
    hdr.reserved   = 0;
    memcpy(buf.data(), &hdr, sizeof(hdr));

    if (count > 0)
        memcpy(buf.data() + sizeof(hdr), records.data(),
               count * sizeof(WalletSaveRecord));

    if (g_persistence->writeAtomic(dir, WALLET_FILENAME, buf.data(), total))
        Logf("currency  saved %zu wallet(s) into %s\\%s", count, dir, WALLET_FILENAME);
    else
        Logf("currency  WARN  could not write %s\\%s", dir, WALLET_FILENAME);
}

// A missing or unusable sidecar leaves an empty wallet map. The entries
// are keyed by citizen ID and must not survive from the previous world.
static void ResetWallets(void)
{
    EnterCriticalSection(&g_lock);
    g_wallets.clear();
    LeaveCriticalSection(&g_lock);
}

static void LoadWallets(const char* dir)
{
    size_t len = 0;
    void* raw = g_persistence->readFile(dir, WALLET_FILENAME, &len);
    if (!raw) {
        ResetWallets();

        Logf("currency  no %s in this save - starting with an empty wallet map",
             WALLET_FILENAME);
        return;
    }

    if (len < sizeof(WalletSaveHeader)) {
        Logf("currency  %s is too short (%zu bytes) - ignored", WALLET_FILENAME, len);
        ResetWallets();
        free(raw);
        return;
    }

    WalletSaveHeader hdr;
    memcpy(&hdr, raw, sizeof(hdr));
    if (memcmp(hdr.magic, WALLET_MAGIC, 8) != 0 ||
        hdr.version    != WALLET_VERSION ||
        hdr.recordSize != sizeof(WalletSaveRecord))
    {
        Logf("currency  %s is version %u, this build writes %u - ignored",
             WALLET_FILENAME, hdr.version, WALLET_VERSION);
        ResetWallets();
        free(raw);
        return;
    }

    size_t want = sizeof(hdr) + (size_t)hdr.count * sizeof(WalletSaveRecord);
    if (want > len) {
        Logf("currency  %s claims %u records but is only %zu bytes - ignored",
             WALLET_FILENAME, hdr.count, len);
        ResetWallets();
        free(raw);
        return;
    }

    EnterCriticalSection(&g_lock);
    g_wallets.clear();
    g_wallets.reserve(hdr.count);
    // B5: loaded survivors get a fresh lease - stamp now (memory-only,
    // no format change) so a long-lived save is not pruned on arrival.
    int now = CurrentAbsDay();
    const BYTE* p = (const BYTE*)raw + sizeof(hdr);
    for (uint32_t i = 0; i < hdr.count; i++) {
        WalletSaveRecord r;
        memcpy(&r, p, sizeof(r));
        p += sizeof(r);
        // A corrupt record must not poison the map: the wallet service
        // refuses non-finite balances, so +inf would be invisible to
        // WalletGet while the total and the next save expose it.
        if (!std::isfinite(r.wallet) || r.wallet < 0.0f || r.wallet > 1.0e9f)
            continue;
        WalletEntry e;
        e.balance        = r.wallet;
        e.lastSeenAbsDay = now;
        g_wallets.emplace(r.citizenId, e);
    }
    size_t loaded = g_wallets.size();
    LeaveCriticalSection(&g_lock);

    free(raw);
    Logf("currency  loaded %zu wallet(s) from %s\\%s", loaded, dir, WALLET_FILENAME);
}

static void OnSaveLoad(TsmSaveEvent event, const char* dir, void* /*userData*/)
{
    if (!dir || !g_persistence) return;
    if      (event == TSM_SAVE_WRITING) SaveWallets(dir);
    else if (event == TSM_SAVE_LOADING) LoadWallets(dir);
}

static void ReadSettings()
{
    const char* ini = "plugins\\currency.ini";
    char v[32];

    g_enabled = H->configInt(ini, "currency", "enabled", 1);
    g_probe   = H->configInt(ini, "currency", "probe",   1);

    // v4: unified global currency. `rub` (default) keeps the pre-v4
    // behaviour: labelled treasury ops route to game+0x588 and flow
    // events carry TSM_CURRENCY_RUB. `usd` routes to game+0x580.
    g_activeCurrency = TSM_CURRENCY_RUB;
    if (H->configString(ini, "currency", "primary_currency",
                        v, sizeof(v), "rub") && v[0]) {
        if (_stricmp(v, "usd") == 0 || _stricmp(v, "dollar") == 0 ||
            _stricmp(v, "dollars") == 0 || _stricmp(v, "$") == 0)
            g_activeCurrency = TSM_CURRENCY_USD;
    }

    g_startingWalletSet = 0;
    if (H->configString(ini, "currency", "starting_wallet", v, sizeof(v), "150.0") && v[0]) {
        g_startingWalletCfg = (float)atof(v);
        g_startingWalletSet = 1;
    }

    // v4 scale calibration: USD numbers run far smaller than RUB
    // (~1 USD = 20..60 RUB). When the ini leaves starting_wallet at its
    // shipped RUB default and USD is active, scale it down so a newborn
    // wallet is not 150 USD. An explicit starting_wallet_usd always wins;
    // an explicit starting_wallet is honoured verbatim in either mode.
    g_startingWallet = g_startingWalletCfg;
    if (H->configString(ini, "currency", "starting_wallet_usd",
                        v, sizeof(v), "") && v[0]) {
        float usd = (float)atof(v);
        if (usd == usd && usd >= 0.0f && usd <= 1.0e9f &&
            g_activeCurrency == TSM_CURRENCY_USD)
            g_startingWallet = usd;
    } else if (g_activeCurrency == TSM_CURRENCY_USD && !g_startingWalletSet) {
        g_startingWallet = g_startingWalletCfg / 50.0f;
    }

    if (g_startingWallet < 0.0f)          g_startingWallet = 0.0f;
    if (!(g_startingWallet == g_startingWallet)) g_startingWallet = 150.0f;
    if (g_startingWallet > 1.0e9f)        g_startingWallet = 1.0e9f;

    // B5: idle threshold for wallet GC. 90 days = one fiscal quarter at
    // the monthly payday default - 2 cycles (60d) cuts too close for
    // welfare gaps and hospital stays. 0 disables pruning entirely.
    g_pruneAfterDays = H->configInt(ini, "currency", "prune_after_days", 90);
    if (g_pruneAfterDays < 0)    g_pruneAfterDays = 0;
    if (g_pruneAfterDays > 3650) g_pruneAfterDays = 3650;
}

extern "C" __declspec(dllexport) unsigned TsmPluginApiVersion(void)
{
    return TSM_API_VERSION;
}

extern "C" __declspec(dllexport) int TsmPluginInit(const TsmHost* host, TsmPluginInfo* info)
{
    TsmBind(host);
    info->name    = "currency";
    info->version = "4.1";

    ReadSettings();
    if (!g_enabled) {
        Logf("currency  disabled");
        return 1;
    }

    g_wallets.reserve(8192);

    static const TsmCurrencyApi kApi = {
        sizeof(TsmCurrencyApi),
        Api_GetWalletBalance,
        Api_AddWalletBalance,
        Api_ChargeCitizen,
        Api_ChargeCitizenPartial,
        Api_ForEachWallet,
        Api_GetTotalWalletBalance,
        Api_GetTreasuryBalance,
        Api_ModifyTreasury,
        Api_MoveTreasury,
        Api_CitizenToState,
        Api_CitizenToStatePartial,
        Api_StateToCitizen,
        RegisterFlowObserver,
        nullptr,
        Api_GetActiveCurrency,
        Api_GetCurrencySymbol,
    };

    static TsmCurrencyApi kApiMut;
    memcpy(&kApiMut, &kApi, sizeof(kApi));
    kApiMut.unregisterFlowObserver = [](int handle) {
        if (handle < 1 || handle > g_flowObserverCount) return;
        g_flowObservers[handle - 1].active = 0;
    };

    // v4 ABI: the registry matches versions exactly, so publish both the
    // v4 iface (full struct) and a v3 view (prefix only). A consumer built
    // against v3 asks for v3 and reads exactly the prefix it expects;
    // a v4 consumer asks for v4 and gets the active-currency tail.
    static TsmCurrencyApi kApiV3;
    memcpy(&kApiV3, &kApiMut, sizeof(kApiV3));
    kApiV3.structSize = (unsigned)offsetof(TsmCurrencyApi, getActiveCurrency);
    H->provide(TSM_SERVICE_CURRENCY, TSM_CURRENCY_VERSION_V3, &kApiV3);
    H->provide(TSM_SERVICE_CURRENCY, TSM_CURRENCY_VERSION, &kApiMut);
    return 0;
}

extern "C" __declspec(dllexport) int TsmPluginStart(void)
{
    if (!g_enabled) return 1;

    // B5: clock is optional - wallets work without it, only GC sleeps.
    g_simClock = (const TsmSimClockApi*)
        H->consume(TSM_SERVICE_SIM_CLOCK, TSM_SIM_CLOCK_VERSION);
    if (g_simClock && g_pruneAfterDays > 0)
        g_simClock->registerScheduled(TSM_FREQ_MONTHLY, OnMonthPrune, nullptr);
    else if (!g_simClock && g_probe)
        Logf("currency  no sim_clock - wallet GC disabled for this session");

    g_persistence = (const TsmPersistenceApi*)
        H->consume(TSM_SERVICE_PERSISTENCE, TSM_PERSISTENCE_VERSION);

    if (!g_persistence) {
        Logf("currency  no persistence service - wallets will not survive a save/load.");
        return 0;
    }

    g_persHandle = g_persistence->registerHandler("currency", OnSaveLoad, nullptr);
    if (g_persHandle <= 0)
        Logf("currency  WARN  persistence refused the handler");

    Logf("currency  live v4.0: primary %s, starting wallet %.2f, prune_after_days %d, %s",
         (g_activeCurrency == TSM_CURRENCY_USD) ? "USD" : "RUB",
         g_startingWallet, g_pruneAfterDays,
         g_persHandle > 0 ? "persistent" : "in-memory only");
    return 0;
}

BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID) { return TRUE; }