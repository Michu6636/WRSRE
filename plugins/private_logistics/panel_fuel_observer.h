// Read-only diagnostic of the verified 7D5460 panel quantity. NOT a burn meter.
// No native calls, allocations, logging, persistence, tracker or money here.
#ifndef PRIVATE_LOGISTICS_PANEL_FUEL_OBSERVER_H
#define PRIVATE_LOGISTICS_PANEL_FUEL_OBSERVER_H
#include <cstddef>
#include <cstdint>
#include <cmath>

namespace private_logistics_panel {
using Address = std::uintptr_t;
using Read = bool (*)(Address, void*, std::size_t, void*);
constexpr unsigned MaxObjects = 9; // root + at most eight links; never partial totals
constexpr unsigned MaxFleet = 256;
struct Memory {
    Read read;
    void* user;
    template<class T> bool Get(Address base, Address offset, T& out) const {
        if (!read || !base || offset > UINTPTR_MAX - base ||
            sizeof(T) > UINTPTR_MAX - (base + offset)) return false;
        return read(base + offset, &out, sizeof(T), user);
    }
};
enum class Match { Unavailable, NotRoadOffice, InvalidFleet, Absent, Found };
enum class Status { Unreadable, TooManyLinks, NonFinite, Overflow, Complete };
struct Object {
    Address pointer = 0, metadata = 0, homeCandidate = 0;
    float raw = 0, capacity = 0;
    bool quantityRead = false;
    Match match = Match::Unavailable;
    unsigned fleetIndex = 0, occurrences = 0;
};
struct Snapshot {
    Address context = 0, window = 0, root = 0;
    Status status = Status::Unreadable;
    unsigned count = 0, links = 0;
    Object objects[MaxObjects]{};
    int contextMode = -1, metadataKind = -1;
    std::uint8_t flag8681 = 0, flag8682 = 0;
    bool flagsRead = false, adjustmentRead = false, adjustmentNeeded = false;
    bool aggregateValid = false, ratioValid = false;
    float current = 0, unadjustedCapacity = 0, litresCandidate = 0, ratio = 0;
};
// This tests ONLY object+4F8 as an office candidate. An absent match is NOT a
// search of all DOs. Type 43 and literal fleet-slot pointer equality are needed.
inline void FindMembership(const Memory& m, Object& o) {
    if (!m.Get(o.pointer, 0x4F8, o.homeCandidate) || !o.homeCandidate) return;
    Address descriptor = 0, begin = 0, end = 0;
    int type = -1;
    if (!m.Get(o.homeCandidate, 0x318, descriptor) || !m.Get(descriptor, 0x360, type)) return;
    if (type != 43) { o.match = Match::NotRoadOffice; return; }
    if (!m.Get(o.homeCandidate, 0xC70, begin) || !m.Get(o.homeCandidate, 0xC78, end)) return;
    if (end < begin || (end - begin) % sizeof(Address) ||
        (end - begin) / sizeof(Address) > MaxFleet || (!begin && end)) {
        o.match = Match::InvalidFleet; return;
    }
    const unsigned count = static_cast<unsigned>((end - begin) / sizeof(Address));
    for (unsigned i = 0; i < count; ++i) {
        Address entry = 0;
        if (!m.Get(begin, i * sizeof(Address), entry)) { o.match = Match::Unavailable; return; }
        if (entry == o.pointer) {
            if (!o.occurrences) o.fleetIndex = i;
            ++o.occurrences;
        }
    }
    o.match = o.occurrences ? Match::Found : Match::Absent;
}
// false = not an eligible visible detail window. true may contain an INVALID
// snapshot: callers must check status/aggregateValid, not interpret defaults as 0.
inline bool Capture(const Memory& m, Address context, Address window, Snapshot& s) {
    s = Snapshot{};
    s.context = context; s.window = window;
    std::uint8_t hidden = 1, live = 0;
    int primary = -1, subkind = -1, origin = -1, submode = -1;
    if (!m.Get(window, 1, hidden) || !m.Get(window, 0x20, live) || hidden || !live ||
        !m.Get(window, 0x34, primary) || !m.Get(window, 0x248, subkind) ||
        !m.Get(window, 0x24C, origin) || !m.Get(window, 0x23C, submode) ||
        primary != 0 || subkind != 1 || origin != 2 || submode != 0) return false;
    if (!m.Get(window, 0x240, s.root) || !s.root) return true;
    Address begin = 0, end = 0;
    if (!m.Get(s.root, 0x3A0, begin) || !m.Get(s.root, 0x3A8, end) ||
        end < begin || (end - begin) % sizeof(Address) || (!begin && end)) return true;
    const Address links = (end - begin) / sizeof(Address);
    if (links >= MaxObjects) { s.status = Status::TooManyLinks; return true; }
    s.links = static_cast<unsigned>(links); s.count = s.links + 1;
    for (unsigned i = 0; i < s.count; ++i) {
        Object& o = s.objects[i];
        o.pointer = s.root;
        if (i && !m.Get(begin, (i - 1) * sizeof(Address), o.pointer)) return true;
        FindMembership(m, o); // independent of quantity-read validity
        if (!m.Get(o.pointer, 0x1708, o.metadata) ||
            !m.Get(o.metadata, 0x8684, o.capacity) || !m.Get(o.pointer, 0x5F0, o.raw)) return true;
        o.quantityRead = true;
        if (!std::isfinite(o.capacity) || !std::isfinite(o.raw)) {
            s.status = Status::NonFinite; return true;
        }
    }
    m.Get(context, 0x5B0, s.contextMode);
    m.Get(s.objects[0].metadata, 0x294, s.metadataKind);
    s.flagsRead = m.Get(s.objects[0].metadata, 0x8681, s.flag8681) &&
                  m.Get(s.objects[0].metadata, 0x8682, s.flag8682);
    Address adjustBegin = 0, adjustEnd = 0;
    s.adjustmentRead = m.Get(s.root, 0x19E0, adjustBegin) &&
                       m.Get(s.root, 0x19E8, adjustEnd) && adjustEnd >= adjustBegin &&
                       (adjustEnd - adjustBegin) % sizeof(Address) == 0 &&
                       (adjustBegin || !adjustEnd);
    s.adjustmentNeeded = s.adjustmentRead && adjustEnd != adjustBegin;
    if (s.objects[0].capacity > 0) {
        for (unsigned i = 0; i < s.count; ++i) {
            const Object& o = s.objects[i];
            if (o.capacity <= 0) continue;
            s.unadjustedCapacity += o.capacity;
            if (o.raw > 0) s.current += o.raw;
        }
    }
    // The actual PE constant is 00 00 7A 44 = 1000.0f. Verify it at install.
    s.litresCandidate = s.current * 1000.0f;
    if (!std::isfinite(s.current) || !std::isfinite(s.unadjustedCapacity) ||
        !std::isfinite(s.litresCandidate)) { s.status = Status::Overflow; return true; }
    s.aggregateValid = true; s.status = Status::Complete;
    // Do not call the untyped native capacity adjustment or guess its result.
    s.ratioValid = s.adjustmentRead && !s.adjustmentNeeded && s.unadjustedCapacity > 0;
    if (s.ratioValid) s.ratio = s.current / s.unadjustedCapacity;
    if (!std::isfinite(s.ratio)) { s.ratioValid = false; }
    // flag8681/8682 can prove an alternate presentation, but both zero cannot
    // prove literal-litres mode without the native +7C58 helper's result.
    return true;
}
} // namespace private_logistics_panel
#endif
