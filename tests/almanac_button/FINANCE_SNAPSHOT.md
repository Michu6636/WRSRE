# Read-only Almanac Finance snapshot

## Scope and integration

- `src/almanac_finance_snapshot.h`: `BuildFinanceSnapshot(currency, taxes)`
  returns owned scalar data and per-field availability. No retained service
  pointers, native offsets, UI context, allocation or writes.
- `src/almanac_finance_view.h`: `BuildFinanceView(snapshot)` produces ten
  fixed-capacity English label/value rows plus a disclosure note. It is not
  a window, translated UI, layout implementation or editing interface.
- `tests/almanac_button/almanac_finance_test.cpp`: actual production builder
  and formatter tested against fake public service tables, with all mutators
  absent. No provider ABI is changed.

A future adapter resolves exact versions (currency v4, optionally v3; taxes
v3, falling back to v2/v1), calls the builder while services are alive at an approved
UI phase, and passes the copied snapshot to presentation. Check `Available()`
before using any field: default numeric storage is not a fallback value.
Render the view's note along with its rows, not just the numbers. Refresh by
building a fresh snapshot; missing services never preserve stale values.

## Data semantics and verified source

- Treasury is read using an explicit validated active currency code. USD/RUB
  labels come from that same code, not a second symbol getter that could race
  a currency change. Negative balances and genuine zero are valid.
- Currency v3 has no active-currency getter. Do NOT use the convenience
  helper's legacy RUB fallback and label it an observed active currency.
  Active treasury is unavailable with that prefix.
- VAT/profit effective values are fractions [0,1], displayed as percentages.
  `GetVatRateImpl`/`GetProfitTaxRateImpl` in `plugins/taxes/taxes.cpp` report
  zero when suspended. Never label them stored/configured rates. An enabled
  zero rate and a suspended zero rate remain distinct when status is exposed.
- Enabled-state reads require only their guarded getter, not the write-policy
  callbacks required by `TsmTaxesHasPolicy`. Taxes v1 still exposes effective
  rates but not enabled status. Each member access checks `structSize` first.
- Taxes v3 exposes configured VAT/profit values even when suspended. The
  consumer uses one `getPolicySnapshot` call, derives effective rates from
  that same policy, and does not call legacy getters on this path. With v1/v2
  providers, configured values remain unavailable rather than inferred.
- Property base is an active-currency amount PER ENTERPRISE, not a percentage
  or government-wide total. Taxes v3 exposes the live (post startup currency
  conversion) base and whether `SettlePropertyAll` scales it by attractiveness.
  Almanac displays the rule but never calculates a final building tax bill.
  With older providers both fields remain unavailable.
- Missing service, missing getter, not exposed and invalid value are distinct
  reasons. NaN/infinite balances, non-finite/out-of-range fractions and unknown
  currency/status codes are rejected. Finite zero is not marked unavailable.

## Limits we cannot hide

The v3 policy getter copies all tax fields under one provider lock. Failure
(before provider startup, for example) marks tax fields ReadFailed; malformed
output marks them InvalidValue. Neither case silently falls back to unrelated
legacy reads. Legacy providers still use sequential best-effort getters, where
status/rate mismatches are possible and preserved rather than rewritten.
Treasury/currency and policy are NOT one atomic financial snapshot. The tax
snapshot does not carry a currency code; property display assumes the provider
and currency service retain the same active currency established at startup.
Service lifetime and calling phase remain the adapter's responsibility.

`plugins/currency/transaction_treasury.h::Api_GetTreasuryBalance` delegates to
`transaction_native.h::ReadTreasuryNative`, which returns zero for null,
unreadable and non-finite native balances. The existing API has no success
flag. An Available treasury means a finite SERVICE-REPORTED balance, not an
independently verified native read. A future versioned provider snapshot/try-get
would be needed to distinguish those failures from genuine zero. We do not
change existing consumers or manufacture such validity here.

Formatting is bounded and allocation-free; Windows uses `_snprintf_s` and the
Linux host harness uses `snprintf`. Amounts too long for the display buffer
produce `Out of display range`, never truncated misleading digits. English
labels are initial presentation metadata; native Unicode rendering/localization
and currency-specific rounding policies remain integration work.

## Host verification

From the repository root:

```sh
mkdir -p build
g++ -std=c++17 -O2 -Wall -Wextra -Werror -pedantic -Isrc \
  tests/almanac_button/almanac_finance_test.cpp -o build/almanac_finance_test
build/almanac_finance_test
```

For ASan/UBSan, replace `-O2` with
`-O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer`.

Coverage: missing/partial/old service views, every advertised byte-size
boundary for both public tables, absent getters, independent getter support
with no write callbacks, zeros, negative treasury, USD/RUB, invalid values,
stored/effective distinction, no fabricated property values, fresh snapshots
on service loss, copied-data ownership and bounded display overflow.

Optimized and ASan/UBSan Linux runs passed on 2026-11-14. The panel lifecycle,
button-core and row-geometry host tests also pass. No Windows/native build or
runtime integration was performed. Independent child review is unavailable;
this work is not independently reviewed.

## Taxes API v3 provider verification

`TsmTaxPolicySnapshot` and the appended `getPolicySnapshot(out, outSize)` are
read-only. Taxes 4.6 publishes v1/v2 prefixes and v3; no old field moves.
The getter copies configured rates, enabled flags and the property rule under
one existing lock, then releases it before copying to caller storage. Null,
undersized and pre-Start requests fail without touching output. The ready flag
is set under that lock only after successful Start, after USD base conversion
and service/persistence registration. No native hook, save format, tax setter,
settlement schedule, accrual or billing calculation is changed.

The base is `g_propertyTaxFlat`, not the raw `g_propertyTaxFlatCfg`: the former
is the active live policy used by settlements and UI. This does not add dynamic
currency conversion when the global active currency changes; the existing
provider initializes that conversion at startup. Snapshot consumers must not
claim a final attractiveness-adjusted bill or atomic cross-service currency.

Because the current service version advances to 3, enterprise_base 9.19 and
private_shops 3.5 now request v3 -> v2 -> v1. Without that fallback, rebuilt
consumers could lose taxes with an older installed provider. Existing binaries
still resolve their original versions from the new provider.

Run:

```sh
python3 tests/almanac_button/run_tax_policy_tests.py
python3 tests/almanac_button/run_tax_policy_tests.py --sanitize
```

The runner extracts the actual getter/API table, publication block and both
consumer lookup blocks to ignored build storage, compiling them with a fake
host/critical-section shim and the real Finance headers. It checks original
prefix layout sizes/offsets, registered versions, fallback lookup order,
configured values during suspension, single lock balance, copied-data ownership
across a simulated post-unlock update, output bounds, readiness failure,
normalization, property display, and v3 preference without legacy reads.
Consumer tests also reject malformed/failed snapshots and exercise every
advertised API byte-size boundary. This is not a real threaded stress test or
a Windows critical-section implementation test.

Both runner modes, existing Finance optimized/sanitizer tests, lifecycle,
button-core and row-geometry tests pass. A C11 compile smoke test of the new
public header also passes. No Windows toolchain/native game is available here;
DLL build/deployment and independent child review remain outstanding.
