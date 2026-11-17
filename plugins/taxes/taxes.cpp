// v4.5 updates (City Hall shared-row-budget fix):
//   - Widget mode no longer duplicates the three rate/state text rows and
//     the Controls help row beneath equivalent widgets. With ui_panels v6's
//     two-row content-flow TAX button, the old expanded dashboard needed 16
//     rows by itself (17 with republic_bank), silently dropping the property
//     slider and badges from the 14-row budget. Macro rows + all controls +
//     the bank slider now take 13. The text-only fallback is unchanged.
// v4.4 updates (City Hall macro census & render-thread cache fix):
//   - Fixed the 64-wallet sample truncation bug in SnapshotMacroStats:
//     replaced both fixed-size stack buffers (id[64]/bal[64]) with a
//     single O(1)-stack streaming WalletAccum visitor so avgWallet and
//     Poverty Rate survey 100% of citizen wallets regardless of city size.
//   - Added a 500 ms + calendar-day render-thread cache (MacroDisplayCache,
//     invalidated atomically on TSM_SAVE_LOADING) in TaxPanelRows so an
//     open City Hall window at 60-144 FPS no longer acquires currency's
//     g_lock, heap-copies g_wallets, or walks g_companies every frame.
//   - Distinguishes "0.0% (no wallets yet)" on unpopulated/pre-payday maps
//     from genuine 0.0% poverty. Version -> 4.4.
// v4.2 updates (Task 7 - grounded City Hall macro readouts):
//   - Cost of Living is now the TRUE daily CPI survival basket
//     (resource_pricing v3 getCpiDailyBasket with enterprise_base's real
//     monthly residential rent injected), shown as "X.XX SYM/day" - the
//     old figure (average wallet) was a wealth proxy, not a cost.
//   - Poverty Rate is now the share of wallets under
//     poverty_runway_days (7) x the daily CPI - the same rationing
//     threshold citizen_retail's Liquidity Runway classifies Low on, so
//     the panel and the retail engine can never disagree about who is
//     poor. The old fixed poverty_wallet_floor is retired.
//   - NEW 4th macro row: Commercial Solvency - share of private
//     enterprises with positive capital plus the investment-pool
//     balance (enterprise_base getters, all lock-bounded).
//   - consumes resource_pricing (prefer v3, fallback v2/v1; Level 2 -> 1
//     is hierarchy-legal). Version -> 4.2.
// v4.1 updates (TAX tile role correction):
//   - The TAX tile is a BIG tab-row tile again (widgetButton), same size
//     as the native City Hall tab icons, docked next to the "Domestic
//     production & consumption" icon - the tester's "same size, next to
//     that button, like what we did with the privatise button". v4.0's
//     small toolbar tile was the wrong role - that belongs to the
//     construction panel's CON button.
//   - Position refined through ui_panels v5.0's tab-dock calibration
//     (pitch 63 / rowDy 185): the old fixed 70/175 sat ~22px right and
//     ~8px above the natives.
// taxes - the tax engine.
//
// Level 2. Consumes sim_clock, currency and enterprise_base; provides
// its own service.
//
// Three taxes, each with its own rate and its own calendar schedule:
//
//   VAT             a percentage of every private-shop sale, accrued
//                   the moment the sale is charged and remitted at the
//                   schedule's boundary.
//   Profit tax      a percentage of the period's net revenue minus
//                   business costs, computed at the settlement boundary
//                   from the enterprise's period totals.
//   Property tax    a flat per-enterprise charge, on no base at all.
//
// Settlement is per enterprise: debit the enterprise's capital by the
// amount due, credit the treasury under a named flow category, and
// clear the accrual. All three of these go through the enterprise_base
// public API. Direct struct access is not possible and not desired -
// the enterprise owns its own accrual fields, this service reads and
// writes them through the API it exposes for exactly that.
//
// WHAT THIS DOES NOT OWN
//
// The VAT rate is asked for by enterprise_base's retail interceptor on
// every sale, and by private_shops' pub service. Those plugins do not
// decide the rate; this service does. If this plugin is missing, no
// VAT is charged anywhere, and the enterprise_base card shows no tax
// income.

// v3.0 updates (native in-window widgets - replaces the v2 hotkeys):
//   - TaxPanelRows keeps its read-only text dashboard (one row per tax
//     plus the tune hint) so rates stay visible even when widgets are
//     unavailable; it no longer polls Tab/T, [ ] or Space;
//   - TaxPanelWidgets (new ui_panels v2 widget provider) draws the native
//     clickable controls through widgetSlider / widgetCheckbox:
//       VAT slider      0..100 -> 0..50% (step = tax_slider_step_pct);
//       Profit slider   0..100 -> 0..50% (same step);
//       Property slider 0..100 -> 0..property_slider_max (flat units);
//       one [x] Tax Enabled checkbox per tax;
//   - commit-on-change only: rates copied under g_lock, helpers called
//     with NO lock held, setters run only on change. The render thread
//     never blocks the sim thread.
//
// v3.4 updates (live re-test 3: tile clickable but mis-placed):
//   - PLACEMENT: the dashboard body (3 sliders + 3 badges) is drawn first
//     and the toggle tile LAST, so it sits at the bottom of the panel as a
//     footer. Drawn first (v3.2/v3.3) it landed between the "Controls:"
//     text row and the first slider - the mid-panel position reported in
//     the live test. Collapsed it is still the panel's only row.
//   - Budget unchanged at the exact 12-row fit: 4 text + 3 sliders +
//     3 badges + the 2-row tile.
// v3.3 updates (ui_panels v3.3 two-row native-sized button):
//   - The TAX tile now matches the native City Hall tab square (70 units
//     = 2 row pitches) and consumes TWO window+0x250 rows; the cosmetic
//     "Fiscal Policy" title row is dropped while widgets are present so
//     the expanded dashboard still fits exactly:
//     4 text + (2 + 3 + 3) widgets = 12 = TSM_PANEL_MAX_ROWS.
//     Collapsed = the 2-row button tile only.
//
// v3.2 updates (native square button header):
//   - The collapsed-panel header toggle is now a REAL graphical square
//     button tile (ui_panels v3 widgetButton: engine 9-patch chrome,
//     hover highlight, gui_click audio, active/depressed state) drawing
//     the centered label "TAX" - matching the City Hall icon-button
//     aesthetic. The v3.1 text checkbox remains as the fallback when
//     ui_panels is a v2 build (TsmUiPanelsHasButton guard);
//   - consume chain widened to v3 -> v2 -> v1 for mixed installs.
//
// v3.1 updates (polish + collapsible City Hall panel):
//   - Status badges replace the action-style checkbox labels: the label
//     always reads the LIVE state - "[X] VAT: Active" in green
//     (0xFF60E060) when levied, "[ ] VAT: Suspended" in grey
//     (0xFFA0A0A0) when off - instead of the confusing "VAT Enabled";
//   - Slider captions are clean rate readouts ("Rate: 42%",
//     "Rate: 250.00 RUB"); the [-]/[+] bar itself is the exe widget at
//     left + dpi*180 (owned by the ui_panels helper, no overlap);
//   - Collapsible panel: a header toggle checkbox
//     ("[ Fiscal Policy & Taxes ]") is always drawn first.
//     Collapsed, it is the ONLY line (1 row). Expanded, the dashboard
//     text rows plus 3 sliders + 3 badges follow (5 text + 7 widget =
//     12 = TSM_PANEL_MAX_ROWS, exact fit). The flag is render-thread-
//     only (widget pass writes, text pass reads, same thread) so no
//     lock is taken; without widgets the full dashboard stays.

#include "../../src/tesmio_plugin.h"
#include "../../src/game_world.h"
#include "../../src/sim_clock_api.h"
#include "../../src/currency_api.h"
#include "../../src/enterprise_base_api.h"
#include "../../src/resource_pricing_api.h"   // v4.2 (Task 7): the true CPI basket
#include "../../src/ui_panels_api.h"
#include "../../src/taxes_api.h"
#include "../../src/persistence_api.h"

#include <cstdlib>

// ---------------------------------------------------------------- services

