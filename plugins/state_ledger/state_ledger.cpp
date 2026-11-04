// state_ledger - monthly accounting of every money movement.
//
// Level 1. Consumes sim_clock, currency, persistence and ui_overlay;
// provides nothing.
//
// v1.4 updates:
//   - Unified global currency (v4): ReadTreasury follows the active
//     currency leg (game+0x580 USD / +0x588 RUB), the CSV header's
//     treasury columns carry the active symbol (TreasuryOpen_USD, ...),
//     and every HUD line is formatted with RUB/USD.
//
// v1.3 updates:
//   - Corrects HUD sign formatting for wages/pensions/welfare to display as
//     an expense (-RUB) alongside positive revenues (+RUB).
//   - Adds backward-time / calendar rewind guard in OnTick: if year/month
//     jumps backward (e.g. loading an earlier save mid-session), accumulator
//     resets gracefully without dumping a corrupt row to CSV.

#include "../../src/tesmio_plugin.h"
#include "../../src/game_world.h"
#include "../../src/sim_clock_api.h"
#include "../../src/currency_api.h"
#include "../../src/persistence_api.h"
#include "../../src/ui_overlay_api.h"
#include "../../src/enterprise_base_api.h"
#include "ledger_reconcile.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <stdint.h>

// ---------------------------------------------------------------- services

static const TsmSimClockApi*    g_simClock    = nullptr;
static const TsmCurrencyApi*    g_currency    = nullptr;
static const TsmPersistenceApi* g_persistence = nullptr;
static const TsmUiOverlayApi*   g_ui          = nullptr;
static const TsmEnterpriseBaseApi* g_enterprise = nullptr;
static int                      g_persHandle  = -1;

// ---------------------------------------------------------------- config

static int   g_enabled      = 1;
static int   g_probe        = 1;
static int   g_csvEnabled   = 1;
static int   g_showOverlay  = 1;
static int   g_crossCheck   = 0;

static char  g_csvFilename[64] = "tesmio_ledger.csv";

// ---------------------------------------------------------------- state

static std::unordered_map<std::string, double> g_monthFlows;

static int   g_lastYear    = -1;
static int   g_lastMonth   = -1;
static bool  g_firstTick   = true;

static double g_treasuryAtMonthStart  = 0.0;
static double g_treasuryLastObserved  = 0.0;
static double g_flowSum               = 0.0;
static bool   g_crossCheckWarned      = false;

// v2.0 whole-economy stock tracking
static double g_stocksTreasuryOpen = 0.0;
static double g_stocksWalletsOpen  = 0.0;
static double g_stocksEnterpriseOpen = 0.0;
static double g_stocksPoolOpen     = 0.0;

// HUD Display Snapshot: Keeps the most recently closed month's totals
// visible on the overlay so the numbers don't vanish between boundaries.
static int    g_hudClosedYear    = 0;
static int    g_hudClosedMonth   = 0;
static double g_hudClosedWages   = 0.0;
static double g_hudClosedSales   = 0.0;
static double g_hudClosedWhole   = 0.0;
static double g_hudClosedTaxes   = 0.0;
static double g_hudClosedNet     = 0.0;

static char g_saveDir[MAX_PATH] = {0};

// ---------------------------------------------------------------- forward decls

static void DumpToCsv(int year, int month,
                      double openBalance, double closeBalance,
                      double observedDelta,
                      double closeWallets, double closeEnterprise, double closePool,
                      double openTotal, double closeTotal,
                      double privSales, double privWages, double privTaxes, double privPool,
                      double unexplained);

// ---------------------------------------------------------------- the flow callback

static void OnFlow(void* /*ud*/, const char* category,
                   double signedAmount, int /*currency*/)
{
    if (!category) category = "?";

    EnterCriticalSection(&g_lock);
    g_monthFlows[category] += signedAmount;
    g_flowSum              += signedAmount;
    LeaveCriticalSection(&g_lock);
}

// ---------------------------------------------------------------- treasury

