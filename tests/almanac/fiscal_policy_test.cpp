#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "almanac_ui_api.h"
#include "almanac_shell.h"
#include "almanac_pages.h"
#include "host_contract.h"
#include "host_validation.h"
#include "fiscal_policy.h"
#include "taxes_api.h"
#include "republic_bank_api.h"
#include "bank_ledger.h"

using namespace almanac;
using namespace almanac::fiscal_policy;

#define CHECK(x) do { if(!(x)){ std::fprintf(stderr,"FAIL %d: %s\n",__LINE__,#x); std::exit(1);} } while(0)

// ---- fake taxes state -------------------------------------------------------
static int g_vatEnabled = 1;
static int g_profitEnabled = 0;
static int g_propertyEnabled = 1;
static float g_vatRate = 0.20f;
static float g_profitRate = 0.15f;
static float g_propertyAmount = 250.0f;
static int g_stepPct = 2;
static float g_propMax = 500.0f;

static int fake_isTaxEnabled(int k){ if(k==TSM_TAX_VAT) return g_vatEnabled; if(k==TSM_TAX_PROFIT) return g_profitEnabled; if(k==TSM_TAX_PROPERTY) return g_propertyEnabled; return 0; }
static int fake_setTaxRate(int k, float r){
    if(k==TSM_TAX_VAT){ if(r<0||r>1) return 0; g_vatRate=r; return 1; }
    if(k==TSM_TAX_PROFIT){ if(r<0||r>1) return 0; g_profitRate=r; return 1; }
    if(k==TSM_TAX_PROPERTY){ if(r<0) return 0; g_propertyAmount=r; return 1; }
    return 0;
}
static int fake_setTaxEnabled(int k, int en){
    if(k==TSM_TAX_VAT){ g_vatEnabled=en?1:0; return 1; }
    if(k==TSM_TAX_PROFIT){ g_profitEnabled=en?1:0; return 1; }
    if(k==TSM_TAX_PROPERTY){ g_propertyEnabled=en?1:0; return 1; }
    return 0;
}
static int fake_getConfiguredRate(int k, float* out){
    if(!out) return 0;
    if(k==TSM_TAX_VAT){ *out=g_vatRate; return 1; }
    if(k==TSM_TAX_PROFIT){ *out=g_profitRate; return 1; }
    if(k==TSM_TAX_PROPERTY){ *out=g_propertyAmount; return 1; }
    return 0;
}
static int fake_getStep(){ return g_stepPct; }
static float fake_getPropMax(){ return g_propMax; }
static int fake_getPolicy(TsmTaxPolicy* out){
    if(!out || out->structSize < sizeof(TsmTaxPolicy)) return 0;
    out->vatEnabled=g_vatEnabled; out->profitEnabled=g_profitEnabled; out->propertyEnabled=g_propertyEnabled;
    out->vatRate=g_vatRate; out->profitRate=g_profitRate; out->propertyAmount=g_propertyAmount;
    out->sliderStepPct=g_stepPct; out->propertySliderMax=g_propMax;
    return 1;
}

static TsmTaxesApi make_taxes_api(bool configuredTail) {
    TsmTaxesApi api{};
    api.structSize = configuredTail
        ? sizeof(TsmTaxesApi)
        : offsetof(TsmTaxesApi, getTaxConfiguredRate);
    api.getVatRate = [](){ return g_vatEnabled ? g_vatRate : 0.0f; };
    api.getProfitTaxRate = [](){ return g_profitEnabled ? g_profitRate : 0.0f; };
    api.isTaxEnabled = fake_isTaxEnabled;
    api.setTaxRate = fake_setTaxRate;
    api.setTaxEnabled = fake_setTaxEnabled;
    if (configuredTail) {
        api.getTaxConfiguredRate = fake_getConfiguredRate;
        api.getTaxSliderStepPct = fake_getStep;
        api.getTaxPropertySliderMax = fake_getPropMax;
        api.getTaxPolicy = fake_getPolicy;
    } else {
        api.getTaxConfiguredRate = nullptr;
        api.getTaxSliderStepPct = nullptr;
        api.getTaxPropertySliderMax = nullptr;
        api.getTaxPolicy = nullptr;
    }
    return api;
}