static const TsmSimClockApi*       g_simClock   = nullptr;
static const TsmCurrencyApi*       g_currency   = nullptr;
static const TsmEnterpriseBaseApi* g_enterprise = nullptr;
static const TsmResourcePricingApi* g_pricing   = nullptr;   // v4.2 (Task 7)
static const TsmUiPanelsApi*       g_panels     = nullptr;
static const TsmPersistenceApi*    g_taxPers    = nullptr;
static int                         g_taxPersHandle = -1;

// ---------------------------------------------------------------- config

static int    g_enabled           = 1;
static int    g_probe             = 1;

static float  g_vatRate           = 0.20f;
static int    g_vatSchedule       = TSM_FREQ_MONTHLY;

static float  g_profitTaxRate     = 0.15f;
static int    g_profitSchedule    = TSM_FREQ_QUARTERLY;

static float  g_propertyTaxFlat   = 250.0f;
static int    g_propertySchedule  = TSM_FREQ_YEARLY;

// v4: RUB-denominated base for the flat property tax; USD-effective
// value resolves at Start (property_tax_flat_usd or usd_divisor).
static float  g_propertyTaxFlatCfg = 250.0f;
static float  g_usdDivisor         = 50.0f;

// v2 (Task 4): each tax carries an on/off switch the City Hall panel toggles.
// A suspended tax stops NEW accrual (getVatRate/getProfitTaxRate report 0),
// but any liability already accrued this period still settles at the boundary.
static int    g_vatEnabled       = 1;
static int    g_profitEnabled    = 1;
static int    g_propertyEnabled  = 1;

// v2 (Task 4): land-value property tax. The flat base is scaled by the
// building's attractiveness: Tax = base * (0.50 + 1.50 * attr), so ugly
// industrial land pays ~0.5x and a park-adjacent villa up to ~2x. 0 keeps the
// historic flat charge for every building.
static int    g_propertyLandValue = 1;

// v2 (Task 4): City Hall fiscal-policy panel. g_panelRows gates the whole
// feature. v3 widgets: g_sliderStepPct is the -/+ increment per click
// (percent points for VAT/profit, default 2); g_propertySliderMax is the
// flat-tax slider ceiling in active-currency units (default 500).
static int    g_panelRows = 1;     // city_hall_rows
static int    g_sliderStepPct  = 2;     // -/+ percent points per click
static float  g_propertySliderMax = 500.0f; // property slider ceiling (flat)
static int    g_panelsWidgets = 0; // v2 widget tail present
static int    g_panelsButtons = 0; // v3 button tail present (native square tile)
static int    g_panelsSmall   = 0; // v4.9 toolbar-docked small tile

// v7 (Task 3): Accounting Office macro indicators. g_macroIndicators gates
// the read-only rows pushed above the tax status rows in TaxPanelRows.
// v4.2 (Task 7): g_povertyRunwayDays replaces the fixed poverty_wallet_floor
// - the poverty floor is now days x the TRUE daily CPI basket (the same
// rationing threshold citizen_retail's Liquidity Runway uses).
static int    g_macroIndicators = 1;     // macro_indicators
static float  g_povertyRunwayDays = 7.0f;  // poverty_runway_days

// v3.1: collapsible City Hall panel. Render-thread-only flag: the widget
// pass writes it (header toggle checkbox), the text pass reads it, same
// thread, so no lock is taken. Default collapsed (1 line); expanding
// reveals the dashboard + sliders + badges (12 rows = the cap).
static bool   s_showTaxPanel = false;

// v3.1 status badges: green = levied, grey = suspended.
#define TAX_BADGE_ON_COLOUR  0xFF60E060
#define TAX_BADGE_OFF_COLOUR 0xFFA0A0A0

static const char* CurSym(void) { return TsmCurrencySymbol(g_currency); }

// Macro-wide readouts used above the tax status rows. Enumerated under the
// currency lock via forEachWallet (a consistent snapshot); each wallet is a
// plain float, no observer ordering depends on it.
// v4.2 (Task 7): the poverty floor is now poverty_runway_days x the TRUE
// daily CPI basket - the same rationing threshold citizen_retail's
// Liquidity Runway classifies Low on, so the panel and the retail engine
// can never disagree about who is poor.
struct MacroStats {
    double avgWallet;    // per-capita average liquid funds
    double totalWallet;
    size_t count;
    size_t belowFloor;   // wallets below runway x CPI -> poverty
    float  cpi;          // the daily basket the floor was measured against
};

// v4.2 (Task 7): the TRUE per-day rent for the CPI basket.
// enterprise_base v8 exposes the residential rent SETTING (per tenant
// per month, already in active-currency units); daysPerMonth comes from
// sim_clock (structSize-guarded, the same tail pattern as currency
// v3/v4). Returns 0 when unavailable so the pricing provider's neutral
// default_daily_rent (0.33 RUB) rides along inside getCpiDailyBasket.
static float CpiDailyRent(void)
{
    float monthly = 0.0f;
    if (g_enterprise &&
        g_enterprise->structSize >=
            offsetof(TsmEnterpriseBaseApi, getResidentialRentMonthly) +
                sizeof(g_enterprise->getResidentialRentMonthly) &&
        g_enterprise->getResidentialRentMonthly) {
        monthly = (float)g_enterprise->getResidentialRentMonthly();
    }
    if (!(monthly > 0.0f) || !(monthly == monthly)) return 0.0f;

    int dpm = 30;
    if (g_simClock &&
        g_simClock->structSize >=
            offsetof(TsmSimClockApi, getDaysPerMonth) +
                sizeof(g_simClock->getDaysPerMonth)) {
        int d = g_simClock->getDaysPerMonth();
        if (d >= 28 && d <= 31) dpm = d;
    }
    return monthly / (float)dpm;
}

struct WalletAccum {
    double total;
    size_t count;
    size_t belowFloor;
    float  floor;
    int    hasFloor;
};

static void SnapshotMacroStats(MacroStats* out, float cpi)
{
    out->avgWallet = 0.0;
    out->totalWallet = 0.0;
    out->count = 0;
    out->belowFloor = 0;
    out->cpi = (cpi == cpi && cpi > 0.0f) ? cpi : 0.0f;
    if (!g_currency || !g_currency->forEachWallet) return;

    // H5 FIX: when CPI unavailable (resource_pricing disabled), poverty is n/a — don't report 0%
    const int hasFloor = (out->cpi > 0.0f) ? 1 : 0;
    if (!hasFloor) {
        static bool logged = false;
        if (!logged) {
            Logf("taxes  CPI unavailable (resource_pricing disabled) — poverty rate n/a");
            logged = true;
        }
    }

    // v4.4: stream over 100% of wallets in O(1) stack space instead of
    // truncating at the first 64 entries of g_wallets.
    WalletAccum acc = {
        0.0,
        0,
        0,
        hasFloor ? (g_povertyRunwayDays * out->cpi) : 0.0f,
        hasFloor
    };
    g_currency->forEachWallet([](uint64_t /*cid*/, float bal, void* ud) -> int {
        WalletAccum* a = (WalletAccum*)ud;
        const float clean = (bal == bal && bal > 0.0f) ? bal : 0.0f;
        a->total += (double)clean;
        a->count++;
        if (a->hasFloor && clean < a->floor)
            a->belowFloor++;
        return 1;
    }, &acc);

    out->totalWallet = acc.total;
    out->count       = acc.count;
    out->belowFloor  = acc.belowFloor;
    if (acc.count > 0)
        out->avgWallet = acc.total / (double)acc.count;
}

// Render-thread cache TTL and cross-thread reload-invalidation epoch for
// City Hall macro readouts (avoids per-frame lock + O(N) wallet vector copy).
#define MACRO_CACHE_TTL_MS 500u
static volatile LONG g_macroCacheEpoch = 1;

// ---------------------------------------------------------------- persistence (v4.3)

#define TAXES_MAGIC          "TSMTAX01"
#define TAXES_VERSION        1u
#define TAXES_FILENAME       "tesmio_taxes.dat"