static double ReadTreasury(void)
{
    BYTE* game = TsmGameObject(g_exeBase);
    if (!game || !ReadablePtr(game, 0x600)) return 0.0;
    // v4: read the treasury leg the unified currency routes to - USD is
    // game+0x580, RUB game+0x588. Falls back to RUB before Start (g_currency
    // null) exactly as the pre-v4 code did.
    size_t off = (TsmCurrencyActive(g_currency) == TSM_CURRENCY_USD)
        ? TSM_GO_TREASURY_USD
        : TSM_GO_TREASURY_RUB;
    return *(double*)(game + off);
}

// ---------------------------------------------------------------- the tick

static void OnTick(void* /*ud*/, float /*dt*/, float /*dayDelta*/)
{
    if (!g_simClock) return;

    int year  = g_simClock->getYear();
    int month = g_simClock->getMonth();
    if (year < 0 || month < 0) return;

    if (g_firstTick) {
        g_firstTick            = false;
        g_lastYear             = year;
        g_lastMonth            = month;
        g_treasuryAtMonthStart = ReadTreasury();
        g_treasuryLastObserved = g_treasuryAtMonthStart;
        g_stocksTreasuryOpen   = g_currency->getTreasuryBalance(TsmCurrencyActive(g_currency));
        g_stocksWalletsOpen    = g_currency->getTotalWalletBalance();
        g_stocksEnterpriseOpen = 0.0;
        if (g_enterprise) {
            size_t n = g_enterprise->getEnterpriseCount();
            double caps[1024];
            // guard
            if (n > 1024) n = 1024;
            for (size_t i = 0; i < n; ++i) {
                void* b = g_enterprise->getEnterpriseByIndex(i);
                caps[i] = b ? g_enterprise->getCompanyCapital(b) : 0.0;
            }
            g_stocksEnterpriseOpen = state_ledger::reconcile::SumEnterpriseCapital(caps, n);
            g_stocksPoolOpen = g_enterprise->getPrivateInvestmentPool();
        }
        return;
    }

    // Time jumped backward (e.g. loaded an earlier save): reset baseline without dumping
    if (year < g_lastYear || (year == g_lastYear && month < g_lastMonth)) {
        g_lastYear             = year;
        g_lastMonth            = month;
        g_treasuryAtMonthStart = ReadTreasury();
        g_treasuryLastObserved = g_treasuryAtMonthStart;
        g_stocksTreasuryOpen   = g_currency->getTreasuryBalance(TsmCurrencyActive(g_currency));
        g_stocksWalletsOpen    = g_currency->getTotalWalletBalance();
        g_stocksEnterpriseOpen = 0.0;
        g_stocksPoolOpen       = 0.0;
        if (g_enterprise) {
            size_t n = g_enterprise->getEnterpriseCount();
            double caps[1024];
            if (n > 1024) n = 1024;
            for (size_t i = 0; i < n; ++i) {
                void* b = g_enterprise->getEnterpriseByIndex(i);
                caps[i] = b ? g_enterprise->getCompanyCapital(b) : 0.0;
            }
            g_stocksEnterpriseOpen = state_ledger::reconcile::SumEnterpriseCapital(caps, n);
            g_stocksPoolOpen = g_enterprise->getPrivateInvestmentPool();
        }
        EnterCriticalSection(&g_lock);
        g_monthFlows.clear();
        g_flowSum = 0.0;
        LeaveCriticalSection(&g_lock);
        return;
    }

    if (month != g_lastMonth || year != g_lastYear) {
        double closing = ReadTreasury();

        int closedYear  = g_lastYear;
        int closedMonth = g_lastMonth;

        double observedDelta = closing - g_treasuryAtMonthStart;
        double flowDelta     = g_flowSum;
        double divergence    = observedDelta - flowDelta;

        // v2.0 stock snapshot at close
        double closeWallets = g_currency->getTotalWalletBalance();
        double closeEnterprise = 0.0;
        double closePool = 0.0;
        if (g_enterprise) {
            size_t n = g_enterprise->getEnterpriseCount();
            double caps[1024];
            if (n > 1024) n = 1024;
            for (size_t i = 0; i < n; ++i) {
                void* b = g_enterprise->getEnterpriseByIndex(i);
                caps[i] = b ? g_enterprise->getCompanyCapital(b) : 0.0;
            }
            closeEnterprise = state_ledger::reconcile::SumEnterpriseCapital(caps, n);
            closePool = g_enterprise->getPrivateInvestmentPool();
        }
        double openTotal = g_stocksTreasuryOpen + g_stocksWalletsOpen + g_stocksEnterpriseOpen + g_stocksPoolOpen;
        double closeTotal = g_currency->getTreasuryBalance(TsmCurrencyActive(g_currency)) + closeWallets + closeEnterprise + closePool;

        double privSales=0, privWages=0, privTaxes=0, privPool=0;
        if (g_enterprise) {
            g_enterprise->getMonthlyEconomySummary(&privSales, &privWages, &privTaxes, &privPool);
        }

        double unexplained = (closing - g_treasuryAtMonthStart) - g_flowSum;

        if (g_crossCheck && !g_crossCheckWarned &&
            (divergence > 1.0 || divergence < -1.0))
        {
            g_crossCheckWarned = true;
            Logf("state_ledger  cross-check %d-%02d: treasury moved "
                 "%.2f, %zu flow(s) reported %.2f, divergence %.2f",
                 closedYear, closedMonth,
                 observedDelta, g_monthFlows.size(), flowDelta,
                 divergence);
        }

        // Snapshot closed totals for HUD display before clearing accumulator
        EnterCriticalSection(&g_lock);
        auto val = [](const char* k) -> double {
            auto it = g_monthFlows.find(k);
            return (it == g_monthFlows.end()) ? 0.0 : it->second;
        };

        g_hudClosedYear  = closedYear;
        g_hudClosedMonth = closedMonth;
        g_hudClosedWages = val("wage") + val("pension") + val("welfare");
        g_hudClosedSales = val("state_store_sale");
        g_hudClosedWhole = val("wholesale_b2b");
        g_hudClosedTaxes = val("vat") + val("profit_tax") + val("property_tax");
        g_hudClosedNet   = g_flowSum;
        LeaveCriticalSection(&g_lock);

        DumpToCsv(closedYear, closedMonth,
                  g_treasuryAtMonthStart, closing,
                  observedDelta,
                  closeWallets, closeEnterprise, closePool,
                  openTotal, closeTotal,
                  privSales, privWages, privTaxes, privPool,
                  unexplained);

        EnterCriticalSection(&g_lock);
        g_monthFlows.clear();
        g_flowSum = 0.0;
        LeaveCriticalSection(&g_lock);

        g_treasuryAtMonthStart = closing;
        g_treasuryLastObserved = closing;
        g_crossCheckWarned     = false;

        g_stocksTreasuryOpen   = g_currency->getTreasuryBalance(TsmCurrencyActive(g_currency));
        g_stocksWalletsOpen    = closeWallets;
        g_stocksEnterpriseOpen = closeEnterprise;
        g_stocksPoolOpen       = closePool;

        g_lastYear  = year;
        g_lastMonth = month;

        if (g_probe)
            Logf("state_ledger  month boundary: %d-%02d closed at %.2f, "
                 "next month begins", closedYear, closedMonth, closing);
    } else {
        g_treasuryLastObserved = ReadTreasury();
    }
}