// ---- fake bank state --------------------------------------------------------
static double g_bankRate = 0.06;
static double fake_bank_getRate(){ return g_bankRate; }
static void fake_bank_setRate(double r){ g_bankRate = republic_bank::NormalizeBankRate(r); }

static TsmRepublicBankApi make_bank_api() {
    TsmRepublicBankApi api{};
    api.structSize = sizeof(TsmRepublicBankApi);
    api.getAnnualInterestRate = fake_bank_getRate;
    api.setAnnualInterestRate = fake_bank_setRate;
    return api;
}

static AlmanacHostSurface make_host(std::uint64_t frameNumber = 1u,
                                    std::uint64_t generation = 1u,
                                    float dpi = 2.0f) {
    AlmanacHostSurface s{};
    s.capabilities = AlmanacHostCapability_Visibility |
                     AlmanacHostCapability_SelectionValidation |
                     AlmanacHostCapability_Geometry |
                     AlmanacHostCapability_Dpi |
                     AlmanacHostCapability_Clipping |
                     AlmanacHostCapability_Scrolling |
                     AlmanacHostCapability_PointerInput |
                     AlmanacHostCapability_FrameIdentity |
                     AlmanacHostCapability_LifecycleInvalidation |
                     AlmanacHostCapability_FlowLayout;
    s.visible = true; s.selected = true;
    s.geometry.dpi = dpi;
    s.geometry.origin_x = 10.0f; s.geometry.origin_y = 20.0f;
    s.geometry.extent_width = 100.0f; s.geometry.extent_height = 200.0f;
    s.clip.enabled = true; s.clip.x = 10.0f; s.clip.y = 20.0f; s.clip.width = 100.0f; s.clip.height = 200.0f;
    s.scroll.can_scroll_y = true; s.scroll.max_offset_y = 50.0f; s.scroll.offset_y = 5.0f;
    s.pointer.action = AlmanacPointerAction_Down;
    s.pointer.x = 15.0f; s.pointer.y = 30.0f; s.pointer.in_bounds = true;
    s.pointer.frame_id = frameNumber;
    s.frame.frame_number = frameNumber; s.frame.generation = generation;
    s.lifecycle.generation = generation; s.lifecycle.invalidated = false;
    return s;
}

static int find_row(const AlmanacPageSnapshot* snap, const char* label) {
    if (!snap) return -1;
    for (std::size_t i = 0; i < snap->rowCount; ++i) {
        if (std::strcmp(snap->rows[i].label, label) == 0) return (int)i;
    }
    return -1;
}

