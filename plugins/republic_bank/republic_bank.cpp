// republic_bank - working-capital credit lines and term loans for private enterprises.
// v1.2: native drags snap to the same whole-percent grid as the slider buttons;
// v1.1 fractional-drag saves are normalised on load. Credit caps and the retained
// cash buffer follow resource_pricing's live active-currency CPI basket.

#include "../../src/tesmio_plugin.h"
#include "../../src/sim_clock_api.h"
#include "../../src/currency_api.h"
#include "../../src/resource_pricing_api.h"
#include "../../src/enterprise_base_api.h"
#include "../../src/persistence_api.h"
#include "../../src/ui_panels_api.h"
#include "../../src/republic_bank_api.h"
#include "../../src/game_world.h"
#include "bank_ledger.h"

#include <vector>
#include <cstring>
#include <cstdio>

using namespace republic_bank;

static int  g_enabled = 1;
static int  g_autoOverdraft = 0;

static TsmSimClockApi*      g_clock = nullptr;
static TsmCurrencyApi*      g_currency = nullptr;
static const TsmResourcePricingApi* g_pricing = nullptr;
static TsmEnterpriseBaseApi* g_enterprise = nullptr;
static TsmPersistenceApi*   g_persistence = nullptr;
static TsmUiPanelsApi*     g_ui = nullptr;
static int g_persistenceHandle = 0;

static LoanBook g_book;
static double g_referenceCpiRub = 2.15;
static double g_usdDivisor = 50.0;
static double g_lastLoggedScale = -1.0;
static int g_lastLoggedCurrency = -1;

static LoanKey keyForBuilding(void* b) {
    std::string objName = "unknown";
    bool gotName = false;
    if (g_enterprise && b) {
        const char* on = g_enterprise->getBuildingObjectName(b);
        if (on && on[0]) { objName = on; gotName = true; }
    }
    float x = 0.0f, z = 0.0f;
    bool got = false;
    if (b && TsmIsValidGamePtr(b) && ReadablePtr(b, 0x400)) {
        const BYTE* nodeDirect = (const BYTE*)b + 0x320;
        if (ReadablePtr(nodeDirect, 0x40)) {
            float xx = *(const float*)(nodeDirect + 0x30);
            float zz = *(const float*)(nodeDirect + 0x38);
            if (xx == xx && zz == zz && (fabsf(xx) > 1.5f || fabsf(zz) > 1.5f) &&
                fabsf(xx) < 100000.0f && fabsf(zz) < 100000.0f) {
                x = xx; z = zz; got = true;
            }
        }
        if (!got) {
            const BYTE* nodePtr = *(const BYTE* const*)((const BYTE*)b + 0x320);
            if (TsmIsValidGamePtr(nodePtr) && ReadablePtr(nodePtr, 0x40)) {
                float xx = *(const float*)(nodePtr + 0x30);
                float zz = *(const float*)(nodePtr + 0x38);
                if (xx == xx && zz == zz && (fabsf(xx) > 1.5f || fabsf(zz) > 1.5f) &&
                    fabsf(xx) < 100000.0f && fabsf(zz) < 100000.0f) {
                    x = xx; z = zz; got = true;
                }
            }
        }
    }
    if (!gotName && !got) {
        char ptrKey[32];
        _snprintf_s(ptrKey, sizeof(ptrKey), _TRUNCATE, "ptr:%p", b);
        objName = ptrKey;
    }
    return LoanBook::makeKey(objName, x, z);
}

// The INI limits are reference-RUB amounts, not live nominal constants.
// resource_pricing's v3 CPI basket is already denominated in the ACTIVE
// currency, so live CPI / reference RUB CPI performs currency conversion and
// inflation/deflation indexation together.  With an older/missing pricing
// service, retain neutral prices but still honour the active currency.
static double CurrentEconomicScale(void) {
    const int currency = TsmCurrencyActive(g_currency);
    const double divisor = (g_usdDivisor >= 1.0) ? g_usdDivisor : 50.0;
    const double fallback = (currency == TSM_CURRENCY_USD) ? 1.0 / divisor : 1.0;
    if (!TsmPricingHasCpi(g_pricing)) return fallback;
    const float cpi = g_pricing->getCpiDailyBasket(0.0f);
    return BankEconomicScale((double)cpi, g_referenceCpiRub, fallback);
}

