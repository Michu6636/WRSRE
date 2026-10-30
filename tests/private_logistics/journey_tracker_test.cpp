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
