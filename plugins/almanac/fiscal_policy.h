#pragma once

#include "almanac_ui_api.h"
#include "almanac_shell.h"
#include "taxes_api.h"
#include "republic_bank_api.h"

#include <cstddef>
#include <cstdint>

namespace almanac {
namespace fiscal_policy {

// Stable action IDs for the Fiscal Policy page. The shell dispatches one of
// these per pointer-down on an enabled interactive row.
constexpr unsigned ACTION_VAT_TOGGLE = 1u;
constexpr unsigned ACTION_VAT_RATE_UP = 2u;
constexpr unsigned ACTION_VAT_RATE_DOWN = 3u;

constexpr unsigned ACTION_PROFIT_TOGGLE = 4u;
constexpr unsigned ACTION_PROFIT_RATE_UP = 5u;
constexpr unsigned ACTION_PROFIT_RATE_DOWN = 6u;

constexpr unsigned ACTION_PROPERTY_TOGGLE = 7u;
constexpr unsigned ACTION_PROPERTY_AMOUNT_UP = 8u;
constexpr unsigned ACTION_PROPERTY_AMOUNT_DOWN = 9u;

constexpr unsigned ACTION_BANK_TOGGLE = 10u; // reserved; bank has no on/off
constexpr unsigned ACTION_BANK_RATE_UP = 11u;
constexpr unsigned ACTION_BANK_RATE_DOWN = 12u;

// Adapter context. This is the userData stored with the shell provider. It
// never owns the services; callers keep them alive for the adapter lifetime.
struct Context {
    const TsmTaxesApi* taxes = nullptr;
    const TsmRepublicBankApi* bank = nullptr;
    int currency = 0; // 0 = RUB, 1 = USD
    bool alive = true;
};

// Population callback (C ABI). Builds a bounded authoritative snapshot.
int populate(const AlmanacHostSurface* host, AlmanacPageSnapshot* out, void* userData);

// Action callback (C ABI).
void action(AlmanacPageId page, unsigned actionId, void* userData);

// Apply one action. Returns 1 if a setter was invoked, 0 otherwise.
int apply_action(Context* ctx, unsigned actionId);

// Adapter owns registration/unregistration and guards against use-after-free.
class Adapter {
public:
    Adapter(AlmanacShell* shell, const TsmTaxesApi* taxes, const TsmRepublicBankApi* bank,
            int currency = 0);
    ~Adapter();

    Adapter(const Adapter&) = delete;
    Adapter& operator=(const Adapter&) = delete;

    int handle() const { return handle_; }
    bool registered() const { return handle_ > 0; }
    void unregister();

    Context& context() { return ctx_; }
    const Context& context() const { return ctx_; }

    static int populateThunk(const AlmanacHostSurface* host, AlmanacPageSnapshot* out,
                             void* userData) {
        return populate(host, out, userData);
    }
    static void actionThunk(AlmanacPageId page, unsigned actionId, void* userData) {
        action(page, actionId, userData);
    }

private:
    AlmanacShell* shell_ = nullptr;
    int handle_ = 0;
    Context ctx_{};
};

// Convenience aliases.
using FiscalPolicy = Adapter;
using FiscalPolicyAdapter = Adapter;
using FiscalPolicyPage = Adapter;

// C-style registration helper. Heap-allocates an adapter that lives until the
// process exits; suitable for integration. Returns the shell handle or 0.
int register_provider(AlmanacShell* shell, const TsmTaxesApi* taxes,
                      const TsmRepublicBankApi* bank, int currency = 0);

} // namespace fiscal_policy
} // namespace almanac
