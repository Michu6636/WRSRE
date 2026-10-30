// Private to this plugin. Included after its services/globals are defined.
// Process-lifetime hooks, like the plugin's existing transfer hooks.
#ifndef PRIVATE_LOGISTICS_PANEL_FUEL_RUNTIME_H
#define PRIVATE_LOGISTICS_PANEL_FUEL_RUNTIME_H
#include "panel_fuel_observer.h"

static CRITICAL_SECTION g_panelQueueLock;
static volatile LONG g_panelEpoch = 1, g_panelStarted = 0, g_panelFault = 0;
static LONG g_panelQueueEpoch = 0;
static unsigned g_panelHead = 0, g_panelCount = 0, g_panelSamples = 0, g_panelDropped = 0;
static ULONGLONG g_panelLastMs = 0;
static bool g_panelCapLogged = false, g_panelFaultLogged = false;
struct PanelRecord {
    private_logistics_panel::Snapshot snapshot;
    LONG epoch;
    unsigned sequence;
    ULONGLONG milliseconds;
};
static PanelRecord g_panelQueue[16];
using PanelRenderer = void (__fastcall*)(void*, void*);
static void* g_panelTrampoline = nullptr;

static void PanelObserverLoading(void)
{
    // File thread: no game reads, queue lock, retained identities or persistence.
    InterlockedIncrement(&g_panelEpoch);
}
static bool PanelRead(uintptr_t address, void* out, size_t size, void*)
{
    if (address < 0x10000 || !ReadablePtr((const void*)address, size)) return false;
    memcpy(out, (const void*)address, size);
    return true;
}
static void PanelResetQueueLocked(LONG epoch)
{
    if (g_panelQueueEpoch == epoch) return;
    g_panelQueueEpoch = epoch;
    g_panelHead = g_panelCount = g_panelSamples = g_panelDropped = 0;
    g_panelLastMs = 0;
    g_panelCapLogged = false;
}
static void PanelCapture(void* context, void* window)
{
    if (!InterlockedCompareExchange(&g_panelStarted, 0, 0) ||
        InterlockedCompareExchange(&g_panelFault, 0, 0) ||
        !TryEnterCriticalSection(&g_panelQueueLock)) return;
    const LONG epoch = InterlockedCompareExchange(&g_panelEpoch, 0, 0);
    PanelResetQueueLocked(epoch);
    const ULONGLONG now = GetTickCount64();
    if (g_panelSamples >= 240 || (g_panelLastMs && now - g_panelLastMs < 1000)) {
        LeaveCriticalSection(&g_panelQueueLock); return;
    }
    if (g_panelCount == 16) {
        g_panelLastMs = now;
        if (g_panelDropped < 0x7FFFFFFFu) ++g_panelDropped;
        LeaveCriticalSection(&g_panelQueueLock); return;
    }
    const unsigned slot = (g_panelHead + g_panelCount) % 16;
    bool captured = false;
    __try {
        captured = private_logistics_panel::Capture({PanelRead, nullptr},
            (uintptr_t)context, (uintptr_t)window, g_panelQueue[slot].snapshot);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        InterlockedExchange(&g_panelFault, 1); // no logging/I/O in the render hook
    }
    if (captured && epoch == InterlockedCompareExchange(&g_panelEpoch, 0, 0)) {
        g_panelLastMs = now; // inactive windows must not consume the sample slot
        g_panelQueue[slot].epoch = epoch;
        g_panelQueue[slot].sequence = ++g_panelSamples;
        g_panelQueue[slot].milliseconds = now;
        ++g_panelCount;
    }
    LeaveCriticalSection(&g_panelQueueLock);
}
static void __fastcall PanelRendererDetour(void* context, void* window)
{
    ((PanelRenderer)g_panelTrampoline)(context, window);
    PanelCapture(context, window); // POST render, not an atomic read at the format call
}
static const char* PanelStatus(private_logistics_panel::Status s)
{
    using private_logistics_panel::Status;
    switch (s) {
    case Status::Complete: return "complete";
    case Status::TooManyLinks: return "too_many_links";
    case Status::NonFinite: return "nonfinite";
    case Status::Overflow: return "overflow";
    default: return "unreadable";
    }
}
static const char* PanelMatch(private_logistics_panel::Match m)
{
    using private_logistics_panel::Match;
    switch (m) {
    case Match::Found: return "exact_fleet_pointer";
    case Match::Absent: return "absent_from_candidate_home";
    case Match::NotRoadOffice: return "candidate_home_not_road_do";
    case Match::InvalidFleet: return "invalid_candidate_fleet";
    default: return "unavailable";
    }
}
static void PanelObserverTick(void*, float, float)
{
    PanelRecord record{};
    bool have = false, cap = false, fault = false;
    unsigned dropped = 0;
    if (!TryEnterCriticalSection(&g_panelQueueLock)) return;
    const LONG epoch = InterlockedCompareExchange(&g_panelEpoch, 0, 0);
    PanelResetQueueLocked(epoch);
    if (g_panelCount) {
        record = g_panelQueue[g_panelHead];
        g_panelHead = (g_panelHead + 1) % 16; --g_panelCount; have = true;
    }
    dropped = g_panelDropped;
    cap = g_panelSamples >= 240 && !g_panelCapLogged;
    if (cap) g_panelCapLogged = true;
    fault = InterlockedCompareExchange(&g_panelFault, 0, 0) && !g_panelFaultLogged;
    if (fault) g_panelFaultLogged = true;
    LeaveCriticalSection(&g_panelQueueLock);
    if (fault) Logf("[PANEL FUEL] observer disabled after guarded read fault; no game fuel writes or billing");
    if (cap) Logf("[PANEL FUEL] epoch=%ld capture cap=240 reached; restart/load for another bounded capture", epoch);
    if (!have || record.epoch != InterlockedCompareExchange(&g_panelEpoch, 0, 0)) return;
    const auto& s = record.snapshot;
    Logf("[PANEL FUEL] epoch=%ld sample=%u ms=%llu phase=post_render window=%p root=%p "
         "status=%s aggregate_valid=%d current_raw=%.9g litres_candidate=%.2f "
         "capacity_unadjusted=%.9g ratio_valid=%d ratio=%.9g links=%u "
         "mode=%d metadata_kind=%d flags_read=%d flag8681=%u flag8682=%u "
         "adjustment_read=%d adjustment_needed=%d presentation=%s dropped=%u "
         "physical_fuel=unverified exact_burn=0 billing=off",
         record.epoch, record.sequence, (unsigned long long)record.milliseconds,
         (void*)s.window, (void*)s.root, PanelStatus(s.status), s.aggregateValid ? 1 : 0,
         (double)s.current, (double)s.litresCandidate, (double)s.unadjustedCapacity,
         s.ratioValid ? 1 : 0, (double)s.ratio, s.links, s.contextMode, s.metadataKind,
         s.flagsRead ? 1 : 0, (unsigned)s.flag8681, (unsigned)s.flag8682,
         s.adjustmentRead ? 1 : 0, s.adjustmentNeeded ? 1 : 0,
         s.flagsRead && (s.flag8681 || s.flag8682) ? "alternate_if_shown" : "unresolved_7c58",
         dropped);
    for (unsigned i = 0; i < s.count; ++i) {
        const auto& o = s.objects[i];
        Logf("[PANEL OBJECT] epoch=%ld sample=%u index=%u role=%s object=%p metadata=%p "
             "quantity_read=%d raw5f0=%.9g capacity8684=%.9g candidate_home=%p "
             "membership=%s fleet_index=%u occurrences=%u private_status=not_classified",
             record.epoch, record.sequence, i, i ? "linked" : "root", (void*)o.pointer,
             (void*)o.metadata, o.quantityRead ? 1 : 0, (double)o.raw, (double)o.capacity,
             (void*)o.homeCandidate, PanelMatch(o.match), o.fleetIndex, o.occurrences);
    }
}
static bool PanelTargetMatches(void)
{
    // Fingerprint, not a recomputed SHA-256. Operator must verify the file hash.
    if (!g_exeBase || g_exeSize < 0x90B080 || !ReadablePtr(g_exeBase, sizeof(IMAGE_DOS_HEADER))) return false;
    const auto* dos = (const IMAGE_DOS_HEADER*)g_exeBase;
    if (dos->e_magic != IMAGE_DOS_SIGNATURE || dos->e_lfanew < 0 || dos->e_lfanew > 0x100000) return false;
    const auto* nt = (const IMAGE_NT_HEADERS64*)(g_exeBase + dos->e_lfanew);
    if (!ReadablePtr(nt, sizeof(*nt)) || nt->Signature != IMAGE_NT_SIGNATURE ||
        nt->FileHeader.Machine != IMAGE_FILE_MACHINE_AMD64 ||
        nt->FileHeader.TimeDateStamp != 0x6A3EB6ADu ||
        nt->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC) return false;
    static const BYTE scale[] = {0x00, 0x00, 0x7A, 0x44};
    static const BYTE format[] = {0x25,0,0x2E,0,0x32,0,0x66,0,0x6C,0,0,0};
    return ReadablePtr(g_exeBase + 0x90B07C, sizeof(scale)) &&
        ReadablePtr(g_exeBase + 0x8F3E98, sizeof(format)) &&
        memcmp(g_exeBase + 0x90B07C, scale, sizeof(scale)) == 0 &&
        memcmp(g_exeBase + 0x8F3E98, format, sizeof(format)) == 0;
}
static void PanelObserverStart(void)
{
    if (!g_panelObserverEnabled) { Logf("[PANEL FUEL] disabled by INI"); return; }
    if (g_billing || !g_simClock || !g_simClock->registerTick || !g_persistenceHandle) {
        Logf("[PANEL FUEL] unavailable: requires billing=0, sim_clock tick and registered load signal"); return;
    }
    if (!PanelTargetMatches()) { Logf("[PANEL FUEL] target/data fingerprint mismatch; no hook installed"); return; }
    // 20 complete bytes through LEA RBP,[RSP-2EB0]; no RIP-relative operand.
    static const BYTE expect[] = {0x40,0x55,0x56,0x57,0x41,0x54,0x41,0x55,
        0x41,0x56,0x41,0x57,0x48,0x8D,0xAC,0x24,0x50,0xD1,0xFF,0xFF};
    if (!ReadablePtr(g_exeBase + 0x7D5460, sizeof(expect)) ||
        memcmp(g_exeBase + 0x7D5460, expect, sizeof(expect))) {
        Logf("[PANEL FUEL] renderer prologue mismatch; no hook installed"); return;
    }
    InitializeCriticalSection(&g_panelQueueLock);
    if (!g_simClock->registerTick(PanelObserverTick, nullptr)) {
        Logf("[PANEL FUEL] tick registration failed; no hook installed"); return;
    }
    if (!InstallInlineHook(g_exeBase + 0x7D5460, (void*)PanelRendererDetour,
                          &g_panelTrampoline, expect, sizeof(expect), "private_logistics panel fuel observer")) {
        Logf("[PANEL FUEL] renderer hook refused; capture unavailable"); return;
    }
    InterlockedExchange(&g_panelStarted, 1);
    Logf("[PANEL FUEL] installed renderer=0x7D5460 stolen=20 scale=1000 format=%%.2fl "
         "interval_ms=1000 cap=240 queue=16; logs drain on unpaused ticks; "
         "candidate_home fleet comparison only; no exact burn, tracker or billing");
}
#endif