static double RefreshEconomicScale(void) {
    const double scale = CurrentEconomicScale();
    EnterCriticalSection(&g_lock);
    g_book.config.currencyScale = scale;
    LeaveCriticalSection(&g_lock);
    return scale;
}

static double getRateImpl(void) { EnterCriticalSection(&g_lock); double r=g_book.getAnnualInterestRate(); LeaveCriticalSection(&g_lock); return r; }
static void setRateImpl(double r) { EnterCriticalSection(&g_lock); g_book.setAnnualInterestRate(NormalizeBankRate(r)); LeaveCriticalSection(&g_lock); }

static double requestCreditDrawImpl(void* building, double neededAmount, const char* reason) {
    if (!g_enterprise || !building || !g_enabled) return 0.0;
    RefreshEconomicScale();
    LoanKey k = keyForBuilding(building);
    int streak = g_enterprise->isCompanySolvent(building) ? 1 : 0;
    EnterCriticalSection(&g_lock);
    double approved = g_book.requestCreditDraw(k, neededAmount, reason, streak);
    LeaveCriticalSection(&g_lock);
    if (approved > 0.0) {
        g_enterprise->modifyCompanyCapital(building, approved, TSM_CAP_OTHER);
        if (g_currency) g_currency->moveTreasury(-approved, "bank_loan_issue");
    }
    return approved;
}

static void repayPrincipalImpl(void* building, double amount) {
    if (!building) return;
    LoanKey k = keyForBuilding(building);
    double p = 0.0, mi = 0.0; int m = 0;
    EnterCriticalSection(&g_lock);
    g_book.getEnterpriseDebt(k, p, mi, m);
    LeaveCriticalSection(&g_lock);
    if (amount > p) amount = p;
    if (amount < 0.0) amount = 0.0;
    double cap = g_enterprise ? g_enterprise->getCompanyCapital(building) : amount;
    if (amount > cap) amount = cap;
    if (amount < 0.0) amount = 0.0;
    EnterCriticalSection(&g_lock);
    g_book.repayPrincipal(k, amount);
    LeaveCriticalSection(&g_lock);
    if (g_enterprise) g_enterprise->modifyCompanyCapital(building, -amount, TSM_CAP_OTHER);
    if (g_currency && amount > 0.0) g_currency->moveTreasury(amount, "bank_loan_repay");
}

static void getEnterpriseDebtImpl(void* building, double* outP, double* outMI, int* outMissed) {
    LoanKey k = keyForBuilding(building);
    double p = 0.0, mi = 0.0; int m = 0;
    EnterCriticalSection(&g_lock);
    g_book.getEnterpriseDebt(k, p, mi, m);
    LeaveCriticalSection(&g_lock);
    if (outP) *outP = p;
    if (outMI) *outMI = mi;
    if (outMissed) *outMissed = m;
}

static const TsmRepublicBankApi kApi = {
    sizeof(TsmRepublicBankApi),
    requestCreditDrawImpl,
    repayPrincipalImpl,
    getEnterpriseDebtImpl,
    getRateImpl,
    setRateImpl
};