// ---------------------------------------------------------------- CSV

// v4: header is built from the active currency so a USD save and a RUB
// save are distinguishable from the column names alone. Rows written
// before a currency switch keep the header they were created under - the
// tag check below only requires the "Year,Month" prefix.
static std::string CsvHeader(void)
{
    const char* sym = TsmCurrencySymbol(g_currency);
    char h[512];
    _snprintf_s(h, sizeof(h), _TRUNCATE,
        "Year,Month,Wages,Pensions,Welfare,"
        "StateStoreSales,WholesaleB2B,"
        "VAT,ProfitTax,PropertyTax,"
        "Other,NetTreasury,TreasuryOpen_%s,TreasuryClose_%s,"
        "CitizenWallets,EnterpriseCapitalSum,InvestmentPool,"
        "TotalMoneySupplyOpen,TotalMoneySupplyClose,MoneySupplyDelta,"
        "PrivateSales,PrivateWages,PrivateTaxes,PrivatePool,"
        "TreasuryUnexplained\r\n",
        sym, sym);
    return std::string(h);
}
static const char kCsvTag[] = "Year,Month";

static void DumpToCsv(int year, int month,
                      double openBalance, double closeBalance,
                      double /*observedDelta*/,
                      double closeWallets, double closeEnterprise, double closePool,
                      double openTotal, double closeTotal,
                      double privSales, double privWages, double privTaxes, double privPool,
                      double unexplained)
{
    if (!g_csvEnabled || !g_persistence) return;

    if (!g_saveDir[0]) {
        Logf("state_ledger  no save directory known - not writing %s",
             g_csvFilename);
        return;
    }

    std::unordered_map<std::string, double> snapshot;
    EnterCriticalSection(&g_lock);
    snapshot = g_monthFlows;
    LeaveCriticalSection(&g_lock);

    auto value = [&snapshot](const char* key) -> double {
        auto it = snapshot.find(key);
        return (it == snapshot.end()) ? 0.0 : it->second;
    };

    double wages        = value("wage");
    double pension      = value("pension");
    double welfare      = value("welfare");
    double retailSales  = value("state_store_sale");
    double wholesale    = value("wholesale_b2b");
    double vat          = value("vat");
    double profitTax    = value("profit_tax");
    double propertyTax  = value("property_tax");

    double known = wages + pension + welfare
                 + retailSales + wholesale
                 + vat + profitTax + propertyTax;

    double totalSum = 0.0;
    for (auto& kv : snapshot) totalSum += kv.second;
    double other = totalSum - known;

    double net = totalSum;

    char row[1024];
    _snprintf_s(row, sizeof(row), _TRUNCATE,
        "%d,%02d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,"
        "%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n",
        year, month,
        wages, pension, welfare,
        retailSales, wholesale,
        vat, profitTax, propertyTax,
        other, net,
        openBalance, closeBalance,
        closeWallets, closeEnterprise, closePool,
        openTotal, closeTotal, (closeTotal - openTotal),
        privSales, privWages, privTaxes, privPool,
        unexplained);

    const size_t tagLen = sizeof(kCsvTag) - 1;

    std::string payload;
    size_t existingLen = 0;
    void* existing = g_persistence->readFile(g_saveDir, g_csvFilename,
                                             &existingLen);

    if (!existing) {
        char path[MAX_PATH * 2];
        _snprintf_s(path, sizeof(path), _TRUNCATE, "%s\\%s",
                    g_saveDir, g_csvFilename);
        if (GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES) {
            Logf("state_ledger  WARN  %s exists but could not be read - "
                 "this month's row is not written", path);
            return;
        }
        payload = CsvHeader();
    } else if (existingLen >= tagLen &&
               memcmp(existing, kCsvTag, tagLen) == 0) {
        payload.assign((const char*)existing, existingLen);
    } else {
        if (existingLen > 0)
            Logf("state_ledger  %s\\%s does not begin with the expected "
                 "header - starting a fresh file",
                 g_saveDir, g_csvFilename);
        payload = CsvHeader();
    }
    if (existing) free(existing);

    payload += row;

    if (!g_persistence->writeAtomic(g_saveDir, g_csvFilename,
                                    payload.data(), payload.size())) {
        Logf("state_ledger  WARN  could not write %s\\%s",
             g_saveDir, g_csvFilename);
        return;
    }

    if (g_probe)
        Logf("state_ledger  wrote %d-%02d: net %.2f "
             "(wages %.2f, pension %.2f, welfare %.2f, sales %.2f, wholesale %.2f, taxes %.2f)",
             year, month, net, wages, pension, welfare, retailSales, wholesale,
             vat + profitTax + propertyTax);
}

