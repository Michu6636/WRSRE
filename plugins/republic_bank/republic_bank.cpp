// republic_bank - working-capital credit lines and term loans for private enterprises.

#include "../../src/tesmio_plugin.h"
#include "../../src/sim_clock_api.h"
#include "../../src/currency_api.h"
#include "../../src/enterprise_base_api.h"
#include "../../src/persistence_api.h"
#include "../../src/ui_panels_api.h"
#include "../../src/republic_bank_api.h"
#include "bank_ledger.h"

#include <vector>
#include <cstring>

using namespace republic_bank;

static int  g_enabled = 1;
static int  g_autoOverdraft = 0;

static TsmSimClockApi*      g_clock = nullptr;
static TsmCurrencyApi*      g_currency = nullptr;
static TsmEnterpriseBaseApi* g_enterprise = nullptr;
static TsmPersistenceApi*   g_persistence = nullptr;
static TsmUiPanelsApi*     g_ui = nullptr;
static int g_persistenceHandle = 0;

static LoanBook g_book;

static LoanKey keyForBuilding(void* b) {
    LoanKey k;
    k.qx = 0;
    k.qz = 0;
    if (g_enterprise && b) {
        const char* on = g_enterprise->getBuildingObjectName(b);
        if (on) k.objectName = on;
    }
    if (k.objectName.empty()) k.objectName = "unknown";
    return k;
}

static double getRateImpl(void) { return g_book.getAnnualInterestRate(); }
static void setRateImpl(double r) { g_book.setAnnualInterestRate(r); }

static double requestCreditDrawImpl(void* building, double neededAmount, const char* reason) {
    if (!g_enterprise || !building || !g_enabled) return 0.0;
    LoanKey k = keyForBuilding(building);
    int streak = g_enterprise->isCompanySolvent(building) ? 1 : 0;
    double approved = g_book.requestCreditDraw(k, neededAmount, reason, streak);
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
    g_book.getEnterpriseDebt(k, p, mi, m);
    if (amount > p) amount = p;
    if (amount < 0.0) amount = 0.0;
    g_book.repayPrincipal(k, amount);
    if (g_enterprise) g_enterprise->modifyCompanyCapital(building, -amount, TSM_CAP_OTHER);
    if (g_currency && amount > 0.0) g_currency->moveTreasury(amount, "bank_loan_repay");
}

static void getEnterpriseDebtImpl(void* building, double* outP, double* outMI, int* outMissed) {
    LoanKey k = keyForBuilding(building);
    double p = 0.0, mi = 0.0; int m = 0;
    g_book.getEnterpriseDebt(k, p, mi, m);
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
        auto bytes = g_book.saveBytes();
        g_persistence->writeAtomic(dir, "republic_bank.bin", bytes.data(), bytes.size());
    } else {
        size_t len = 0;
        void* buf = g_persistence->readFile(dir, "republic_bank.bin", &len);
        if (buf) {
            std::vector<char> v((char*)buf, (char*)buf + len);
            g_book.loadBytes(v);
            free(buf);
        }
    }
}

static void onMonth(void* userData, int day, int month, int year) {
    if (!g_enterprise) return;
    size_t n = g_enterprise->getEnterpriseCount();
    for (size_t i = 0; i < n; i++) {
        void* b = g_enterprise->getEnterpriseByIndex(i);
        if (!b) continue;
        LoanKey k = keyForBuilding(b);
        double capital = g_enterprise->getCompanyCapital(b);
        Settlement s = g_book.monthlySettle(k, capital);
        if (s.enterprise_delta != 0.0 && g_enterprise)
            g_enterprise->modifyCompanyCapital(b, s.enterprise_delta, TSM_CAP_OTHER);
        if (g_currency) {
            if (s.interest_transfer > 0.0) g_currency->moveTreasury(s.interest_transfer, "bank_interest");
            if (s.principal_sweep > 0.0) g_currency->moveTreasury(s.principal_sweep, "bank_loan_repay");
        }
    }
}