static void onSave(TsmSaveEvent event, const char* dir, void* userData) {
    if (!g_persistence || !dir) return;
    if (event == TSM_SAVE_WRITING) {
        RefreshEconomicScale();
        EnterCriticalSection(&g_lock);
        auto bytes = g_book.saveBytes();
        LeaveCriticalSection(&g_lock);
        g_persistence->writeAtomic(dir, "republic_bank.bin", bytes.data(), bytes.size());
    } else {
        size_t len = 0;
        void* buf = g_persistence->readFile(dir, "republic_bank.bin", &len);
        const bool hadState = (buf != nullptr);
        if (buf) {
            std::vector<char> v((char*)buf, (char*)buf + len);
            EnterCriticalSection(&g_lock);
            g_book.loadBytes(v);
            LeaveCriticalSection(&g_lock);
            free(buf);
        } else {
            EnterCriticalSection(&g_lock);
            auto cfg = g_book.config;
            g_book = LoanBook();
            g_book.config = cfg;
            LeaveCriticalSection(&g_lock);
        }

        double rate = 0.0;
        size_t entries = 0;
        EnterCriticalSection(&g_lock);
        // v1.1 could save off-grid drag values such as 3.75% beneath a 4%
        // caption. Migrate them to the visible whole-percent choice.
        g_book.setAnnualInterestRate(NormalizeBankRate(g_book.getAnnualInterestRate()));
        rate = g_book.getAnnualInterestRate();
        entries = g_book.loans.size();
        LeaveCriticalSection(&g_lock);

        // v1 files contain a scale for backwards compatibility, but it is a
        // derived runtime value.  Never let a stale saved scale freeze the
        // limits against the newly loaded world's prices or currency.
        const double scale = RefreshEconomicScale();
        Logf("republic_bank  %s %s\\republic_bank.bin: rate %.2f%%, "
             "ledger entries %zu, economic index x%.4f",
             hadState ? "loaded state from" : "no saved state at", dir,
             rate * 100.0, entries, scale);
        g_lastLoggedScale = -1.0;
        g_lastLoggedCurrency = -1;
    }
}

static void onMonth(void* userData, int day, int month, int year) {
    RefreshEconomicScale();
    if (!g_enterprise) return;
    size_t n = g_enterprise->getEnterpriseCount();
    for (size_t i = 0; i < n; i++) {
        void* b = g_enterprise->getEnterpriseByIndex(i);
        if (!b) continue;
        LoanKey k = keyForBuilding(b);
        double capital = g_enterprise->getCompanyCapital(b);
        EnterCriticalSection(&g_lock);
        Settlement s = g_book.monthlySettle(k, capital);
        LeaveCriticalSection(&g_lock);
        if (s.enterprise_delta != 0.0 && g_enterprise)
            g_enterprise->modifyCompanyCapital(b, s.enterprise_delta, TSM_CAP_OTHER);
        if (g_currency) {
            if (s.interest_transfer > 0.0) g_currency->moveTreasury(s.interest_transfer, "bank_interest");
            if (s.principal_sweep > 0.0) g_currency->moveTreasury(s.principal_sweep, "bank_loan_repay");
        }
    }
}

static void onDay(void* userData, int day, int month, int year) {
    const double scale = RefreshEconomicScale();
    const int currency = TsmCurrencyActive(g_currency);
    if (g_lastLoggedScale < 0.0 || currency != g_lastLoggedCurrency ||
        fabs(scale - g_lastLoggedScale) > 0.0001) {
        EnterCriticalSection(&g_lock);
        const double maxDebt = g_book.config.maxDebtPerEnterprise * scale;
        const double creditLimit = g_book.config.creditLimit * scale;
        const double cashBuffer = g_book.config.cashBufferKeep * scale;
        LeaveCriticalSection(&g_lock);
        Logf("republic_bank  economic index x%.4f (%s): debt cap %.2f, "
             "draw limit %.2f, cash buffer %.2f",
             scale, TsmCurrencySymbol(g_currency), maxDebt, creditLimit,
             cashBuffer);
        g_lastLoggedScale = scale;
        g_lastLoggedCurrency = currency;
    }
    if (!g_autoOverdraft) return;
    if (!g_enterprise) return;
    size_t n = g_enterprise->getEnterpriseCount();
    for (size_t i = 0; i < n; i++) {
        void* b = g_enterprise->getEnterpriseByIndex(i);
        if (!b) continue;
        double capital = g_enterprise->getCompanyCapital(b);
        if (capital >= 0.0) continue;
        LoanKey k = keyForBuilding(b);
        EnterCriticalSection(&g_lock);
        int isDef = g_book.isInDefault(k);
        LeaveCriticalSection(&g_lock);
        if (isDef) continue;
        int streak = g_enterprise->isCompanySolvent(b) ? 1 : 0;
        EnterCriticalSection(&g_lock);
        double approved = g_book.dailyOverdraftRescue(k, capital, streak);
        LeaveCriticalSection(&g_lock);
        if (approved > 0.0) {
            g_enterprise->modifyCompanyCapital(b, approved, TSM_CAP_OTHER);
            if (g_currency) g_currency->moveTreasury(-approved, "bank_loan_issue");
        }
    }
}