// ---------------------------------------------------------------- save dir tracking

static void OnSaveLoad(TsmSaveEvent ev, const char* dir, void* /*ud*/)
{
    if (!dir) return;

    if (ev == TSM_SAVE_LOADING) {
        static DWORD lastLoadTime = 0;
        DWORD now = GetTickCount();
        if (now - lastLoadTime < 2000 && strcmp(g_saveDir, dir) == 0) return;
        lastLoadTime = now;

        _snprintf_s(g_saveDir, sizeof(g_saveDir), _TRUNCATE, "%s", dir);

        EnterCriticalSection(&g_lock);
        g_monthFlows.clear();
        g_flowSum          = 0.0;
        g_firstTick        = true;
        g_crossCheckWarned = false;
        g_hudClosedYear    = 0;
        g_hudClosedMonth   = 0;
        LeaveCriticalSection(&g_lock);

        if (g_probe)
            Logf("state_ledger  loaded - accumulator reset, ledger file %s\\%s",
                 dir, g_csvFilename);
    } else {
        _snprintf_s(g_saveDir, sizeof(g_saveDir), _TRUNCATE, "%s", dir);
    }
}

// ---------------------------------------------------------------- overlay

static float DrawCard(void* /*ud*/, float x, float y, float step)
{
    if (!g_ui || !g_currency) return y;

    EnterCriticalSection(&g_lock);
    int cYear     = g_hudClosedYear;
    int cMonth    = g_hudClosedMonth;
    double cWages = g_hudClosedWages; // Negative sum of wage + pension + welfare
    double cSales = g_hudClosedSales;
    double cWhole = g_hudClosedWhole;
    double cTaxes = g_hudClosedTaxes;
    double cNet   = g_hudClosedNet;

    auto it = g_monthFlows.find("state_store_sale");
    double liveSales = (it != g_monthFlows.end()) ? it->second : 0.0;
    LeaveCriticalSection(&g_lock);

    wchar_t buf[256];
    // v4: the whole card is denominated in the active currency.
    const wchar_t* sym = (TsmCurrencyActive(g_currency) == TSM_CURRENCY_USD)
        ? L"USD" : L"RUB";

    if (cYear > 0) {
        _snwprintf_s(buf, _countof(buf), _TRUNCATE,
            L"=== STATE BUDGET (Last Closed: %d-%02d, %ls) ===",
            cYear, cMonth, sym);
        g_ui->drawTextOutlined(x, y, 0xFF70C8FF, buf);
        y += step;

        _snwprintf_s(buf, _countof(buf), _TRUNCATE,
            L" Monthly Net: %+.0f %ls   (Revenue %.0f / Spend %.0f)",
            cNet, sym, cSales + cWhole + cTaxes, -cWages);
        g_ui->drawTextOutlined(x, y,
            cNet >= 0.0 ? 0xFF90F090 : 0xFFFF7060, buf);
        y += step;

        _snwprintf_s(buf, _countof(buf), _TRUNCATE,
            L"   Wages/pensions/welfare: %+.0f %ls", cWages, sym);
        g_ui->drawTextOutlined(x, y, 0xFFDCDCDC, buf);
        y += step;

        _snwprintf_s(buf, _countof(buf), _TRUNCATE,
            L"   State store sales:      %+.0f %ls", cSales, sym);
        g_ui->drawTextOutlined(x, y, 0xFFDCDCDC, buf);
        y += step;

        _snwprintf_s(buf, _countof(buf), _TRUNCATE,
            L"   Corporate taxes:        %+.0f %ls", cTaxes, sym);
        g_ui->drawTextOutlined(x, y, 0xFFDCDCDC, buf);
        y += step;
    } else {
        g_ui->drawTextOutlined(x, y, 0xFF70C8FF, L"=== STATE BUDGET ===");
        y += step;
        g_ui->drawTextOutlined(x, y, 0xFFDCDCDC, L" Awaiting first month close...");
        y += step;
    }

    _snwprintf_s(buf, _countof(buf), _TRUNCATE,
        L" Live store sales this month: +%.0f %ls", liveSales, sym);
    g_ui->drawTextOutlined(x, y, 0xFFB0B0B0, buf);
    y += step;

    return y;
}

