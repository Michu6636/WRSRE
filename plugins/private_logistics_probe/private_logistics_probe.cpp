// private_logistics_probe - enabled-by-default diagnostic probe.
//
// Exploratory read-only snapshot of ONE road DO truck via the sim_clock
// SUB_TICK subscriber (the only supported recurring callback in the loader
// API). Captures: session/sequence, sim day/time-of-day, selected DO and
// vehicle identity, cargo resources/quantities, candidate fuel-slot identity
// and quantity, relevant raw state values, timestamp, and snapshot
// failures/gaps.
//
// LABELLED AS EXPLORATORY TELEMETRY ONLY:
//   - NOT accumulated actual fuel burn
//   - NOT complete trip-boundary verification
//   - NOT trip-cost validation
//   - NO payments, no treasury API, no game-memory writes
//
// Threading model:
//   - OnTick (simulation thread): reads game memory, builds a
//     ProbeSnapshotPayload, pushes it onto a lock-protected queue. NEVER
//     calls Logf or any other blocking/synchronous operation.
//   - Drain thread: pops payloads and calls Logf/emitters off the sim thread.
//   - Enabled modules are pinned for process lifetime. DllMain does nothing.
//     No queue/lock is freed while the worker can run. Restart to remove probe.
//
// Default loading (enabled = 1): consumes sim_clock, registers SUB_TICK.
// Explicit opt-out (enabled = 0): inert, no callback, no game reads.

#define WIN32_LEAN_AND_MEAN
#include "../../src/tesmio_plugin.h"
#include "../../src/game_world.h"
#include "../../src/sim_clock_api.h"

#include <stdint.h>
#include <stdio.h>
#include <windows.h>

// ===========================================================================
// versioned schema
// ===========================================================================

#define PROBE_SCHEMA_VERSION 1u

/* record kinds emitted to the structured log */
#define PROBE_REC_SESSION  1
#define PROBE_REC_EVENT    2
#define PROBE_REC_INTERP   3
#define PROBE_REC_TRIP     4
#define PROBE_REC_DROP     5
#define PROBE_REC_SYNTH    6
#define PROBE_REC_SNAPSHOT 7

/* event flags (bitmask) */
#define PROBE_F_COMMITTED  0x01
#define PROBE_F_LOAD       0x02
#define PROBE_F_UNLOAD     0x04
#define PROBE_F_COMPLETE   0x08   // snapshot_ok = 1 for snapshot records
#define PROBE_F_GAP        0x10
#define PROBE_F_ERROR      0x20
#define PROBE_F_SYNTH      0x40

/* snapshot record type strings */
#define PROBE_SNAP_TYPE_SELECTION   "selection"
#define PROBE_SNAP_TYPE_MEASUREMENT "measurement"
#define PROBE_SNAP_TYPE_NO_MATCH    "no_match"
#define PROBE_SNAP_TYPE_LOST        "vehicle_lost"
#define PROBE_SNAP_TYPE_WORLD_RESET "world_reset"

/* validity flag values */
#define PROBE_VAL_INVALID  0
#define PROBE_VAL_VALID    1

// ===========================================================================
// settings
// ===========================================================================

static int   g_enabled        = 1;   // Enabled by default; INI enabled=0 opts out.
static int   g_tickInterval   = 60;
static int   g_maxSnapshots   = 100;
static int   g_maxQueue       = 256;
static int   g_selftest       = 0;

// ===========================================================================
// session state
// ===========================================================================

static char      g_sessionId[32];
static double    g_sessionStartTs = 0.0;
static unsigned  g_eventSeq       = 0;     // protected by g_seqLock
static CRITICAL_SECTION g_seqLock;
static unsigned  g_eventsLoggedToday = 0; // protected by g_logLock
static unsigned  g_eventsDroppedToday = 0;
static CRITICAL_SECTION g_logLock;

static double NowSeconds(void)
{
    return (double)GetTickCount64() / 1000.0;
}

static void MakeSessionId(char* out, size_t n)
{
    unsigned long long seed = (unsigned long long)GetTickCount64();
    seed ^= ((unsigned long long)GetCurrentProcessId() << 32);
    _snprintf_s(out, n, _TRUNCATE, "%016llX", seed);
}

static unsigned NextSeq(void)
{
    EnterCriticalSection(&g_seqLock);
    unsigned s = ++g_eventSeq;
    LeaveCriticalSection(&g_seqLock);
    return s;
}

static int LoggedTodayInc(void)
{
    EnterCriticalSection(&g_logLock);
    if (g_eventsLoggedToday >= 200u) {
        g_eventsDroppedToday++;
        LeaveCriticalSection(&g_logLock);
        return 0; // dropped
    }
    g_eventsLoggedToday++;
    unsigned d = g_eventsDroppedToday;
    LeaveCriticalSection(&g_logLock);
    return 1;
}

// ===========================================================================
// explicit snapshot payload (one record, all fields separate)
// ===========================================================================

#include "snapshot_record.h"

// ===========================================================================
// bounded raw-event ring (for EVENT/INTERP/TRIP/DROP/SYNTH records)
// ===========================================================================

typedef struct {
    unsigned  seq;
    double    ts;
    unsigned long thread;
    int       day, month, year;
    float     tod;
    char      site[32];
    unsigned  mode;
    const void* vehicle;
    const void* do_bld;
    const void* res;
    double    fuel_before_t;
    double    fuel_after_t;
    double    deduction_t;
    double    cargo_before_t;
    double    cargo_after_t;
    char      confidence[16];
    unsigned  flags;
} ProbeRawEvent;

static ProbeRawEvent* g_ring = nullptr;
static int            g_ringCap = 0;
static int            g_ringHead = 0;
static int            g_ringCount = 0;
static unsigned       g_ringDrops = 0;

static bool RingInit(int cap)
{
    if (cap < 1) cap = 1;
    if (cap > 4096) cap = 4096;
    g_ring = (ProbeRawEvent*)HeapAlloc(GetProcessHeap(), 0,
                                       (SIZE_T)cap * sizeof(ProbeRawEvent));
    if (!g_ring) return false;
    g_ringCap   = cap;
    g_ringHead  = 0;
    g_ringCount = 0;
    g_ringDrops = 0;
    return true;
}

static void RingTerm(void)
{
    if (g_ring) { HeapFree(GetProcessHeap(), 0, g_ring); g_ring = nullptr; }
    g_ringCap = 0; g_ringHead = 0; g_ringCount = 0;
}

static void RingAppend(const ProbeRawEvent* ev)
{
    if (!g_ring) return;
    if (g_ringCount >= g_ringCap) {
        g_ringHead = (g_ringHead + 1) % g_ringCap;
        g_ringDrops++;
    } else {
        g_ringCount++;
    }
    int slot = (g_ringHead + g_ringCount - 1) % g_ringCap;
    g_ring[slot] = *ev;
}

