// Selected-panel observation ledger ONLY. No native job IDs, costing or payments.
#ifndef PRIVATE_LOGISTICS_SCHEDULE_ASSIGNMENT_LEDGER_H
#define PRIVATE_LOGISTICS_SCHEDULE_ASSIGNMENT_LEDGER_H
#include "schedule_observer.h"

namespace private_logistics_assignment {
namespace sch = private_logistics_schedule;
using sch::Address;
struct Point {
    unsigned sample=0;
    std::uint64_t ms=0;
    bool fuelValid=false;
    float litres=0;
};
struct Candidate {
    std::uint64_t id=0,epoch=0;
    Address root=0,home=0;
    Point first{},last{};
    bool unknownStart=false,observationGap=false,roleUncertain=false;
};
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
    static void Add(Result& r,Kind kind,const char* reason,const Candidate& c,const Point& p) {
        Point boundary=p;
        // An abandoned interval has no observed end reading for its old context.
        // In particular, never attach another vehicle/load's fuel to it.
        if(kind==Kind::Abandoned) {boundary.fuelValid=false;boundary.litres=0;}
        r.events[r.count++]={kind,reason,c,boundary}; // At most close/abandon + open/adopt.
    }
public:
    explicit Ledger(std::uint64_t initialSerial=0):serial_(initialSerial) {}
    bool Active() const {return active_.id!=0;}
    const Candidate& Current() const {return active_;}
    void Break() {if(Active()) active_.observationGap=true;}
    Result Observe(const sch::Snapshot& s,const sch::Delta& d,const sch::FieldChanges& changes,
                   std::uint64_t epoch,Address root,Address home,Point point) {
        Result result;
        point.fuelValid=point.fuelValid && std::isfinite(point.litres) && point.litres>=0;
        if(!point.fuelValid) point.litres=0; // Explicitly invalid, not observed zero.
        const bool changedIdentity=Active() &&
            (active_.epoch!=epoch || active_.root!=root || active_.home!=home);
        if(changedIdentity) {
            active_.observationGap=true;
            Add(result,Kind::Abandoned,"identity_or_load_changed",active_,point);
            active_=Candidate{};
        }
        if(Active() && (point.sample<=active_.last.sample || point.ms<active_.last.ms)) {
            active_.observationGap=true;
            Add(result,Kind::Uncertain,"out_of_order_not_removal",active_,point);
            return result;
        }
        const auto pattern=sch::ClassifyPattern(s,home);
        if(pattern==sch::Pattern::Unavailable || !root || !home || !epoch || !point.sample) {
            if(Active()) {
                active_.observationGap=true;
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