// ---------------------------------------------------------------- settings

static void ReadSettings(void)
{
    const char* ini = "plugins\\state_ledger.ini";
    char v[64];

    g_enabled     = H->configInt(ini, "state_ledger", "enabled",      1);
    g_probe       = H->configInt(ini, "state_ledger", "probe",        1);
    g_csvEnabled  = H->configInt(ini, "state_ledger", "csv",          1);
    g_showOverlay = H->configInt(ini, "state_ledger", "show_overlay",  1);
    g_crossCheck  = H->configInt(ini, "state_ledger", "cross_check",   0);

    if (H->configString(ini, "state_ledger", "csv_filename",
                        v, sizeof(v), "") && v[0])
        strncpy_s(g_csvFilename, sizeof(g_csvFilename), v, _TRUNCATE);
}

// ---------------------------------------------------------------- lifecycle

extern "C" __declspec(dllexport) unsigned TsmPluginApiVersion(void)
{
    return TSM_API_VERSION;
}

extern "C" __declspec(dllexport) int TsmPluginInit(const TsmHost* host,
                                                   TsmPluginInfo* info)
{
    TsmBind(host);
    info->name    = "state_ledger";
    info->version = "2.0";

    ReadSettings();
    if (!g_enabled) {
        Logf("state_ledger  disabled");
        return 1;
    }
    return 0;
}

