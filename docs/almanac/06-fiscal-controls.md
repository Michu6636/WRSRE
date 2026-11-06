# Almanac Fiscal Policy page and UI-neutral adapter

## Purpose

This change implements the Almanac Fiscal Policy page adapter. It exposes the
existing tax and republic_bank controls through the Almanac shell without
duplicating economic state or creating parallel setters. City Hall remains
unchanged as the migration fallback.

Owned files:

- `plugins/almanac/fiscal_policy.h`
- `plugins/almanac/fiscal_policy.cpp`
- `tests/almanac/fiscal_policy_test.cpp`
- `tests/almanac/test_fiscal_policy.py`
- `docs/almanac/06-fiscal-controls.md`
- narrow additions to `src/taxes_api.h` and `plugins/taxes/taxes.cpp`

## Authoritative state and setter paths

- VAT enabled state and configured rate: `TsmTaxesApi.isTaxEnabled(TSM_TAX_VAT)`,
  `setTaxEnabled`, `setTaxRate`, `getTaxConfiguredRate`.
- Profit tax enabled state and configured rate: same with `TSM_TAX_PROFIT`.
- Property tax enabled state and flat active-currency amount: same with
  `TSM_TAX_PROPERTY`. Property tax remains a flat amount, never a percentage.
- Republic Bank annual interest rate: `TsmRepublicBankApi.getAnnualInterestRate`
  and `setAnnualInterestRate`.
- The adapter holds no authoritative policy state. It reads through the services
  and commits only through their setters.

## Action IDs and command-step sources

Stable action IDs are defined in `fiscal_policy.h`:

- `ACTION_VAT_TOGGLE`, `ACTION_VAT_RATE_UP`, `ACTION_VAT_RATE_DOWN`
- `ACTION_PROFIT_TOGGLE`, `ACTION_PROFIT_RATE_UP`, `ACTION_PROFIT_RATE_DOWN`
- `ACTION_PROPERTY_TOGGLE`, `ACTION_PROPERTY_AMOUNT_UP`, `ACTION_PROPERTY_AMOUNT_DOWN`
- `ACTION_BANK_RATE_UP`, `ACTION_BANK_RATE_DOWN`

Command steps come only from authoritative service metadata:

- Tax rate steps use `TsmTaxesApi.getTaxSliderStepPct()` (percent points).
- Property amount steps use that step combined with
  `TsmTaxesApi.getTaxPropertySliderMax()` (flat active-currency units).
- Bank rate commands use the bank's four-unit / 1% grid via
  `NormalizeBankRate`.

If step metadata is unavailable, the corresponding rate/amount command rows are
disabled rather than using a guessed increment.

## Taxes ABI addition and compatibility guards

`src/taxes_api.h` is extended from version 2 to version 3 in an append-only
manner:

- `TSM_TAXES_VERSION` is now 3.
- `TSM_TAXES_VERSION_V2` (2) and `TSM_TAXES_VERSION_V1` (1) are published.
- New tail functions are appended after `setTaxEnabled`:
  `getTaxConfiguredRate`, `getTaxSliderStepPct`, `getTaxPropertySliderMax`,
  `getTaxPolicy`.
- `TsmTaxPolicy` snapshot struct is added.
- `TsmTaxesHasPolicy` guards the v2 tail.
- `TsmTaxesHasConfiguredPolicy` guards the v3 tail.
- `plugins/taxes/taxes.cpp` publishes v1, v2, and v3 views with structSize
  set to the appropriate prefix and reports plugin version 4.6. Existing
  function offsets and behaviour are unchanged.

## Bank normalization/readback behaviour

- Bank reads use `getAnnualInterestRate`.
- Bank writes use `setAnnualInterestRate`, which normalizes to the four-unit /
  1% grid.
- The adapter rereads the stored value after a set; it never optimistically
  displays the requested value.
- Legacy fractional saves continue to normalize through republic_bank's load path.

## Provider/userData lifetime

- The adapter registers one provider on `ALMANAC_PAGE_FISCAL_POLICY` and
  attaches one action callback to that provider. If action registration fails,
  it rolls back its own provider registration and reports itself unregistered.
- The callback accepts only the Fiscal Policy page, so stale or misrouted page
  commands cannot mutate economic state.
- `Context` is the userData. `alive` is cleared before unregistration.
- Unregistration calls `almanac_shell_unregister_provider`, preventing later
  actions into destroyed adapter state.
- The adapter does not retain service or shell pointers after unregistration.

## City Hall fallback

- City Hall row/widget providers in `plugins/taxes/taxes.cpp` are unchanged.
- Both City Hall and Almanac may use the same authoritative services.
- City Hall remains usable if Almanac is unavailable.

## Registration left for integration

- Production host enablement, loader registration, and address registration are
  not performed here.
- Portable root construction with consumed taxes and republic_bank services is
  implemented in `07-composition.md`; no production plugin entry point is added.

## Deferred runtime acceptance

- Native host geometry, selection, input, lifecycle, and hook-safety acceptance
  remain deferred.
- Runtime validation of displayed versus stored values on the live game build
  remains deferred.
- No Windows DLL build or native game validation was performed for this change.