static unsigned RingDrops(void) { return g_ringDrops; }

// ===========================================================================
// bounded queue + drain thread (defer Logf off the simulation thread)
// ===========================================================================

static CRITICAL_SECTION g_queueLock;
static HANDLE           g_drainEvent  = NULL;
static HANDLE           g_drainStop   = NULL;
static HANDLE           g_drainThread = NULL;
static DWORD            g_drainThreadId = 0;
static volatile long    g_shutdown = 0;

static ProbeSnapshotPayload* g_queue = NULL;
static int            g_queueCap = 0;
static int            g_queueHead = 0;
static int            g_queueCount = 0;
static volatile LONG  g_queueDrops = 0;

static bool QueueInit(int cap)
{
    if (cap < 1) cap = 1;
    if (cap > 4096) cap = 4096;
    g_queue = (ProbeSnapshotPayload*)HeapAlloc(GetProcessHeap(), 0,
                                               (SIZE_T)cap * sizeof(ProbeSnapshotPayload));
    if (!g_queue) return false;
    g_queueCap   = cap;
    g_queueHead  = 0;
    g_queueCount = 0;
    g_queueDrops = 0;
    return true;
}

static void QueueTerm(void)
{
    if (g_queue) { HeapFree(GetProcessHeap(), 0, g_queue); g_queue = NULL; }
    g_queueCap = 0; g_queueHead = 0; g_queueCount = 0;
}

static bool QueuePush(const ProbeSnapshotPayload* ev)
{
    if (!g_queue) return false;
    if (!TryEnterCriticalSection(&g_queueLock)) {
        InterlockedIncrement(&g_queueDrops);
        return false;
    }
    if (g_queueCount >= g_queueCap) {
        InterlockedIncrement(&g_queueDrops);
        LeaveCriticalSection(&g_queueLock);
        return false;
    }
    int slot = (g_queueHead + g_queueCount) % g_queueCap;
    g_queue[slot] = *ev;
    g_queueCount++;
    LeaveCriticalSection(&g_queueLock);
    SetEvent(g_drainEvent);
    return true;
}

static int QueuePop(ProbeSnapshotPayload* out, int max)
{
    if (!g_queue) return 0;
    EnterCriticalSection(&g_queueLock);
    int count = g_queueCount;
    if (count > max) count = max;
    for (int i = 0; i < count; i++) {
        out[i] = g_queue[(g_queueHead + i) % g_queueCap];
    }
    g_queueHead = (g_queueHead + count) % g_queueCap;
    g_queueCount -= count;
    LeaveCriticalSection(&g_queueLock);
    return count;
}

static unsigned QueueDrops(void)
{
    return static_cast<unsigned>(InterlockedCompareExchange(&g_queueDrops, 0, 0));
}

// Forward declaration.
static void EmitSnapshotRecord(const ProbeSnapshotPayload* p);
static void LogDrop(const char* reason, unsigned queueDrops);

static DWORD WINAPI DrainThreadProc(LPVOID param)
{
    (void)param;
    ProbeSnapshotPayload batch[32];
    unsigned reportedDrops = 0;

    while (1)
    {
        HANDLE waits[2] = { g_drainStop, g_drainEvent };
        DWORD rc = WaitForMultipleObjects(2, waits, FALSE, 250);

        // Drain everything currently queued (handles both stop and data).
        int n;
        do {
            n = QueuePop(batch, 32);
            for (int i = 0; i < n; i++) {
                EmitSnapshotRecord(&batch[i]);
            }
        } while (n > 0);

        unsigned drops = QueueDrops();
        if (drops != reportedDrops) {
            LogDrop("snapshot_queue_loss", drops);
            reportedDrops = drops;
        }
        if (rc == WAIT_FAILED) return 1;
        if (rc == WAIT_OBJECT_0) {
            // Stop requested: after draining remaining items, exit.
            break;
        }
    }
    return 0;
}

// ===========================================================================
// diagnostic accumulator (fuel-only, no payment APIs)
// ===========================================================================

typedef struct {
    const void* vehicle;
    int         state;
    unsigned    seq;
    unsigned    load_seq;
    unsigned    unload_seq;
    double      fuel_before;
    double      fuel_after;
    double      deduction;
    double      cargo_before;
    double      cargo_after;
    const void* resource;
    const void* load_site;
    const void* unload_site;
    int         load_day;
    int         unload_day;
    int         confirmed;
    int         complete_emitted;
} ProbeTrip;

static ProbeTrip  g_trips[16];
static int        g_tripCount = 0;
static unsigned   g_tripsCompleted = 0;
static unsigned   g_tripsReplayed = 0;
static unsigned   g_tripsUnsupported = 0;
static unsigned   g_tripsDupLoad = 0;

static int TripIndexFor(const void* vehicle)
{
    for (int i = 0; i < g_tripCount; i++)
        if (g_trips[i].vehicle == vehicle) return i;
    return -1;
}

static int TripAlloc(const void* vehicle)
{
    if (g_tripCount >= 16) return -1;
    int i = g_tripCount++;
    memset(&g_trips[i], 0, sizeof(g_trips[i]));
    g_trips[i].vehicle = vehicle;
    g_trips[i].state   = 0;
    return i;
}

static void ProbeBeginLoad(const void* vehicle, const void* resource,
                           const void* site, double fuel, double cargo,
                           int day, unsigned seq)
{
    int i = TripIndexFor(vehicle);
    if (i < 0) { i = TripAlloc(vehicle); if (i < 0) return; }
    ProbeTrip* t = &g_trips[i];
    if (t->state == 1) g_tripsDupLoad++;
    t->state        = 1;
    t->seq          = seq;
    t->load_seq     = seq;
    t->unload_seq   = 0;
    t->fuel_before  = fuel;
    t->fuel_after   = 0.0;
    t->deduction    = 0.0;
    t->cargo_before = cargo;
    t->cargo_after  = 0.0;
    t->resource     = resource;
    t->load_site    = site;
    t->unload_site  = nullptr;
    t->load_day     = day;
    t->unload_day   = -1;
    t->confirmed    = 0;
    t->complete_emitted = 0;
}

static void ProbeCompleteUnload(const void* vehicle, const void* site,
                                double fuel, double cargo, int day, unsigned seq)
{
    int i = TripIndexFor(vehicle);
    if (i < 0) { i = TripAlloc(vehicle); if (i < 0) return; }
    ProbeTrip* t = &g_trips[i];

    if (t->state != 1) {
        t->state = 3;
        g_tripsUnsupported++;
        return;
    }

    t->state        = 2;
    t->unload_seq   = seq;
    t->fuel_after   = fuel;
    t->cargo_after  = cargo;
    t->unload_site  = site;
    t->unload_day   = day;
    t->deduction    = (t->fuel_before >= 0.0 && fuel >= 0.0)
                      ? (t->fuel_before - fuel) : -1.0;
    if (t->deduction < 0.0) {
        t->state = 3;
        g_tripsUnsupported++;
    } else {
        g_tripsCompleted++;
        t->complete_emitted = 1;
    }
}

