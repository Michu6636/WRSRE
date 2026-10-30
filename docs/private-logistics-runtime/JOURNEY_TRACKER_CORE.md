# Loaded-journey tracker core — implementation and actual checks

Date: 2026-10-29. Branch: `arena/15fdac3e-wrsre`.

## Status and scope

Implemented a production C++ core in `plugins/private_logistics/journey_tracker.h`, included by `private_logistics.cpp`, and tested that exact header natively. This is **not an active in-game tracker**: no instance, event adapter, hook, runtime logging call or settlement is connected. No validated fuel quantity has been discovered by this work. The ongoing native panel investigation remains the integration gate.

The user's approval to implement this isolated core supersedes the older README's sequencing restriction for this bounded work only. It does not authorize speculative memory reads, active hooks or billing. The runtime version remains v0.18.5-source10. Dispatch, permissions, INI settings and source fallback defaults are unchanged; billing=0/dry_run=1 remain covered by regression tests. There is no new configuration switch to enable: no new runtime behavior needs a game test yet.

No game binary, Ghidra project, save, actual fuel capture or Windows compiler is present here. Native Linux C++ execution is not a Windows DLL build or in-game validation.

## Implemented behavior

One `Tracker` instance represents one vehicle incarnation and office binding in one session. It contains no game pointers, game offsets, heap allocation, IO, native inventory changes, price access or payment code.

| Input/state | Behavior |
|---|---|
| Initially unknown/loaded | Wait for authoritative empty goods state; do not invent an in-progress journey |
| Confirmed transfer whose before-state is empty | That before-state can establish empty; positive after-state starts one journey |
| Extra loading / partial unload | Retain the same journey and accumulated actual deductions |
| Confirmed all-goods-unloaded | Emit one cargo-completed summary; immediately ready for a later genuine journey |
| Positive residual goods, however small | Still loaded; no speculative epsilon-based completion |
| Fuel-only inventory changes | Never start or prolong an ordinary-goods journey |
| Actual clamped deduction while loaded | Accumulate the supplied actual tonnes; never requested burn |
| Refuel while loaded | Record separately; never subtract from or reset burn |
| Sampled/requested/unknown burn | Do not add it as actual consumption; mark the journey's consumption uncertain |
| Burn/refuel on an empty leg | Excluded from loaded-journey accounting |
| Duplicate identical last envelope | No second transition, burn accumulation or completion |
| Same-sized burn with a new valid sequence | Counts as a separate actual event, not a heuristic duplicate |
| Sequence gap/drop/order error or invalid identity/data | Emit at most one incomplete summary if loaded; abandon continuity |
| Snapshot changes goods without a confirmed transfer | Cannot start/complete a journey; invalidate any active one |
| Reset/reload/reassignment | Explicitly invalidate; never restore a live journey or fuel-proof flag |

An incomplete/uncertain result must never be charged. `completed=true` means only that the supplied confirmed cargo boundary completed; it does NOT mean fuel was measured exactly or that settlement is permitted. `fuelExact` defaults false unless the future adapter explicitly binds a proven complete actual-deduction stream. Even a journey with no burn records remains uncertain with the default coverage. With a proven stream, a genuine zero-deduction journey can be exact. Arithmetic uses doubles and is subject to floating-point rounding, not arbitrary mathematical precision; tests use an explicit 1e-12 tolerance for expected ordinary totals. Overflow/nonfinite inputs fail closed.

## Future adapter contract — mandatory before runtime connection

1. Bind nonzero opaque session, vehicle-incarnation and office tokens. A raw address alone is not a vehicle identity: addresses can be reused and cannot carry continuity across reload. Create/reset the core explicitly when bindings change. Calls must be serialized; this core does not provide a thread-safe registry.
2. Supply authoritative complete goods before/after aggregates at committed transfers tied to the SAME vehicle, not site-only callbacks or a sampled positive-cargo transition. Keep engine fuel inventory in `Cargo.fuelInventoryTons`, out of `goodsTons`. Native resource classification is still the adapter's job; the core cannot verify a caller's labels. Before-state equality is deliberately strict; canonicalize native aggregates deterministically rather than silently ignoring mismatches. Exact zero is required for empty.
3. Assign a dense sequence per tracker covering all relevant events. A filtered process-wide sequence is NOT dense. Lost native events before routing must also propagate an explicit `Gap` to every potentially affected tracker: assigning fresh dense numbers after a drop does not repair continuity. An identical retry must retain its original envelope sequence. Deduplicate complementary/native callback observations using proven native operation identity, not equal amounts.
4. Event order must reflect actual boundary order, including burn during loading/unloading. A complete-deduction guarantee requires actual, post-clamp, per-vehicle native deductions exactly once, with proven units and event-order semantics across the first load and last unload. Constructor/reset `CompleteActualDeductionStream` is an upstream assertion, NOT a discovery mechanism. An `ActualClampedDeduction` event alone cannot promote an unverified stream to exact.
5. Endpoint subtraction and sums of sampled decreases cannot distinguish simultaneous burn/refill. They may be logged as `RequestedOrSampled`/`Unknown` evidence, but are not counted as actual burn and poison exact-consumption status. Refills cannot repair uncertainty. Do not silently reuse legacy mirror readings as exact deductions.
6. The adapter must consume/queue returned outcomes using a bounded loss-aware path, check `FormatSummary` success, and preserve `(session, vehicle, office, journey)` as the record key. The core emits once per accepted journey; it does not guarantee exactly-once log delivery or payment across crashes, queues or storage. Older/conflicting replay envelopes invalidate continuity, rather than reconstructing past payments.
7. No active state persistence. A vehicle found loaded after reset is incomplete until empty is observed. Rebinding defaults fuel coverage back to unverified; within the same core object journey serials do not roll back on reset. A reconstructed tracker requires a new session/incarnation namespace if its serials restart.
8. Later settlement, separately gated, must use actual tonnes burned × in-game fuel price at settlement time, with valid price/currency and an idempotent settlement ledger. No wear, margin, per-fragment tariff, or monetary mutation is implemented here.