int main() {
    // initial authoritative snapshot with configured tail
    {
        TsmTaxesApi taxes = make_taxes_api(true);
        TsmRepublicBankApi bank = make_bank_api();
        AlmanacShell* sh = almanac_shell_create();
        Adapter adapter(sh, &taxes, &bank, 0);
        CHECK(adapter.registered());
        AlmanacHostSurface host = make_host();
        CHECK(almanac_shell_navigate(sh, ALMANAC_PAGE_FISCAL_POLICY));
        CHECK(almanac_shell_populate(sh, &host));
        const AlmanacPageSnapshot* snap = almanac_shell_page_snapshot(sh, ALMANAC_PAGE_FISCAL_POLICY);
        CHECK(snap && snap->available);
        int r = find_row(snap, "VAT rate");
        CHECK(r >= 0);
        CHECK(std::strcmp(snap->rows[r].value, "20%") == 0);
        // suspended profit tax retains configured rate (profitEnabled=0 but rate 15%)
        r = find_row(snap, "Profit tax rate");
        CHECK(r >= 0);
        CHECK(std::strcmp(snap->rows[r].value, "15%") == 0);
        r = find_row(snap, "Property tax amount");
        CHECK(r >= 0);
        CHECK(std::strcmp(snap->rows[r].value, "250.00") == 0);
        // bank rate
        r = find_row(snap, "Bank interest rate");
        CHECK(r >= 0);
        CHECK(std::strcmp(snap->rows[r].value, "6%") == 0);
        adapter.unregister();
        almanac_shell_destroy(sh);
    }

    // actionIds, interactive/enabled flags
    {
        TsmTaxesApi taxes = make_taxes_api(true);
        TsmRepublicBankApi bank = make_bank_api();
        AlmanacShell* sh = almanac_shell_create();
        Adapter adapter(sh, &taxes, &bank, 0);
        AlmanacHostSurface host = make_host();
        almanac_shell_navigate(sh, ALMANAC_PAGE_FISCAL_POLICY);
        almanac_shell_populate(sh, &host);
        const AlmanacPageSnapshot* snap = almanac_shell_page_snapshot(sh, ALMANAC_PAGE_FISCAL_POLICY);
        int r = find_row(snap, "VAT toggle");
        CHECK(r >= 0);
        CHECK(snap->rows[r].interactive == 1);
        CHECK(snap->rows[r].enabled == 1);
        CHECK(snap->rows[r].actionId == ACTION_VAT_TOGGLE);
        r = find_row(snap, "VAT rate");
        CHECK(r >= 0);
        CHECK(snap->rows[r].interactive == 0);
        adapter.unregister();
        almanac_shell_destroy(sh);
    }

    // pointer-down dispatch
    {
        TsmTaxesApi taxes = make_taxes_api(true);
        TsmRepublicBankApi bank = make_bank_api();
        AlmanacShell* sh = almanac_shell_create();
        Adapter adapter(sh, &taxes, &bank, 0);
        AlmanacHostSurface host = make_host();
        almanac_shell_navigate(sh, ALMANAC_PAGE_FISCAL_POLICY);
        almanac_shell_populate(sh, &host);
        // find row index for VAT toggle
        const AlmanacPageSnapshot* snap = almanac_shell_page_snapshot(sh, ALMANAC_PAGE_FISCAL_POLICY);
        int target = -1;
        for (std::size_t i = 0; i < snap->rowCount; ++i) {
            if (snap->rows[i].actionId == ACTION_VAT_TOGGLE) { target = (int)i; break; }
        }
        CHECK(target >= 0);
        // point at that row
        host.pointer.y = 20.0f + (target * 35.0f * 2.0f) + 5.0f; // dpi 2 -> rowHeight 70? wait make_host dpi 2 => rowHeight 70
        // Actually rowHeight = dpi*35 = 70. contentY = visibleY + scroll(5). Let's just set y so row target.
        host.geometry.extent_height = 1000.0f;
        host.clip.height = 1000.0f;
        host.scroll.max_offset_y = 1000.0f;
        host.scroll.offset_y = (float)target * 70.0f;
        host.pointer.y = 20.0f + 5.0f;
        host.pointer.x = 15.0f;
        host.pointer.action = AlmanacPointerAction_Down;
        int before = g_vatEnabled;
        CHECK(almanac_shell_handle_pointer(sh, &host));
        CHECK(g_vatEnabled != before);
        adapter.unregister();
        almanac_shell_destroy(sh);
    }

    // movement no mutation
    {
        TsmTaxesApi taxes = make_taxes_api(true);
        TsmRepublicBankApi bank = make_bank_api();
        AlmanacShell* sh = almanac_shell_create();
        Adapter adapter(sh, &taxes, &bank, 0);
        AlmanacHostSurface host = make_host();
        almanac_shell_navigate(sh, ALMANAC_PAGE_FISCAL_POLICY);
        almanac_shell_populate(sh, &host);
        int beforeVat = g_vatEnabled;
        host.pointer.action = AlmanacPointerAction_Move;
        almanac_shell_handle_pointer(sh, &host);
        CHECK(g_vatEnabled == beforeVat);
        adapter.unregister();
        almanac_shell_destroy(sh);
    }

    // stale generation no mutation
    {
        TsmTaxesApi taxes = make_taxes_api(true);
        TsmRepublicBankApi bank = make_bank_api();
        AlmanacShell* sh = almanac_shell_create();
        Adapter adapter(sh, &taxes, &bank, 0);
        AlmanacHostSurface host1 = make_host(1, 1, 2.0f);
        almanac_shell_navigate(sh, ALMANAC_PAGE_FISCAL_POLICY);
        almanac_shell_populate(sh, &host1);
        AlmanacHostSurface host2 = make_host(2, 2, 2.0f);
        int before = g_vatEnabled;
        CHECK(!almanac_shell_handle_pointer(sh, &host2));
        CHECK(g_vatEnabled == before);
        adapter.unregister();
        almanac_shell_destroy(sh);
    }

    // disabled row no mutation (unavailable bank)
    {
        TsmTaxesApi taxes = make_taxes_api(true);
        TsmRepublicBankApi bank{}; // null funcs -> unavailable
        AlmanacShell* sh = almanac_shell_create();
        Adapter adapter(sh, &taxes, &bank, 0);
        AlmanacHostSurface host = make_host();
        almanac_shell_navigate(sh, ALMANAC_PAGE_FISCAL_POLICY);
        almanac_shell_populate(sh, &host);
        const AlmanacPageSnapshot* snap = almanac_shell_page_snapshot(sh, ALMANAC_PAGE_FISCAL_POLICY);
        int target = -1;
        for (std::size_t i = 0; i < snap->rowCount; ++i) {
            if (snap->rows[i].actionId == ACTION_BANK_RATE_UP) { target = (int)i; break; }
        }
        CHECK(target >= 0);
        CHECK(snap->rows[target].interactive == 0);
        CHECK(snap->rows[target].enabled == 0);
        adapter.unregister();
        almanac_shell_destroy(sh);
    }

    // one action call per command (use a counting wrapper by checking state changes once)
    {
        // toggle twice -> two changes
        TsmTaxesApi taxes = make_taxes_api(true);
        TsmRepublicBankApi bank = make_bank_api();
        AlmanacShell* sh = almanac_shell_create();
        Adapter adapter(sh, &taxes, &bank, 0);
        g_vatEnabled = 1;
        adapter.context().taxes = &taxes;
        apply_action(&adapter.context(), ACTION_VAT_TOGGLE);
        CHECK(g_vatEnabled == 0);
        apply_action(&adapter.context(), ACTION_VAT_TOGGLE);
        CHECK(g_vatEnabled == 1);
        adapter.unregister();
        almanac_shell_destroy(sh);
    }

    // property amount vs percentage semantics: property command uses step amount not percent
    {
        TsmTaxesApi taxes = make_taxes_api(true);
        TsmRepublicBankApi bank = make_bank_api();
        g_propertyAmount = 250.0f;
        g_stepPct = 2;
        g_propMax = 500.0f;
        Context ctx{&taxes, &bank, 0, true};
        apply_action(&ctx, ACTION_PROPERTY_AMOUNT_UP);
        // step = 500*2/100 = 10
        CHECK(fabsf(g_propertyAmount - 260.0f) < 1e-3f);
        // VAT uses percent step: 2% of 1? fraction 0.02
        g_vatRate = 0.20f;
        apply_action(&ctx, ACTION_VAT_RATE_UP);
        CHECK(fabsf(g_vatRate - 0.22f) < 1e-5f);
    }

    // reread-after-set: display after populate reflects new configured rate
    {
        TsmTaxesApi taxes = make_taxes_api(true);
        TsmRepublicBankApi bank = make_bank_api();
        AlmanacShell* sh = almanac_shell_create();
        Adapter adapter(sh, &taxes, &bank, 0);
        g_vatRate = 0.20f;
        apply_action(&adapter.context(), ACTION_VAT_RATE_UP);
        CHECK(fabsf(g_vatRate - 0.22f) < 1e-5f);
        AlmanacHostSurface host = make_host();
        almanac_shell_navigate(sh, ALMANAC_PAGE_FISCAL_POLICY);
        almanac_shell_populate(sh, &host);
        const AlmanacPageSnapshot* snap = almanac_shell_page_snapshot(sh, ALMANAC_PAGE_FISCAL_POLICY);
        int r = find_row(snap, "VAT rate");
        CHECK(r >= 0);
        CHECK(std::strcmp(snap->rows[r].value, "22%") == 0);
        adapter.unregister();
        almanac_shell_destroy(sh);
    }

    // missing-tail degradation: v2 taxes (no configured tail) -> unavailable tax rows
    {
        TsmTaxesApi taxes = make_taxes_api(false);
        TsmRepublicBankApi bank = make_bank_api();
        AlmanacShell* sh = almanac_shell_create();
        Adapter adapter(sh, &taxes, &bank, 0);
        AlmanacHostSurface host = make_host();
        almanac_shell_navigate(sh, ALMANAC_PAGE_FISCAL_POLICY);
        almanac_shell_populate(sh, &host);
        const AlmanacPageSnapshot* snap = almanac_shell_page_snapshot(sh, ALMANAC_PAGE_FISCAL_POLICY);
        int r = find_row(snap, "VAT toggle");
        CHECK(r >= 0);
        CHECK(snap->rows[r].interactive == 0);
        CHECK(snap->rows[r].enabled == 0);
        adapter.unregister();
        almanac_shell_destroy(sh);
    }

    // unavailable taxes service
    {
        TsmRepublicBankApi bank = make_bank_api();
        AlmanacShell* sh = almanac_shell_create();
        Adapter adapter(sh, nullptr, &bank, 0);
        AlmanacHostSurface host = make_host();
        almanac_shell_navigate(sh, ALMANAC_PAGE_FISCAL_POLICY);
        almanac_shell_populate(sh, &host);
        const AlmanacPageSnapshot* snap = almanac_shell_page_snapshot(sh, ALMANAC_PAGE_FISCAL_POLICY);
        int r = find_row(snap, "VAT rate");
        CHECK(r >= 0);
        CHECK(snap->rows[r].available == 0);
        adapter.unregister();
        almanac_shell_destroy(sh);
    }

    // unavailable bank service
    {
        TsmTaxesApi taxes = make_taxes_api(true);
        AlmanacShell* sh = almanac_shell_create();
        Adapter adapter(sh, &taxes, nullptr, 0);
        AlmanacHostSurface host = make_host();
        almanac_shell_navigate(sh, ALMANAC_PAGE_FISCAL_POLICY);
        almanac_shell_populate(sh, &host);
        const AlmanacPageSnapshot* snap = almanac_shell_page_snapshot(sh, ALMANAC_PAGE_FISCAL_POLICY);
        int r = find_row(snap, "Bank interest rate");
        CHECK(r >= 0);
        CHECK(snap->rows[r].available == 0);
        adapter.unregister();
        almanac_shell_destroy(sh);
    }

    // unregistration prevents actions
    {
        TsmTaxesApi taxes = make_taxes_api(true);
        TsmRepublicBankApi bank = make_bank_api();
        AlmanacShell* sh = almanac_shell_create();
        Adapter* adapter = new Adapter(sh, &taxes, &bank, 0);
        adapter->unregister();
        CHECK(!adapter->registered());
        int before = g_vatEnabled;
        apply_action(&adapter->context(), ACTION_VAT_TOGGLE);
        // alive false -> no mutation
        CHECK(g_vatEnabled == before);
        delete adapter;
        almanac_shell_destroy(sh);
    }

    // bank snapping: set via api normalizes
    {
        g_bankRate = 0.0375; // legacy fraction
        // simulate load normalization
        g_bankRate = republic_bank::NormalizeBankRate(g_bankRate);
        CHECK(fabs(g_bankRate - 0.04) < 1e-9); // 3.75% -> 4%
    }

    // bank rate command rereads normalized value
    {
        TsmTaxesApi taxes = make_taxes_api(true);
        TsmRepublicBankApi bank = make_bank_api();
        g_bankRate = 0.06;
        AlmanacShell* sh = almanac_shell_create();
        Adapter adapter(sh, &taxes, &bank, 0);
        apply_action(&adapter.context(), ACTION_BANK_RATE_UP);
        // 0.06 + 0.01 = 0.07 normalized to grid: 0.07*400=28 -> 28/400=0.07
        CHECK(fabs(g_bankRate - 0.07) < 1e-9);
        AlmanacHostSurface host = make_host();
        almanac_shell_navigate(sh, ALMANAC_PAGE_FISCAL_POLICY);
        almanac_shell_populate(sh, &host);
        const AlmanacPageSnapshot* snap = almanac_shell_page_snapshot(sh, ALMANAC_PAGE_FISCAL_POLICY);
        int r = find_row(snap, "Bank interest rate");
        CHECK(r >= 0);
        CHECK(std::strcmp(snap->rows[r].value, "7%") == 0);
        adapter.unregister();
        almanac_shell_destroy(sh);
    }

    std::printf("ALL TESTS PASSED\n");
    return 0;
}