static void ProbeConfirm(unsigned seq) { (void)seq; }

static void ProbeReset(void)
{
    for (int i = 0; i < g_tripCount; i++) {
        g_trips[i].state = 0;
        g_trips[i].vehicle = nullptr;
        g_trips[i].complete_emitted = 0;
    }
    g_tripCount = 0;
    g_tripsCompleted = 0;
    g_tripsReplayed = 0;
    g_tripsUnsupported = 0;
    g_tripsDupLoad = 0;
}

// ===========================================================================
// exploratory snapshot state (ONE road DO truck, rate-limited)
// ===========================================================================

static const TsmSimClockApi* g_simClock = nullptr;
static int                   g_tickHandle = 0;
static unsigned              g_tickCounter = 0;
static unsigned              g_snapshotCount = 0;
static unsigned              g_captureSeq = 0;
static int                   g_prevDay = -1;

static const void* g_selectedVehicle = nullptr;
static const void* g_selectedDO      = nullptr;
static int         g_selectionResolved = 0;
static int         g_selectionValid   = 0;

// Vehicle-read offsets (from Ghidra machine evidence + private_logistics.cpp v0.13).
#define VEH_KIND        0x50C
#define VEH_STATE       0x448
#define VEH_MODE        0x440
#define VEH_HOME_DEPOT  0x4F8
#define VEH_CARGO_VEC   0x3B8
#define VEH_CARGO_END   0x3C0
#define VEH_FUEL_MIRROR 0x5F0
#define BLD_FLEET_BEG   0xC70
#define BLD_FLEET_END   0xC78
#define GAME_OBJ_RVA    0x9D4F10
#define BLD_TYPE_43     43

static bool VehReadInt(const void* v, size_t off, int* out)
{
    if (!ReadablePtr((const BYTE*)v + off, 4)) return false;
    *out = *(const int*)((const BYTE*)v + off);
    return true;
}

static bool VehReadPtr(const void* v, size_t off, void** out)
{
    if (!ReadablePtr((const BYTE*)v + off, 8)) return false;
    *out = *(void* const*)((const BYTE*)v + off);
    return true;
}

static double VehFuelSlotTons(const void* v, int* found)
{
    *found = PROBE_VAL_INVALID;
    const BYTE* p = (const BYTE*)v;
    if (!ReadablePtr(p + VEH_CARGO_VEC, 16)) return -1.0;

    const BYTE* beg = *(const BYTE* const*)(p + VEH_CARGO_VEC);
    const BYTE* end = *(const BYTE* const*)(p + VEH_CARGO_END);
    if (!beg || !end || end < beg) return -1.0;

    const size_t n = (size_t)(end - beg) / 16;
    if (n > 64 || n == 0) return -1.0;
    if (!ReadablePtr(beg, n * 16)) return -1.0;

    const BYTE* game = g_exeBase + GAME_OBJ_RVA;
    if (!ReadablePtr(game + 0xC3F0, 8)) return -1.0;
    void* fuelRes = *(void* const*)(game + 0xC3F0);
    if (!fuelRes || !TsmIsValidGamePtr(fuelRes)) return -1.0;

    for (size_t i = 0; i < n; i++) {
        const BYTE* e = beg + i * 16;
        if (*(const void* const*)e != fuelRes) continue;
        float t = *(const float*)(e + 8);
        if (!(t == t) || t < -1000.0f || t > 1000.0f) return -1.0;
        if (t < 0.0f) return (double)t; // preserve invalid raw value; validity stays false
        *found = PROBE_VAL_VALID;
        return (double)t;
    }
    return -1.0;
}

static double VehCargoTons(const void* v, int* slots)
{
    *slots = -1;
    const BYTE* p = (const BYTE*)v;
    if (!ReadablePtr(p + VEH_CARGO_VEC, 16)) return -1.0;

    const BYTE* beg = *(const BYTE* const*)(p + VEH_CARGO_VEC);
    const BYTE* end = *(const BYTE* const*)(p + VEH_CARGO_END);
    if (!beg || !end || end <= beg) return -1.0;

    const size_t n = (size_t)(end - beg) / 16;
    if (n == 0 || n > 64) return -1.0;
    if (!ReadablePtr(beg, n * 16)) return -1.0;

    double t = 0.0;
    int cnt = 0;
    for (size_t i = 0; i < n; i++) {
        const float tons = *(const float*)(beg + i * 16 + 8);
        if (tons == tons && tons > 0.0f && tons < 1.0e6f) {
            t += (double)tons;
            cnt++;
        }
    }
    *slots = cnt;
    return t;
}

static void VehCargoList(const void* v, char* out, size_t n)
{
    out[0] = 0;
    const BYTE* p = (const BYTE*)v;
    if (!ReadablePtr(p + VEH_CARGO_VEC, 16)) return;

    const BYTE* beg = *(const BYTE* const*)(p + VEH_CARGO_VEC);
    const BYTE* end = *(const BYTE* const*)(p + VEH_CARGO_END);
    if (!beg || !end || end <= beg) return;

    const size_t cnt = (size_t)(end - beg) / 16;
    if (cnt == 0 || cnt > 64) return;
    if (!ReadablePtr(beg, cnt * 16)) return;

    size_t used = 0;
    for (size_t i = 0; i < cnt && used < n - 1; i++) {
        const BYTE* e = beg + i * 16;
        const float tons = *(const float*)(e + 8);
        if (!(tons == tons) || tons == 0.0f) continue;
        const void* res = *(const void* const*)e;
        char rname[16];
        rname[0] = 0;
        if (res && TsmResourceName(res, rname, sizeof(rname))) {}
        else _snprintf_s(rname, sizeof(rname), _TRUNCATE, "?");

        int written = _snprintf_s(out + used, n - used, _TRUNCATE,
                                  "%s=%.3ft%s", rname, (double)tons,
                                  (i + 1 < cnt) ? " " : "");
        if (written < 0) break;
        used += (size_t)written;
        if (used >= n - 1) break;
    }
}

static bool VehIsCandidate(const void* v, const void* doBld)
{
    void* home = nullptr;
    if (!VehReadPtr(v, VEH_HOME_DEPOT, &home)) return false;
    return (home == (void*)doBld);
}

static bool BldIsRoadDO(const void* bld)
{
    return TsmBuildingType(bld) == BLD_TYPE_43;
}