static void readSettings() {
    const char* ini = "plugins\\republic_bank.ini";
    g_enabled = H->configInt(ini, "republic_bank", "enabled", 1);
    g_autoOverdraft = H->configInt(ini, "republic_bank", "auto_overdraft_enabled", 0);
    g_book.config.autoOverdraftEnabled = g_autoOverdraft;
    char v[64];
    double rate = 0.06;
    if (H->configString(ini, "republic_bank", "annual_interest_rate", v, sizeof(v), "") && v[0]) rate = atof(v);
    if (!std::isfinite(rate) || rate < 0.0) rate = 0.06;
    g_book.setAnnualInterestRate(NormalizeBankRate(rate));
    double md = 100000.0;
    if (H->configString(ini, "republic_bank", "max_debt_per_enterprise", v, sizeof(v), "") && v[0]) md = atof(v);
    if (!std::isfinite(md) || md < 0.0) md = 100000.0;
    g_book.config.maxDebtPerEnterprise = md;
    double cl = 50000.0;
    if (H->configString(ini, "republic_bank", "credit_limit", v, sizeof(v), "") && v[0]) cl = atof(v);
    if (!std::isfinite(cl) || cl < 0.0) cl = 50000.0;
    g_book.config.creditLimit = cl;
    double cb = 1000.0;
    if (H->configString(ini, "republic_bank", "cash_buffer_keep", v, sizeof(v), "") && v[0]) cb = atof(v);
    if (!std::isfinite(cb) || cb < 0.0) cb = 1000.0;
    g_book.config.cashBufferKeep = cb;
    if (H->configString(ini, "republic_bank", "reference_cpi_rub", v, sizeof(v), "") && v[0]) {
        const double ref = atof(v);
        if (std::isfinite(ref) && ref > 0.0) g_referenceCpiRub = ref;
    }
    if (H->configString(ini, "republic_bank", "usd_divisor", v, sizeof(v), "") && v[0]) {
        const double d = atof(v);
        if (std::isfinite(d) && d >= 1.0 && d <= 1000.0) g_usdDivisor = d;
    }
    g_book.config.maxMissedMonths = H->configInt(ini, "republic_bank", "max_missed_months", 3);
}

extern "C" __declspec(dllexport) unsigned TsmPluginApiVersion(void) {
    return TSM_API_VERSION;
}

extern "C" __declspec(dllexport) int TsmPluginInit(const TsmHost* host, TsmPluginInfo* info) {
    TsmBind(host);
    info->name = "republic_bank";
    info->version = "1.2";

    readSettings();
    if (!g_enabled) {
        Logf("republic_bank  disabled");
        return 1;
    }

    H->provide(TSM_SERVICE_REPUBLIC_BANK, TSM_REPUBLIC_BANK_VERSION, &kApi);
    return 0;
}

