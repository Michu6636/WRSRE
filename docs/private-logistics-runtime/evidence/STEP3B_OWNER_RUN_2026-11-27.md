# Step 3b owner-run evidence — 2026-11-27

## Provenance and limits

The owner supplied `C:\WRSRE\scratch\tesmioloader-3.log` and states it is from
the same build as `main` at `be946b9`, with plugin code unchanged since PR #102.
The log itself is local/ignored and is not copied into the repository.

- SHA-256: `38669f286e93b62f335c0c4ad0859d0c0fd16db3ba16b03e3488f44b6c0dcf86`
- Size: 1,821,223 bytes; 10,844 lines
- Startup line 178 reports `private_logistics live v0.19.1-live-billing:
  billing=1 dry_run=0`.
- This report preserves selected derived results, not the full game log. The
  build identity is owner-reported; no binary hash is present in the log.

## Receiver-pair check

Ran the owner's `do_receiver_check.py` logic against the supplied log. The
tracked verifier is [do_receiver_check.py](./do_receiver_check.py); the command
run from the repository root was:

```text
python docs\private-logistics-runtime\evidence\do_receiver_check.py C:\WRSRE\scratch\tesmioloader-3.log
```

Its summary output is:

```text
0x68F792 committed_deliveries=661 adjacent_committed_0x68F7F9_pairs=661 tonnage_matched_pairs=661 p9==vehicle_home=661
deliveries with no open assignment for that truck: {'residental_brick_7': 105}
DO trips=32 exactly-one-receiver-equal-to-trip-target=32
```

The verifier requires a positive, committed mode-0 building leg and the next
logged `[B2B SITE]` row to be a committed, negative mode-1 vehicle leg within
two physical log lines. It checks matching absolute tonnage with the engine's
0.5%/0.0005-tonne tolerance and requires the building-side `p9` to equal the
vehicle leg's `vhome`.

Independent checks of the input also found 661 committed `0x68F792` site rows
and 661 committed `0x68F7F9` rows. The first pair begins at log line 1488; the
last building-side event is at line 10742. The log shows destination privacy
classification for the observed receivers: state buildings include
`DLC3_department_store_3` and `DLC3_food_factory_small`; private buildings
include `DLC3_distillery_small`, `DLC3_pub`, and `fabric_factory`.

The first example pair is adjacent in the log:

```text
1488 [21:22:02.460] [B2B SITE] rva=0x68F792 mode=0 sign=+ tons=0.077 commit=1 flag=0 p2=0000024F25475130 "DLC3_department_store_3" priv=0 p9=0000024F2547BA80 vhome=0000000000000000 class=delivery
1490 [21:22:02.460] [B2B SITE] rva=0x68F7F9 mode=1 sign=- tons=-0.077 commit=1 flag=0 p2=0000024F25492900 "-" priv=0 p9=0000024F2547BA80 vhome=0000024F2547BA80 class=transship
```

This is direct runtime evidence for the 661 logged transfers in this run. It
contradicts older documentation that classified this pair as sea/rail-only.
The old logger has no explicit transfer-sequence field: the verifier establishes
adjacency among logged site rows, not the absence of an unlogged transfer hook
between them. The implementation must therefore enforce pair sequence at the
detour and fail closed on ambiguity. This observation does not establish that
all game modes or builds use this pair, and it does not prove a receiver for
assignments with no such pair.
The source file remains local by design; reviewers can inspect the verifier,
its exact output above, the input hash, and these bounded raw excerpts, but
cannot rerun the complete extraction without the original log.

## Missing-cargo investigation

The source explains why a matched transfer can produce no goods payment:
`Engine::matchMode1Delivery` consumes a matching pending building-side delivery
and returns `true` even when the vehicle's tracked cargo lots are absent or
insufficient (`plugins/private_logistics/b2b_settlement.h`). The caller then
iterates seller parts only under `md.cargoFound`; there is no `else` diagnostic
(`plugins/private_logistics/private_logistics.cpp`). Thus the missing payment
is a confirmed silent fail-closed path, not a transfer refusal.

The owner's addendum states that all 206 affected deliveries had earlier
committed `0x694F40 -> 0x6953C4` load pairs for the same truck and enough
tonnage. This rules out a missing load hook or insufficient loaded amount for
those reported cases, but those facts are not emitted in the old log as a
per-vehicle cargo-ledger trace.

The source inspection found a concrete cargo-state eviction defect:

1. `VehSlotIdx` reads `vehicle+0x10` and treats any value below 16,384 as a
   direct index; otherwise it hashes the vehicle pointer into the same
   16,384-entry table. The tracked native layout instead identifies
   `vehicle+0x04` as the global vehicle index
   (`docs/engine/private-industry-re.md`, consolidated offset table). No
   evidence in the repo establishes `+0x10` as a vehicle index.
2. Each indexed `VehCargoState` stores its owning `vehPtr`. When a load arrives
   with a different pointer at that index, `matchMode1Load` replaces the owner
   pointer and clears all four cargo lots. When a delivery arrives with a
   different pointer at that index, `matchMode1Delivery` does the same before
   attempting to pop cargo. Thus two trucks aliased to the same unverified
   `+0x10` value or hash bucket can erase each other's cargo even though the
   original truck had a successful load match and sufficient tonnage.
3. The only production call to `resetAll()` is on `TSM_SAVE_LOADING`
   (`private_logistics.cpp`); it clears every cargo state. No daily reset is
   present. The old run log does not record slot indexes, owner changes, or
   save-load callbacks, so it cannot prove which of these reset events occurred
   for each of the 206 cases.

The best-supported source-level cause is therefore an unverified per-truck
slot key combined with destructive owner-mismatch clearing; it can explain
loss after successful loads when vehicles alias the same slot. The exact
number of the 206 attributable to index aliasing versus any save-load reset
cannot be independently established from this log. The step-3b implementation
now reads the documented native `+0x04` index, fails closed rather than hashing
an unavailable or out-of-range index, and keeps the pointer identity guard.
Host tests cover invalid indices and pointer reuse; they do not prove which
mechanism caused each historical loss. Cargo is still reset on save loading
and is not persisted.

The caller also needs a capped `[B2B GOODS] ERROR NO CARGO RECORD` line and
per-day `no_cargo` count. Those report the missing attribution without
pretending to identify an upstream cause.

The owner reports 206 no-cargo matches in the early run and 554.09 RUB of
unbilled private-buyer deliveries (DLC3_pub 327.13, DLC3_distillery_small
152.00, fabric_factory 74.96). Those per-match `cargoFound` and locked-load
amounts are not printed by the existing build, so this report treats them as
owner-reported values rather than independently reconstructed measurements.
The new bounded error line and day counter will make subsequent runs
independently auditable.

## Vehicle-leg label

For all 661 `0x68F7F9` rows, `p9 == vehicleHome`; the generic
`isStaging` predicate therefore classified the vehicle-side delivery leg as a
load. That branch left `s.unitPrice` at zero, so the generic observer printed
`@UNPRICED (no native price; bill refused)` even though the priced building
leg is the one used for goods settlement. This was a logging-classification
error, not evidence that the goods leg was refused. The step-3b branch now
prints this site as a vehicle leg; no changed-build in-game output is claimed.