### API sketch (not runtime wiring)

```cpp
using namespace private_logistics_journey;
Tracker tracker(Identity{sessionToken, vehicleIncarnationToken, officeToken});
// Unverified coverage by default. Do not opt into complete coverage on guesses.
Event event;
event.identity = identity;
event.sequence = nextDenseVehicleSequence;
event.kind = Kind::GoodsTransfer; // Only when committed vehicle-specific transfer is proven.
event.before = authoritativeBefore;
event.after = authoritativeAfter;
Outcome out = tracker.Consume(event);
// out.started: one start; out.emitted: completed/incomplete summary to queue.
// FormatSummary only formats; it never writes logs, changes inventory or pays.
```

`ObserveCargo` can establish a baseline or confirm unchanged goods; a changed snapshot is not promoted to a committed transfer. After a sequence gap the gapped event payload is discarded; a subsequent authoritative empty state/before-state is needed. Invalid-identity envelopes are not allowed to change the bound identity or import their sequence numbers into its stream.

## Actual validation

The Python test runner compiles the C++ harness against the production header with `-std=c++17 -Wall -Wextra -Werror -pedantic`. Thirty named native scenarios cover the transitions above, invalid numbers/enums, sequence exhaustion, identity separation, reset-proof loss, bounded formatting, and 1,000 seeded generated journeys with repeated loading, partial unloading, burn and refill events. Two additional Python checks guard deferred production wiring and emulate Windows min/max macro pollution. That macro check is NOT a Windows build.

Commands executed after the final implementation:

```sh
python3 -m unittest discover -s tests/private_logistics -v
python3 -m unittest discover -s tests/private_logistics_probe -v
python3 -m unittest discover -s tests/private_logistics_v018_model -v
python3 -m unittest discover -s tests/private_shops -v
python3 -m unittest discover -s tests/vehicle_gauge_strings -v

g++ -std=c++17 -Wall -Wextra -Werror -pedantic -O2 \
  -I plugins/private_logistics tests/private_logistics/journey_tracker_test.cpp \
  -o .cache/journey_tracker_test
.cache/journey_tracker_test

g++ -std=c++17 -Wall -Wextra -Werror -pedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g -O1 \
  -I plugins/private_logistics tests/private_logistics/journey_tracker_test.cpp \
  -o .cache/journey_tracker_sanitized
.cache/journey_tracker_sanitized

git diff --check
```

An initial optimized sanitizer compile flagged intentional small-buffer `snprintf` truncation under `-Werror`. The formatter was changed to format into a bounded 512-byte local record and copy only a complete record that fits; the final optimized and sanitizer commands pass. No warning suppression was added. Native compiler artifacts/captured outputs stay in ignored `.cache`, not Git.

Full new source and actual final results follow, all in this one Markdown for handover. Existing historical Python-model tests are regression checks only, not an alternative implementation promoted into production.

## Actual final regression output

```text
private_logistics: Ran 45 tests in 1.130s
OK
private_logistics_probe: Ran 93 tests in 0.093s
OK
private_logistics_v018_model: Ran 38 tests in 0.002s
OK
private_shops: Ran 4 tests in 0.174s
OK
vehicle_gauge_strings: Ran 14 tests in 0.034s
OK

Total: 194 tests, all PASS.
Optimized C++ harness: 30 scenarios PASS.
AddressSanitizer + UndefinedBehaviorSanitizer: 30 scenarios PASS; no diagnostics.
git diff --check: PASS.
```

### Complete sanitizer scenario output

```text
PASS boundaries
PASS fuel_excluded
PASS two_journeys
PASS initially_loaded
PASS transfer_before_is_baseline
PASS snapshots_not_transfers
PASS partial_snapshots_and_residue
PASS refill_does_not_cancel_burn
PASS uncertain_evidence
PASS coverage_default_is_unverified
PASS duplicate_completion
PASS duplicate_burn_and_equal_values
PASS gap_empty_payload_not_baseline
PASS sequence_exhaustion
PASS conflicting_duplicate
PASS sequence_gap
PASS sequence_order
PASS explicit_gap
PASS identity_changes
PASS reload_reset
PASS mismatched_cargo
PASS invalid_cargo
PASS invalid_fuel
PASS overflow
PASS invalid_enums
PASS zero_deductions
PASS formatted_output
PASS format_incomplete
PASS interleaved_vehicles
PASS randomized_journeys
```

## Complete new production and test source

### plugins/private_logistics/journey_tracker.h