extern "C" __declspec(dllexport) int TsmPluginStart(void) {
    if (!g_enabled) return 1;
    g_clock = (TsmSimClockApi*)H->consume(TSM_SERVICE_SIM_CLOCK, TSM_SIM_CLOCK_VERSION);
    g_currency = (TsmCurrencyApi*)H->consume(TSM_SERVICE_CURRENCY, TSM_CURRENCY_VERSION);
    if (!g_currency) g_currency = (TsmCurrencyApi*)H->consume(TSM_SERVICE_CURRENCY, TSM_CURRENCY_VERSION_V3);
    g_pricing = (const TsmResourcePricingApi*)H->consume(
        TSM_SERVICE_RESOURCE_PRICING, TSM_RESOURCE_PRICING_VERSION);
    if (!g_pricing) g_pricing = (const TsmResourcePricingApi*)H->consume(
        TSM_SERVICE_RESOURCE_PRICING, TSM_RESOURCE_PRICING_VERSION_V3);
    if (!g_pricing) g_pricing = (const TsmResourcePricingApi*)H->consume(
        TSM_SERVICE_RESOURCE_PRICING, TSM_RESOURCE_PRICING_VERSION_V2);
    if (!g_pricing) g_pricing = (const TsmResourcePricingApi*)H->consume(
        TSM_SERVICE_RESOURCE_PRICING, TSM_RESOURCE_PRICING_VERSION_V1);
    if (!TsmPricingHasCpi(g_pricing))
        Logf("republic_bank  WARN  resource_pricing v3 CPI unavailable - "
             "using neutral prices with active-currency scaling");
    g_enterprise = (TsmEnterpriseBaseApi*)H->consume(TSM_SERVICE_ENTERPRISE_BASE, TSM_ENTERPRISE_BASE_VERSION);
    if (!g_enterprise) g_enterprise = (TsmEnterpriseBaseApi*)H->consume(TSM_SERVICE_ENTERPRISE_BASE, TSM_ENTERPRISE_BASE_VERSION_V7);
    g_persistence = (TsmPersistenceApi*)H->consume(TSM_SERVICE_PERSISTENCE, TSM_PERSISTENCE_VERSION);
    g_ui = (TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS, TSM_UI_PANELS_VERSION);
    if (!g_ui) g_ui = (TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS, TSM_UI_PANELS_VERSION_V5);
    if (g_persistence) {
        g_persistenceHandle = g_persistence->registerHandler("republic_bank", onSave, nullptr);
    }
    if (g_clock) {
        g_clock->registerScheduled(TSM_FREQ_MONTHLY, onMonth, nullptr);
        g_clock->registerDay(onDay, nullptr);
    }
    if (g_ui && TsmUiPanelsHasWidgets(g_ui)) {
        g_ui->registerRowProvider([](void* building, TsmPanelRows* out, void* ud) {
            if (!out) return;
            if (!g_enterprise || !g_enterprise->isBuildingPrivate(building)) return;
            double p = 0, mi = 0; int m = 0;
            getEnterpriseDebtImpl(building, &p, &mi, &m);
            if (p <= 0.0) return;
            if (out->count >= out->capacity) return;
            TsmPanelRow* r = &out->rows[out->count++];
            const wchar_t* lbl = L"Bank debt";
            for (int i = 0; i < TSM_PANEL_LABEL - 1 && lbl[i]; i++) r->label[i] = lbl[i];
            r->label[TSM_PANEL_LABEL-1] = 0;
            wchar_t buf[64];
            _snwprintf_s(buf, 64, _TRUNCATE, L"%.2f (int %.2f)", p, mi);
            for (int i = 0; i < TSM_PANEL_VALUE - 1 && buf[i]; i++) r->value[i] = buf[i];
            r->value[TSM_PANEL_VALUE-1] = 0;
            r->colour = 0;
        }, nullptr);
        g_ui->registerWidgetProvider([](void* building, void* ud) {
            if (!g_ui || !TsmUiPanelsHasWidgets(g_ui)) return;
            if (TsmBuildingType(building) != 39) return;
            const double rate = getRateImpl();
            unsigned v = BankRateToSlider(rate);
            wchar_t caption[64];
            _snwprintf_s(caption, _countof(caption), _TRUNCATE,
                         L"Bank rate: %.0f%%", rate * 100.0);
            if (g_ui->widgetSlider(caption, &v, 4, 1)) {
                v = BankRateSnapSlider(v);
                const double newRate = BankRateFromSlider(v);
                setRateImpl(newRate);
                Logf("republic_bank  [WIDGET RATE] Bank rate -> %.2f%% "
                     "(slider %u)", newRate * 100.0, v);
            }
        }, nullptr);
    }
    Logf("republic_bank  live");
    return 0;
}

BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID) { return TRUE; }
