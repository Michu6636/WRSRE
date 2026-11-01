// Copied-value observation ledger and provisional raw endpoint hypothesis. No actual burn,
// native job IDs, price service, settlement or payments in this portable core.
#ifndef PRIVATE_LOGISTICS_SCHEDULE_ASSIGNMENT_LEDGER_H
#define PRIVATE_LOGISTICS_SCHEDULE_ASSIGNMENT_LEDGER_H
#include "schedule_observer.h"

namespace private_logistics_assignment {
namespace sch = private_logistics_schedule;
using sch::Address;
// Signed native values, separate from the historical positive-only UI litres.
struct NativeFuel {
    unsigned count=0;
    Address objects[private_logistics_panel::MaxObjects]{};
    Address metadata[private_logistics_panel::MaxObjects]{};
    float raw[private_logistics_panel::MaxObjects]{}, capacity[private_logistics_panel::MaxObjects]{};
    bool valid=false;
};
inline bool ValidFuel(const NativeFuel& f) {
    if(!f.valid || !f.count || f.count>private_logistics_panel::MaxObjects) return false;
    for(unsigned i=0;i<f.count;++i) {
        if(!f.objects[i] || !f.metadata[i] || !std::isfinite(f.raw[i]) ||
           !std::isfinite(f.capacity[i]) || f.capacity[i]<=0) return false;
        for(unsigned j=0;j<i;++j) if(f.objects[i]==f.objects[j]) return false;
    }
    return true;
}
inline double NativeTotal(const NativeFuel& f) {
    if(!ValidFuel(f)) return 0; // Callers also log the explicit validity flag.
    double total=0;for(unsigned i=0;i<f.count;++i) total+=double(f.raw[i]);
    return total;
}
inline bool SameFuel(const NativeFuel& a,const NativeFuel& b) {
    if(!ValidFuel(a) || !ValidFuel(b) || a.count!=b.count) return false;
    for(unsigned i=0;i<a.count;++i)
        if(a.objects[i]!=b.objects[i] || a.metadata[i]!=b.metadata[i] || a.capacity[i]!=b.capacity[i]) return false;
    return true;
}
struct Point {
    unsigned sample=0;
    std::uint64_t ms=0;
    bool fuelValid=false;
    float litres=0; // Historical UI candidate only; NEVER used for tonne pricing.
    NativeFuel native{};
};
struct Candidate {
    std::uint64_t id=0,epoch=0;
    Address root=0,home=0;
    // Frozen first-observed single unloading-row target, NOT a verified payer.
    // Adopted candidates can have an unknown true original target/start.
    Address originalTarget=0;
    unsigned originalTargetRows=0;
    Point first{},last{};
    // Approximation only: sum positive per-object drops; rises never erase drops.
    double sampledDecrease=0,sampledIncrease=0;
    unsigned sampledPairs=0;
    bool sampleBaselineLost=false;
    bool unknownStart=false,observationGap=false,roleUncertain=false;
    bool fuelUncertain=false,receiverChanged=false,observedRise=false;
};
struct Endpoint {
    bool valid=false,observedRise=false;
    double net=0; // end minus start, native raw units; NOT actual consumption.
};
inline Endpoint Estimate(const Candidate& c,const Point& end) {
    Endpoint e{};e.observedRise=c.observedRise;
    if(!c.id || c.unknownStart || c.observationGap || c.roleUncertain ||
       c.fuelUncertain || c.receiverChanged || !SameFuel(c.first.native,end.native)) return e;
    for(unsigned i=0;i<end.native.count;++i) {
        e.net+=double(end.native.raw[i])-double(c.first.native.raw[i]);
        e.observedRise=e.observedRise || end.native.raw[i]>c.last.native.raw[i];
    }
    e.valid=std::isfinite(e.net);
    return e;
}
enum class Kind { Opened, Adopted, Updated, Closed, Abandoned, Uncertain, IdExhausted };
inline const char* KindName(Kind k) {
    switch(k) {
        case Kind::Opened:return "observed_open";
        case Kind::Adopted:return "adopt_existing";
        case Kind::Updated:return "observed_update";
        case Kind::Closed:return "observed_close";
        case Kind::Abandoned:return "abandon_unobserved";
        case Kind::Uncertain:return "observation_unavailable";
        default:return "id_exhausted";
    }
}
struct Event {
    Kind kind=Kind::Uncertain;
    const char* reason="unavailable";
    Candidate candidate{};
    Point boundary{}; // Current captured endpoint, NOT the exact native boundary.
};
struct Result { Event events[2]{}; unsigned count=0; };
class Ledger {
    Candidate active_{};
    std::uint64_t serial_=0;
    bool invalidStreak_=false;
    static void CheckFuel(Candidate& c,const Point& p,bool comparable) {
        if(!ValidFuel(p.native)) {c.fuelUncertain=true;c.sampleBaselineLost=true;return;}
        if(ValidFuel(c.first.native) && !SameFuel(c.first.native,p.native)) c.receiverChanged=true;
        const bool same=SameFuel(c.last.native,p.native);
        if(same) {
            for(unsigned i=0;i<p.native.count;++i) {
                const double delta=double(p.native.raw[i])-double(c.last.native.raw[i]);
                c.observedRise=c.observedRise || delta>0;
                if(comparable && !c.sampleBaselineLost) {
                    if(delta<0) c.sampledDecrease-=delta;else c.sampledIncrease+=delta;
                }
            }
            if(comparable && !c.sampleBaselineLost) ++c.sampledPairs;
        }
        c.sampleBaselineLost=false; // Rebase; never bridge a known invalid interval.
    }
    static void Add(Result& r,Kind kind,const char* reason,const Candidate& c,const Point& p) {
        Point boundary=p;
        // An abandoned interval has no observed end reading for its old context.
        // In particular, never attach another vehicle/load's fuel to it.
        if(kind==Kind::Abandoned) {boundary.fuelValid=false;boundary.litres=0;boundary.native=NativeFuel{};}
        r.events[r.count++]={kind,reason,c,boundary}; // At most close/abandon + open/adopt.
    }
public:
    explicit Ledger(std::uint64_t initialSerial=0):serial_(initialSerial) {}
    bool Active() const {return active_.id!=0;}
    const Candidate& Current() const {return active_;}
    void Break() {if(Active()) {active_.observationGap=true;active_.sampleBaselineLost=true;}}
    Result Abandon(const char* reason) {
        Result r;
        if(Active()) {
            active_.observationGap=true;
            Add(r,Kind::Abandoned,reason,active_,Point{});
            active_=Candidate{};
        }
        return r; // Original target survives in copied event; no refund or native dereference.
    }
    Result Observe(const sch::Snapshot& s,const sch::Delta& d,const sch::FieldChanges& changes,
                   std::uint64_t epoch,Address root,Address home,Point point) {
        Result result;
        point.fuelValid=point.fuelValid && std::isfinite(point.litres) && point.litres>=0;
        if(!point.fuelValid) point.litres=0; // Explicitly invalid, not observed zero.
        if(!ValidFuel(point.native) || point.native.objects[0]!=root) point.native.valid=false;
        const bool changedIdentity=Active() &&
            (active_.epoch!=epoch || active_.root!=root || active_.home!=home);
        if(changedIdentity) {
            active_.observationGap=true;
            Add(result,Kind::Abandoned,"identity_or_load_changed",active_,point);
            active_=Candidate{};
        }
        if(Active() && (point.sample<=active_.last.sample || point.ms<active_.last.ms)) {
            Break();
            Add(result,Kind::Uncertain,"out_of_order_not_removal",active_,point);
            return result;
        }
        const auto pattern=sch::ClassifyPattern(s,home);
        if(pattern==sch::Pattern::Unavailable || !root || !home || !epoch || !point.sample) {
            if(Active()) {
                Break();
                if(!invalidStreak_) Add(result,Kind::Uncertain,"invalid_capture_not_removal",active_,point);
            }
            invalidStreak_=true;
            return result;
        }
        invalidStreak_=false;
        if(Active() && (!d.endpointCompared || !changes.comparable)) {
            active_.observationGap=true;
            Add(result,Kind::Abandoned,"comparison_unavailable",active_,point);
            active_=Candidate{};
        }
        if(Active()) {
            active_.observationGap=active_.observationGap || d.endpointAcrossGap;
            CheckFuel(active_,point,!d.endpointAcrossGap); // Includes quiet samples and the closing boundary.
            if(changes.comparable && changes.Structure()) {
                const char* reason=pattern==sch::Pattern::Empty ? "observed_removed" :
                    pattern==sch::Pattern::LoadUnload ? "observed_targets_replaced" :
                    pattern==sch::Pattern::OfficeOnly ? "observed_office_only" : "observed_structure_changed";
                Add(result,Kind::Closed,reason,active_,point);
                active_=Candidate{};
            } else {
                // Flags/config availability/scalars/index/storage do not create
                // another assignment. Losing role recognition is uncertainty.
                active_.roleUncertain=active_.roleUncertain || pattern!=sch::Pattern::LoadUnload;
                active_.last=point; // Quiet accepted samples matter for endpoints.
                if(d.emit) Add(result,Kind::Updated,"same_observed_targets",active_,point);
                return result;
            }
        }
        if(pattern==sch::Pattern::LoadUnload) {
            // Saturate rather than reuse a process-local ID. No durable native ID.
            if(serial_==UINT64_MAX) {
                Add(result,Kind::IdExhausted,"no_id_reuse",Candidate{},point);
                return result;
            }
            const bool appeared=!changedIdentity && d.endpointCompared && changes.comparable && changes.Structure();
            active_.id=++serial_;active_.epoch=epoch;active_.root=root;active_.home=home;
            active_.first=active_.last=point;
            for(unsigned i=0;i<s.count;++i) {
                const auto& row=s.rows[i];
                if(row.configRead && row.target && row.target!=home && row.flag00==0 && row.flagC8==1) {
                    ++active_.originalTargetRows;
                    active_.originalTarget=row.target;
                }
            }
            if(active_.originalTargetRows!=1) active_.originalTarget=0; // No arbitrary buyer selection.
            active_.fuelUncertain=!ValidFuel(point.native);
            active_.unknownStart=!appeared;
            active_.observationGap=d.endpointAcrossGap;
            Add(result,appeared?Kind::Opened:Kind::Adopted,
                appeared?"observed_appearance":"start_not_observed",active_,point);
        }
        return result;
    }
};
} // namespace private_logistics_assignment
#endif