```cpp
#ifndef PRIVATE_LOGISTICS_JOURNEY_TRACKER_H
#define PRIVATE_LOGISTICS_JOURNEY_TRACKER_H

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>

// Production tracker core only: no game offsets, hooks, IO, persistence or money.
// One instance per vehicle incarnation. The future adapter must serialize calls
// and supply authoritative, fully ordered events, not sampled inferred trips.
namespace private_logistics_journey {

struct Identity {
    // Opaque nonzero adapter tokens, NOT raw pointers or IDs reused after reload.
    std::uint64_t session = 0, vehicle = 0, office = 0;
    bool valid() const { return session && vehicle && office; }
    bool operator==(const Identity& b) const {
        return session == b.session && vehicle == b.vehicle && office == b.office;
    }
};

struct Cargo {
    // Adapter must classify inventory first. Engine fuel is never ordinary goods.
    double goodsTons = 0.0;
    double fuelInventoryTons = 0.0;
};

enum class Kind { ObserveCargo, GoodsTransfer, FuelBurn, Refuel, Gap };
enum class FuelEvidence { ActualClampedDeduction, RequestedOrSampled, Unknown };
enum class FuelCoverage { Unverified, CompleteActualDeductionStream };
enum class Phase { WaitingForEmpty, ArmedEmpty, Loaded };
enum class Reason {
    None, Completed, Reset, InvalidIdentity, SequenceGap, OutOfOrder,
    ExplicitGap, InvalidCargo, CargoMismatch, UnconfirmedCargoChange,
    InvalidFuel, UncertainFuel, NumericOverflow, InvalidEvent
};

inline const char* ReasonName(Reason r) {
    switch (r) {
        case Reason::None: return "none";
        case Reason::Completed: return "completed";
        case Reason::Reset: return "reset";
        case Reason::InvalidIdentity: return "invalid_identity";
        case Reason::SequenceGap: return "sequence_gap";
        case Reason::OutOfOrder: return "out_of_order";
        case Reason::ExplicitGap: return "explicit_gap";
        case Reason::InvalidCargo: return "invalid_cargo";
        case Reason::CargoMismatch: return "cargo_mismatch";
        case Reason::UnconfirmedCargoChange: return "unconfirmed_cargo_change";
        case Reason::InvalidFuel: return "invalid_fuel";
        case Reason::UncertainFuel: return "uncertain_fuel";
        case Reason::NumericOverflow: return "numeric_overflow";
        case Reason::InvalidEvent: return "invalid_event";
    }
    return "invalid_event";
}

struct Event {
    Identity identity;
    // Dense, monotonically increasing PER-TRACKER stream; all relevant events.
    // A global sequence filtered down to one truck is not suitable here.
    std::uint64_t sequence = 0;
    Kind kind = Kind::ObserveCargo;
    Cargo before, after;
    double fuelTons = 0.0;
    FuelEvidence fuelEvidence = FuelEvidence::Unknown;
};

struct Summary {
    Identity identity;
    std::uint64_t journey = 0, startSequence = 0, endSequence = 0;
    bool completed = false;
    bool fuelExact = false;
    double actualBurnTons = 0.0, refillTons = 0.0;
    std::uint64_t burnEvents = 0;
    Reason reason = Reason::None;
};

struct Outcome {
    bool started = false, emitted = false, duplicate = false;
    Reason issue = Reason::None;
    // Start metadata when started; terminal record when emitted. Otherwise empty.
    Summary summary;
};

class Tracker {
public:
    explicit Tracker(Identity identity = Identity(),
                     FuelCoverage coverage = FuelCoverage::Unverified)
        : identity_(identity), coverage_(coverage) {}

    Phase phase() const { return phase_; }

    // Always explicit on world reset/reload, vehicle reuse or office reassignment.
    // No pointer continuity or active journey is restored from a save.
    Outcome Reset(Identity identity, FuelCoverage coverage = FuelCoverage::Unverified) {
        Outcome out = Invalidate(Reason::Reset, lastSequence_);
        identity_ = identity;
        coverage_ = coverage;
        seen_ = false;
        lastSequence_ = 0;
        return out;
    }

    Outcome Consume(const Event& e) {
        if (!identity_.valid() || !e.identity.valid() || !(e.identity == identity_))
            return Invalidate(Reason::InvalidIdentity, lastSequence_);
        if (!e.sequence) return Invalidate(Reason::OutOfOrder, lastSequence_);
        if (seen_ && e.sequence == lastSequence_ && SameEvent(e, lastEvent_)) {
            Outcome out;
            out.duplicate = true;
            return out;
        }
        if (seen_ && e.sequence <= lastSequence_)
            return Invalidate(Reason::OutOfOrder, lastSequence_);
        const bool gap = seen_ && e.sequence - lastSequence_ != 1;
        seen_ = true;
        lastSequence_ = e.sequence;
        lastEvent_ = e;
        // Consume the envelope but not its payload after a gap. A subsequent
        // authoritative empty state is required; never bridge missing events.
        if (gap) return Invalidate(Reason::SequenceGap, e.sequence);

        switch (e.kind) {
            case Kind::ObserveCargo: return Observe(e);
            case Kind::GoodsTransfer: return Transfer(e);
            case Kind::FuelBurn: return Burn(e);
            case Kind::Refuel: return Refill(e);
            case Kind::Gap: return Invalidate(Reason::ExplicitGap, e.sequence);
        }
        return Invalidate(Reason::InvalidEvent, e.sequence);
    }

private:
    Identity identity_;
    FuelCoverage coverage_;
    Phase phase_ = Phase::WaitingForEmpty;
    bool goodsKnown_ = false, seen_ = false;
    double goods_ = 0.0;
    std::uint64_t serial_ = 0, lastSequence_ = 0;
    Event lastEvent_;
    Summary active_;

    static bool Quantity(double x) { return std::isfinite(x) && x >= 0.0; }
    static bool ValidCargo(const Cargo& c) {
        return Quantity(c.goodsTons) && Quantity(c.fuelInventoryTons);
    }
    static bool SameEvent(const Event& a, const Event& b) {
        return a.identity == b.identity && a.sequence == b.sequence &&
            a.kind == b.kind && a.before.goodsTons == b.before.goodsTons &&
            a.before.fuelInventoryTons == b.before.fuelInventoryTons &&
            a.after.goodsTons == b.after.goodsTons &&
            a.after.fuelInventoryTons == b.after.fuelInventoryTons &&
            a.fuelTons == b.fuelTons && a.fuelEvidence == b.fuelEvidence;
    }
    Outcome Invalidate(Reason why, std::uint64_t end) {
        Outcome out;
        out.issue = why;
        if (phase_ == Phase::Loaded) {
            out.emitted = true;
            out.summary = active_;
            out.summary.completed = false;
            out.summary.fuelExact = false;
            out.summary.reason = why;
            out.summary.endSequence = end;
        }
        active_ = Summary();
        phase_ = Phase::WaitingForEmpty;
        goodsKnown_ = false;
        return out;
    }
    void Baseline(const Cargo& c) {
        goodsKnown_ = true;
        goods_ = c.goodsTons;
        phase_ = goods_ == 0.0 ? Phase::ArmedEmpty : Phase::WaitingForEmpty;
    }
    Outcome Observe(const Event& e) {
        if (!ValidCargo(e.after)) return Invalidate(Reason::InvalidCargo, e.sequence);
        if (goodsKnown_ && goods_ != e.after.goodsTons) {
            Outcome out = Invalidate(Reason::UnconfirmedCargoChange, e.sequence);
            Baseline(e.after); // This observation cannot start/complete a trip.
            return out;
        }
        if (phase_ != Phase::Loaded) Baseline(e.after);
        return Outcome();
    }
    Outcome Transfer(const Event& e) {
        if (!ValidCargo(e.before) || !ValidCargo(e.after))
            return Invalidate(Reason::InvalidCargo, e.sequence);
        if (goodsKnown_ && goods_ != e.before.goodsTons) {
            Outcome out = Invalidate(Reason::CargoMismatch, e.sequence);
            Baseline(e.after);
            return out; // No replacement journey from an inconsistent transfer.
        }
        if (!goodsKnown_) Baseline(e.before);
        goodsKnown_ = true;
        goods_ = e.after.goodsTons;
        Outcome out;
        if (phase_ == Phase::Loaded) {
            if (goods_ == 0.0) {
                out.emitted = true;
                out.summary = active_;
                out.summary.completed = true;
                out.summary.endSequence = e.sequence;
                out.summary.reason = active_.fuelExact ? Reason::Completed : Reason::UncertainFuel;
                active_ = Summary();
                phase_ = Phase::ArmedEmpty;
            }
        } else if (phase_ == Phase::ArmedEmpty && goods_ > 0.0) {
            if (serial_ == UINT64_MAX)
                return Invalidate(Reason::NumericOverflow, e.sequence);
            active_ = Summary();
            active_.identity = identity_;
            active_.journey = ++serial_;
            active_.startSequence = e.sequence;
            // Absence of burn events is NOT evidence of zero consumption unless
            // the future adapter explicitly guarantees complete actual events.
            active_.fuelExact = coverage_ == FuelCoverage::CompleteActualDeductionStream;
            phase_ = Phase::Loaded;
            out.started = true;
            out.summary = active_; // Expose the stable key immediately, without emitting completion.
        } else if (goods_ == 0.0) {
            phase_ = Phase::ArmedEmpty;
        }
        return out;
    }
    Outcome Burn(const Event& e) {
        if (!Quantity(e.fuelTons)) return Invalidate(Reason::InvalidFuel, e.sequence);
        if (e.fuelEvidence != FuelEvidence::ActualClampedDeduction &&
            e.fuelEvidence != FuelEvidence::RequestedOrSampled &&
            e.fuelEvidence != FuelEvidence::Unknown)
            return Invalidate(Reason::InvalidEvent, e.sequence);
        if (phase_ != Phase::Loaded) return Outcome(); // Exclude empty legs.
        if (e.fuelEvidence != FuelEvidence::ActualClampedDeduction) {
            active_.fuelExact = false;
            Outcome out;
            out.issue = Reason::UncertainFuel;
            return out; // Never add sampled/requested quantities as actual burn.
        }
        const double sum = active_.actualBurnTons + e.fuelTons;
        if (!Quantity(sum) || active_.burnEvents == UINT64_MAX)
            return Invalidate(Reason::NumericOverflow, e.sequence);
        active_.actualBurnTons = sum;
        ++active_.burnEvents;
        return Outcome();
    }
    Outcome Refill(const Event& e) {
        if (!Quantity(e.fuelTons)) return Invalidate(Reason::InvalidFuel, e.sequence);
        if (phase_ != Phase::Loaded) return Outcome();
        const double sum = active_.refillTons + e.fuelTons;
        if (!Quantity(sum)) return Invalidate(Reason::NumericOverflow, e.sequence);
        active_.refillTons = sum; // Never subtract from, or reset, actual burn.
        return Outcome();
    }
};

// Optional bounded text formatting only; caller owns storage/IO. Never settles.
// False means invalid destination or truncation; partial text is not a record.
inline bool FormatSummary(const Summary& s, char* buffer, std::size_t size) {
    if (!buffer || !size) return false;
    buffer[0] = '\0';
    char record[512];
    const int n = std::snprintf(record, sizeof(record),
        "[JOURNEY_CORE] schema=1 session=%llu vehicle=%llu office=%llu journey=%llu "
        "start_seq=%llu end_seq=%llu completed=%u fuel_exact=%u "
        "actual_burn_t=%.17g refill_t=%.17g burn_events=%llu reason=%s billing=off",
        static_cast<unsigned long long>(s.identity.session),
        static_cast<unsigned long long>(s.identity.vehicle),
        static_cast<unsigned long long>(s.identity.office),
        static_cast<unsigned long long>(s.journey),
        static_cast<unsigned long long>(s.startSequence),
        static_cast<unsigned long long>(s.endSequence),
        s.completed ? 1u : 0u, s.fuelExact ? 1u : 0u, s.actualBurnTons, s.refillTons,
        static_cast<unsigned long long>(s.burnEvents), ReasonName(s.reason));
    if (n < 0 || static_cast<std::size_t>(n) >= sizeof(record) ||
        static_cast<std::size_t>(n) >= size) return false;
    std::memcpy(buffer, record, static_cast<std::size_t>(n) + 1);
    return true;
}

} // namespace private_logistics_journey
#endif
```

