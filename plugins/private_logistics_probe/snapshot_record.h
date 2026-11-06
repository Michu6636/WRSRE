// Shared by the Windows probe and the portable producer/queue/serializer test.
// No Windows/game APIs; queued records own ALL text. Game pointers are identity
// values only and are never dereferenced by the serializer/drain thread.
#pragma once
#include <cstdint>
#include <cstdio>
#include <cstring>

typedef struct {
    unsigned  seq;
    double    ts;
    unsigned long thread;
    int       day, month, year;
    float     tod;

    char record_type[32];   // selection | measurement | no_match | vehicle_lost | world_reset
    int       snapshot_ok;     // 0|1
    char fail_reason[64];   // "" | "no_match" | "vehicle_gone" | "world_reset"

    const void* vehicle;
    const void* do_bld;

    int       fuel_slot_valid;
    double    fuel_tons;
    int       mirror_valid;
    double    mirror_tons;
    int       cargo_valid;
    int       cargo_slots;
    double    cargo_total_tons;
    char cargo_list[128];

    int       kind_valid;
    int       kind;
    int       state_valid;
    int       state;
    int       mode_valid;
    int       mode;
    int       home_matches_valid;
    int       home_matches;
} ProbeSnapshotPayload;

inline void ProbeCopyText(char* dst, std::size_t size, const char* src)
{
    if (size) std::snprintf(dst, size, "%s", src ? src : "");
}

inline void ProbePointerText(char* dst, std::size_t size, const void* p)
{
    if (p) std::snprintf(dst, size, "%p", p);
    else ProbeCopyText(dst, size, "null");
}

// Same serializer used by the DLL and host test. Logger accepts printf args.
template<class Logger>
void ProbeSerializeSnapshot(const ProbeSnapshotPayload& p, unsigned logSeq, Logger log)
{
    char vbuf[32], dbuf[32], tbuf[32];
    ProbePointerText(vbuf, sizeof(vbuf), p.vehicle);
    ProbePointerText(dbuf, sizeof(dbuf), p.do_bld);
    ProbePointerText(tbuf, sizeof(tbuf), reinterpret_cast<const void*>(static_cast<std::uintptr_t>(p.thread)));
    log("[PROBE SNAPSHOT] seq=%u ts=%.4f thr=%s kind=snapshot schema=1 committed=0 load=0 unload=0 complete=%d gap=0 error=0 synth=0", logSeq, p.ts, tbuf, p.snapshot_ok ? 1 : 0);
    log(" observation_seq=%u day=%d mon=%d yr=%d tod=%.4f", p.seq, p.day, p.month, p.year, static_cast<double>(p.tod));
    log(" veh=%s do=%s", vbuf, dbuf);
    log(" record_type=%s snapshot_ok=%d fail_reason=\"%s\"", p.record_type, p.snapshot_ok, p.fail_reason);
    log(" fuel_slot_valid=%d fuel_tons=%.6f", p.fuel_slot_valid, p.fuel_tons);
    log(" mirror_valid=%d mirror_tons=%.6f", p.mirror_valid, p.mirror_tons);
    log(" cargo_valid=%d cargo_slots=%d cargo_total_tons=%.6f cargo=\"%s\"", p.cargo_valid, p.cargo_slots, p.cargo_total_tons, p.cargo_list);
    log(" kind_valid=%d kind=%d", p.kind_valid, p.kind);
    log(" state_valid=%d state=%d", p.state_valid, p.state);
    log(" mode_valid=%d mode=%d", p.mode_valid, p.mode);
    log(" home_matches_valid=%d home_matches=%d", p.home_matches_valid, p.home_matches);
}