// Returns 1 if exactly one candidate selected, 0 if none or ambiguous.
// On ambiguous, logs nothing from OnTick context (no Logf here).
static int ResolveSelection(BYTE* game, const void** outDO, const void** outVeh)
{
    if (!ReadablePtr(game + TSM_GO_BLD_VEC_BEGIN, 16)) return 0;
    void** bldBegin = *(void***)(game + TSM_GO_BLD_VEC_BEGIN);
    void** bldEnd   = *(void***)(game + TSM_GO_BLD_VEC_END);
    if (!bldBegin || !bldEnd || bldEnd <= bldBegin) return 0;

    size_t bldCount = (size_t)(bldEnd - bldBegin);
    if (bldCount > 25000) return 0; // incomplete scan cannot prove uniqueness
    if (!ReadablePtr(bldBegin, bldCount * sizeof(void*))) return 0;

    const void* chosenDO = nullptr;
    const void* chosenVeh = nullptr;
    int candidateCount = 0;

    for (size_t i = 0; i < bldCount; i++) {
        const void* bld = bldBegin[i];
        if (!bld || !TsmIsValidGamePtr(bld)) continue;
        if (!BldIsRoadDO(bld)) continue;

        if (!ReadablePtr((const BYTE*)bld + BLD_FLEET_BEG, 16)) continue;
        void** fltBeg = *(void***)((const BYTE*)bld + BLD_FLEET_BEG);
        void** fltEnd = *(void***)((const BYTE*)bld + BLD_FLEET_END);
        if (!fltBeg || !fltEnd || fltEnd <= fltBeg) continue;

        size_t fltCount = (size_t)(fltEnd - fltBeg);
        if (fltCount > 64) return 0; // fail closed, never silently truncate candidates
        if (!ReadablePtr(fltBeg, fltCount * sizeof(void*))) continue;

        for (size_t j = 0; j < fltCount; j++) {
            const void* v = fltBeg[j];
            if (!v || !TsmIsValidGamePtr(v)) continue;
            if (!VehIsCandidate(v, bld)) continue;

            candidateCount++;
            if (candidateCount == 1) {
                chosenDO = bld;
                chosenVeh = v;
            }
        }
    }

    if (candidateCount == 1) {
        *outDO = chosenDO;
        *outVeh = chosenVeh;
        return 1;
    }
    return 0; // 0 or ambiguous: no selection
}

static int VerifySelection(void)
{
    if (!g_selectionResolved || !g_selectedVehicle) return 0;
    if (!TsmIsValidGamePtr(g_selectedVehicle)) return 0;
    if (!VehIsCandidate(g_selectedVehicle, g_selectedDO)) return 0;
    return 1;
}

// ===========================================================================
// structured log emission (called ONLY from drain thread or init)
// ===========================================================================

static const char* ConfName(int kind)
{
    switch (kind) {
        case PROBE_REC_SESSION: return "session";
        case PROBE_REC_EVENT:   return "event";
        case PROBE_REC_INTERP:  return "interp";
        case PROBE_REC_TRIP:    return "trip";
        case PROBE_REC_DROP:    return "drop";
        case PROBE_REC_SYNTH:   return "synth";
        case PROBE_REC_SNAPSHOT:return "snapshot";
        default:                return "?";
    }
}

static void PtrStr(char* out, size_t n, const void* p)
{
    if (!p) { _snprintf_s(out, n, _TRUNCATE, "null"); return; }
    _snprintf_s(out, n, _TRUNCATE, "%p", p);
}

static int DayNow(void)
{
    if (g_simClock && g_simClock->getDay) return g_simClock->getDay();
    return -1;
}

static int MonthNow(void)
{
    if (g_simClock && g_simClock->getMonth) return g_simClock->getMonth();
    return -1;
}

static int YearNow(void)
{
    if (g_simClock && g_simClock->getYear) return g_simClock->getYear();
    return -1;
}

static float TodNow(void)
{
    if (g_simClock && g_simClock->getTimeOfDay) return g_simClock->getTimeOfDay();
    return 0.0f;
}

static void LogHeader(const char* recKind, unsigned seq, double ts,
                      unsigned long thread, unsigned flags, int synth)
{
    char pbuf[32];
    PtrStr(pbuf, sizeof(pbuf), (const void*)(uintptr_t)thread);
    Logf("[PROBE %s] seq=%u ts=%.4f thr=%s kind=%s schema=%u "
         "committed=%d load=%d unload=%d complete=%d gap=%d error=%d synth=%d",
         recKind, seq, ts, pbuf, ConfName((int)seq % 6 + 1), PROBE_SCHEMA_VERSION,
         !!(flags & PROBE_F_COMMITTED), !!(flags & PROBE_F_LOAD),
         !!(flags & PROBE_F_UNLOAD), !!(flags & PROBE_F_COMPLETE),
         !!(flags & PROBE_F_GAP), !!(flags & PROBE_F_ERROR), synth);
}

static void LogSession(const char* event, const char* note)
{
    if (!LoggedTodayInc()) return;
    unsigned seq = NextSeq();
    double ts  = NowSeconds() - g_sessionStartTs;
    unsigned long thr = GetCurrentThreadId();
    LogHeader("SESSION", seq, ts, thr, 0, 0);
    int d = DayNow(), m = MonthNow(), y = YearNow();
    Logf(" session_id=%s event=%s day=%d mon=%d yr=%d note=\"%s\"",
         g_sessionId, event, d, m, y, note);
}

static void LogDrop(const char* reason, unsigned queueDrops)
{
    if (!LoggedTodayInc()) return;
    unsigned seq = NextSeq();
    double ts = NowSeconds() - g_sessionStartTs;
    unsigned long thr = GetCurrentThreadId();
    LogHeader("DROP", seq, ts, thr, PROBE_F_ERROR, 0);
    Logf(" reason=\"%s\" ring_drops=%u queue_drops=%u today_drops=%u",
         reason, g_ringDrops, queueDrops, g_eventsDroppedToday);
}

// Emit a SNAPSHOT record. Called ONLY from the drain thread.
static void EmitSnapshotRecord(const ProbeSnapshotPayload* p)
{
    // Only this worker emits snapshots. Never dereference game pointers here.
    ProbeSerializeSnapshot(*p, NextSeq(), [](const char* fmt, auto... args) {
        Logf(fmt, args...);
    });
}

static unsigned LogRawEvent(const char* site, unsigned mode,
                            const void* vehicle, const void* do_bld,
                            const void* res, const void* load_site,
                            const void* unload_site,
                            double fuel_before, double fuel_after,
                            double deduction, double cargo_before,
                            double cargo_after, const char* confidence,
                            unsigned flags);

static void LogInterp(const char* what, const void* vehicle,
                      double fuel_before, double fuel_after,
                      double deduction, unsigned flags);

static void LogTripSummary(const void* vehicle, double fuel_before,
                           double fuel_after, double deduction, int trips);