### tests/private_logistics/journey_tracker_test.cpp

```cpp
// Executes the exact production header. Synthetic inputs, NOT game evidence.
#include "journey_tracker.h"
#include <cassert>
#include <cmath>
#include <cstring>
#include <limits>
#include <random>
#include <cstdio>
#ifdef NDEBUG
#error This harness requires assertions enabled
#endif
using namespace private_logistics_journey;

static bool Near(double a, double b) { return std::fabs(a-b) < 1e-12; }
struct Rig {
    Identity id;
    Tracker tracker;
    std::uint64_t seq = 0;
    Event last;
    explicit Rig(bool complete = true)
        : id{1, 10, 100}, tracker(id, complete ? FuelCoverage::CompleteActualDeductionStream
                                             : FuelCoverage::Unverified) {}
    Outcome Send(Event e) { e.identity = id; e.sequence = ++seq; last = e; return tracker.Consume(e); }
    Outcome Observe(double goods, double fuel = 0) {
        Event e; e.kind = Kind::ObserveCargo; e.after = Cargo{goods, fuel}; return Send(e);
    }
    Outcome Move(double before, double after, double fuelBefore = 0, double fuelAfter = 0) {
        Event e; e.kind = Kind::GoodsTransfer;
        e.before = Cargo{before, fuelBefore}; e.after = Cargo{after, fuelAfter}; return Send(e);
    }
    Outcome Burn(double tons, FuelEvidence proof = FuelEvidence::ActualClampedDeduction) {
        Event e; e.kind = Kind::FuelBurn; e.fuelTons = tons; e.fuelEvidence = proof; return Send(e);
    }
    Outcome Refill(double tons) { Event e; e.kind = Kind::Refuel; e.fuelTons = tons; return Send(e); }
    void Load(double tons = 2) {
        auto out = Move(0, tons);
        assert(out.started && !out.emitted && out.summary.journey > 0);
        assert(out.summary.identity == id && out.summary.startSequence == seq);
        assert(!out.summary.completed && out.summary.endSequence == 0);
    }
};
static void Completed(const Outcome& out, double burn, bool exact = true) {
    assert(out.emitted && out.summary.completed && out.summary.fuelExact == exact);
    assert(Near(out.summary.actualBurnTons, burn));
    assert(out.summary.reason == (exact ? Reason::Completed : Reason::UncertainFuel));
}
static void Aborted(const Outcome& out, Reason reason) {
    assert(out.emitted && !out.summary.completed && !out.summary.fuelExact);
    assert(out.issue == reason && out.summary.reason == reason);
}

static void boundaries() {
    Rig r; r.Observe(0); r.Load(); r.Burn(.1);
    assert(!r.Move(2, 5).started); r.Burn(.2);
    assert(!r.Move(5, 1).emitted); assert(r.tracker.phase() == Phase::Loaded);
    r.Burn(.3); auto out = r.Move(1, 0); Completed(out, .6);
    assert(out.summary.startSequence == 2 && out.summary.endSequence == 8);
    assert(out.summary.burnEvents == 3 && out.summary.identity == r.id);
    assert(r.tracker.phase() == Phase::ArmedEmpty);
    assert(!r.Move(0, 0).emitted);
}
static void fuel_excluded() {
    Rig r; r.Observe(0, .5);
    assert(!r.Move(0, 0, .5, 1).started);
    assert(!r.Move(0, 0, 1, .1).emitted);
    assert(r.Move(0, 3, .1, .1).started);
    auto out = r.Move(3, 0, .1, .1); Completed(out, 0);
    assert(r.tracker.phase() == Phase::ArmedEmpty); // Fuel remains aboard.
}
static void two_journeys() {
    Rig r; r.Burn(99); r.Load(); r.Burn(.2);
    auto first = r.Move(2, 0); Completed(first, .2);
    r.Burn(88); r.Refill(7); r.Load(4); r.Burn(.3);
    auto second = r.Move(4, 0); Completed(second, .3);
    assert(first.summary.journey == 1 && second.summary.journey == 2);
    assert(second.summary.refillTons == 0);
}
static void initially_loaded() {
    Rig r; r.Observe(3); r.Burn(.5);
    assert(!r.Move(3, 5).started && r.tracker.phase() == Phase::WaitingForEmpty);
    assert(!r.Move(5, 2).emitted); assert(!r.Move(2, 0).emitted);
    r.Load(); r.Burn(.1); Completed(r.Move(2, 0), .1);
}
static void transfer_before_is_baseline() {
    Rig r; assert(r.Move(0, 1).started); Completed(r.Move(1, 0), 0);
    Rig unknown; assert(!unknown.Move(3, 1).started);
    assert(!unknown.Move(1, 0).emitted); unknown.Load();
}
static void snapshots_not_transfers() {
    Rig r; r.Observe(0); auto out = r.Observe(3);
    assert(!out.started && !out.emitted && out.issue == Reason::UnconfirmedCargoChange);
    assert(!r.Move(3, 0).emitted); r.Load(); r.Burn(.1);
    Aborted(r.Observe(0), Reason::UnconfirmedCargoChange);
    r.Load(); Completed(r.Move(2, 0), 0);
}
static void partial_snapshots_and_residue() {
    Rig r; r.Load(); r.Burn(.1);
    assert(!r.Observe(2).emitted);
    assert(!r.Move(2, 1e-15).emitted); // No arbitrary cargo epsilon.
    assert(r.tracker.phase() == Phase::Loaded);
    Completed(r.Move(1e-15, 0), .1);
}
static void refill_does_not_cancel_burn() {
    Rig r; r.Load(); r.Burn(.3); r.Refill(4);
    auto duplicate = r.tracker.Consume(r.last); assert(duplicate.duplicate);
    r.Burn(.4); auto out = r.Move(2, 0); Completed(out, .7);
    assert(out.summary.refillTons == 4 && out.summary.burnEvents == 2);
}
static void uncertain_evidence() {
    for (auto evidence : {FuelEvidence::RequestedOrSampled, FuelEvidence::Unknown}) {
        Rig r; r.Load(); r.Burn(.1);
        assert(r.Burn(.9, evidence).issue == Reason::UncertainFuel);
        r.Refill(3); r.Burn(.2); auto out = r.Move(2, 0);
        Completed(out, .3, false); assert(out.summary.burnEvents == 2);
    }
}
static void coverage_default_is_unverified() {
    Rig r(false); r.Load(); r.Burn(.2); Completed(r.Move(2, 0), .2, false);
    Rig noEvents(false); noEvents.Load(); Completed(noEvents.Move(2, 0), 0, false);
    Identity id{9, 8, 7}; Tracker t(id); // Actual API default, not the Rig default.
    Event e; e.identity = id; e.sequence = 1; e.kind = Kind::GoodsTransfer;
    e.after.goodsTons = 1; assert(t.Consume(e).started);
    e.sequence = 2; e.before.goodsTons = 1; e.after.goodsTons = 0;
    Completed(t.Consume(e), 0, false);
}
static void duplicate_completion() {
    Rig r; r.Load(); r.Burn(.1); Completed(r.Move(2, 0), .1);
    auto duplicate = r.tracker.Consume(r.last);
    assert(duplicate.duplicate && !duplicate.emitted && !duplicate.started);
    r.Load(); Completed(r.Move(2, 0), 0);
}
static void duplicate_burn_and_equal_values() {
    Rig r; r.Load(); r.Burn(.125);
    assert(r.tracker.Consume(r.last).duplicate); // Retry, not another deduction.
    r.Burn(.125); // Distinct sequence: equal-sized actual deduction is legitimate.
    auto out = r.Move(2, 0); Completed(out, .25);
    assert(out.summary.burnEvents == 2);
}
static void gap_empty_payload_not_baseline() {
    Rig r; r.Load(); r.seq += 1;
    Aborted(r.Observe(0), Reason::SequenceGap); // Discard this payload.
    assert(r.tracker.phase() == Phase::WaitingForEmpty);
    r.Observe(3); // Already loaded after lost events: still cannot start.
    assert(!r.Move(3, 0).emitted); r.Load(); Completed(r.Move(2, 0), 0);
}
static void sequence_exhaustion() {
    Rig r; Event e; e.identity = r.id; e.kind = Kind::GoodsTransfer;
    e.sequence = std::numeric_limits<std::uint64_t>::max(); e.after.goodsTons = 2;
    assert(r.tracker.Consume(e).started);
    e.sequence = 0; e.before.goodsTons = 2; e.after.goodsTons = 0;
    Aborted(r.tracker.Consume(e), Reason::OutOfOrder);
    r.tracker.Reset(r.id); r.seq = 0; r.Load(); Completed(r.Move(2, 0), 0, false);
}
static void conflicting_duplicate() {
    Rig r; r.Load(); Event changed = r.last; changed.after.goodsTons = 9;
    Aborted(r.tracker.Consume(changed), Reason::OutOfOrder);
    assert(!r.Move(2, 0).emitted); r.Load(); Completed(r.Move(2, 0), 0);
}
static void sequence_gap() {
    Rig r; r.Load(); r.Burn(.1); r.seq += 2;
    Aborted(r.Burn(.2), Reason::SequenceGap);
    assert(r.tracker.phase() == Phase::WaitingForEmpty);
    assert(!r.Move(2, 3).started); assert(!r.Move(3, 0).emitted);
    r.Load(); r.Burn(.4); Completed(r.Move(2, 0), .4);
}
static void sequence_order() {
    Rig r; r.Load(); r.Burn(.1); Event stale = r.last; --stale.sequence;
    Aborted(r.tracker.Consume(stale), Reason::OutOfOrder);
    assert(!r.Move(2, 0).emitted);
    Event zero = r.last; zero.sequence = 0;
    assert(r.tracker.Consume(zero).issue == Reason::OutOfOrder);
}
static void explicit_gap() {
    Rig r; r.Load(); Event e; e.kind = Kind::Gap;
    Aborted(r.Send(e), Reason::ExplicitGap);
    assert(!r.Move(2, 1).started); r.Move(1, 0); r.Load();
    Completed(r.Move(2, 0), 0);
}
static void identity_changes() {
    for (unsigned field = 0; field < 3; ++field) {
        Rig r; r.Load(); Event wrong = r.last; wrong.sequence++;
        if (field == 0) ++wrong.identity.session;
        if (field == 1) ++wrong.identity.vehicle;
        if (field == 2) ++wrong.identity.office;
        Aborted(r.tracker.Consume(wrong), Reason::InvalidIdentity);
        assert(!r.Move(2, 0).emitted);
    }
    Tracker unbound; Event e; e.sequence = 1;
    assert(unbound.Consume(e).issue == Reason::InvalidIdentity);
    Rig r; r.Load(); Event invalid = r.last; invalid.identity.vehicle = 0;
    Aborted(r.tracker.Consume(invalid), Reason::InvalidIdentity);
}
static void reload_reset() {
    Rig r; r.Load(); r.Burn(.2);
    Identity next{2, 10, 100}; // Same vehicle number is NOT continuity across sessions.
    auto out = r.tracker.Reset(next); Aborted(out, Reason::Reset);
    assert(out.summary.identity == r.id && Near(out.summary.actualBurnTons, .2));
    assert(!r.tracker.Reset(next).emitted);
    r.id = next; r.seq = 0; r.Observe(2); r.Burn(.4); r.Move(2, 0);
    r.Load(); r.Burn(.1); out = r.Move(2, 0);
    Completed(out, .1, false); // Reset defaults to UNVERIFIED, never inherits proof.
    assert(out.summary.journey == 2 && out.summary.identity == next);
}
static void mismatched_cargo() {
    Rig r; r.Load(); r.Burn(.1);
    Aborted(r.Move(3, 4), Reason::CargoMismatch);
    assert(!r.Move(4, 0).emitted); r.Load(); Completed(r.Move(2, 0), 0);
}
static void invalid_cargo() {
    for (double bad : {-1.0, std::numeric_limits<double>::infinity(),
                       std::numeric_limits<double>::quiet_NaN()}) {
        Rig r; r.Load(); Aborted(r.Move(2, bad), Reason::InvalidCargo);
        Rig fuel; fuel.Load(); Aborted(fuel.Observe(2, bad), Reason::InvalidCargo);
        Rig before; before.Load(); Aborted(before.Move(bad, 0), Reason::InvalidCargo);
    }
}
static void invalid_fuel() {
    for (double bad : {-1.0, std::numeric_limits<double>::infinity(),
                       std::numeric_limits<double>::quiet_NaN()}) {
        Rig r; r.Load(); Aborted(r.Burn(bad), Reason::InvalidFuel);
        Rig refill; refill.Load(); Aborted(refill.Refill(bad), Reason::InvalidFuel);
    }
}
static void overflow() {
    const double big = std::numeric_limits<double>::max();
    Rig r; r.Load(); r.Burn(big);
    auto out = r.Burn(big); Aborted(out, Reason::NumericOverflow);
    assert(std::isfinite(out.summary.actualBurnTons));
    Rig refill; refill.Load(); refill.Refill(big);
    Aborted(refill.Refill(big), Reason::NumericOverflow);
}
static void invalid_enums() {
    Rig r; r.Load(); Event e; e.kind = static_cast<Kind>(999);
    Aborted(r.Send(e), Reason::InvalidEvent);
    Rig burn; burn.Load(); Aborted(burn.Burn(.1, static_cast<FuelEvidence>(999)), Reason::InvalidEvent);
}
static void zero_deductions() {
    Rig r; r.Load(); r.Burn(0); r.Refill(0); auto out = r.Move(2, 0);
    Completed(out, 0); assert(out.summary.burnEvents == 1);
}
static void formatted_output() {
    Rig r; r.Load(); r.Burn(.125); r.Refill(.25); auto out = r.Move(2, 0);
    char buffer[1024]; assert(FormatSummary(out.summary, buffer, sizeof(buffer)));
    for (const char* needle : {"[JOURNEY_CORE] schema=1", "session=1", "vehicle=10",
            "office=100", "journey=1", "completed=1", "fuel_exact=1",
            "actual_burn_t=0.125", "refill_t=0.25", "billing=off"})
        assert(std::strstr(buffer, needle));
    char tiny[8] = "old"; assert(!FormatSummary(out.summary, tiny, sizeof(tiny)) && tiny[0] == 0);
    assert(!FormatSummary(out.summary, nullptr, 0));
    assert(!FormatSummary(out.summary, buffer, 0));
    assert(!r.tracker.Reset(r.id).emitted); // Already completed: no second summary.
}
static void format_incomplete() {
    Rig r; r.Load(); auto out = r.tracker.Reset(r.id);
    char buffer[1024]; assert(FormatSummary(out.summary, buffer, sizeof(buffer)));
    assert(std::strstr(buffer, "completed=0") && std::strstr(buffer, "fuel_exact=0"));
    assert(std::strstr(buffer, "reason=reset"));
}
static void interleaved_vehicles() {
    Rig a, b; b.id.vehicle = 11;
    b.tracker.Reset(b.id, FuelCoverage::CompleteActualDeductionStream);
    a.Load(); b.Load(5); a.Burn(.1); b.Burn(.7); a.Refill(1);
    auto first = a.Move(2, 0); auto second = b.Move(5, 0);
    Completed(first, .1); Completed(second, .7);
    assert(first.summary.identity.vehicle != second.summary.identity.vehicle);
    assert(first.summary.refillTons == 1 && second.summary.refillTons == 0);
}
static void randomized_journeys() {
    Rig r; std::mt19937 random(6712);
    for (unsigned trip = 1; trip <= 1000; ++trip) {
        r.Burn(100); r.Load(1); double goods = 1, expected = 0, refills = 0;
        for (unsigned i = 0; i < 15; ++i) {
            switch (random() % 4) {
                case 0: { double b = (random() % 1000) / 10000.0;
                    expected += b; r.Burn(b); break; }
                case 1: { double added = (random() % 1000) / 100.0;
                    refills += added; r.Refill(added); break; }
                case 2: r.Move(goods, goods + 1); goods += 1; break;
                case 3: if (goods > 1) { r.Move(goods, goods - 1); goods -= 1; } break;
            }
        }
        auto out = r.Move(goods, 0); Completed(out, expected);
        assert(out.summary.journey == trip && Near(out.summary.refillTons, refills));
        assert(r.tracker.Consume(r.last).duplicate);
    }
}

struct Test { const char* name; void (*run)(); };
static const Test tests[] = {
#define TEST(name) {#name, name}
    TEST(boundaries), TEST(fuel_excluded), TEST(two_journeys), TEST(initially_loaded),
    TEST(transfer_before_is_baseline), TEST(snapshots_not_transfers), TEST(partial_snapshots_and_residue),
    TEST(refill_does_not_cancel_burn), TEST(uncertain_evidence), TEST(coverage_default_is_unverified),
    TEST(duplicate_completion), TEST(duplicate_burn_and_equal_values),
    TEST(gap_empty_payload_not_baseline), TEST(sequence_exhaustion), TEST(conflicting_duplicate), TEST(sequence_gap), TEST(sequence_order),
    TEST(explicit_gap), TEST(identity_changes), TEST(reload_reset), TEST(mismatched_cargo),
    TEST(invalid_cargo), TEST(invalid_fuel), TEST(overflow), TEST(invalid_enums),
    TEST(zero_deductions), TEST(formatted_output), TEST(format_incomplete),
    TEST(interleaved_vehicles), TEST(randomized_journeys)
#undef TEST
};
int main(int argc, char** argv) {
    bool found = false;
    for (const auto& test : tests) if (argc == 1 || std::strcmp(argv[1], test.name) == 0) {
        test.run(); found = true; std::printf("PASS %s\n", test.name);
    }
    return found ? 0 : 2;
}
```

