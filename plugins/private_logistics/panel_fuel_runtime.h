// Private to this plugin. Included after its services/globals are defined.
// Process-lifetime hooks, like the plugin's existing transfer hooks.
#ifndef PRIVATE_LOGISTICS_PANEL_FUEL_RUNTIME_H
#define PRIVATE_LOGISTICS_PANEL_FUEL_RUNTIME_H
#include "panel_fuel_observer.h"
#include "schedule_observer.h"
#include "schedule_assignment_ledger.h"

static constexpr char kGateDiagnosticVersion[] = "0.19.0-simplified-settlement";
static bool g_scheduleReady = false;
static volatile LONG g_scheduleFault = 0;
static private_logistics_schedule::History g_scheduleHistory; // simulation tick only
static private_logistics_assignment::Ledger g_scheduleLedger; // simulation tick only
static LONG g_scheduleLastEpoch = 0;
static unsigned g_scheduleLastCaptureDrops = 0;
static CRITICAL_SECTION g_panelQueueLock;
static volatile LONG g_panelEpoch = 1, g_panelStarted = 0, g_panelFault = 0, g_panelContention = 0;
static LONG g_panelQueueEpoch = 0;
static unsigned g_panelHead = 0, g_panelCount = 0, g_panelSamples = 0, g_panelDropped = 0;
static ULONGLONG g_panelLastMs = 0;
static unsigned g_panelDroppedReported = 0;
static ULONGLONG g_panelFirstDropMs = 0, g_panelLastDropMs = 0;
static bool g_panelCapLogged = false, g_panelFaultLogged = false;
struct PanelRecord {
    private_logistics_panel::Snapshot snapshot;
    private_logistics_schedule::Snapshot schedule;
    LONG epoch;
    unsigned sequence;
    ULONGLONG milliseconds;
    ULONGLONG firstMilliseconds;
    unsigned queueFullAtCapture, coalesced;
    bool paused;
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
// Optional fields must not reach the observer-wide mandatory-read fault latch.
// Memory::Get validates base/offset/end arithmetic before calling PanelRead.
template<class T>
static bool PanelDiagnosticRead(std::uintptr_t base, std::uintptr_t offset, T& out)
{
    const private_logistics_panel::Memory memory{PanelRead, nullptr};
    T value{};
    bool ok = false;
    __try {
        ok = memory.Get(base, offset, value);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
    if (ok) out = value;
    return ok;
}
static void PanelCaptureDiagnostics(private_logistics_panel::Snapshot& s)
{
    if (!s.root) return;
    s.root514Read = PanelDiagnosticRead(s.root, 0x514, s.root514);
    s.root4F0Read = PanelDiagnosticRead(s.root, 0x4F0, s.root4F0);
    constexpr std::uintptr_t globalRva = 0x9D54C0;
    if (g_exeBase && globalRva <= g_exeSize &&
        sizeof(s.gateGlobal) <= g_exeSize - globalRva) {
        s.gateGlobalRead = PanelDiagnosticRead(
            reinterpret_cast<std::uintptr_t>(g_exeBase), globalRva, s.gateGlobal);
    }
    for (unsigned i = 0; i < s.count && i < private_logistics_panel::MaxObjects; ++i) {
        auto& o = s.objects[i];
        o.raw1794Read = PanelDiagnosticRead(o.pointer, 0x1794, o.raw1794);
        // Metadata may be available even if a later mandatory quantity read failed.
        o.proto8680Read = PanelDiagnosticRead(o.metadata, 0x8680, o.proto8680);
    }
}
static void PanelResetQueueLocked(LONG epoch)
{
    if (g_panelQueueEpoch == epoch) return;
    g_panelQueueEpoch = epoch;
    g_panelHead = g_panelCount = g_panelSamples = g_panelDropped = 0;
    g_panelLastMs = 0;
    g_panelDroppedReported = 0;
    g_panelFirstDropMs = g_panelLastDropMs = 0;
    g_panelCapLogged = false;
    InterlockedExchange(&g_panelContention, 0);
}
static void PanelCapture(void* context, void* window)
{
    if (!InterlockedCompareExchange(&g_panelStarted, 0, 0) ||
        InterlockedCompareExchange(&g_panelFault, 0, 0)) return;
    if (!TryEnterCriticalSection(&g_panelQueueLock)) {
        InterlockedExchange(&g_panelContention, 1); return;
    }
    const LONG epoch = InterlockedCompareExchange(&g_panelEpoch, 0, 0);
    PanelResetQueueLocked(epoch);
    const ULONGLONG now = GetTickCount64();
    if (g_panelSamples >= 240 || (g_panelLastMs && now - g_panelLastMs < 1000)) {
        LeaveCriticalSection(&g_panelQueueLock); return;
    }
    const bool paused = g_simClock && g_simClock->isPaused && g_simClock->isPaused() == 1;
    const unsigned tail = (g_panelHead + g_panelCount + 15) % 16;
    // Only replace a queued paused snapshot of the SAME window/root. Preserve
    // other vehicles and unpaused samples; no extra storage or render-thread I/O.
    private_logistics_panel::Snapshot snapshot{};
    bool captured = false;
    __try {
        captured = private_logistics_panel::Capture({PanelRead, nullptr},
            (uintptr_t)context, (uintptr_t)window, snapshot);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        InterlockedExchange(&g_panelFault, 1);
    }
    if (captured) PanelCaptureDiagnostics(snapshot);
    private_logistics_schedule::Snapshot schedule{};
    if (captured && g_scheduleReady) {
        schedule.status = private_logistics_schedule::Status::Ineligible;
        // Only the root itself, uniquely present in the verified road-DO fleet.
        // Never walk home as buyer, follow linked vehicles, or read +650's pointee.
        if (snapshot.count && snapshot.objects[0].pointer == snapshot.root &&
            snapshot.objects[0].match == private_logistics_panel::Match::Found &&
            snapshot.objects[0].occurrences == 1) {
            if (!InterlockedCompareExchange(&g_scheduleFault, 0, 0)) {
                __try {
                    private_logistics_schedule::Capture({PanelRead, nullptr}, snapshot.root, schedule);
                } __except (EXCEPTION_EXECUTE_HANDLER) {
                    InterlockedExchange(&g_scheduleFault, 1);
                    schedule.status = private_logistics_schedule::Status::GuardedFault;
                }
            } else schedule.status = private_logistics_schedule::Status::GuardedFault;
        }
    }
    if (captured && epoch == InterlockedCompareExchange(&g_panelEpoch, 0, 0)) {
        g_panelLastMs = now; // inactive windows do not consume the capture slot
        const bool coalesce = paused && snapshot.root && g_panelCount && g_panelQueue[tail].paused &&
            g_panelQueue[tail].snapshot.window == snapshot.window &&
            g_panelQueue[tail].snapshot.root == snapshot.root;
        if (coalesce) {
            auto& record = g_panelQueue[tail];
            record.snapshot = snapshot;
            record.schedule = schedule;
            record.milliseconds = now;
            record.queueFullAtCapture = g_panelDropped;
            if (record.coalesced < 0x7FFFFFFFu) ++record.coalesced;
        } else if (g_panelCount == 16) {
            if (!g_panelFirstDropMs) g_panelFirstDropMs = now;
            g_panelLastDropMs = now;
            if (g_panelDropped < 0x7FFFFFFFu) ++g_panelDropped;
        } else {
            auto& record = g_panelQueue[(g_panelHead + g_panelCount) % 16];
            record.snapshot = snapshot;
            record.schedule = schedule;
            record.epoch = epoch;
            record.sequence = ++g_panelSamples;
            record.milliseconds = record.firstMilliseconds = now;
            record.queueFullAtCapture = g_panelDropped;
            record.coalesced = 0;
            record.paused = paused;
            ++g_panelCount;
        }
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
static private_logistics_assignment::NativeFuel PanelNativeFuel(const private_logistics_panel::Snapshot& s)
{
    private_logistics_assignment::NativeFuel f{};
    if(s.status!=private_logistics_panel::Status::Complete || !s.count ||
       s.count>private_logistics_panel::MaxObjects || s.objects[0].pointer!=s.root) return f;
    f.count=s.count;
    for(unsigned i=0;i<s.count;++i) {
        const auto& o=s.objects[i];
        if(!o.quantityRead) return private_logistics_assignment::NativeFuel{};
        f.objects[i]=o.pointer;f.metadata[i]=o.metadata;f.raw[i]=o.raw;f.capacity[i]=o.capacity;
    }
    f.valid=true;
    f.valid=private_logistics_assignment::ValidFuel(f);
    return f;
}
struct PanelEndpointPrice {
    bool available=false;
    double amount=0;
    float perTonne=0;
    int currency=TSM_CURRENCY_RUB;
    const char* reason="not_closed";
};
// Public service quote only: copied values, no retained resource pointer or money writes.
static PanelEndpointPrice DiagnosticFuelPrice()
{
    PanelEndpointPrice q{};
    q.reason="pricing_api_unavailable";
    if(!g_pricing || g_pricing->structSize<offsetof(TsmResourcePricingApi,getPrice)+sizeof(g_pricing->getPrice) ||
       !g_pricing->getByName || !g_pricing->getPrice) return q;
    void* resource=g_pricing->getByName(gFuelResource);
    q.reason="resource_unavailable";if(!resource) return q;
    // The public helper inspects BOTH appended function pointers. Guard the full
    // pair before calling it, including providers exposing only an older prefix.
    const bool currencyTail=g_currency && g_currency->structSize>=
        offsetof(TsmCurrencyApi,getCurrencySymbol)+sizeof(g_currency->getCurrencySymbol);
    q.currency=TsmCurrencyActive(currencyTail ? g_currency : nullptr); // RUB fallback.
    // resource_pricing v6: native price only, no market multiplier, 0 = UNPRICED.
    const double quote=(double)g_pricing->getPrice(resource,q.currency);
    q.reason="invalid_price";if(!std::isfinite(quote) || quote<=0 || quote>1e9) return q;
    q.perTonne=(float)quote;q.available=true;q.reason="global_native_price";
    return q;
}
static PanelEndpointPrice PanelHypotheticalPrice(const private_logistics_assignment::Event& event,
                                                const private_logistics_assignment::Endpoint& e)
{
    PanelEndpointPrice q{};
    if(event.kind!=private_logistics_assignment::Kind::Closed) return q;
    q.reason="uncertain_endpoint";if(!e.valid || !std::isfinite(e.net)) return q;
    q.reason="observed_level_rise";if(e.observedRise) return q;
    q.reason="nonnegative_net";if(e.net>=0) return q;
    q=DiagnosticFuelPrice();if(!q.available) return q;
    q.available=false;q.reason="invalid_product";
    const double amount=-e.net*double(q.perTonne);
    if(!std::isfinite(amount) || amount<=0) return q;
    q.amount=amount;q.available=true;q.reason="no_refill_hypothesis_only";
    return q;
}
static void ScheduleAssignmentLog(const PanelRecord& record,
                                  const private_logistics_schedule::Delta& delta,
                                  const private_logistics_schedule::FieldChanges& changes,
                                  uintptr_t home)
{
    const auto& fuel = record.snapshot;
    const auto result = g_scheduleLedger.Observe(record.schedule, delta, changes,
        (unsigned long long)record.epoch, fuel.root, home,
        {record.sequence, record.milliseconds, fuel.aggregateValid, fuel.litresCandidate, PanelNativeFuel(fuel)});
    for (unsigned i = 0; i < result.count; ++i) {
        const auto& event = result.events[i];
        const auto& c = event.candidate;
        const auto endpoint=private_logistics_assignment::Estimate(c,event.boundary);
        const auto price=PanelHypotheticalPrice(event,endpoint);
        Logf("[PANEL ASSIGNMENT] observation_epoch=%ld sample=%u observation_root=%p "
             "id=%llu candidate_epoch=%llu root=%p home=%p event=%s reason=%s "
             "original_target_candidate=%p original_target_rows=%u original_target_resolved=0 "
             "first_sample=%u first_ms=%llu first_fuel_valid=%d first_litres_candidate=%.9g "
             "last_sample=%u last_ms=%llu last_fuel_valid=%d last_litres_candidate=%.9g "
             "boundary_sample=%u boundary_ms=%llu boundary_fuel_valid=%d boundary_litres_candidate=%.9g "
             "unknown_start=%d observation_gap=%d role_uncertain=%d "
             "native_start_valid=%d native_start=%.9g native_end_valid=%d native_end=%.9g "
             "net_change_valid=%d net_change_native=%.9g unit_assumption=tonnes "
             "receiver_changed=%d fuel_uncertain=%d observed_level_rise=%d refill_coverage=unverified "
             "hypothetical_cost_if_no_refill_available=%d hypothetical_cost_if_no_refill=%.9g "
             "hypothetical_cost_reason=%s fuel_price_per_tonne=%.9g currency=%d "
             "currency_policy=public_api_or_RUB_fallback price_timing=observation_drain "
             "id_scope=process_local_selected_panel native_boundary=unverified completion=unverified "
             "exact_burn=0 actual_cost=not_computed settlement=none billing=off",
             record.epoch, record.sequence, (void*)fuel.root,
             (unsigned long long)c.id, (unsigned long long)c.epoch, (void*)c.root, (void*)c.home,
             private_logistics_assignment::KindName(event.kind), event.reason,
             (void*)c.originalTarget, c.originalTargetRows,
             c.first.sample, (unsigned long long)c.first.ms, c.first.fuelValid ? 1 : 0, (double)c.first.litres,
             c.last.sample, (unsigned long long)c.last.ms, c.last.fuelValid ? 1 : 0, (double)c.last.litres,
             event.boundary.sample, (unsigned long long)event.boundary.ms,
             event.boundary.fuelValid ? 1 : 0, (double)event.boundary.litres,
             c.unknownStart ? 1 : 0, c.observationGap ? 1 : 0, c.roleUncertain ? 1 : 0,
             private_logistics_assignment::ValidFuel(c.first.native) ? 1 : 0,
             private_logistics_assignment::NativeTotal(c.first.native),
             private_logistics_assignment::ValidFuel(event.boundary.native) ? 1 : 0,
             private_logistics_assignment::NativeTotal(event.boundary.native),
             endpoint.valid ? 1 : 0, endpoint.valid ? endpoint.net : 0.0,
             c.receiverChanged ? 1 : 0, c.fuelUncertain ? 1 : 0, endpoint.observedRise ? 1 : 0,
             price.available ? 1 : 0, price.amount, price.reason, (double)price.perTonne, price.currency);
    }
}
static void ScheduleObserverLog(const PanelRecord& record, bool gap)
{
    if (!g_scheduleReady) return;
    if (gap || record.coalesced ||
        (g_scheduleLastEpoch == record.epoch &&
         g_scheduleLastCaptureDrops != record.queueFullAtCapture)) g_scheduleHistory.Break();
    g_scheduleLastEpoch = record.epoch;
    g_scheduleLastCaptureDrops = record.queueFullAtCapture;
    const auto& fuel = record.snapshot;
    const auto& s = record.schedule;
    const uintptr_t home = fuel.count ? fuel.objects[0].homeCandidate : 0;
    // Simulation-thread copy of previously captured VALUES, not native reads.
    const auto previous = g_scheduleHistory.LastValidValues();
    const auto d = g_scheduleHistory.Observe(s, (unsigned long long)record.epoch, fuel.root, home,
                                           record.sequence, record.milliseconds);
    const auto changes = d.endpointCompared
        ? private_logistics_schedule::ClassifyChanges(previous, s)
        : private_logistics_schedule::FieldChanges{};
    // Every accepted record updates the ledger, including quiet fuel endpoints.
    ScheduleAssignmentLog(record, d, changes, home);
    if (!d.emit) return;
    Logf("[PANEL SCHEDULE] epoch=%ld sample=%u ms=%llu root=%p candidate_home=%p "
         "status=%s baseline=%s observed_fields_changed=%d storage_changed=%d "
         "row_index_changed=%d opaque_context_changed=%d begin680=%p end688=%p configs638=%p "
         "raw_row_count=%llu captured_rows=%u index698=%d flag5fd=%u "
         "raw4a0=%p raw610=%p raw650=%p fuel_valid=%d fuel_root_raw=%.9g "
         "aggregate_litres_candidate=%.2f visibility=panel_only phase=post_render "
         "native_event_coverage=unverified native_job_id=unverified buyer=unverified "
         "completion=unverified exact_burn=0 billing=off",
         record.epoch, record.sequence, (unsigned long long)record.milliseconds,
         (void*)fuel.root, (void*)home, private_logistics_schedule::StatusName(s.status),
         private_logistics_schedule::BaselineName(d.baseline), d.fields ? 1 : 0,
         d.storage ? 1 : 0, d.index ? 1 : 0, d.context ? 1 : 0,
         (void*)s.begin, (void*)s.end, (void*)s.configs,
         (unsigned long long)(s.end >= s.begin ? (s.end - s.begin) / 8 : 0),
         s.status == private_logistics_schedule::Status::Complete ? s.count : 0,
         (int)s.index698, (unsigned)s.flag5fd, (void*)s.raw4a0, (void*)s.raw610, (void*)s.raw650,
         fuel.aggregateValid ? 1 : 0, fuel.count ? (double)fuel.objects[0].raw : 0.0,
         (double)fuel.litresCandidate);
    if (s.status != private_logistics_schedule::Status::Complete) return;
    const auto currentPattern = private_logistics_schedule::ClassifyPattern(s, home);
    const auto previousPattern = d.endpointCompared
        ? private_logistics_schedule::ClassifyPattern(previous, home)
        : private_logistics_schedule::Pattern::Unavailable;
    const bool indexOnly = d.endpointCompared && d.endpointIndex && !d.endpointFields &&
                           !d.endpointStorage && !d.endpointContext;
    Logf("[PANEL SCHEDULE CLASSIFICATION] epoch=%ld sample=%u previous_sample=%u "
         "root=%p comparison_available=%d compared_across_gap=%d "
         "previous_pattern=%s current_pattern=%s row_count_changed=%d ordered_targets_changed=%d "
         "structure_changed=%d config_availability_changed=%d task_flags_changed=%d "
         "scalar_settings_changed=%d settings_only=%d active_index_only=%d "
         "comparison_scope=captured_endpoints pattern_semantics=observed_flags_only "
         "resource_identity=unverified native_job_boundary=unverified billing=off",
         record.epoch, record.sequence, d.previousSample, (void*)fuel.root,
         changes.comparable ? 1 : 0, d.endpointAcrossGap ? 1 : 0,
         private_logistics_schedule::PatternName(previousPattern),
         private_logistics_schedule::PatternName(currentPattern),
         changes.count ? 1 : 0, changes.targets ? 1 : 0, changes.Structure() ? 1 : 0,
         changes.availability ? 1 : 0, changes.flags ? 1 : 0, changes.scalars ? 1 : 0,
         changes.SettingsOnly() ? 1 : 0, indexOnly ? 1 : 0);
    if (d.endpointCompared && d.endpointAcrossGap) {
        Logf("[PANEL SCHEDULE ENDPOINT] epoch=%ld sample=%u ms=%llu root=%p candidate_home=%p "
             "previous_sample=%u previous_ms=%llu previous_rows=%u current_rows=%u "
             "endpoint_fields_differ=%d endpoint_storage_differ=%d endpoint_index_differ=%d "
             "endpoint_context_differ=%d compared_across_gap=1 continuity_valid=0 "
             "comparison=last_valid_captured_values native_event_time=unknown "
             "native_event_coverage=unverified completion=unverified billing=off",
             record.epoch, record.sequence, (unsigned long long)record.milliseconds,
             (void*)fuel.root, (void*)home, d.previousSample, (unsigned long long)d.previousMs,
             previous.count, s.count, d.endpointFields ? 1 : 0, d.endpointStorage ? 1 : 0,
             d.endpointIndex ? 1 : 0, d.endpointContext ? 1 : 0);
        // The last valid sample may not have emitted rows (unchanged). Include
        // its values when content differs so both endpoints are reconstructible.
        if (d.endpointFields) for (unsigned i = 0; i < previous.count; ++i) {
            const auto& r = previous.rows[i];
            Logf("[PANEL SCHEDULE PREVIOUS ROW] epoch=%ld sample=%u compared_to_sample=%u "
                 "row=%u target_raw=%p config_fields_read=%d flag00=%u value04=%.9g "
                 "flagc8=%u valuecc=%.9g values=cached_not_native_read billing=off",
                 record.epoch, d.previousSample, record.sequence, i, (void*)r.target,
                 r.configRead ? 1 : 0, (unsigned)r.flag00, (double)r.value04,
                 (unsigned)r.flagC8, (double)r.valueCC);
        }
    }
    for (unsigned i = 0; i < s.count; ++i) {
        const auto& r = s.rows[i];
        Logf("[PANEL SCHEDULE ROW] epoch=%ld sample=%u row=%u target_raw=%p "
             "config_fields_read=%d flag00=%u value04=%.9g flagc8=%u valuecc=%.9g "
             "target_role=unverified buyer=unverified",
             record.epoch, record.sequence, i, (void*)r.target, r.configRead ? 1 : 0,
             (unsigned)r.flag00, (double)r.value04, (unsigned)r.flagC8, (double)r.valueCC);
    }
}
static void PanelObserverTick(void*, float, float)
{
    PanelRecord record{};
    bool have = false, cap = false, fault = false, contention = false;
    unsigned dropped = 0, newDrops = 0;
    ULONGLONG firstDropMs = 0, lastDropMs = 0;
    if (!TryEnterCriticalSection(&g_panelQueueLock)) return;
    const LONG epoch = InterlockedCompareExchange(&g_panelEpoch, 0, 0);
    PanelResetQueueLocked(epoch);
    if (g_panelCount) {
        record = g_panelQueue[g_panelHead];
        g_panelHead = (g_panelHead + 1) % 16; --g_panelCount; have = true;
    }
    contention = InterlockedExchange(&g_panelContention, 0) != 0;
    dropped = g_panelDropped;
    newDrops = dropped - g_panelDroppedReported;
    g_panelDroppedReported = dropped;
    firstDropMs = g_panelFirstDropMs; lastDropMs = g_panelLastDropMs;
    g_panelFirstDropMs = g_panelLastDropMs = 0;
    cap = g_panelSamples >= 240 && !g_panelCapLogged;
    if (cap) g_panelCapLogged = true;
    fault = InterlockedCompareExchange(&g_panelFault, 0, 0) && !g_panelFaultLogged;
    if (fault) g_panelFaultLogged = true;
    LeaveCriticalSection(&g_panelQueueLock);
    if (epoch != InterlockedCompareExchange(&g_panelEpoch, 0, 0)) return;
    if (fault) Logf("[PANEL GAP] epoch=%ld reason=guarded_read_fault observer=disabled exact_burn=0", epoch);
    if (cap) Logf("[PANEL GAP] epoch=%ld reason=capture_cap cap=240 capture=stopped exact_burn=0", epoch);
    if (contention)
        Logf("[PANEL GAP] epoch=%ld reason=render_trylock_contention skipped_render_attempts=unknown "
             "simulation_event_coverage=unverified exact_burn=0", epoch);
    if (newDrops)
        Logf("[PANEL GAP] epoch=%ld reason=queue_full skipped_snapshot_captures=%u "
             "first_ms=%llu last_ms=%llu queue_full_total_at_drain=%u "
             "sample_ids=accepted_records_only simulation_event_coverage=unverified exact_burn=0",
             epoch, newDrops, (unsigned long long)firstDropMs,
             (unsigned long long)lastDropMs, dropped);
    if (g_scheduleReady && (fault || cap || contention || newDrops)) {
        g_scheduleHistory.Break();
        g_scheduleLedger.Break();
    }
    if (!have || record.epoch != InterlockedCompareExchange(&g_panelEpoch, 0, 0)) return;
    if (record.coalesced)
        Logf("[PANEL GAP] epoch=%ld sample=%u reason=paused_snapshot_coalescing "
             "discarded_paused_snapshots=%u first_ms=%llu latest_ms=%llu "
             "policy=keep_latest_same_window_root simulation_event_coverage=unverified exact_burn=0",
             record.epoch, record.sequence, record.coalesced,
             (unsigned long long)record.firstMilliseconds, (unsigned long long)record.milliseconds);
    const auto& s = record.snapshot;
    Logf("[PANEL FUEL] epoch=%ld sample=%u ms=%llu phase=post_render window=%p root=%p "
         "status=%s aggregate_valid=%d current_raw=%.9g litres_candidate=%.2f "
         "capacity_unadjusted=%.9g ratio_valid=%d ratio=%.9g links=%u "
         "mode=%d metadata_kind=%d flags_read=%d flag8681=%u flag8682=%u "
         "adjustment_read=%d adjustment_needed=%d presentation=%s "
         "queue_full_total_at_capture=%u queue_full_total_at_drain=%u "
         "captured_paused=%d coalesced_snapshots=%u sample_ids=accepted_records_only "
         "root514_read=%d root514=%u root4f0_read=%d root4f0=%p "
         "gate_global_read=%d gate_global=%d gate_diag=%s native_execution=unobserved "
         "physical_fuel=unverified exact_burn=0 billing=off",
         record.epoch, record.sequence, (unsigned long long)record.milliseconds,
         (void*)s.window, (void*)s.root, PanelStatus(s.status), s.aggregateValid ? 1 : 0,
         (double)s.current, (double)s.litresCandidate, (double)s.unadjustedCapacity,
         s.ratioValid ? 1 : 0, (double)s.ratio, s.links, s.contextMode, s.metadataKind,
         s.flagsRead ? 1 : 0, (unsigned)s.flag8681, (unsigned)s.flag8682,
         s.adjustmentRead ? 1 : 0, s.adjustmentNeeded ? 1 : 0,
         s.flagsRead && (s.flag8681 || s.flag8682) ? "alternate_if_shown" : "unresolved_7c58",
         record.queueFullAtCapture, dropped, record.paused ? 1 : 0, record.coalesced,
         s.root514Read ? 1 : 0, (unsigned)s.root514, s.root4F0Read ? 1 : 0, (void*)s.root4F0,
         s.gateGlobalRead ? 1 : 0, (int)s.gateGlobal, kGateDiagnosticVersion);
    for (unsigned i = 0; i < s.count; ++i) {
        const auto& o = s.objects[i];
        Logf("[PANEL OBJECT] epoch=%ld sample=%u index=%u role=%s object=%p metadata=%p "
             "quantity_read=%d raw5f0=%.9g capacity8684=%.9g candidate_home=%p "
             "membership=%s fleet_index=%u occurrences=%u private_status=not_classified "
             "raw1794_read=%d raw1794=%u proto8680_read=%d proto8680=%u",
             record.epoch, record.sequence, i, i ? "linked" : "root", (void*)o.pointer,
             (void*)o.metadata, o.quantityRead ? 1 : 0, (double)o.raw, (double)o.capacity,
             (void*)o.homeCandidate, PanelMatch(o.match), o.fleetIndex, o.occurrences,
             o.raw1794Read ? 1 : 0, (unsigned)o.raw1794,
             o.proto8680Read ? 1 : 0, (unsigned)o.proto8680);
    }
    ScheduleObserverLog(record, fault || cap || contention || newDrops);
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
// Additional read-only schedule gate. A mismatch disables ONLY schedule reads;
// it never installs another hook or prevents the unchanged fuel observer.
struct ScheduleFingerprint { uintptr_t rva; unsigned size; BYTE bytes[40]; };
static const ScheduleFingerprint g_scheduleFingerprints[] = {
        {0x7D5550, 7, {0x4C,0x8B,0xBA,0x40,0x02,0x00,0x00}},
        {0x7E356E, 4, {0x4C,0x8B,0x7D,0x40}},
        {0x7E6904, 25, {0x49,0x8B,0x8F,0x88,0x06,0x00,0x00,0x49,0x2B,0x8F,0x80,0x06,0x00,0x00,0x48,0xC1,0xF9,0x03,0x41,0x38,0xBF,0xFD,0x05,0x00,0x00}},
        {0x7E693C, 7, {0x49,0x39,0xB7,0xA0,0x04,0x00,0x00}},
        {0x7E6BDB, 8, {0x49,0x83,0xBF,0x50,0x06,0x00,0x00,0x00}},
        {0x7E815D, 32, {0x49,0x8B,0x87,0x80,0x06,0x00,0x00,0x48,0x8B,0x4D,0x38,0x4C,0x8B,0x34,0xC8,0x48,0x69,0xF2,0x98,0x01,0x00,0x00,0x49,0x03,0xB7,0x38,0x06,0x00,0x00,0x4D,0x85,0xF6}},
        {0x7E81B3, 19, {0x41,0x80,0xBF,0xFD,0x05,0x00,0x00,0x00,0x74,0x06,0x4C,0x8B,0xF1,0x48,0x8B,0xF1,0x40,0x32,0xFF}},
        {0x7E8438, 7, {0x41,0x39,0x87,0x98,0x06,0x00,0x00}},
        {0x7E8CC6, 7, {0x4D,0x3B,0xB7,0x10,0x06,0x00,0x00}},
        {0x7E8EB9, 7, {0x0F,0xB6,0x86,0xC8,0x00,0x00,0x00}},
        {0x7E8ED2, 3, {0x80,0x3E,0x00}},
        {0x7E8F07, 8, {0xF3,0x0F,0x10,0x86,0xCC,0x00,0x00,0x00}},
        {0x7E9273, 5, {0xF3,0x0F,0x10,0x46,0x04}},
};
static bool ScheduleTargetMatches(void)
{
    for (const auto& f : g_scheduleFingerprints)
        if (f.rva > g_exeSize || f.size > g_exeSize - f.rva ||
            !ReadablePtr(g_exeBase + f.rva, f.size) ||
            memcmp(g_exeBase + f.rva, f.bytes, f.size)) return false;
    return true;
}
static void PanelObserverStart(void)
{
    if (!g_panelObserverEnabled) { Logf("[PANEL FUEL] disabled by INI"); return; }
    if (!g_simClock || !g_simClock->registerTick || !g_persistenceHandle) {
        Logf("[PANEL FUEL] unavailable: requires sim_clock tick and registered load signal"); return;
    }
    if (!PanelTargetMatches()) { Logf("[PANEL FUEL] target/data fingerprint mismatch; no hook installed"); return; }
    // 20 complete bytes through LEA RBP,[RSP-2EB0]; no RIP-relative operand.
    static const BYTE expect[] = {0x40,0x55,0x56,0x57,0x41,0x54,0x41,0x55,
        0x41,0x56,0x41,0x57,0x48,0x8D,0xAC,0x24,0x50,0xD1,0xFF,0xFF};
    if (!ReadablePtr(g_exeBase + 0x7D5460, sizeof(expect)) ||
        memcmp(g_exeBase + 0x7D5460, expect, sizeof(expect))) {
        Logf("[PANEL FUEL] renderer prologue mismatch; no hook installed"); return;
    }
    g_scheduleReady = g_scheduleObserverEnabled && ScheduleTargetMatches();
    Logf("[PANEL SCHEDULE] enabled=%d fingerprint=%s mode=read_only_selected_fields "
         "max_rows=16 last_flag5fd_row_config=not_read gap_endpoints=enabled_cached_values_only "
         "classification=enabled_captured_values assignment_ledger=selected_panel_dry_run job_lifecycle=unverified billing=off",
         g_scheduleObserverEnabled ? 1 : 0, g_scheduleReady ? "matched" : "disabled_or_mismatch");
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
         "interval_ms=1000 cap=240 queue=16 paused_coalescing=%s; logs drain on unpaused ticks; "
         "candidate_home fleet comparison only; accepted sample IDs do not prove coverage; "
         "no exact burn, tracker or billing", g_simClock->isPaused ? "keep_latest_same_window_root" : "unavailable");
}
#endif