#pragma pack(push, 1)
struct TaxSaveHeader {
    char     magic[8];
    uint32_t version;
    float    vatRate;
    float    profitTaxRate;
    float    propertyTaxFlat;
    uint32_t vatEnabled;
    uint32_t profitEnabled;
    uint32_t propertyEnabled;
};
#pragma pack(pop)

static void SaveTaxes(const char* dir)
{
    if (!g_taxPers) return;

    TaxSaveHeader hdr;
    memset(&hdr, 0, sizeof(hdr));
    memcpy(hdr.magic, TAXES_MAGIC, 8);
    hdr.version       = TAXES_VERSION;
    hdr.vatRate       = g_vatRate;
    hdr.profitTaxRate = g_profitTaxRate;
    hdr.propertyTaxFlat = g_propertyTaxFlat;
    hdr.vatEnabled    = (uint32_t)g_vatEnabled;
    hdr.profitEnabled = (uint32_t)g_profitEnabled;
    hdr.propertyEnabled = (uint32_t)g_propertyEnabled;

    if (g_taxPers->writeAtomic(dir, TAXES_FILENAME, &hdr, sizeof(hdr)))
        Logf("taxes  saved rates+flags to %s\\%s", dir, TAXES_FILENAME);
    else
        Logf("taxes  WARN  could not write %s\\%s", dir, TAXES_FILENAME);
}

static void LoadTaxes(const char* dir)
{
    if (!g_taxPers) return;

    size_t len = 0;
    void* raw = g_taxPers->readFile(dir, TAXES_FILENAME, &len);
    if (!raw) return;
    if (len < sizeof(TaxSaveHeader)) { free(raw); return; }

    TaxSaveHeader hdr;
    memcpy(&hdr, raw, sizeof(hdr));

    if (memcmp(hdr.magic, TAXES_MAGIC, 8) != 0 ||
        hdr.version != TAXES_VERSION) {
        free(raw);
        return;
    }

    EnterCriticalSection(&g_lock);
    // A corrupt sidecar must not inject NaN/inf into the fiscal state.
    // Invalid fields keep the INI/Start value that is already loaded.
    if (hdr.vatRate >= 0.0f && hdr.vatRate <= 1.0f)
        g_vatRate = hdr.vatRate;
    if (hdr.profitTaxRate >= 0.0f && hdr.profitTaxRate <= 1.0f)
        g_profitTaxRate = hdr.profitTaxRate;
    if (hdr.propertyTaxFlat >= 0.0f && hdr.propertyTaxFlat <= 1.0e6f)
        g_propertyTaxFlat = hdr.propertyTaxFlat;
    g_vatEnabled   = hdr.vatEnabled ? 1 : 0;
    g_profitEnabled = hdr.profitEnabled ? 1 : 0;
    g_propertyEnabled = hdr.propertyEnabled ? 1 : 0;
    LeaveCriticalSection(&g_lock);

    free(raw);
    Logf("taxes  loaded rates+flags from %s\\%s (VAT %.0f%%, profit %.0f%%, property %.2f flat)",
         dir, TAXES_FILENAME,
         g_vatRate * 100.0f, g_profitTaxRate * 100.0f, g_propertyTaxFlat);
}

static void OnTaxesSaveLoad(TsmSaveEvent ev, const char* dir, void* /*ud*/)
{
    if (ev == TSM_SAVE_LOADING)
        InterlockedIncrement(&g_macroCacheEpoch);
    if (!dir || !g_taxPers) return;
    if (ev == TSM_SAVE_WRITING)  SaveTaxes(dir);
    else if (ev == TSM_SAVE_LOADING) LoadTaxes(dir);
}

// ---------------------------------------------------------------- settlement

// modifyCompanyCapital is a void façade: it refuses an invalid or decayed
// building internally. Crediting the treasury regardless would create
// money and drop the liability, so check the refusal conditions first.
// Enumerated enterprises are private by construction, so validity and the
// engine decay flag are what is left to test.
static bool CanDebitEnterprise(void* b)
{
    return b && TsmIsValidGamePtr(b) && !TsmIsDecayed(b);
}

// Serialises settlement against the retail accrual path. Without it a
// settleNow from the UI thread can interleave between the instant branch's
// getAccruals / addAccrual / clearAccrual and settle the same sale twice
// (or clear it without a debit). Lock order is taxes -> enterprise /
// currency; no enterprise or currency path calls back into settlement
// while holding its own lock.
struct TaxSettlementLock {
    TaxSettlementLock()  { EnterCriticalSection(&g_lock); }
    ~TaxSettlementLock() { LeaveCriticalSection(&g_lock); }
    TaxSettlementLock(const TaxSettlementLock&) = delete;
    TaxSettlementLock& operator=(const TaxSettlementLock&) = delete;
};

static void SettleVatAll(void)
{
    TaxSettlementLock settleLock;
    if (!g_enterprise || !g_currency) return;
    if (!g_vatEnabled) return;   // suspended: nothing NEW accrued, nothing to settle

    size_t n = g_enterprise->getEnterpriseCount();
    double total = 0.0;
    int count = 0;

    for (size_t i = 0; i < n; i++) {
        void* b = g_enterprise->getEnterpriseByIndex(i);
        if (!CanDebitEnterprise(b)) continue;

        double vat = 0.0, profit = 0.0;
        g_enterprise->getAccruals(b, &vat, &profit);
        if (vat <= 0.01) continue;

        g_enterprise->modifyCompanyCapital(b, -vat, TSM_CAP_TAX);
        g_currency->moveTreasury(vat, "vat");
        g_enterprise->clearAccrual(b, TSM_ACCRUAL_VAT);

        total += vat;
        count++;
    }

    if (g_probe && count > 0)
        Logf("taxes  vat settlement: %.2f %s from %d enterprise(s)",
             total, CurSym(), count);
}

static void SettleProfitAll(void)
{
    TaxSettlementLock settleLock;
    if (!g_enterprise) return;
    if (!g_profitEnabled) {
        // A suspended profit tax still owns the period boundary: drop the
        // running totals so re-enabling cannot tax the suspended window.
        // Clearing totals needs no debit eligibility: a decayed entry that
        // is still in the register must lose them too, or it would carry
        // the suspended window's profit into a later active period.
        size_t n = g_enterprise->getEnterpriseCount();
        for (size_t i = 0; i < n; i++) {
            void* b = g_enterprise->getEnterpriseByIndex(i);
            if (b) g_enterprise->clearPeriodTotals(b);
        }
        return;
    }
    if (!g_currency) return;

    size_t n = g_enterprise->getEnterpriseCount();
    double total = 0.0;
    int count = 0;

    for (size_t i = 0; i < n; i++) {
        void* b = g_enterprise->getEnterpriseByIndex(i);
        if (!CanDebitEnterprise(b)) continue;

        double gs = 0, vatAccrued = 0, ws = 0, wages = 0, foreign = 0;
        g_enterprise->getPeriodTotals(b, &gs, &vatAccrued,
                                      &ws, &wages, &foreign);

        double accruedVat = 0, accruedProfit = 0;
        g_enterprise->getAccruals(b, &accruedVat, &accruedProfit);

        // Net revenue is gross sales less the VAT that was collected on
        // them - the enterprise never owned that money. Business costs
        // are wholesale, foreign imports, and wages. Capital
        // expenditure is not deductible under this model.
        double netRevenue = gs - vatAccrued;
        double netCosts   = ws + foreign + wages;
        double profit     = netRevenue - netCosts;

        double computed = (profit > 0.0)
                        ? profit * (double)g_profitTaxRate : 0.0;
        double due = computed + accruedProfit;

        // The period ends at the profit schedule's boundary, whether or
        // not there was a profit. Clear the running totals before we
        // settle so a stall partway through cannot double count.
        g_enterprise->clearPeriodTotals(b);

        if (due > 0.01) {
            g_enterprise->modifyCompanyCapital(b, -due, TSM_CAP_TAX);
            g_currency->moveTreasury(due, "profit_tax");
            g_enterprise->clearAccrual(b, TSM_ACCRUAL_PROFIT);

            total += due;
            count++;
        }
    }

    if (g_probe && count > 0)
        Logf("taxes  profit settlement: %.2f %s from %d enterprise(s)",
             total, CurSym(), count);
}