### tests/private_logistics/test_journey_tracker.py

```python
"""Native tests of the production C++ journey core, never a duplicate model.

Only synthetic event streams. No offsets, hooks, or game lifecycle are validated.
"""
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
HARNESS = ROOT / 'tests/private_logistics/journey_tracker_test.cpp'


class JourneyTrackerTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        compiler = shutil.which('g++') or shutil.which('clang++')
        if not compiler:
            raise unittest.SkipTest('C++ compiler required')
        cls.compiler = compiler
        cls.directory = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.directory.cleanup)
        cls.executable = Path(cls.directory.name) / 'journey_tracker_test'
        result = subprocess.run([
            compiler, '-std=c++17', '-Wall', '-Wextra', '-Werror', '-pedantic',
            '-I', str(ROOT / 'plugins/private_logistics'),
            str(HARNESS), '-o', str(cls.executable),
        ], capture_output=True, text=True)
        if result.returncode:
            raise AssertionError(result.stderr)

    def test_runtime_wiring_remains_deferred(self):
        source = (ROOT / 'plugins/private_logistics/private_logistics.cpp').read_text()
        self.assertIn('#include "journey_tracker.h"', source)
        self.assertNotIn('private_logistics_journey::Tracker', source)
        self.assertNotIn('private_logistics_journey::FormatSummary', source)
        header = (ROOT / 'plugins/private_logistics/journey_tracker.h').read_text()
        for forbidden in ('windows.h', 'currency_api', 'game_world.h', 'reinterpret_cast'):
            self.assertNotIn(forbidden, header)

    def test_windows_min_max_macro_compatibility(self):
        # Emulate macro pollution from windows.h; this is not a Windows DLL build.
        source = Path(self.directory.name) / 'windows_macros.cpp'
        source.write_text('''#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))
#include "journey_tracker.h"
int main() { private_logistics_journey::Tracker tracker; return 0; }
''')
        result = subprocess.run([
            self.compiler, '-std=c++17', '-O2', '-Wall', '-Wextra', '-Werror',
            '-I', str(ROOT / 'plugins/private_logistics'), str(source),
            '-o', str(Path(self.directory.name) / 'windows_macros'),
        ], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)

    def run_case(self, case):
        result = subprocess.run([str(self.executable), case], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn('PASS ' + case, result.stdout)


def native_case(case):
    def run(self):
        self.run_case(case)
    return run


# Register every case in the C++ dispatch table, not a separately maintained list.
for name in re.findall(r'TEST\((\w+)\)', HARNESS.read_text()):
    if name != 'name':  # Ignore the macro definition itself.
        setattr(JourneyTrackerTest, 'test_' + name, native_case(name))


if __name__ == '__main__':
    unittest.main()
```

### Complete change to the plugin translation unit

Only this include/comment was added after the existing `task_audit.h` include:

```cpp
// Compiled/tested core only. No runtime consumer until native vehicle, cargo
// boundaries and actual fuel-deduction provenance/order have been validated.
#include "journey_tracker.h"
```

The README now links this document and distinguishes the new disconnected core
from historical probe instructions. No INI or source fallback default was changed.

## Remaining integration gates

- Proven road-truck/DO identity and native panel-to-object association.
- Confirmed complete vehicle-specific goods transfer boundaries, resource classification and ordering.
- Proven actual clamped per-vehicle burn events/units, refill/reset behavior and loss detection.
- Logging-only adapter and bounded delivery of outcomes, with its required test profile shipped in BOTH INI and source defaults.
- A controlled real-game run and independent validation of actual logs before any settlement work.

Until those gates pass, do not copy a DLL to test this core as if the fuel reader
were fixed. It deliberately has no game effect yet. This is real implemented C++
logic with native synthetic tests, not a claim of runtime deployment or validation.
