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