// ===========================================================================
// tick callback (sim_clock SUB_TICK) - NEVER calls Logf
// ===========================================================================

static void OnTick(void* userData, float dt, float dayDelta)
{
    (void)userData; (void)dt; (void)dayDelta;

    if (!g_enabled || !g_simClock || !g_simClock->getDay) return;

    g_tickCounter++;
    if (g_tickCounter % (unsigned)g_tickInterval != 0) return;
    if (g_snapshotCount >= (unsigned)g_maxSnapshots) return;

    int day = DayNow();
    int mon = MonthNow();
    int yr  = YearNow();
    float tod = TodNow();

    // World load/unload detection.
    if (day == -1) {
        if (g_selectionResolved) {
            g_selectionResolved = 0;
            g_selectionValid = 0;
            g_selectedVehicle = nullptr;
            g_selectedDO = nullptr;

            ProbeSnapshotPayload p;
            memset(&p, 0, sizeof(p));
            p.seq        = ++g_captureSeq;
            p.ts         = NowSeconds() - g_sessionStartTs;
            p.thread     = GetCurrentThreadId();
            p.day        = -1; p.month = -1; p.year = -1; p.tod = 0.0f;
            ProbeCopyText(p.record_type, sizeof(p.record_type), PROBE_SNAP_TYPE_WORLD_RESET);
            p.snapshot_ok   = 0;
            ProbeCopyText(p.fail_reason, sizeof(p.fail_reason), "world_reset");
            p.vehicle       = nullptr;
            p.do_bld        = nullptr;
            p.fuel_slot_valid = 0; p.fuel_tons = -1.0;
            p.mirror_valid    = 0; p.mirror_tons = -1.0;
            p.cargo_valid     = 0; p.cargo_slots = -1; p.cargo_total_tons = -1.0; ProbeCopyText(p.cargo_list, sizeof(p.cargo_list), "");
            p.kind_valid      = 0; p.kind = -1;
            p.state_valid     = 0; p.state = -1;
            p.mode_valid      = 0; p.mode = -1;
            p.home_matches_valid = 0; p.home_matches = -1;
            QueuePush(&p);
            g_snapshotCount++;
        }
        g_prevDay = -1;
        return;
    }

    if (g_prevDay == -1 && day != -1) {
        g_selectionResolved = 0;
        g_selectionValid = 0;
        g_selectedVehicle = nullptr;
        g_selectedDO = nullptr;
        g_prevDay = day;
        // Session event emitted by drain path; queue a marker.
        ProbeSnapshotPayload p;
        memset(&p, 0, sizeof(p));
        p.seq        = ++g_captureSeq;
        p.ts         = NowSeconds() - g_sessionStartTs;
        p.thread     = GetCurrentThreadId();
        p.day        = day; p.month = mon; p.year = yr; p.tod = tod;
        ProbeCopyText(p.record_type, sizeof(p.record_type), PROBE_SNAP_TYPE_WORLD_RESET);
        p.snapshot_ok   = 0;
        ProbeCopyText(p.fail_reason, sizeof(p.fail_reason), "world_loaded");
        p.vehicle       = nullptr;
        p.do_bld        = nullptr;
        p.fuel_slot_valid = 0; p.fuel_tons = -1.0;
        p.mirror_valid    = 0; p.mirror_tons = -1.0;
        p.cargo_valid     = 0; p.cargo_slots = -1; p.cargo_total_tons = -1.0; ProbeCopyText(p.cargo_list, sizeof(p.cargo_list), "");
        p.kind_valid      = 0; p.kind = -1;
        p.state_valid     = 0; p.state = -1;
        p.mode_valid      = 0; p.mode = -1;
        p.home_matches_valid = 0; p.home_matches = -1;
        QueuePush(&p);
        g_snapshotCount++;
        return;
    }
    g_prevDay = day;

    BYTE* game = g_exeBase + GAME_OBJ_RVA;
    if (!game || !TsmIsValidGamePtr(game)) return;

    if (!g_selectionResolved) {
        const void* selDO = nullptr;
        const void* selVeh = nullptr;
        int selected = ResolveSelection(game, &selDO, &selVeh);
        g_selectionResolved = 1;
        g_selectedDO   = selDO;
        g_selectedVehicle = selVeh;
        g_selectionValid   = selected;

        ProbeSnapshotPayload p;
        memset(&p, 0, sizeof(p));
        p.seq        = ++g_captureSeq;
        p.ts         = NowSeconds() - g_sessionStartTs;
        p.thread     = GetCurrentThreadId();
        p.day        = day; p.month = mon; p.year = yr; p.tod = tod;
        if (selected) {
            ProbeCopyText(p.record_type, sizeof(p.record_type), PROBE_SNAP_TYPE_SELECTION);
            p.snapshot_ok   = 1;
            ProbeCopyText(p.fail_reason, sizeof(p.fail_reason), "");
            p.vehicle       = selVeh;
            p.do_bld        = selDO;
            p.fuel_slot_valid = 0; p.fuel_tons = -1.0;
            p.mirror_valid    = 0; p.mirror_tons = -1.0;
            p.cargo_valid     = 0; p.cargo_slots = -1; p.cargo_total_tons = -1.0; ProbeCopyText(p.cargo_list, sizeof(p.cargo_list), "");
            p.kind_valid      = 0; p.kind = -1;
            p.state_valid     = 0; p.state = -1;
            p.mode_valid      = 0; p.mode = -1;
            p.home_matches_valid = 1; p.home_matches = 1;
        } else {
            ProbeCopyText(p.record_type, sizeof(p.record_type), PROBE_SNAP_TYPE_NO_MATCH);
            p.snapshot_ok   = 0;
            ProbeCopyText(p.fail_reason, sizeof(p.fail_reason), "no_match");
            p.vehicle       = nullptr;
            p.do_bld        = nullptr;
            p.fuel_slot_valid = 0; p.fuel_tons = -1.0;
            p.mirror_valid    = 0; p.mirror_tons = -1.0;
            p.cargo_valid     = 0; p.cargo_slots = -1; p.cargo_total_tons = -1.0; ProbeCopyText(p.cargo_list, sizeof(p.cargo_list), "");
            p.kind_valid      = 0; p.kind = -1;
            p.state_valid     = 0; p.state = -1;
            p.mode_valid      = 0; p.mode = -1;
            p.home_matches_valid = 0; p.home_matches = -1;
        }
        QueuePush(&p);
        g_snapshotCount++;
        return;
    }

    if (!VerifySelection()) {
        ProbeSnapshotPayload p;
        memset(&p, 0, sizeof(p));
        p.seq        = ++g_captureSeq;
        p.ts         = NowSeconds() - g_sessionStartTs;
        p.thread     = GetCurrentThreadId();
        p.day        = day; p.month = mon; p.year = yr; p.tod = tod;
        ProbeCopyText(p.record_type, sizeof(p.record_type), PROBE_SNAP_TYPE_LOST);
        p.snapshot_ok   = 0;
        ProbeCopyText(p.fail_reason, sizeof(p.fail_reason), "vehicle_gone");
        p.vehicle       = g_selectedVehicle;
        p.do_bld        = g_selectedDO;
        p.fuel_slot_valid = 0; p.fuel_tons = -1.0;
        p.mirror_valid    = 0; p.mirror_tons = -1.0;
        p.cargo_valid     = 0; p.cargo_slots = -1; p.cargo_total_tons = -1.0; ProbeCopyText(p.cargo_list, sizeof(p.cargo_list), "");
        p.kind_valid      = 0; p.kind = -1;
        p.state_valid     = 0; p.state = -1;
        p.mode_valid      = 0; p.mode = -1;
        p.home_matches_valid = 0; p.home_matches = -1;
        QueuePush(&p);
        g_snapshotCount++;
        g_selectionResolved = 0;
        g_selectionValid   = 0;
        g_selectedVehicle = nullptr;
        g_selectedDO = nullptr;
        return;
    }

    // Capture measurement snapshot.
    const void* v = g_selectedVehicle;
    const void* d = g_selectedDO;

    int kind = -1, state = -1, mode = -1;
    int kindValid = VehReadInt(v, VEH_KIND, &kind) ? PROBE_VAL_VALID : PROBE_VAL_INVALID;
    int stateValid = VehReadInt(v, VEH_STATE, &state) ? PROBE_VAL_VALID : PROBE_VAL_INVALID;
    int modeValid = VehReadInt(v, VEH_MODE, &mode) ? PROBE_VAL_VALID : PROBE_VAL_INVALID;

    void* home = nullptr;
    int homeValid = VehReadPtr(v, VEH_HOME_DEPOT, &home) ? PROBE_VAL_VALID : PROBE_VAL_INVALID;
    int homeMatches = (homeValid && home == (void*)d) ? PROBE_VAL_VALID : PROBE_VAL_INVALID;

    int fuelFound = PROBE_VAL_INVALID;
    double fuelTons = VehFuelSlotTons(v, &fuelFound);
    double mirrorTons = -1.0;
    int mirrorValid = PROBE_VAL_INVALID;
    {
        const BYTE* pv = (const BYTE*)v;
        if (ReadablePtr(pv + VEH_FUEL_MIRROR, 4)) {
            float f = *(const float*)(pv + VEH_FUEL_MIRROR);
            if (f == f && f >= -1000.0f && f <= 1000.0f) {
                mirrorTons = (double)f;
                mirrorValid = PROBE_VAL_VALID;
            }
        }
    }

    int cargoSlots = -1;
    double cargoTotal = VehCargoTons(v, &cargoSlots);
    int cargoValid = (cargoSlots >= 0) ? PROBE_VAL_VALID : PROBE_VAL_INVALID;
    char cargoList[128];
    cargoList[0] = 0;
    if (cargoValid) VehCargoList(v, cargoList, sizeof(cargoList));

    ProbeSnapshotPayload p;
    memset(&p, 0, sizeof(p));
    p.seq        = ++g_captureSeq;
    p.ts         = NowSeconds() - g_sessionStartTs;
    p.thread     = GetCurrentThreadId();
    p.day        = day; p.month = mon; p.year = yr; p.tod = tod;
    ProbeCopyText(p.record_type, sizeof(p.record_type), PROBE_SNAP_TYPE_MEASUREMENT);
    p.snapshot_ok   = 1;
    ProbeCopyText(p.fail_reason, sizeof(p.fail_reason), "");
    p.vehicle       = v;
    p.do_bld        = d;
    p.fuel_slot_valid = fuelFound; p.fuel_tons = fuelTons;
    p.mirror_valid    = mirrorValid; p.mirror_tons = mirrorTons;
    p.cargo_valid     = cargoValid; p.cargo_slots = cargoSlots; p.cargo_total_tons = cargoTotal; ProbeCopyText(p.cargo_list, sizeof(p.cargo_list), cargoList);
    p.kind_valid      = kindValid; p.kind = kind;
    p.state_valid     = stateValid; p.state = state;
    p.mode_valid      = modeValid; p.mode = mode;
    p.home_matches_valid = homeValid; p.home_matches = homeValid ? homeMatches : -1;
    QueuePush(&p);
    g_snapshotCount++;
}