static void SettlePropertyAll(void)
{
    TaxSettlementLock settleLock;
    if (!g_enterprise || !g_currency) return;
    if (!g_propertyEnabled) return;             // suspended
    if (!(g_propertyTaxFlat > 0.01f)) return;

    size_t n = g_enterprise->getEnterpriseCount();
    double total = 0.0;
    int count = 0;

    for (size_t i = 0; i < n; i++) {
        void* b = g_enterprise->getEnterpriseByIndex(i);
        if (!CanDebitEnterprise(b)) continue;
        if (!ReadablePtr(b, TSM_BLD_MIN_READABLE)) continue;

        // v2 (Task 4): land-value property tax. The flat base is scaled by the
        // building's environmental attractiveness (0..1, neutral 0.5 fallback):
        //   due = base * (0.50 + 1.50 * attr)
        // so derelict industrial land pays ~0.5x and a park-adjacent villa up
        // to ~2x the flat figure. TsmAttractiveness never faults; the pointer
        // is already validated above.
        double due = (double)g_propertyTaxFlat;
        if (g_propertyLandValue) {
            float attr = TsmAttractiveness(b);
            due *= (double)(0.50f + 1.50f * attr);
        }

        g_enterprise->modifyCompanyCapital(b, -due, TSM_CAP_TAX);
        g_currency->moveTreasury(due, "property_tax");
        total += due;
        count++;
    }

    if (g_probe && count > 0)
        Logf("taxes  property settlement: %.2f %s from %d "
             "enterprise(s)%s", total, CurSym(), count,
             g_propertyLandValue ? " (land-value scaled)" : "");
}

static void SettleFor(TsmScheduleFrequency freq)
{
    if ((TsmScheduleFrequency)g_vatSchedule      == freq) SettleVatAll();
    if ((TsmScheduleFrequency)g_profitSchedule   == freq) SettleProfitAll();
    if ((TsmScheduleFrequency)g_propertySchedule == freq) SettlePropertyAll();
}

// ---------------------------------------------------------------- calendar events

static void OnMonthly(void* /*ud*/, int /*d*/, int /*m*/, int /*y*/)
{
    SettleFor(TSM_FREQ_MONTHLY);
}

static void OnQuarterly(void* /*ud*/, int /*d*/, int /*m*/, int /*y*/)
{
    SettleFor(TSM_FREQ_QUARTERLY);
}

static void OnYearly(void* /*ud*/, int /*d*/, int /*m*/, int /*y*/)
{
    SettleFor(TSM_FREQ_YEARLY);
}

// ---------------------------------------------------------------- the API

// A suspended tax reports a zero rate, so no NEW liability accrues through
// the retail interceptor. An amount that already accrued before the switch
// is retained (VAT) or dropped with the period totals (profit) rather than
// settled while the tax is off; nothing is collected until it is re-enabled.
static float GetVatRateImpl(void)
{ return g_vatEnabled ? g_vatRate : 0.0f; }
static float GetProfitTaxRateImpl(void)
{ return g_profitEnabled ? g_profitTaxRate : 0.0f; }

static void RecordVatAccrualImpl(void* entity, double amount)
{
    if (!entity) return;
    // Positive, finite amount only: +inf would pass the old NaN test and
    // poison both the liability and periodVatAccrued (the API contract says
    // non-finite amounts are ignored).
    if (!(amount > 0.0) || !(amount <= 1.0e300)) return;
    if (!g_enterprise) return;

    TaxSettlementLock settleLock;

    if (g_vatSchedule == TSM_FREQ_INSTANT) {
        // Settle the moment it is accrued. The sale credited this VAT to
        // the shop as part of gross revenue, so it MUST reach the period
        // VAT total: SettleProfitAll derives net revenue as
        // grossSales - vatAccrued, and leaving this out taxed the VAT
        // itself. addAccrual bumps both fields; clearAccrual then drops
        // the liability.
        //
        // clearAccrual zeroes the WHOLE liability, so a figure that was
        // already outstanding (for example a sidecar saved under the
        // monthly schedule) must be settled in the same debit. Otherwise
        // the first instant sale would erase it without paying it.
        double priorVat = 0.0, priorProfit = 0.0;
        g_enterprise->getAccruals(entity, &priorVat, &priorProfit);
        g_enterprise->addAccrual(entity, TSM_ACCRUAL_VAT, amount);

        // No usable payer or no treasury: keep the liability rather than
        // clearing it without a matching debit/credit. The period total
        // still carries the sale so profit tax nets the VAT out.
        if (!CanDebitEnterprise(entity) || !g_currency) return;

        double due = amount + ((priorVat > 0.0) ? priorVat : 0.0);
        g_enterprise->clearAccrual(entity, TSM_ACCRUAL_VAT);
        g_enterprise->modifyCompanyCapital(entity, -due, TSM_CAP_TAX);
        g_currency->moveTreasury(due, "vat");
        return;
    }

    g_enterprise->addAccrual(entity, TSM_ACCRUAL_VAT, amount);
}

static void SettleNowImpl(void)
{
    // Manual settlement closes the current period exactly like a scheduled
    // boundary: SettleProfitAll clears the running totals even while the
    // profit tax is disabled, so re-enabling it cannot tax a settled window.
    SettleVatAll();
    SettleProfitAll();
    SettlePropertyAll();
}

// --- v2: interactive fiscal policy -----------------------------------------

static int IsTaxEnabledImpl(int taxKind)
{
    EnterCriticalSection(&g_lock);
    int e = 0;
    switch (taxKind) {
        case TSM_TAX_VAT:      e = g_vatEnabled      ? 1 : 0; break;
        case TSM_TAX_PROFIT:   e = g_profitEnabled   ? 1 : 0; break;
        case TSM_TAX_PROPERTY: e = g_propertyEnabled ? 1 : 0; break;
        default:               e = 0; break;
    }
    LeaveCriticalSection(&g_lock);
    return e;
}

static int SetTaxRateImpl(int taxKind, float rate)
{
    if (!(rate == rate)) return 0;                 // reject NaN
    EnterCriticalSection(&g_lock);
    int ok = 0;
    switch (taxKind) {
        case TSM_TAX_VAT:
            if (rate >= 0.0f && rate <= 1.0f) { g_vatRate = rate; ok = 1; }
            break;
        case TSM_TAX_PROFIT:
            if (rate >= 0.0f && rate <= 1.0f) { g_profitTaxRate = rate; ok = 1; }
            break;
        case TSM_TAX_PROPERTY:
            // A flat rate is not a fraction, but it must still be finite:
            // +inf would flow into modifyCompanyCapital and moveTreasury.
            if (rate >= 0.0f && rate <= 1.0e6f) {
                g_propertyTaxFlat = rate; ok = 1;
            }
            break;
        default: break;
    }
    LeaveCriticalSection(&g_lock);
    return ok;
}

static int SetTaxEnabledImpl(int taxKind, int enabled)
{
    int on = enabled ? 1 : 0;
    EnterCriticalSection(&g_lock);
    int ok = 0;
    switch (taxKind) {
        case TSM_TAX_VAT:      g_vatEnabled      = on; ok = 1; break;
        case TSM_TAX_PROFIT:   g_profitEnabled   = on; ok = 1; break;
        case TSM_TAX_PROPERTY: g_propertyEnabled = on; ok = 1; break;
        default: break;
    }
    LeaveCriticalSection(&g_lock);
    return ok;
}

// Render-thread snapshot: copied under g_lock, so the widget pass calls
// the helpers with NO lock held and commits only on change.
struct TaxSnapshot {
    float rate[3];    // VAT, profit fractions; property flat
    int   enabled[3];
};

