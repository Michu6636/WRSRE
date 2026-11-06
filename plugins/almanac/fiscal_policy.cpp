#include "fiscal_policy.h"

#include <cstdio>
#include <cstring>

namespace {
void copy_text(char* dst, std::size_t cap, const char* src) {
    if (!dst || cap == 0) return;
    if (!src) { dst[0] = '\0'; return; }
    std::size_t i = 0;
    for (; i + 1 < cap && src[i] != '\0'; ++i) dst[i] = src[i];
    dst[i] = '\0';
}

int taxes_available(const almanac::fiscal_policy::Context* ctx) {
    if (!ctx || !ctx->taxes) return 0;
    if (!TsmTaxesHasConfiguredPolicy(ctx->taxes)) return 0;
    return 1;
}

int bank_available(const almanac::fiscal_policy::Context* ctx) {
    if (!ctx || !ctx->bank) return 0;
    if (!ctx->bank->getAnnualInterestRate || !ctx->bank->setAnnualInterestRate) return 0;
    return 1;
}

} // namespace

namespace almanac {
namespace fiscal_policy {

int apply_action(Context* ctx, unsigned actionId) {
    if (!ctx || !ctx->alive) return 0;

    // Taxes actions.
    if (taxes_available(ctx)) {
        const TsmTaxesApi* t = ctx->taxes;
        switch (actionId) {
            case ACTION_VAT_TOGGLE: {
                int en = t->isTaxEnabled ? t->isTaxEnabled(TSM_TAX_VAT) : 0;
                if (t->setTaxEnabled) t->setTaxEnabled(TSM_TAX_VAT, en ? 0 : 1);
                return 1;
            }
            case ACTION_VAT_RATE_UP: {
                int stepPct = t->getTaxSliderStepPct ? t->getTaxSliderStepPct() : 0;
                if (stepPct <= 0) return 0;
                float r = 0.0f;
                if (t->getTaxConfiguredRate && t->getTaxConfiguredRate(TSM_TAX_VAT, &r)) {
                    float nr = r + (float)stepPct / 100.0f;
                    if (nr > 1.0f) nr = 1.0f;
                    if (t->setTaxRate) t->setTaxRate(TSM_TAX_VAT, nr);
                    return 1;
                }
                return 0;
            }
            case ACTION_VAT_RATE_DOWN: {
                int stepPct = t->getTaxSliderStepPct ? t->getTaxSliderStepPct() : 0;
                if (stepPct <= 0) return 0;
                float r = 0.0f;
                if (t->getTaxConfiguredRate && t->getTaxConfiguredRate(TSM_TAX_VAT, &r)) {
                    float nr = r - (float)stepPct / 100.0f;
                    if (nr < 0.0f) nr = 0.0f;
                    if (t->setTaxRate) t->setTaxRate(TSM_TAX_VAT, nr);
                    return 1;
                }
                return 0;
            }
            case ACTION_PROFIT_TOGGLE: {
                int en = t->isTaxEnabled ? t->isTaxEnabled(TSM_TAX_PROFIT) : 0;
                if (t->setTaxEnabled) t->setTaxEnabled(TSM_TAX_PROFIT, en ? 0 : 1);
                return 1;
            }
            case ACTION_PROFIT_RATE_UP: {
                int stepPct = t->getTaxSliderStepPct ? t->getTaxSliderStepPct() : 0;
                if (stepPct <= 0) return 0;
                float r = 0.0f;
                if (t->getTaxConfiguredRate && t->getTaxConfiguredRate(TSM_TAX_PROFIT, &r)) {
                    float nr = r + (float)stepPct / 100.0f;
                    if (nr > 1.0f) nr = 1.0f;
                    if (t->setTaxRate) t->setTaxRate(TSM_TAX_PROFIT, nr);
                    return 1;
                }
                return 0;
            }
            case ACTION_PROFIT_RATE_DOWN: {
                int stepPct = t->getTaxSliderStepPct ? t->getTaxSliderStepPct() : 0;
                if (stepPct <= 0) return 0;
                float r = 0.0f;
                if (t->getTaxConfiguredRate && t->getTaxConfiguredRate(TSM_TAX_PROFIT, &r)) {
                    float nr = r - (float)stepPct / 100.0f;
                    if (nr < 0.0f) nr = 0.0f;
                    if (t->setTaxRate) t->setTaxRate(TSM_TAX_PROFIT, nr);
                    return 1;
                }
                return 0;
            }
            case ACTION_PROPERTY_TOGGLE: {
                int en = t->isTaxEnabled ? t->isTaxEnabled(TSM_TAX_PROPERTY) : 0;
                if (t->setTaxEnabled) t->setTaxEnabled(TSM_TAX_PROPERTY, en ? 0 : 1);
                return 1;
            }
            case ACTION_PROPERTY_AMOUNT_UP: {
                int stepPct = t->getTaxSliderStepPct ? t->getTaxSliderStepPct() : 0;
                float maxAmt = t->getTaxPropertySliderMax ? t->getTaxPropertySliderMax() : 0.0f;
                if (stepPct <= 0 || !(maxAmt > 0.0f)) return 0;
                float step = maxAmt * (float)stepPct / 100.0f;
                float r = 0.0f;
                if (t->getTaxConfiguredRate && t->getTaxConfiguredRate(TSM_TAX_PROPERTY, &r)) {
                    float nr = r + step;
                    if (nr > maxAmt) nr = maxAmt;
                    if (t->setTaxRate) t->setTaxRate(TSM_TAX_PROPERTY, nr);
                    return 1;
                }
                return 0;
            }
            case ACTION_PROPERTY_AMOUNT_DOWN: {
                int stepPct = t->getTaxSliderStepPct ? t->getTaxSliderStepPct() : 0;
                float maxAmt = t->getTaxPropertySliderMax ? t->getTaxPropertySliderMax() : 0.0f;
                if (stepPct <= 0 || !(maxAmt > 0.0f)) return 0;
                float step = maxAmt * (float)stepPct / 100.0f;
                float r = 0.0f;
                if (t->getTaxConfiguredRate && t->getTaxConfiguredRate(TSM_TAX_PROPERTY, &r)) {
                    float nr = r - step;
                    if (nr < 0.0f) nr = 0.0f;
                    if (t->setTaxRate) t->setTaxRate(TSM_TAX_PROPERTY, nr);
                    return 1;
                }
                return 0;
            }
            default: break;
        }
    }

    // Bank actions.
    if (bank_available(ctx)) {
        const TsmRepublicBankApi* b = ctx->bank;
        switch (actionId) {
            case ACTION_BANK_RATE_UP: {
                double r = b->getAnnualInterestRate ? b->getAnnualInterestRate() : 0.0;
                if (b->setAnnualInterestRate) b->setAnnualInterestRate(r + 0.01);
                return 1;
            }
            case ACTION_BANK_RATE_DOWN: {
                double r = b->getAnnualInterestRate ? b->getAnnualInterestRate() : 0.0;
                if (b->setAnnualInterestRate) b->setAnnualInterestRate(r - 0.01);
                return 1;
            }
            default: break;
        }
    }

    return 0;
}

void action(AlmanacPageId page, unsigned actionId, void* userData) {
    (void)page;
    Context* ctx = static_cast<Context*>(userData);
    if (!ctx || !ctx->alive) return;
    apply_action(ctx, actionId);
}

int populate(const AlmanacHostSurface* host, AlmanacPageSnapshot* out, void* userData) {
    (void)host;
    Context* ctx = static_cast<Context*>(userData);
    if (!out) return 0;
    out->populated = 1;
    out->coherent = 1;
    out->rowCount = 0;
    out->available = 0;
    out->stale = 0;

    const int tAvail = taxes_available(ctx);
    const int bAvail = bank_available(ctx);

    if (tAvail || bAvail) out->available = 1;

    char buf[ALMANAC_TEXT_LEN];
    auto add_row = [&](const char* label, const char* value, int available, int interactive,
                       int enabled, unsigned actionId, int currency, const char* unit) {
        if (out->rowCount >= ALMANAC_MAX_ROWS) return;
        AlmanacDisplayRow& row = out->rows[out->rowCount++];
        row = AlmanacDisplayRow{};
        copy_text(row.label, ALMANAC_TEXT_LEN, label);
        copy_text(row.value, ALMANAC_TEXT_LEN, value);
        copy_text(row.unit, ALMANAC_UNIT_LEN, unit ? unit : "");
        row.available = available;
        row.interactive = interactive;
        row.enabled = enabled;
        row.actionId = actionId;
        row.currency = currency;
        row.valueClass = ALMANAC_VALUE_ACTUAL;
    };

    const char* sym = (ctx && ctx->currency == 1) ? "USD" : "RUB";

    // Taxes display rows.
    if (ctx && ctx->taxes) {
        const TsmTaxesApi* t = ctx->taxes;
        // VAT
        {
            float r = 0.0f;
            int hasRate = tAvail && t->getTaxConfiguredRate && t->getTaxConfiguredRate(TSM_TAX_VAT, &r);
            int en = 0;
            if (t->isTaxEnabled) en = t->isTaxEnabled(TSM_TAX_VAT);
            if (hasRate) {
                std::snprintf(buf, sizeof(buf), "%.0f%%", (double)r * 100.0);
                add_row("VAT rate", buf, 1, 0, 0, 0, -1, "");
            } else {
                add_row("VAT rate", "unavailable", tAvail ? 1 : 0, 0, 0, 0, -1, "");
            }
            std::snprintf(buf, sizeof(buf), "%s", en ? "ON" : "OFF");
            add_row("VAT enabled", buf, tAvail ? 1 : 0, 0, 0, 0, -1, "");
        }
        // Profit
        {
            float r = 0.0f;
            int hasRate = tAvail && t->getTaxConfiguredRate && t->getTaxConfiguredRate(TSM_TAX_PROFIT, &r);
            int en = 0;
            if (t->isTaxEnabled) en = t->isTaxEnabled(TSM_TAX_PROFIT);
            if (hasRate) {
                std::snprintf(buf, sizeof(buf), "%.0f%%", (double)r * 100.0);
                add_row("Profit tax rate", buf, 1, 0, 0, 0, -1, "");
            } else {
                add_row("Profit tax rate", "unavailable", tAvail ? 1 : 0, 0, 0, 0, -1, "");
            }
            std::snprintf(buf, sizeof(buf), "%s", en ? "ON" : "OFF");
            add_row("Profit tax enabled", buf, tAvail ? 1 : 0, 0, 0, 0, -1, "");
        }
        // Property
        {
            float r = 0.0f;
            int hasRate = tAvail && t->getTaxConfiguredRate && t->getTaxConfiguredRate(TSM_TAX_PROPERTY, &r);
            int en = 0;
            if (t->isTaxEnabled) en = t->isTaxEnabled(TSM_TAX_PROPERTY);
            if (hasRate) {
                std::snprintf(buf, sizeof(buf), "%.2f", (double)r);
                add_row("Property tax amount", buf, 1, 0, 0, 0, ctx ? ctx->currency : -1, sym);
            } else {
                add_row("Property tax amount", "unavailable", tAvail ? 1 : 0, 0, 0, 0, -1, "");
            }
            std::snprintf(buf, sizeof(buf), "%s", en ? "ON" : "OFF");
            add_row("Property tax enabled", buf, tAvail ? 1 : 0, 0, 0, 0, -1, "");
        }
    } else {
        // No taxes service: unavailable rows.
        add_row("VAT rate", "unavailable", 0, 0, 0, 0, -1, "");
        add_row("VAT enabled", "unavailable", 0, 0, 0, 0, -1, "");
        add_row("Profit tax rate", "unavailable", 0, 0, 0, 0, -1, "");
        add_row("Profit tax enabled", "unavailable", 0, 0, 0, 0, -1, "");
        add_row("Property tax amount", "unavailable", 0, 0, 0, 0, -1, "");
        add_row("Property tax enabled", "unavailable", 0, 0, 0, 0, -1, "");
    }

    // Taxes command rows.
    if (ctx && ctx->taxes) {
        const int interact = tAvail ? 1 : 0;
        const int enabled = tAvail ? 1 : 0;
        add_row("VAT toggle", "", tAvail ? 1 : 0, interact, enabled, ACTION_VAT_TOGGLE, -1, "");
        // rate commands only if step available
        int stepPct = 0;
        if (tAvail && ctx->taxes->getTaxSliderStepPct) stepPct = ctx->taxes->getTaxSliderStepPct();
        const int rateInteract = (tAvail && stepPct > 0) ? 1 : 0;
        add_row("VAT rate increase", "", tAvail ? 1 : 0, rateInteract, rateInteract, ACTION_VAT_RATE_UP, -1, "");
        add_row("VAT rate decrease", "", tAvail ? 1 : 0, rateInteract, rateInteract, ACTION_VAT_RATE_DOWN, -1, "");

        add_row("Profit tax toggle", "", tAvail ? 1 : 0, interact, enabled, ACTION_PROFIT_TOGGLE, -1, "");
        add_row("Profit tax rate increase", "", tAvail ? 1 : 0, rateInteract, rateInteract, ACTION_PROFIT_RATE_UP, -1, "");
        add_row("Profit tax rate decrease", "", tAvail ? 1 : 0, rateInteract, rateInteract, ACTION_PROFIT_RATE_DOWN, -1, "");

        add_row("Property tax toggle", "", tAvail ? 1 : 0, interact, enabled, ACTION_PROPERTY_TOGGLE, -1, "");
        // property step requires slider max too
        float maxAmt = 0.0f;
        if (tAvail && ctx->taxes->getTaxPropertySliderMax) maxAmt = ctx->taxes->getTaxPropertySliderMax();
        const int propInteract = (tAvail && stepPct > 0 && maxAmt > 0.0f) ? 1 : 0;
        add_row("Property tax amount increase", "", tAvail ? 1 : 0, propInteract, propInteract, ACTION_PROPERTY_AMOUNT_UP, -1, "");
        add_row("Property tax amount decrease", "", tAvail ? 1 : 0, propInteract, propInteract, ACTION_PROPERTY_AMOUNT_DOWN, -1, "");
    }

    // Bank display row.
    if (ctx && ctx->bank) {
        if (bAvail) {
            double r = ctx->bank->getAnnualInterestRate();
            std::snprintf(buf, sizeof(buf), "%.0f%%", r * 100.0);
            add_row("Bank interest rate", buf, 1, 0, 0, 0, -1, "");
        } else {
            add_row("Bank interest rate", "unavailable", 0, 0, 0, 0, -1, "");
        }
    } else {
        add_row("Bank interest rate", "unavailable", 0, 0, 0, 0, -1, "");
    }

    // Bank command rows.
    if (ctx && ctx->bank) {
        const int interact = bAvail ? 1 : 0;
        add_row("Bank rate increase", "", bAvail ? 1 : 0, interact, interact, ACTION_BANK_RATE_UP, -1, "");
        add_row("Bank rate decrease", "", bAvail ? 1 : 0, interact, interact, ACTION_BANK_RATE_DOWN, -1, "");
    }

    return 1;
}

Adapter::Adapter(AlmanacShell* shell, const TsmTaxesApi* taxes, const TsmRepublicBankApi* bank, int currency)
    : shell_(shell) {
    ctx_.taxes = taxes;
    ctx_.bank = bank;
    ctx_.currency = currency;
    ctx_.alive = true;
    if (shell_) {
        handle_ = almanac_shell_register_provider(shell_, ALMANAC_PAGE_FISCAL_POLICY,
                                                  &Adapter::populateThunk, &ctx_);
        if (handle_ > 0) {
            almanac_shell_set_provider_action(shell_, handle_, &Adapter::actionThunk);
        }
    }
}

Adapter::~Adapter() {
    unregister();
}

void Adapter::unregister() {
    ctx_.alive = false;
    if (shell_ && handle_ > 0) {
        almanac_shell_unregister_provider(shell_, handle_);
    }
    handle_ = 0;
    shell_ = nullptr;
}

int register_provider(AlmanacShell* shell, const TsmTaxesApi* taxes, const TsmRepublicBankApi* bank, int currency) {
    Adapter* a = new Adapter(shell, taxes, bank, currency);
    (void)a; // lifetime managed by integration in production
    return a->handle();
}

} // namespace fiscal_policy
} // namespace almanac