// ===========================================================================
// synthetic accumulator exercise (labelled synth=1, never a game observation)
// ===========================================================================

static void ProbeSelftest(void)
{
    if (!g_selftest) return;
    Logf("private_logistics_probe  selftest: labelled synthetic accumulator exercise begins");

    const void* v = (const void*)(uintptr_t)0xAAAA0001u;
    const void* r = (const void*)(uintptr_t)0xBBBB0002u;
    const void* s1 = (const void*)(uintptr_t)0xCCCC0001u;
    const void* s2 = (const void*)(uintptr_t)0xCCCC0002u;

    static int s_day = 1;

    ProbeBeginLoad(v, r, s1, 0.4500, 0.000, s_day, 101);
    LogRawEvent("synthetic:load", 0, v, nullptr, r, s1, nullptr,
                0.4500, -1.0, -1.0, 0.0, 0.0, "synthetic", PROBE_F_LOAD | PROBE_F_SYNTH);
    LogInterp("trip_open_load", v, 0.4500, -1.0, -1.0, PROBE_F_SYNTH);

    ProbeCompleteUnload(v, s2, 0.4280, 0.000, s_day, 102);
    LogRawEvent("synthetic:unload", 1, v, nullptr, r, nullptr, s2,
                0.4500, 0.4280, 0.0220, 0.0, 0.0, "synthetic",
                PROBE_F_COMMITTED | PROBE_F_UNLOAD | PROBE_F_SYNTH);
    LogInterp("trip_complete", v, 0.4500, 0.4280, 0.0220, PROBE_F_COMPLETE | PROBE_F_SYNTH);
    s_day++;

    ProbeBeginLoad(v, r, s1, 0.4280, 0.000, s_day, 201);
    LogRawEvent("synthetic:load", 0, v, nullptr, r, s1, nullptr,
                0.4280, -1.0, -1.0, 0.0, 0.0, "synthetic", PROBE_F_LOAD | PROBE_F_SYNTH);

    ProbeCompleteUnload(v, s2, 0.4060, 0.000, s_day, 202);
    LogRawEvent("synthetic:unload", 1, v, nullptr, r, nullptr, s2,
                0.4280, 0.4060, 0.0220, 0.0, 0.0, "synthetic",
                PROBE_F_COMMITTED | PROBE_F_UNLOAD | PROBE_F_SYNTH);
    LogInterp("trip_complete_consecutive", v, 0.4280, 0.4060, 0.0220, PROBE_F_COMPLETE | PROBE_F_SYNTH);
    s_day++;

    {
        unsigned before = g_tripsCompleted;
        ProbeCompleteUnload(v, s2, 0.4060, 0.000, s_day, 302);
        LogRawEvent("synthetic:replay_guard", 1, v, nullptr, r, nullptr, s2,
                    0.4060, 0.4060, 0.0, 0.0, 0.0, "synthetic",
                    PROBE_F_UNLOAD | PROBE_F_SYNTH);
        LogInterp("replay_attempted", v, 0.4060, 0.4060, 0.0,
                  PROBE_F_ERROR | PROBE_F_SYNTH);
        if (g_tripsCompleted == before)
            Logf("private_logistics_probe  selftest: replay guard OK (completed=%u)", before);
        else
            Logf("private_logistics_probe  selftest: replay guard FAILED");
    }

    {
        const void* v2 = (const void*)(uintptr_t)0xAAAA0002u;
        ProbeCompleteUnload(v2, s2, 0.4000, 0.000, s_day, 402);
        LogRawEvent("synthetic:unload_no_load", 1, v2, nullptr, r, nullptr, s2,
                    -1.0, 0.4000, -1.0, 0.0, 0.0, "unsupported",
                    PROBE_F_UNLOAD | PROBE_F_SYNTH | PROBE_F_GAP);
        LogInterp("unsupported_unload", v2, -1.0, 0.4000, -1.0, PROBE_F_ERROR | PROBE_F_GAP | PROBE_F_SYNTH);
        ProbeConfirm(402);
        ProbeConfirm(402);
        ProbeConfirm(402);
        LogRawEvent("synthetic:confirm_repeat", 0, v2, nullptr, r, nullptr, s2,
                    -1.0, 0.4000, -1.0, 0.0, 0.0, "unsupported",
                    PROBE_F_SYNTH | PROBE_F_ERROR);
        LogInterp("confirm_cannot_repair", v2, -1.0, 0.4000, -1.0,
                  PROBE_F_ERROR | PROBE_F_SYNTH);
        Logf("private_logistics_probe  selftest: unsupported trip remains invalid after repeated confirm");
    }

    ProbeReset();
    Logf("private_logistics_probe  selftest: reset issued; stale open trips invalidated");
    LogTripSummary(v, 0.4500, 0.4060, 0.0440, 2);

    Logf("private_logistics_probe  selftest: completed=%u replayed=%u unsupported=%u dup_loads=%u",
         g_tripsCompleted, g_tripsReplayed, g_tripsUnsupported, g_tripsDupLoad);
    Logf("private_logistics_probe  selftest: labelled synthetic accumulator exercise ends");
}