// v4.6 / API v3: expose configured policy without triggering any tax work.
static int g_policySnapshotReady = 0; // protected by g_lock; set after Start
static int GetPolicySnapshotImpl(TsmTaxPolicySnapshot* out, unsigned outSize)
{
    if (!out || outSize < sizeof(TsmTaxPolicySnapshot)) return 0;
    TsmTaxPolicySnapshot snapshot = {};
    snapshot.structSize = sizeof(snapshot);
    EnterCriticalSection(&g_lock);
    if (!g_policySnapshotReady) {
        LeaveCriticalSection(&g_lock);
        return 0; // No reads during startup currency conversion; output untouched.
    }
    snapshot.configured[TSM_TAX_VAT] = g_vatRate;
    snapshot.configured[TSM_TAX_PROFIT] = g_profitTaxRate;
    snapshot.configured[TSM_TAX_PROPERTY] = g_propertyTaxFlat;
    snapshot.enabled[TSM_TAX_VAT] = g_vatEnabled ? 1 : 0;
    snapshot.enabled[TSM_TAX_PROFIT] = g_profitEnabled ? 1 : 0;
    snapshot.enabled[TSM_TAX_PROPERTY] = g_propertyEnabled ? 1 : 0;
    snapshot.propertyLandValue = g_propertyLandValue ? 1 : 0;
    LeaveCriticalSection(&g_lock);
    *out = snapshot;
    return 1;
}

static const TsmTaxesApi kApi = {
    sizeof(TsmTaxesApi),
    GetVatRateImpl,
    GetProfitTaxRateImpl,
    RecordVatAccrualImpl,
    SettleNowImpl,
    IsTaxEnabledImpl,
    SetTaxRateImpl,
    SetTaxEnabledImpl,
    GetPolicySnapshotImpl,
};

// ---------------------------------------------------------------- settings

static int ParseFreq(const char* s, int fallback)
{
    if (!s || !s[0]) return fallback;
    if (g_simClock) return (int)g_simClock->parseFrequency(
        s, (TsmScheduleFrequency)fallback);
    if (_stricmp(s, "instant")   == 0) return TSM_FREQ_INSTANT;
    if (_stricmp(s, "monthly")   == 0) return TSM_FREQ_MONTHLY;
    if (_stricmp(s, "quarterly") == 0) return TSM_FREQ_QUARTERLY;
    if (_stricmp(s, "yearly")    == 0) return TSM_FREQ_YEARLY;
    return fallback;
}

static void ReadSettings(void)
{
    const char* ini = "plugins\\taxes.ini";
    char v[64];

    g_enabled = H->configInt(ini, "taxes", "enabled", 1);
    g_probe   = H->configInt(ini, "taxes", "probe",   1);

    if (H->configString(ini, "taxes", "vat_rate",
                        v, sizeof(v), "") && v[0])
        g_vatRate = (float)atof(v);
    if (H->configString(ini, "taxes", "vat_schedule",
                        v, sizeof(v), "") && v[0])
        g_vatSchedule = ParseFreq(v, TSM_FREQ_MONTHLY);

    if (H->configString(ini, "taxes", "profit_tax_rate",
                        v, sizeof(v), "") && v[0])
        g_profitTaxRate = (float)atof(v);
    if (H->configString(ini, "taxes", "profit_tax_schedule",
                        v, sizeof(v), "") && v[0])
        g_profitSchedule = ParseFreq(v, TSM_FREQ_QUARTERLY);

    if (H->configString(ini, "taxes", "property_tax_flat",
                        v, sizeof(v), "") && v[0])
        g_propertyTaxFlatCfg = (float)atof(v);
    g_propertyTaxFlat = g_propertyTaxFlatCfg;
    if (H->configString(ini, "taxes", "property_tax_schedule",
                        v, sizeof(v), "") && v[0])
        g_propertySchedule = ParseFreq(v, TSM_FREQ_YEARLY);

    // v4: RUB->USD divisor for the flat property tax (and any other
    // flat figure a future tax adds). Explicit property_tax_flat_usd
    // overrides it at Start.
    g_usdDivisor = 50.0f;
    if (H->configString(ini, "taxes", "usd_divisor",
                        v, sizeof(v), "") && v[0]) {
        float d = (float)atof(v);
        if (d >= 1.0f && d <= 1000.0f && d == d) g_usdDivisor = d;
    }

    // v2 (Task 4): per-tax on/off switches and the City Hall panel config.
    // The switches are the STARTUP defaults the panel then toggles live.
    g_vatEnabled      = H->configInt(ini, "taxes", "vat_enabled", 1);
    g_profitEnabled   = H->configInt(ini, "taxes", "profit_tax_enabled", 1);
    g_propertyEnabled = H->configInt(ini, "taxes", "property_tax_enabled", 1);

    // Land-value property tax: scale the flat base by attractiveness.
    g_propertyLandValue = H->configInt(ini, "taxes", "property_land_value", 1);

    // v7 (Task 3): Accounting Office macro indicators. macro_indicators gates
    // the rows pushed above the tax status rows. v4.2 (Task 7): the poverty
    // floor is poverty_runway_days x the TRUE daily CPI (same threshold as
    // citizen_retail's Low band); the fixed poverty_wallet_floor is retired.
    g_macroIndicators = H->configInt(ini, "taxes", "macro_indicators", 1);
    g_povertyRunwayDays = 7.0f;
    if (H->configString(ini, "taxes", "poverty_runway_days",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 1.0f && f <= 3650.0f && f == f) g_povertyRunwayDays = f;
    }

    // v3: City Hall fiscal widgets + slider mapping. The v2 tune_step key
    // is still accepted as the percent-point step (fraction x 100).
    g_panelRows = H->configInt(ini, "taxes", "city_hall_rows", 1);
    g_sliderStepPct = 2;
    if (H->configString(ini, "taxes", "tax_slider_step_pct",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 1.0f && f <= 25.0f && f == f)
            g_sliderStepPct = (int)(f + 0.5f);
    } else if (H->configString(ini, "taxes", "tune_step",
                               v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 0.001f && f <= 0.25f && f == f)
            g_sliderStepPct = (int)(f * 100.0f + 0.5f);
    }
    if (g_sliderStepPct < 1)  g_sliderStepPct = 1;
    if (g_sliderStepPct > 50) g_sliderStepPct = 50;
    g_propertySliderMax = 500.0f;
    if (H->configString(ini, "taxes", "property_slider_max",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 10.0f && f <= 100000.0f && f == f) g_propertySliderMax = f;
    }

    if (!(g_vatRate >= 0.0f) || !(g_vatRate <= 1.0f))         g_vatRate = 0.0f;
    if (!(g_profitTaxRate >= 0.0f) ||
        !(g_profitTaxRate <= 1.0f))                           g_profitTaxRate = 0.0f;
    // The flat property tax is not a fraction, but it still has to be a
    // finite, bounded figure: inf/1e30 would reach both capital and the
    // treasury. NaN fails the lower test; inf fails the upper one.
    if (!(g_propertyTaxFlatCfg >= 0.0f) ||
        !(g_propertyTaxFlatCfg <= 1.0e6f))                    g_propertyTaxFlatCfg = 0.0f;
    if (!(g_propertyTaxFlat >= 0.0f) ||
        !(g_propertyTaxFlat <= 1.0e6f))                       g_propertyTaxFlat = 0.0f;
}

// ---------------------------------------------------------------- City Hall panel (v3)
//
// The Republic fiscal-policy dashboard, appended to the City Hall / Party HQ
// window (building type 39) via ui_panels. Two providers on the v2
// contract, both render-thread:
//
//   TaxPanelRows    four read-only text rows: one per tax (state + live
//                   rate) plus the tune hint documenting the sliders.
//                   Visible even when widgets are unavailable.
//   TaxPanelWidgets native clickable controls: one rate slider plus one
//                   [x] Tax Enabled checkbox per tax.
//
// VAT/profit sliders map 0..100 -> 0..50% (the mission cap); the property
// slider maps 0..100 -> 0..property_slider_max flat units. The selected
// category cursor is gone with the Tab/T hotkey; all three taxes are
// live at once.

#define TSM_TYPE_CITYHALL 39

static const wchar_t* kTaxNames[3] = { L"VAT", L"Profit", L"Property" };

