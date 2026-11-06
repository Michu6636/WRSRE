# Almanac root composition (07)

Status: portable composition and synthetic integration tests complete. No native
host, production hook, plugin entry point, loader registration, or Windows build
is added by this change.

## Ownership and lifetime

`plugins/almanac/almanac_composition.{h,cpp}` defines `AlmanacComposition`, the
single owner of one mod-owned `AlmanacShell` and its provider registrations.
Construct it during plugin Start, after dependencies have published services.
The composition itself is not a plugin entry point and does not call `provide`.

A ready composition has seven providers:

- one existing Fiscal Policy provider with its action callback;
- five statistics providers: Overview, Money Flow, Private Economy, Prices &
  Citizens, and Logistics & Production;
- one Causation provider.

Registration is transactional. Failure of any registration unregisters every
provider already installed and destroys the partial shell. `shutdown()` is
idempotent, clears callback lifetime flags, unregisters providers before shell
destruction, and clears derived state. The composition is non-copyable.
City Hall and its fiscal fallback are unchanged.

## Exact service consumption

The root calls `TsmHost.consume` only when the host prefix contains that member.
It requests these exact versions and checks API `structSize` before appended
fields are read:

| Service | Version | Used for |
| --- | ---: | --- |
| `sim_clock` | 1 | Current date and configured days per month |
| `currency` | 4 | Active currency/symbol, treasury, total wallets |
| `enterprise_base` | 8 | Enterprise count/capital, pool, monthly summary, evictions, rent |
| `resource_pricing` | 4 | Resource records/prices, CPI basket, food market multiplier |
| `taxes` | 3 | Existing Fiscal Policy configured-policy provider |
| `republic_bank` | 1 | Existing authoritative bank-rate provider |

A missing or malformed service does not prevent shell construction. Affected
metrics and controls are unavailable; fiscal command rows remain visible but
disabled. Without active-currency metadata, monetary statistics are unavailable
while non-monetary counts, ratios, and dry-run metadata remain supported. If
taxes are available without currency metadata, the property tax remains the
same flat authoritative amount but its unit is explicitly unknown, not guessed
as RUB. No alternate economic state or parallel setters are introduced.

## Update and population contract

- `updateStatistics()` is the simulation/update-thread boundary for statistics
  service reads. Statistics and causation page population only copies bounded
  snapshots and does not consume services. The existing fiscal provider keeps
  its authoritative API readback and setter behavior.
- `recordStatisticsHistory()` refreshes the current snapshot before recording
  one bounded history sample. `statisticsHistoryCount()` exposes only the count.
- Invalid or unavailable clock dates clear a previously valid derived snapshot.
  Backward dates clear statistics history and causation state. Currency changes
  retain the statistics core's reset behavior and update the fiscal display unit.
- `reset(reason)` is the explicit save/world lifecycle boundary. It clears only
  derived statistics/history/causation data, never authoritative economy state.
- Construction may allocate the implementation owner. Page population uses
  fixed-capacity snapshots and rows, with no heap allocation.

## Availability boundaries

The wiring deliberately leaves these values unavailable because no suitable
public closed-period contract is consumed:

- treasury-flow totals and their exact period boundaries;
- logistics fuel/trip/ton metrics and production output;
- citizen count.

The logistics `dry_run` flag remains available as `ACTUAL` with value 1 under
the existing portable-core contract. No billing behavior changes: logistics
billing remains 0 and dry-run remains 1. Dry-run metric rows retain the
`DRY_RUN` value class and are never merged with actual or estimated values.
All statistics labels visibly include their value class.

Resource prices and CPI require live resource records. Configured pricing
fallbacks are not promoted to live `ACTUAL` values before those records exist.
Enterprise capital summation rejects non-finite values, null entries, and an
enumeration whose count changes during the read; the public API still does not
provide an aggregate-lock snapshot.

## Causation provider

`almanac_page_providers.{h,cpp}` presents the existing ledger without inferring
causation. `addCausationNode`, `addCausationEdge`, and `observeCausationTime`
are explicit ingestion boundaries serialized with page population by a mutex.
No live observer is registered by this change.

Four diagnostic rows are reserved before event/edge rows: snapshot status,
events shown/total, edges shown/total, and missing parents. Event and edge rows
show value class or evidence status. The 64-row display cap can never hide the
diagnostics; display-limited and source-truncated snapshots are labelled
incomplete. Missing parents and unresolved edges remain explicit.

## Verification

`tests/almanac/test_almanac_composition.py` builds and runs the offline C++17
integration harness. Coverage includes exact versions, all seven providers,
all statistics pages, no statistics reads during population, real fiscal pointer
dispatch, currency readback, value classes, causation provenance and truncation,
backward/invalid time, missing services, malformed host/API prefixes, and
idempotent shutdown.

Focused shell and fiscal suites also cover provider-specific empty states and
unavailable/disabled command rows. These are synthetic tests only; native
geometry, input, lifecycle, hook safety, Windows build, live service behavior,
and City Hall runtime fallback acceptance remain deferred.