// ===========================================================================
// structured log emission (raw events, interp, trip)
// ===========================================================================

static unsigned LogRawEvent(const char* site, unsigned mode,
                            const void* vehicle, const void* do_bld,
                            const void* res, const void* load_site,
                            const void* unload_site,
                            double fuel_before, double fuel_after,
                            double deduction, double cargo_before,
                            double cargo_after, const char* confidence,
                            unsigned flags)
{
    if (!LoggedTodayInc()) { g_eventsDroppedToday++; return 0; }
    unsigned seq = NextSeq();
    double ts = NowSeconds() - g_sessionStartTs;
    unsigned long thr = GetCurrentThreadId();
    int synth = !!(flags & PROBE_F_SYNTH) ? 1 : 0;

    LogHeader("EVENT", seq, ts, thr, flags, synth);
    Logf(" site=\"%s\" mode=%u", site, mode);

    char vbuf[32], dbuf[32], lsbuf[32], usbuf[32];
    PtrStr(vbuf, sizeof(vbuf), vehicle);
    PtrStr(dbuf, sizeof(dbuf), do_bld);
    PtrStr(lsbuf, sizeof(lsbuf), load_site);
    PtrStr(usbuf, sizeof(usbuf), unload_site);

    char rname[16];
    rname[0] = 0;
    if (res && TsmResourceName(res, rname, sizeof(rname))) {}
    else _snprintf_s(rname, sizeof(rname), _TRUNCATE, "?");

    Logf(" veh=%s do=%s res=%s load_site=%s unload_site=%s", vbuf, dbuf, rname, lsbuf, usbuf);
    Logf(" fb=%.6f fa=%.6f ded=%.6f cb=%.6f ca=%.6f conf=%s day=%d",
         fuel_before, fuel_after, deduction, cargo_before, cargo_after,
         confidence, DayNow());

    ProbeRawEvent ev;
    memset(&ev, 0, sizeof(ev));
    ev.seq = seq; ev.ts = ts; ev.thread = thr;
    ev.day = DayNow(); ev.month = DayNow() / 30 + 1; ev.year = DayNow() / 360 + 1970; ev.tod = 0.0f;
    strncpy_s(ev.site, sizeof(ev.site), site, _TRUNCATE);
    ev.mode = mode; ev.vehicle = vehicle; ev.do_bld = do_bld; ev.res = res;
    ev.fuel_before_t = fuel_before; ev.fuel_after_t = fuel_after;
    ev.deduction_t = deduction; ev.cargo_before_t = cargo_before;
    ev.cargo_after_t = cargo_after;
    strncpy_s(ev.confidence, sizeof(ev.confidence), confidence, _TRUNCATE);
    ev.flags = flags;
    RingAppend(&ev);

    return seq;
}

static void LogInterp(const char* what, const void* vehicle,
                      double fuel_before, double fuel_after,
                      double deduction, unsigned flags)
{
    if (!LoggedTodayInc()) { g_eventsDroppedToday++; return; }
    unsigned seq = NextSeq();
    double ts = NowSeconds() - g_sessionStartTs;
    unsigned long thr = GetCurrentThreadId();
    int synth = !!(flags & PROBE_F_SYNTH) ? 1 : 0;

    LogHeader("INTERP", seq, ts, thr, flags, synth);
    char vbuf[32];
    PtrStr(vbuf, sizeof(vbuf), vehicle);
    Logf(" veh=%s what=\"%s\" fb=%.6f fa=%.6f ded=%.6f complete=%d gap=%d error=%d",
         vbuf, what, fuel_before, fuel_after, deduction,
         !!(flags & PROBE_F_COMPLETE), !!(flags & PROBE_F_GAP), !!(flags & PROBE_F_ERROR));
}