static void PushTaxRow(TsmPanelRows* out, const wchar_t* label,
                       const wchar_t* value)
{
    if (out->count >= out->capacity || out->count >= TSM_PANEL_MAX_ROWS)
        return;
    TsmPanelRow* r = &out->rows[out->count++];
    wcsncpy_s(r->label, TSM_PANEL_LABEL, label, _TRUNCATE);
    wcsncpy_s(r->value, TSM_PANEL_VALUE, value, _TRUNCATE);
    r->colour = 0;   // the hook's default label colour
}

// Render-thread snapshot: copied under g_lock, so both providers and the
// widget pass work lock-free after this.
static void SnapshotTaxes(TaxSnapshot* s)
{
    memset(s, 0, sizeof(*s));
    EnterCriticalSection(&g_lock);
    s->rate[TSM_TAX_VAT]      = g_vatRate;
    s->rate[TSM_TAX_PROFIT]   = g_profitTaxRate;
    s->rate[TSM_TAX_PROPERTY] = g_propertyTaxFlat;
    s->enabled[TSM_TAX_VAT]      = g_vatEnabled      ? 1 : 0;
    s->enabled[TSM_TAX_PROFIT]   = g_profitEnabled   ? 1 : 0;
    s->enabled[TSM_TAX_PROPERTY] = g_propertyEnabled ? 1 : 0;
    LeaveCriticalSection(&g_lock);
}

// Slider mapping. Percent taxes: 0..100 -> 0..0.50 (mission cap 50%).
// Property: 0..100 -> 0..property_slider_max flat units.
static unsigned RateToSlider(float rate)
{
    float p = rate * 200.0f;   // 0.50 -> 100
    if (!(p == p)) p = 0.0f;
    if (p < 0.0f)   p = 0.0f;
    if (p > 100.0f) p = 100.0f;
    return (unsigned)(p + 0.5f);
}

static float SliderToRate(unsigned pct)
{
    if (pct > 100) pct = 100;
    return (float)pct / 200.0f;
}

static unsigned FlatToSlider(float flat)
{
    float m = (g_propertySliderMax > 0.0f) ? g_propertySliderMax : 500.0f;
    float p = flat * 100.0f / m;
    if (!(p == p)) p = 0.0f;
    if (p < 0.0f)   p = 0.0f;
    if (p > 100.0f) p = 100.0f;
    return (unsigned)(p + 0.5f);
}

static float SliderToFlat(unsigned pct)
{
    if (pct > 100) pct = 100;
    float m = (g_propertySliderMax > 0.0f) ? g_propertySliderMax : 500.0f;
    return m * (float)pct / 100.0f;
}

static void PollTaxKeys(void* building)
{
    // v3: hotkeys removed - sliders + checkboxes own interaction now.
    // Kept as a no-op stub so the diff stays reviewable.
    (void)building;
    return;
}

struct MacroDisplayCache {
    MacroStats stats;
    float      cpi;
    size_t     evics;
    size_t     entN;
    size_t     solvent;
    double     pool;
    DWORD      tick;
    int        day;
    int        currency;
    LONG       epoch;
    bool       valid;
};

static void TaxPanelRows(void* building, TsmPanelRows* out, void* /*ud*/)
{
    if (!g_panelRows || !building || !out) return;
    if (!TsmIsValidGamePtr(building) ||
        !ReadablePtr(building, TSM_BLD_MIN_READABLE)) return;
    if (TsmIsDemolished(building)) return;

    int type = TsmBuildingType(building);
    if (type != TSM_TYPE_CITYHALL) return;   // City Hall / Party HQ only

    // v3.1: collapsed + widgets present = zero text rows; the header
    // toggle checkbox is the panel's only line (1 row). Without widgets
    // there is no toggle, so the full dashboard stays (old v3 layout).
    if (g_panelsWidgets && !s_showTaxPanel) return;

    TaxSnapshot s;
    SnapshotTaxes(&s);

    const int curCode = TsmCurrencyActive(g_currency);
    const wchar_t* sym =
        (curCode == TSM_CURRENCY_USD) ? L"USD" : L"RUB";
    wchar_t v[TSM_PANEL_VALUE];

    // v7 (Task 3): Accounting Office macro indicators, pushed above the tax
    // status rows. v4.2 (Task 7): all four are GROUND-TRUTH readouts -
    // Cost of Living is the true daily CPI survival basket (live prices x
    // market multipliers x calibrated tonnes + the real residential rent),
    // Poverty Rate is the share of wallets under 7 days of that basket
    // (the same rationing threshold citizen_retail classifies Low on), and
    // Commercial Solvency surveys the enterprise register. In widget mode
    // these are the only text rows: rates and ON/OFF state already appear on
    // the controls themselves, so duplicating them starves the shared 14-row
    // City Hall budget (also used by republic_bank and the TAX button).
    // v4.4: cached on the render thread for 500 ms (or until day/save change)
    // so 60-144 FPS rendering never hammers currency::g_lock or heap-copies
    // g_wallets every frame.
    if (g_macroIndicators) {
        static MacroDisplayCache s_macroCache = {};
        const DWORD nowTick  = GetTickCount();
        const int   curDay   = (g_simClock && g_simClock->getDay)
                                   ? g_simClock->getDay()
                                   : -1;
        const LONG  curEpoch = InterlockedCompareExchange(&g_macroCacheEpoch, 0, 0);

        if (!s_macroCache.valid ||
            s_macroCache.epoch != curEpoch ||
            s_macroCache.day != curDay ||
            s_macroCache.currency != curCode ||
            (DWORD)(nowTick - s_macroCache.tick) >= MACRO_CACHE_TTL_MS) {

            float cpi = g_pricing
                            ? TsmPricingCpiBasket(g_pricing, CpiDailyRent())
                            : 0.0f;
            MacroStats stats;
            SnapshotMacroStats(&stats, cpi);

            size_t evics = 0;
            if (g_enterprise && g_enterprise->getMonthlyEvictionCount) {
                if (g_enterprise->structSize >=
                        offsetof(TsmEnterpriseBaseApi,
                                 getMonthlyEvictionCount) +
                                    sizeof(g_enterprise->getMonthlyEvictionCount)) {
                    evics = g_enterprise->getMonthlyEvictionCount();
                }
            }

            size_t entN = 0, solvent = 0;
            double pool = 0.0;
            if (g_enterprise && g_enterprise->getEnterpriseCount &&
                g_enterprise->getEnterpriseByIndex &&
                g_enterprise->getCompanyCapital) {
                entN = g_enterprise->getEnterpriseCount();
                for (size_t i = 0; i < entN; i++) {
                    void* b = g_enterprise->getEnterpriseByIndex(i);
                    if (b && g_enterprise->getCompanyCapital(b) > 0.0) solvent++;
                }
                if (g_enterprise->getPrivateInvestmentPool)
                    pool = g_enterprise->getPrivateInvestmentPool();
            }

            s_macroCache.stats    = stats;
            s_macroCache.cpi      = cpi;
            s_macroCache.evics    = evics;
            s_macroCache.entN     = entN;
            s_macroCache.solvent  = solvent;
            s_macroCache.pool     = pool;
            s_macroCache.tick     = nowTick;
            s_macroCache.day      = curDay;
            s_macroCache.currency = curCode;
            s_macroCache.epoch    = curEpoch;
            s_macroCache.valid    = true;
        }

        const MacroStats& stats = s_macroCache.stats;

        // Cost of Living: the TRUE daily CPI basket, "X.XX SYM/day" (or n/a if pricing disabled).
        if (!(s_macroCache.cpi > 0.0f)) {
            _snwprintf_s(v, _countof(v), _TRUNCATE, L"n/a (no CPI)");
        } else {
            _snwprintf_s(v, _countof(v), _TRUNCATE, L"%.2f %ls/day",
                         (double)s_macroCache.cpi, sym);
        }
        PushTaxRow(out, L"Cost of Living", v);

        // Poverty Rate: share of citizens below poverty_runway_days of
        // survival runway (wallet < days x CPI), "XX.X%" — H5 FIX: n/a when CPI missing
        if (!(stats.cpi > 0.0f)) {
            _snwprintf_s(v, _countof(v), _TRUNCATE, L"n/a (no CPI)");
        } else if (stats.count == 0) {
            _snwprintf_s(v, _countof(v), _TRUNCATE, L"0.0%% (no wallets yet)");
        } else {
            double poorPct = (double)stats.belowFloor /
                             (double)stats.count * 100.0;
            _snwprintf_s(v, _countof(v), _TRUNCATE, L"%.1f%%", poorPct);
        }
        PushTaxRow(out, L"Poverty Rate", v);

        // Evictions (Mo): successful evictions executed this month.
        _snwprintf_s(v, _countof(v), _TRUNCATE, L"%zu this month",
                     s_macroCache.evics);
        PushTaxRow(out, L"Evictions (Mo)", v);

        // v4.2 (Task 7): Commercial Solvency - share of private enterprises
        // with positive capital + the investment-pool balance.
        size_t solventPct = (s_macroCache.entN > 0)
                                ? (s_macroCache.solvent * 100) / s_macroCache.entN
                                : 100;
        _snwprintf_s(v, _countof(v), _TRUNCATE,
                     L"Solvent: %zu%% | Pool: %.0f %ls",
                     solventPct, s_macroCache.pool, sym);
        PushTaxRow(out, L"Commercial Solvency", v);
    }

    // The widget dashboard is the live rate/state dashboard; do not print a
    // second copy beneath it. Expanded macro layout: 4 text + 2 TAX-button
    // rows + 6 body widgets = 12, plus republic_bank's one slider = 13/14.
    // Without widgets, retain the complete text-only dashboard below.
    if (g_panelsWidgets) return;

    PushTaxRow(out, L"Fiscal Policy", L"Republic Tax Authority");

    for (int k = 0; k < 3; k++) {
        const wchar_t* name   = kTaxNames[k];
        const wchar_t* state = s.enabled[k] ? L"[ON]" : L"[OFF]";

        if (k == TSM_TAX_PROPERTY) {
            _snwprintf_s(v, _countof(v), _TRUNCATE,
                         L"%ls %.2f %ls flat%s", state,
                         s.rate[k], sym,
                         g_propertyLandValue ? L" (land-value)" : L"");
        } else {
            _snwprintf_s(v, _countof(v), _TRUNCATE,
                         L"%ls %.0f%%", state,
                         s.rate[k] * 100.0f);
        }
        PushTaxRow(out, name, v);
    }

    _snwprintf_s(v, _countof(v), _TRUNCATE,
                 L"Sliders 0-50%%, property 0-%.0f %ls",
                 g_propertySliderMax, sym);
    PushTaxRow(out, L"Controls", v);
}