extern "C" __declspec(dllexport) int TsmPluginStart(void)
{
    if (!g_enabled) return 1;

    g_simClock = (const TsmSimClockApi*)
        H->consume(TSM_SERVICE_SIM_CLOCK, TSM_SIM_CLOCK_VERSION);
    if (!g_simClock) {
        Logf("state_ledger  sim_clock is not installed - cannot run");
        return 1;
    }

    // v4: prefer the unified-currency iface; fall back to v3 (RUB-only
    // routing) so a mixed install still records.
    g_currency = (const TsmCurrencyApi*)
        H->consume(TSM_SERVICE_CURRENCY, TSM_CURRENCY_VERSION);
    if (!g_currency)
        g_currency = (const TsmCurrencyApi*)
            H->consume(TSM_SERVICE_CURRENCY, TSM_CURRENCY_VERSION_V3);
    if (!g_currency) {
        Logf("state_ledger  currency is not installed - cannot run");
        return 1;
    }

    g_persistence = (const TsmPersistenceApi*)
        H->consume(TSM_SERVICE_PERSISTENCE, TSM_PERSISTENCE_VERSION);
    if (!g_persistence) {
        Logf("state_ledger  WARN  no persistence service - the CSV will not be written.");
    } else {
        g_persHandle = g_persistence->registerHandler(
            "state_ledger", OnSaveLoad, nullptr);
        if (g_persHandle <= 0)
            Logf("state_ledger  WARN  persistence refused the handler");
    }

    g_enterprise = (const TsmEnterpriseBaseApi*)
        H->consume(TSM_SERVICE_ENTERPRISE_BASE, TSM_ENTERPRISE_BASE_VERSION);
    if (!g_enterprise)
        g_enterprise = (const TsmEnterpriseBaseApi*)
            H->consume(TSM_SERVICE_ENTERPRISE_BASE, TSM_ENTERPRISE_BASE_VERSION_V7);
    if (!g_enterprise)
        Logf("state_ledger  enterprise_base not installed - money-supply tracking degrades");

    if (g_showOverlay) {
        g_ui = (const TsmUiOverlayApi*)
            H->consume(TSM_SERVICE_UI_OVERLAY, TSM_UI_OVERLAY_VERSION);
        if (!g_ui)
            Logf("state_ledger  WARN  ui_overlay is not installed");
    }

    g_currency->registerFlowObserver(OnFlow, nullptr);
    g_simClock->registerTick(OnTick, nullptr);

    if (g_ui) g_ui->registerDrawer(DrawCard, nullptr);

    Logf("state_ledger  live v1.4: currency %s, csv=%s, overlay=%s, persistence=%s, cross_check=%s",
         TsmCurrencySymbol(g_currency),
         g_csvEnabled ? "on" : "off",
         g_showOverlay ? "on" : "off",
         g_persistence ? "on" : "off",
         g_crossCheck ? "on" : "off");
    return 0;
}

BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID) { return TRUE; }