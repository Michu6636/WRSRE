// Verified renderer schedule reads ONLY. Not a native job identity/completion API.
#ifndef PRIVATE_LOGISTICS_SCHEDULE_OBSERVER_H
#define PRIVATE_LOGISTICS_SCHEDULE_OBSERVER_H
#include "panel_fuel_observer.h"

namespace private_logistics_schedule {
using private_logistics_panel::Address;
using private_logistics_panel::Memory;
static_assert(sizeof(Address)==8,"verified schedule layout is x64 only");
constexpr unsigned MaxRows = 16;
constexpr Address ConfigStride = 0x198;
enum class Status { Disabled, Ineligible, Unreadable, BadBounds, TooManyRows,
                    MissingConfigs, NonFinite, ChangedDuringRead, GuardedFault, Complete };
struct Row {
    Address target = 0;
    bool configRead = false;
    std::uint8_t flag00 = 0, flagC8 = 0;
    float value04 = 0, valueCC = 0;
};
struct Snapshot {
    Status status = Status::Disabled;
    Address begin = 0, end = 0, configs = 0;
    // Opaque renderer context fields: do NOT name these job IDs or buyers.
    Address raw4a0 = 0, raw610 = 0, raw650 = 0;
    std::int32_t index698 = -1;
    std::uint8_t flag5fd = 0;
    unsigned count = 0;
    Row rows[MaxRows]{};
};
inline bool Header(const Memory& m, Address root, Snapshot& s) {
    return m.Get(root,0x680,s.begin) && m.Get(root,0x688,s.end) &&
        m.Get(root,0x638,s.configs) && m.Get(root,0x698,s.index698) &&
        m.Get(root,0x5FD,s.flag5fd) && m.Get(root,0x4A0,s.raw4a0) &&
        m.Get(root,0x610,s.raw610) && m.Get(root,0x650,s.raw650);
}
inline bool SameHeader(const Snapshot& a,const Snapshot& b) {
    return a.begin==b.begin && a.end==b.end && a.configs==b.configs &&
        a.index698==b.index698 && a.flag5fd==b.flag5fd &&
        a.raw4a0==b.raw4a0 && a.raw610==b.raw610 && a.raw650==b.raw650;
}
inline bool SameRow(const Row& a,const Row& b) {
    return a.target==b.target && a.configRead==b.configRead && a.flag00==b.flag00 && a.flagC8==b.flagC8 &&
        a.value04==b.value04 && a.valueCC==b.valueCC;
}
inline bool ReadRow(const Memory& m,const Snapshot& s,unsigned i,Row& r) {
    if(!m.Get(s.begin,Address(i)*8,r.target)) return false;
    // At 7E81B3..7E81C0 the renderer sets RSI=0 for the last row
    // when +5FD is nonzero. Do not invent/read a configuration for it.
    if(s.flag5fd && i+1==s.count) return true;
    const Address offset=Address(i)*ConfigStride;
    r.configRead=m.Get(s.configs,offset,r.flag00) && m.Get(s.configs,offset+4,r.value04) &&
        m.Get(s.configs,offset+0xC8,r.flagC8) && m.Get(s.configs,offset+0xCC,r.valueCC);
    return r.configRead;
}
inline void Capture(const Memory& m,Address root,Snapshot& s) {
    s=Snapshot{};s.status=Status::Unreadable;
    if(!Header(m,root,s)) return;
    if(s.end<s.begin || (s.end-s.begin)%8 || (!s.begin && s.end)) {
        s.status=Status::BadBounds;return;
    }
    const Address count=(s.end-s.begin)/8;
    if(count>MaxRows) {s.status=Status::TooManyRows;return;}
    s.count=static_cast<unsigned>(count);
    if(s.count>(s.flag5fd?1u:0u) && !s.configs) {s.status=Status::MissingConfigs;return;}
    for(unsigned i=0;i<s.count;++i) {
        if(!ReadRow(m,s,i,s.rows[i])) return;
        if(!std::isfinite(s.rows[i].value04) || !std::isfinite(s.rows[i].valueCC)) {
            s.status=Status::NonFinite;return;
        }
    }
    // Recheck header AND selected row fields; reject observed mutation. This
    // is still not an atomic game snapshot (ABA/other fields remain unknown).
    Snapshot after{};
    if(!Header(m,root,after)) return;
    if(!SameHeader(s,after)) {s.status=Status::ChangedDuringRead;return;}
    for(unsigned i=0;i<s.count;++i) {
        Row again{};
        if(!ReadRow(m,s,i,again)) return;
        if(!SameRow(s.rows[i],again)) {s.status=Status::ChangedDuringRead;return;}
    }
    s.status=Status::Complete;
}
inline const char* StatusName(Status s) {
    switch(s) {
        case Status::Disabled:return "disabled";
        case Status::Ineligible:return "not_unique_road_fleet_root";
        case Status::BadBounds:return "bad_bounds";
        case Status::TooManyRows:return "too_many_rows_no_partial_table";
        case Status::MissingConfigs:return "missing_configs";
        case Status::NonFinite:return "nonfinite_config";
        case Status::ChangedDuringRead:return "changed_during_read";
        case Status::GuardedFault:return "guarded_schedule_fault_disabled";
        case Status::Complete:return "complete_selected_fields";
        default:return "unreadable";
    }
}
enum class Baseline { None, First, Epoch, Identity, Gap, Invalid };
inline const char* BaselineName(Baseline b) {
    switch(b) {
        case Baseline::None:return "none";
        case Baseline::Epoch:return "load_epoch";
        case Baseline::Identity:return "vehicle_or_home_change";
        case Baseline::Gap:return "diagnostic_gap";
        case Baseline::Invalid:return "invalid_read";
        default:return "first_observation";
    }
}
struct Delta {
    bool emit=false, fields=false, storage=false, index=false, context=false;
    Baseline baseline=Baseline::None;
};
// One selected vehicle's previous *values*, never a retained native object
// consumer. Windows are transient and are deliberately not identity keys.
class History {
    bool known_=false, gap_=false;
    std::uint64_t epoch_=0;
    Address root_=0,home_=0;
    Snapshot previous_{};
public:
    void Break() {gap_=true;}
    Delta Observe(const Snapshot& s,std::uint64_t epoch,Address root,Address home) {
        Delta d;
        if(s.status!=Status::Complete) {
            known_=false;gap_=true;d.emit=true;d.baseline=Baseline::Invalid;return d;
        }
        if(!known_) d.baseline=gap_?Baseline::Gap:Baseline::First;
        else if(epoch!=epoch_) d.baseline=Baseline::Epoch;
        else if(root!=root_ || home!=home_) d.baseline=Baseline::Identity;
        else if(gap_) d.baseline=Baseline::Gap;
        if(d.baseline==Baseline::None) {
            d.fields=s.count!=previous_.count;
            if(!d.fields) for(unsigned i=0;i<s.count;++i)
                if(!SameRow(s.rows[i],previous_.rows[i])) {d.fields=true;break;}
            d.storage=s.begin!=previous_.begin || s.end!=previous_.end || s.configs!=previous_.configs;
            d.index=s.index698!=previous_.index698;
            d.context=s.raw4a0!=previous_.raw4a0 || s.raw610!=previous_.raw610 ||
                s.raw650!=previous_.raw650 || s.flag5fd!=previous_.flag5fd;
        }
        d.emit=d.baseline!=Baseline::None || d.fields || d.storage || d.index || d.context;
        epoch_=epoch;root_=root;home_=home;previous_=s;known_=true;gap_=false;
        return d;
    }
};
} // namespace private_logistics_schedule
#endif