static void onDay(void* userData, int day, int month, int year) {
    if (!g_book.config.autoOverdraftEnabled) return;
    if (!g_enterprise) return;
    size_t n = g_enterprise->getEnterpriseCount();
    for (size_t i = 0; i < n; i++) {
        void* b = g_enterprise->getEnterpriseByIndex(i);
        if (!b) continue;
        double capital = g_enterprise->getCompanyCapital(b);
        if (capital >= 0.0) continue;
        LoanKey k = keyForBuilding(b);
        if (g_book.isInDefault(k)) continue;
        int streak = g_enterprise->isCompanySolvent(b) ? 1 : 0;
        double approved = g_book.dailyOverdraftRescue(k, capital, streak);
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
    if (rate <= 0) rate = 0.06;
    g_book.setAnnualInterestRate(rate);
    double md = 100000.0;
    if (H->configString(ini, "republic_bank", "max_debt_per_enterprise", v, sizeof(v), "") && v[0]) md = atof(v);
    g_book.config.maxDebtPerEnterprise = md;
    double cl = 50000.0;
    if (H->configString(ini, "republic_bank", "credit_limit", v, sizeof(v), "") && v[0]) cl = atof(v);
    g_book.config.creditLimit = cl;
    double cb = 1000.0;
    if (H->configString(ini, "republic_bank", "cash_buffer_keep", v, sizeof(v), "") && v[0]) cb = atof(v);
    g_book.config.cashBufferKeep = cb;
    g_book.config.maxMissedMonths = H->configInt(ini, "republic_bank", "max_missed_months", 3);
}

extern "C" __declspec(dllexport) unsigned TsmPluginApiVersion(void) {
    return TSM_API_VERSION;
}

extern "C" __declspec(dllexport) int TsmPluginInit(const TsmHost* host, TsmPluginInfo* info) {
    TsmBind(host);
    info->name = "republic_bank";
    info->version = "1.0";

    readSettings();
    if (!g_enabled) {
        Logf("republic_bank  disabled");
        return 1;
    }

    g_clock = (TsmSimClockApi*)H->consume(TSM_SERVICE_SIM_CLOCK, TSM_SIM_CLOCK_VERSION);
    // currency v4 with v3 fallback
    g_currency = (TsmCurrencyApi*)H->consume(TSM_SERVICE_CURRENCY, TSM_CURRENCY_VERSION);
    if (!g_currency) g_currency = (TsmCurrencyApi*)H->consume(TSM_SERVICE_CURRENCY, TSM_CURRENCY_VERSION_V3);
    g_enterprise = (TsmEnterpriseBaseApi*)H->consume(TSM_SERVICE_ENTERPRISE_BASE, TSM_ENTERPRISE_BASE_VERSION);
    if (!g_enterprise) g_enterprise = (TsmEnterpriseBaseApi*)H->consume(TSM_SERVICE_ENTERPRISE_BASE, TSM_ENTERPRISE_BASE_VERSION_V7);
    g_persistence = (TsmPersistenceApi*)H->consume(TSM_SERVICE_PERSISTENCE, TSM_PERSISTENCE_VERSION);
    g_ui = (TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS, TSM_UI_PANELS_VERSION);
    if (!g_ui) g_ui = (TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS, TSM_UI_PANELS_VERSION_V5);

    H->provide(TSM_SERVICE_REPUBLIC_BANK, TSM_REPUBLIC_BANK_VERSION, &kApi);
    return 0;
}

extern "C" __declspec(dllexport) int TsmPluginStart(void) {
    if (!g_enabled) return 1;
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
            const char* name = (g_enterprise) ? g_enterprise->getBuildingObjectName(building) : nullptr;
            int isCity = 0;
            if (name) {
                for (const char* p = name; *p; p++) {
                    if (_strnicmp(p, "city", 4) == 0 || _strnicmp(p, "hall", 4) == 0) { isCity = 1; break; }
                }
            }
            if (!isCity) return;
            unsigned v = (unsigned)(g_book.getAnnualInterestRate() * 100.0 + 0.5);
            if (v > 100) v = 100; // percent 0..25 represented as 0..25? keep 0..25
            if (v > 25) v = 25;
            if (g_ui->widgetSlider(L"Bank rate %", &v, 1, 1)) {
                g_book.setAnnualInterestRate(v / 100.0);
            }
        }, nullptr);
    }
    Logf("republic_bank  live");
    return 0;
}

BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID) { return TRUE; }
