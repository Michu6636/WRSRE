// Read-only road-office polling using already observed fleet/quantity/schedule fields.
// No windows, native calls, allocations, pricing or settlement. Polling != burn coverage.
#ifndef PRIVATE_LOGISTICS_FLEET_FUEL_OBSERVER_H
#define PRIVATE_LOGISTICS_FLEET_FUEL_OBSERVER_H
#include "schedule_assignment_ledger.h"
namespace private_logistics_fleet {
namespace p=private_logistics_panel;
namespace sch=private_logistics_schedule;
namespace a=private_logistics_assignment;
using p::Address;
enum class ListStatus { Invalid, NotRoadOffice, Complete };
struct FleetList { Address roots[p::MaxFleet]{}; unsigned count=0; };
inline ListStatus ReadOffice(const p::Memory& m,Address office,FleetList& list) {
    list=FleetList{};
    Address descriptor=0,begin=0,end=0;int type=-1;
    if(!m.Get(office,0x318,descriptor) || !m.Get(descriptor,0x360,type)) return ListStatus::Invalid;
    if(type!=43) return ListStatus::NotRoadOffice;
    if(!m.Get(office,0xC70,begin) || !m.Get(office,0xC78,end) || end<begin ||
       (end-begin)%sizeof(Address) || (!begin && end) ||
       (end-begin)/sizeof(Address)>p::MaxFleet) return ListStatus::Invalid;
    list.count=static_cast<unsigned>((end-begin)/sizeof(Address));
    for(unsigned i=0;i<list.count;++i) {
        if(!m.Get(begin,i*sizeof(Address),list.roots[i]) || !list.roots[i]) return ListStatus::Invalid;
        for(unsigned j=0;j<i;++j) if(list.roots[i]==list.roots[j]) return ListStatus::Invalid;
    }
    Address check=0;
    if(!m.Get(office,0x318,check) || check!=descriptor ||
       !m.Get(office,0xC70,check) || check!=begin || !m.Get(office,0xC78,check) || check!=end)
        return ListStatus::Invalid;
    for(unsigned i=0;i<list.count;++i)
        if(!m.Get(begin,i*sizeof(Address),check) || check!=list.roots[i]) return ListStatus::Invalid;
    return ListStatus::Complete;
}
// Strictly root+links signed native quantities, never UI-positive sums or cargo slots.
inline a::NativeFuel ReadFuel(const p::Memory& m,Address office,Address root) {
    a::NativeFuel f{};
    p::Object member{};member.pointer=root;p::FindMembership(m,member);
    if(member.homeCandidate!=office || member.match!=p::Match::Found || member.occurrences!=1) return f;
    Address begin=0,end=0;
    if(!m.Get(root,0x3A0,begin) || !m.Get(root,0x3A8,end) || end<begin ||
       (end-begin)%sizeof(Address) || (!begin && end) ||
       (end-begin)/sizeof(Address)>=p::MaxObjects) return f;
    f.count=1+static_cast<unsigned>((end-begin)/sizeof(Address));
    for(unsigned i=0;i<f.count;++i) {
        f.objects[i]=root;
        if(i && !m.Get(begin,(i-1)*sizeof(Address),f.objects[i])) return f;
        if(!m.Get(f.objects[i],0x1708,f.metadata[i]) ||
           !m.Get(f.metadata[i],0x8684,f.capacity[i]) || !m.Get(f.objects[i],0x5F0,f.raw[i])) return f;
    }
    int kind=-1;
    if(!m.Get(f.metadata[0],0x294,kind) || kind!=1) return f; // verified road detail kind only
    Address check=0;
    if(!m.Get(root,0x3A0,check) || check!=begin || !m.Get(root,0x3A8,check) || check!=end) return f;
    for(unsigned i=0;i<f.count;++i) {
        if(i && (!m.Get(begin,(i-1)*sizeof(Address),check) || check!=f.objects[i])) return f;
        if(!m.Get(f.objects[i],0x1708,check) || check!=f.metadata[i]) return f;
    }
    p::Object after{};after.pointer=root;p::FindMembership(m,after);
    if(after.homeCandidate!=office || after.match!=p::Match::Found || after.occurrences!=1) return f;
    f.valid=true;f.valid=a::ValidFuel(f);
    return f;
}
// One state per vehicle. Observed decreases and increases are separate; neither
// is actual burn/refill. Never join samples across invalid reads or missing rounds.
struct Vehicle {
    Address root=0,home=0;
    bool seen=false,previousValid=false;
    unsigned sample=0;
    std::uint64_t lastMs=0,previousEpoch=0;
    sch::History history{};
    a::Ledger ledger{};
    a::NativeFuel previous{};
    double observedDecrease=0,observedIncrease=0;
    unsigned discontinuities=0;
    void Break() {
        history.Break();ledger.Break();previousValid=false;++discontinuities;
    }
    a::Result Observe(const sch::Snapshot& schedule,const a::NativeFuel& fuel,
                      std::uint64_t epoch,std::uint64_t ms) {
        if(sample==UINT32_MAX || (sample && ms<=lastMs)) {
            Break();return ledger.Abandon("sample_or_clock_not_monotonic");
        }
        if(previousEpoch && previousEpoch!=epoch) Break();
        previousEpoch=epoch;
        if(!a::ValidFuel(fuel) || fuel.objects[0]!=root) Break();
        if(previousValid && a::SameFuel(previous,fuel)) {
            for(unsigned i=0;i<fuel.count;++i) {
                const double delta=double(fuel.raw[i])-double(previous.raw[i]);
                if(delta<0) observedDecrease-=delta;else observedIncrease+=delta;
            }
        } else if(previousValid) Break();
        const auto old=history.LastValidValues();++sample;
        const auto d=history.Observe(schedule,epoch,root,home,sample,ms);
        const auto changes=d.endpointCompared?sch::ClassifyChanges(old,schedule):sch::FieldChanges{};
        const auto r=ledger.Observe(schedule,d,changes,epoch,root,home,{sample,ms,false,0,fuel});
        previous=fuel;previousValid=a::ValidFuel(fuel) && fuel.objects[0]==root;
        lastMs=ms;return r;
    }
    a::Result Forget(const char* reason) {
        const auto r=ledger.Abandon(reason);
        root=home=0;seen=previousValid=false;sample=0;lastMs=previousEpoch=0;
        history=sch::History{};previous=a::NativeFuel{};
        observedDecrease=observedIncrease=0;discontinuities=0;
        return r; // Ledger serial retained; no slot-level ID reuse.
    }
};
} // namespace private_logistics_fleet
#endif