// v3.4: the expanded dashboard body, split out of TaxPanelWidgets so the
// toggle tile can be drawn AFTER it (footer placement).
static void TaxPanelBody(void);

// Widget provider: the toggle tile (footer) +, when expanded, 3 rate
// sliders + 3 status badges. Runs on the render thread inside the
// ui_panels post-hook, AFTER the text rows. Commit-on-change only,
// helpers called with NO lock held.
//
// ui_panels v6 places the toggle tile in content flow, where it consumes two
// rows. The expanded macro dashboard uses 4 text + 2 button + 6 body rows =
// 12; republic_bank's City Hall slider brings the shared total to 13/14.
// Collapsed there are no tax text/body rows, only the two-row toggle.
static void TaxPanelWidgets(void* building, void* /*ud*/)
{
    if (!g_panelsWidgets || !g_panels) return;
    if (!g_panelRows || !building) return;
    if (!TsmIsValidGamePtr(building) ||
        !ReadablePtr(building, TSM_BLD_MIN_READABLE)) return;
    if (TsmIsDemolished(building)) return;
    if (TsmBuildingType(building) != TSM_TYPE_CITYHALL) return;

    // Draw the big content-flow TAX tile first, then append the dashboard
    // body at window+0x250. Older ui_panels builds without buttons use the
    // one-row checkbox header instead.
    if (g_panelsButtons) {
        if (g_panels->widgetButton(L"TAX", s_showTaxPanel ? 1 : 0)) {
            s_showTaxPanel = !s_showTaxPanel;
            if (g_probe)
                Logf("taxes  [WIDGET PANEL] City Hall fiscal panel -> %s",
                     s_showTaxPanel ? "expanded" : "collapsed");
        }
    } else {
        unsigned hd = s_showTaxPanel ? 1u : 0u;
        if (g_panels->widgetCheckbox(&hd, L"[ Fiscal Policy & Taxes ]", 0)) {
            s_showTaxPanel = (hd != 0);
            if (g_probe)
                Logf("taxes  [WIDGET PANEL] City Hall fiscal panel -> %s",
                     s_showTaxPanel ? "expanded" : "collapsed");
        }
    }

    // v3.8: the body AFTER the header tile.
    if (s_showTaxPanel) TaxPanelBody();
}

// v3.4/v3.8: the expanded dashboard body (3 rate sliders + 3 status badges).
// Drawn after the header toggle tile (v3.8) rather than before it.
static void TaxPanelBody(void)
{
    TaxSnapshot s;
    SnapshotTaxes(&s);
    const wchar_t* sym =
        (TsmCurrencyActive(g_currency) == TSM_CURRENCY_USD) ? L"USD" : L"RUB";

    for (int k = 0; k < 3; k++) {
        // v3.1: the slider caption is a clean rate readout on its own
        // indented row; the [-]/[+] bar is the exe widget at left+dpi*180
        // (owned by the ui_panels helper).
        wchar_t cap[96];
        unsigned pct;
        if (k == TSM_TAX_PROPERTY) {
            _snwprintf_s(cap, _countof(cap), _TRUNCATE,
                         L"Rate: %.2f %ls", s.rate[k], sym);
            pct = FlatToSlider(s.rate[k]);
        } else {
            _snwprintf_s(cap, _countof(cap), _TRUNCATE,
                         L"Rate: %.0f%%", s.rate[k] * 100.0f);
            pct = RateToSlider(s.rate[k]);
        }
        if (g_panels->widgetSlider(cap, &pct, g_sliderStepPct, 1)) {
            if (k == TSM_TAX_PROPERTY)
                SetTaxRateImpl(k, SliderToFlat(pct));
            else
                SetTaxRateImpl(k, SliderToRate(pct));
            if (g_probe)
                Logf("taxes  [WIDGET RATE] %ls -> slider %u", kTaxNames[k],
                     pct);
        }

        // v3.1 status badge: the label always reads the LIVE state.
        // [X] Name: Active (green) when levied, [ ] Name: Suspended
        // (grey) when off - never an action verb like "Enable X".
        wchar_t lab[96];
        _snwprintf_s(lab, _countof(lab), _TRUNCATE,
                     s.enabled[k] ? L"[X] %ls: Active" : L"[ ] %ls: Suspended",
                     kTaxNames[k]);
        unsigned en = s.enabled[k] ? 1u : 0u;
        if (g_panels->widgetCheckbox(&en, lab,
                s.enabled[k] ? TAX_BADGE_ON_COLOUR
                             : TAX_BADGE_OFF_COLOUR)) {
            SetTaxEnabledImpl(k, (en != 0) ? 1 : 0);
            if (g_probe)
                Logf("taxes  [WIDGET TOGGLE] %ls -> %s", kTaxNames[k],
                     (en != 0) ? "ON" : "OFF");
        }
    }
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
    info->name    = "taxes";
    info->version = "4.6";

    g_simClock = (const TsmSimClockApi*)
        H->consume(TSM_SERVICE_SIM_CLOCK, TSM_SIM_CLOCK_VERSION);

    ReadSettings();
    if (!g_enabled) {
        Logf("taxes  disabled");
        return 1;
    }

    // v3 ABI: retain exact-version v1/v2 prefix views for existing binaries.
    static TsmTaxesApi kApiV1;
    static TsmTaxesApi kApiV2;
    kApiV1 = kApi;
    kApiV1.structSize = (unsigned)offsetof(TsmTaxesApi, isTaxEnabled);
    kApiV1.getPolicySnapshot = nullptr;
    kApiV2 = kApi;
    kApiV2.structSize = (unsigned)offsetof(TsmTaxesApi, getPolicySnapshot);
    kApiV2.getPolicySnapshot = nullptr;
    H->provide(TSM_SERVICE_TAXES, TSM_TAXES_VERSION_V1, &kApiV1);
    H->provide(TSM_SERVICE_TAXES, TSM_TAXES_VERSION_V2, &kApiV2);
    H->provide(TSM_SERVICE_TAXES, TSM_TAXES_VERSION, &kApi);
    return 0;
}