static void LogTripSummary(const void* vehicle, double fuel_before,
                           double fuel_after, double deduction, int trips)
{
    if (!LoggedTodayInc()) { g_eventsDroppedToday++; return; }
    unsigned seq = NextSeq();
    double ts = NowSeconds() - g_sessionStartTs;
    unsigned long thr = GetCurrentThreadId();

    LogHeader("TRIP", seq, ts, thr, PROBE_F_COMPLETE, 0);
    char vbuf[32];
    PtrStr(vbuf, sizeof(vbuf), vehicle);
    Logf(" veh=%s fuel_before=%.6f fuel_after=%.6f deduction=%.6f "
         "trips=%d completed=%d replayed=%d unsupported=%d dup_loads=%d "
         "fuel_price=unavailable currency=unavailable "
         "note=phase1_no_price_source",
         vbuf, fuel_before, fuel_after, deduction,
         trips, g_tripsCompleted, g_tripsReplayed, g_tripsUnsupported,
         g_tripsDupLoad);
}

// ===========================================================================
// entry points
// ===========================================================================

extern "C" __declspec(dllexport) unsigned TsmPluginApiVersion(void)
{
    return TSM_API_VERSION;
}

extern "C" __declspec(dllexport) int TsmPluginInit(const TsmHost* host,
                                                   TsmPluginInfo* info)
{
    TsmBind(host);
    info->name    = "private_logistics_probe";
    info->version = "0.2.3-exploratory";

    const char* ini = "plugins\\private_logistics_probe.ini";

    g_enabled      = H->configInt(ini, "private_logistics_probe", "enabled", 1);
    g_tickInterval = H->configInt(ini, "private_logistics_probe", "tick_interval", 60);
    g_maxSnapshots = H->configInt(ini, "private_logistics_probe", "max_snapshots", 100);
    g_maxQueue     = H->configInt(ini, "private_logistics_probe", "max_queue", 256);
    g_selftest     = H->configInt(ini, "private_logistics_probe", "selftest", 0);

    if (g_tickInterval < 1) g_tickInterval = 1;
    if (g_tickInterval > 10000) g_tickInterval = 10000;
    if (g_maxSnapshots < 1) g_maxSnapshots = 1;
    if (g_maxSnapshots > 10000) g_maxSnapshots = 10000;
    if (g_maxQueue < 16) g_maxQueue = 16;
    if (g_maxQueue > 4096) g_maxQueue = 4096;

    MakeSessionId(g_sessionId, sizeof(g_sessionId));
    g_sessionStartTs = NowSeconds();
    g_prevDay = DayNow();

    InitializeCriticalSection(&g_seqLock);
    InitializeCriticalSection(&g_logLock);
    InitializeCriticalSection(&g_queueLock);
    g_drainEvent = CreateEventA(NULL, FALSE, FALSE, NULL);
    g_drainStop  = CreateEventA(NULL, FALSE, FALSE, NULL);

    if (!g_drainEvent || !g_drainStop || !QueueInit(g_maxQueue)) {
        Logf("private_logistics_probe  allocation/event failure; no observation installed");
        g_enabled = 0;
        return 0;
    }

    if (!g_enabled) {
        LogSession("disabled", "Exploratory probe present; no sim_clock subscription; no game reads");
        Logf("private_logistics_probe  disabled by configuration - no callback registered, no game object dereferenced, "
             "no treasury/payment API consumed. Set [private_logistics_probe] enabled=1 to arm the tick subscriber.");
        return 0;
    }

    LogSession("armed", "probe enabled; awaiting sim_clock subscription at Start");
    Logf("private_logistics_probe  ARMED (exploratory): tick_interval=%d max_snapshots=%d max_queue=%d",
         g_tickInterval, g_maxSnapshots, g_maxQueue);
    return 0;
}

// Cheap in-memory identity guard; operator must also verify the full file hash.
static bool ProbeTargetMatches(void)
{
    if (!g_exeBase || !ReadablePtr(g_exeBase, sizeof(IMAGE_DOS_HEADER))) return false;
    const IMAGE_DOS_HEADER* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(g_exeBase);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE || dos->e_lfanew < 0 || dos->e_lfanew > 0x100000) return false;
    const IMAGE_NT_HEADERS64* nt = reinterpret_cast<const IMAGE_NT_HEADERS64*>(g_exeBase + dos->e_lfanew);
    return ReadablePtr(nt, sizeof(*nt)) && nt->Signature == IMAGE_NT_SIGNATURE &&
           nt->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64 &&
           nt->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC &&
           nt->FileHeader.TimeDateStamp == 0x6A3EB6ADu;
}

extern "C" __declspec(dllexport) int TsmPluginStart(void)
{
    if (!g_enabled) return 0;
    if (!ProbeTargetMatches()) {
        Logf("private_logistics_probe  executable identity mismatch; no callback or game reads installed");
        return 0;
    }

    g_simClock = (const TsmSimClockApi*)H->consume(TSM_SERVICE_SIM_CLOCK,
                                                    TSM_SIM_CLOCK_VERSION);
    if (!g_simClock) {
        Logf("private_logistics_probe  WARN sim_clock not installed - cannot subscribe to ticks, no observations possible");
        LogSession("armed_no_clock", "armed but sim_clock unavailable; no tick callback registered");
        return 0;
    }

    // Fail closed before starting asynchronous work. These modules must outlive
    // the callback/worker; pinning is irreversible until process exit.
    HMODULE pinned = nullptr;
    const DWORD pinFlags = GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_PIN;
    if (!GetModuleHandleExA(pinFlags, reinterpret_cast<LPCSTR>(&TsmPluginStart), &pinned) ||
        !GetModuleHandleExA(pinFlags, reinterpret_cast<LPCSTR>(H->log), &pinned) ||
        !GetModuleHandleExA(pinFlags, reinterpret_cast<LPCSTR>(g_simClock->registerTick), &pinned)) {
        Logf("private_logistics_probe  cannot pin callback/worker dependencies; no observation installed");
        return 0;
    }
    // Start drain thread BEFORE registering tick subscriber.
    g_drainThread = CreateThread(NULL, 0, DrainThreadProc, NULL, 0, &g_drainThreadId);
    if (!g_drainThread) {
        Logf("private_logistics_probe  WARN failed to create drain thread - no observations possible");
        LogSession("armed_no_drain", "armed but drain thread creation failed; no observations possible");
        return 0;
    }

    LogSession("starting", "process-lifetime snapshot probe; one save only; restart before another run");
    g_tickHandle = g_simClock->registerTick(OnTick, nullptr);
    if (!g_tickHandle) {
        Logf("private_logistics_probe  WARN sim_clock subscriber table full - no tick callback registered");
        LogSession("armed_no_sub", "armed but sim_clock subscriber table full; no observations possible");
        return 0;
    }

    Logf("private_logistics_probe  tick subscriber registered (handle=%d)", g_tickHandle);

    return 0;
}

// All enabled probe/service/host modules are pinned before worker startup.
// OS reclaims memory/handles at process termination. No callbacks, locks,
// service calls, frees or worker joins under loader lock. Hot-unload unsupported.
BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID)
{
    return TRUE;
}