extern "C" __declspec(dllexport) int TsmPluginStart(void)
{
    if (!g_enabled) return 1;

    if (!g_simClock)
        g_simClock = (const TsmSimClockApi*)
            H->consume(TSM_SERVICE_SIM_CLOCK, TSM_SIM_CLOCK_VERSION);
    if (!g_simClock) {
        Logf("taxes  sim_clock is not installed - cannot run");
        return 1;
    }

    // v4: prefer the unified-currency iface; fall back to v3 (RUB-only
    // routing) so a mixed install still settles.
    g_currency = (const TsmCurrencyApi*)
        H->consume(TSM_SERVICE_CURRENCY, TSM_CURRENCY_VERSION);
    if (!g_currency)
        g_currency = (const TsmCurrencyApi*)
            H->consume(TSM_SERVICE_CURRENCY, TSM_CURRENCY_VERSION_V3);
    if (!g_currency) {
        Logf("taxes  currency is not installed - cannot settle");
        return 1;
    }

    g_enterprise = (const TsmEnterpriseBaseApi*)
        H->consume(TSM_SERVICE_ENTERPRISE_BASE, TSM_ENTERPRISE_BASE_VERSION);
    if (!g_enterprise) {
        Logf("taxes  enterprise_base is not installed - there is nothing "
             "to tax. Install plugins\\enterprise_base to fix this.");
        return 1;
    }

    // v4.2 (Task 7): the true daily CPI survival basket for the City Hall
    // macro rows. Prefer pricing v3 (getCpiDailyBasket); fall back v2/v1
    // - the TsmPricingCpiBasket helper then degrades to the documented
    // ~2.15 default basket. Level 2 -> 1 consume is hierarchy-legal.
    g_pricing = (const TsmResourcePricingApi*)
        H->consume(TSM_SERVICE_RESOURCE_PRICING, TSM_RESOURCE_PRICING_VERSION);
    if (!g_pricing)
        g_pricing = (const TsmResourcePricingApi*)
            H->consume(TSM_SERVICE_RESOURCE_PRICING,
                       TSM_RESOURCE_PRICING_VERSION_V2);
    if (!g_pricing)
        g_pricing = (const TsmResourcePricingApi*)
            H->consume(TSM_SERVICE_RESOURCE_PRICING,
                       TSM_RESOURCE_PRICING_VERSION_V1);
    if (!g_pricing)
        Logf("taxes  WARN  resource_pricing not installed - the Cost of "
             "Living row degrades to the fixed 2.15 default basket");

    // v4 USD scale: the flat property tax divides the RUB base by
    // usd_divisor unless property_tax_flat_usd is set explicitly.
    if (TsmCurrencyActive(g_currency) == TSM_CURRENCY_USD) {
        const char* ini = "plugins\\taxes.ini";
        char v[64];
        if (H->configString(ini, "taxes", "property_tax_flat_usd",
                            v, sizeof(v), "") && v[0]) {
            float u = (float)atof(v);
            if (u >= 0.0f && u <= 1.0e6f) g_propertyTaxFlat = u;
        } else {
            float d = (g_usdDivisor >= 1.0f) ? g_usdDivisor : 50.0f;
            g_propertyTaxFlat = g_propertyTaxFlatCfg / d;
        }
        // One final gate on the RESOLVED value: the explicit override can
        // be inf, and the divisor path can overflow a tiny divisor.
        if (!(g_propertyTaxFlat >= 0.0f) ||
            !(g_propertyTaxFlat <= 1.0e6f))
            g_propertyTaxFlat = 0.0f;
    }

    g_simClock->registerScheduled(TSM_FREQ_MONTHLY,   OnMonthly,   nullptr);
    g_simClock->registerScheduled(TSM_FREQ_QUARTERLY, OnQuarterly, nullptr);
    g_simClock->registerScheduled(TSM_FREQ_YEARLY,    OnYearly,    nullptr);

    // v3.2: City Hall fiscal panel. ui_panels is the optional in-window
    // layer; TaxPanelRows renders the read-only dashboard, TaxPanelWidgets
    // draws the native square button header + sliders + status badges.
    // Prefer v3 (button), fall back to v2 (checkbox header) then v1.
    g_panels = (const TsmUiPanelsApi*)
        H->consume(TSM_SERVICE_UI_PANELS, TSM_UI_PANELS_VERSION);
    if (!g_panels)
        g_panels = (const TsmUiPanelsApi*)
            H->consume(TSM_SERVICE_UI_PANELS, TSM_UI_PANELS_VERSION_V2);
    if (!g_panels)
        g_panels = (const TsmUiPanelsApi*)
            H->consume(TSM_SERVICE_UI_PANELS, TSM_UI_PANELS_VERSION_V1);
    if (!g_panels)
        Logf("taxes  ui_panels is not installed - no City Hall fiscal panel");
    g_panelsWidgets = TsmUiPanelsHasWidgets(g_panels) ? 1 : 0;
    g_panelsButtons = TsmUiPanelsHasButton(g_panels) ? 1 : 0;
    g_panelsSmall   = TsmUiPanelsHasButtonSmall(g_panels) ? 1 : 0;
    if (g_panels && g_panelRows) {
        if (g_panels->registerRowProvider(TaxPanelRows, nullptr))
            Logf("taxes  City Hall fiscal-policy panel registered "
                 "(type %d, step %d pct, collapsible, %s)%s",
                 TSM_TYPE_CITYHALL, g_sliderStepPct,
                 g_panelsButtons ? "square button header" : "checkbox header",
                 g_panelsWidgets ? ", widgets on" : "");
        else
            Logf("taxes  WARN  ui_panels refused the City Hall row provider");
        if (g_panelsWidgets) {
            if (g_panels->registerWidgetProvider(TaxPanelWidgets, nullptr))
                Logf("taxes  City Hall fiscal widgets registered");
            else {
                g_panelsWidgets = 0;
                g_panelsButtons = 0;
                Logf("taxes  WARN  ui_panels refused the widget provider"
                     " - text rows only");
            }
        }
    }

    // v4.3 (Task: Tax Persistence): consume persistence and register the
    // taxes save handler so rate/badge changes survive a reload.
    g_taxPers = (const TsmPersistenceApi*)
        H->consume(TSM_SERVICE_PERSISTENCE, TSM_PERSISTENCE_VERSION);
    if (!g_taxPers)
        Logf("taxes  WARN  no persistence service - tax rates will reset on reload");
    else {
        g_taxPersHandle = g_taxPers->registerHandler("taxes", OnTaxesSaveLoad, nullptr);
        if (g_taxPersHandle <= 0)
            Logf("taxes  WARN  persistence refused the handler");
    }

    EnterCriticalSection(&g_lock);
    g_policySnapshotReady = 1;
    LeaveCriticalSection(&g_lock);

    Logf("taxes  live v4.6 (currency %s, macro readouts %s, CPI %s)",
         CurSym(),
         g_macroIndicators ? "on" : "off",
         TsmPricingHasCpi(g_pricing) ? "live (pricing v3)"
                                     : "default 2.15 fallback");
    Logf("taxes    VAT %.0f%% (%d, %s), profit %.0f%% (%d, %s), "
         "property %.2f flat (%d, %s%s)",
         g_vatRate * 100.0f, g_vatSchedule, g_vatEnabled ? "on" : "off",
         g_profitTaxRate * 100.0f, g_profitSchedule, g_profitEnabled ? "on" : "off",
         g_propertyTaxFlat, g_propertySchedule, g_propertyEnabled ? "on" : "off",
         g_propertyLandValue ? ", land-value" : "");
    return 0;
}

BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID) { return TRUE; }