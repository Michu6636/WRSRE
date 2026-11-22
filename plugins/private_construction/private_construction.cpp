// v3.6.1 from v3.6.0:
//
//   - RESTORED FROM v3.6.0 + POST-OCT-15 COMPATIBILITY HARDENING.
//     Replaces the 2026-10-22 v3.5.2-STUB placeholder with the real v3.6.0
//     autonomous construction + 0x7838E0 panel-builder implementation:
//       * Multi-version H->consume fallbacks in TsmPluginStart for
//         enterprise_base (v8->v7), ui_panels (v6->v5->v4->v3->v2->v1),
//         citizen_labor (v5->v4->v3->v2), currency (v4->v3), and
//         resource_pricing (v3->v2->v1) so mixed DLL installs still run
//         and the PC button still appends via TsmUiPanelsHasAppend.
//       * Added game_world.h v8.10 TsmLooksLikeBuilding(b) validation in
//         IsValidBuildingWindow and TsmIsDecayed(b) + non-negative dayDelta
//         guards in RunConstructionPass.
//       * Once-latched missing cross-rate warnings in FundCostLocked (reset
//         on save load) so an unset usd_to_rub cannot spam the log at 4 Hz.
//
// v3.6.0 from v3.5.2:
//
//   - CONSTRUCTION-PANEL PRIVATISATION TILE. The construction-site
//     window now runs ui_panels' widget pass after the builder (new v4
//     appendWidgetsToWindow; this plugin owns the 0x7838E0 builder's
//     prologue, so ui_panels' own hooks can't reach it). enterprise_base
//     v9.0's provider draws its smaller toolbar-docked CON tile there,
//     and clicking it registers the site for private construction the
//     tick it lands in g_companies - closing the gap where a building
//     under construction could never be marked (the big PRIV tile only
//     ever existed on finished panels).
//
// private_construction - autonomous private-sector building.
//
// Level 3. Consumes sim_clock, enterprise_base, ui_overlay,
// resource_pricing, and persistence; provides nothing.
//
// v3.5.2 from v3.5.1:
//
//   - UNIFIED GLOBAL CURRENCY (v4). Currency is consumed at Start (v4
//     iface with a v3 fallback) and every price path checks
//     TsmCurrencyActive(g_currency): PriceOne and the native phase walk
//     take the active leg's material price (USD reads the game's own
//     +0x5C price and +0xAC import buy multiplier), labour comes from
//     citizen_labor in RUB mode and from the Western turnkey
//     contractor_wage_per_day_usd rate (default 13.30) in USD mode.
//     FundCostLocked converts the fund row's legs into the pool's unit
//     rather than assuming roubles. The
//     overlay card and the [CLAIMED]/[COST] logs carry the active
//     symbol. Costs deduct from the investment pool in the same unit.
//   - Default fallbacks aligned with the shipped ini: g_wagePerDay
//     3.0, g_costMultiplier 1.0, g_handoffTimeoutDays 0 (wait forever),
//     so a missing key degrades to the documented default rather than a
//     legacy figure; removed a duplicated currency_api.h include.
//
// v3.5.1 from v3.5.0:
//
//   - STATE ACCEPTANCE COMMISSION. handoff_timeout_days = 0 is now the
//     shipped default: a site parks at finish_handoff_frac and waits
//     FOREVER for the engine's own construction - a domestic
//     Construction Office delivering the last percent - to complete
//     it. The v3.4.4 stall timeout could force-finish a site no
//     domestic office reached, and a plugin finish is invisible (no
//     mesh, no bbox, decals stuck) - exactly what the Fabric factory
//     hit after 30 idle days. With the park made permanent, the
//     overlay status and the [HANDOFF]/[WAITING] probe lines say
//     AWAITING STATE INSPECTION instead of "engine" wording: the
//     privately funded site is finished to 99% and the state signs off
//     on the last percent, as the Soviet State Acceptance Commission
//     did in reality.
//
// v3.5.0 from v3.4.4:
//
//   - NATIVE PHASE PRICING. A site's cost no longer depends on scraping
//     the construction panel's text. The game keeps every building
//     type's construction phases on its type descriptor -
//     building+0x318 -> +0x370/+0x378, phase stride 0x21B8, each phase a
//     {resource*, float amount} vector at +0x08. Two independent
//     witnesses pin that layout: FUN_14013FE70 walks it from a placed
//     site (scratch/v3_budget_funcs.c), and the rich tooltip at
//     exe+0x7AC315 / +0x7AC4C0 does the same (the sites Construction
//     Market Estimate probes - layout facts only, no code from that
//     GPL-3.0 plugin). NativeCostForBuilding reads that structure
//     directly, prices each material through resource_pricing at today's
//     market (times the record's import buy multiplier when
//     apply_buy_mult = 1), adds labour from the "workers" entry at
//     contractor_wage_per_day, and applies the same
//     cost_multiplier/contractor_markup/minimum_cost knobs as the panel
//     source.
//
//     Consequences: a site prices on the tick it is tagged - no panel
//     open, no stack scan, no English trigger phrase, works in realistic
//     mode and any UI language, and the "last panel wins" race cannot
//     touch it. fund and panel are unchanged and stay the fallbacks
//     (auto now tries native -> fund -> panel). Length-dependent
//     infrastructure has no phase vector and still falls through.
//
//   - VERSION GUARD. TsmPluginStart byte-checks the two known tooltip
//     sites before the native source may walk anything. A mismatch
//     (game update moved the data) disables native for the session with
//     one WARN line; fund/panel keep working.
//
//   - SHADOW PROBE. With probe = 1 the first priced site logs [COST]
//     native, and where a fund/panel capture exists a [COST] compare
//     line puts both side by side for calibration.
//
// v3.4.4 from v3.4.3:
//
//   - HANDOFF TIMER RESETS ON ENGINE PROGRESS. v3.4.3 parked at 99% and
//     timed out after a fixed handoff_timeout_days even while the engine
//     was visibly advancing the site (0.9976 -> 0.9990 in the latest
//     session). The timeout is now a *stall* timeout: any forward motion
//     of building+0x604 while parked resets the clock, so a slow engine
//     with domestic construction on is never pre-empted. Only a site the
//     engine truly ignores (no materials delivered, domestic off, no
//     builders in reach) still falls back to a plugin finish.
//     handoff_timeout_days default 3.0 -> 30.0 for the same reason; the
//     old value is one bad delivery queue away from a guaranteed
//     invisible finish.
//   - PARKED HEARTBEAT. While parked the pass logged one [HANDOFF] and
//     then silence until [HANDOFF-TIMEOUT], so the log could not say
//     whether the engine was moving. It now logs [WAITING] with the
//     current progress every 15 s, same cadence as [BUILDING].
//
// v3.4.3 from v3.4.2:
//
//   - HANDOFF FINISH. v3.4.2 blamed the crossing step size and bounded it.
//     A later session disproved that. A crossing of 0.0003 - the smallest
//     step the clamp can produce - still finished INVISIBLE, with the
//     completion probe showing `bbox 0` and the construction decals left
//     uncleared, while another building in the same run rendered. What
//     separates them is not the size of the step but WHO takes it: the
//     engine runs a building's completion side-effects - the finished mesh
//     node, the bounding box the renderer culls on, the decal clear - when
//     its own construction advances a site across 1.0, and does not when a
//     raw float write from this plugin does it.
//
//     `finish_handoff_frac` (default 0.99) therefore stops this plugin one
//     percent short and leaves the last sliver to the engine's own
//     construction, which is what runs in a normal game with domestic
//     construction on. `handoff_timeout_days` (default 3.0) bounds the
//     wait, so a site the engine never finishes (domestic construction
//     off) is completed by this plugin anyway rather than stalling
//     forever. Set the fraction to 0 to disable the handoff and restore
//     the v3.4.2 behaviour.
//
//   - The v3.4.2 clamp (`max_tick_advance`) is kept as a backstop on the
//     size of a single tick's step, but it is no longer the fix.
//
// v3.4.2 from v3.4.1:
//
//   - FAST-FORWARD INVISIBILITY. A private site whose progress reaches
//     1.0 in a single tick under extreme fast-forward can finish
//     functional but with no mesh. The engine attaches a building's
//     finished mesh - and drops its construction-phase meshes - when it
//     observes `building+0x604` cross 1.0, and it needs the frames
//     around the crossing to do it. One huge step hands it an
//     already-finished float with no frame in which it saw the
//     transition. `max_tick_advance` bounds one tick's step so the
//     crossing is always its own small step; it is far above the
//     normal rate, so 1x play is unchanged.
//
//   - COMPLETION DIAGNOSTIC. `probe_completion` prints one line on the
//     tick a claimed site crosses into "finished", carrying the node
//     pointer, bbox pointer and demolished flag, so a session in which
//     a completed building goes invisible says whether it differs from
//     the visible case.
//
//   - ALREADY-BUILT IS NOT "FINISHED". A completed building the
//     pending-site match picked up (cost never captured) was logged as
//     `[FINISHED] ... 0%`, which reads as the plugin having built it.
//     The usual cause is benign: the operator completed that building
//     with the game's own "Fund construction using rubles" option,
//     which finishes the whole construction at once - so it is already
//     at 1.0 before this plugin ever captures a price. It now logs
//     `[ALREADY-BUILT]` and says so. The bookkeeping erase is
//     unchanged - the entry must not linger - only the word for it.
//
// v3.4.1 from v3.4:
//
//   - PANEL-BUILDER GATE MATCHES THE LOADER. TsmPluginStart accepted
//     `panel_builder_expect` / `panel_builder_stolen` at >= 5 bytes.
//     Five is the width of a `jmp rel32`; InstallInlineHook writes a
//     14-byte `jmp qword ptr [rip+0]` plus an absolute address and
//     refuses anything shorter (src/tesmioloader.cpp - the refusal
//     exists because a 13-byte steal corrupted the instruction after
//     the prologue and crashed every save; docs/07-pitfalls.md, "A
//     13-byte prologue and a 14-byte jump"). The gate now reads >= 14,
//     so a bad ini is reported as a bad ini here instead of as a
//     loader failure one call later.
//
//   - MEMCMP OVER-READ CLOSED. InstallInlineHook does
//     `memcmp(target, expect, stolen)`. With `stolen > expectLen` it
//     read past the bytes the operator supplied. g_pbExpect is a
//     32-byte zero-initialised static so the read stayed inside this
//     module and could not fault - but it compared live code against
//     zeroes, failed, and logged "prologue mismatch" for an ini whose
//     expect bytes were right as far as they went. The gate now
//     requires `stolen <= expectLen`, and the WARN names the real
//     floor.
//
//   - THE SOCKET IS STILL EMPTY. panel_builder_rva /
//     panel_builder_expect / panel_builder_stolen have no values.
//     Filling them needs SOVIET64.exe: tools/pe/pdata.py resolves the
//     probe's caller RVA to its containing function and dumps the
//     prologue, and the prologue then has to be classified. The
//     classification matters more than the byte count. The stolen
//     range is copied into a trampoline and executed at a new address,
//     so it must be position-independent; a /GS prologue ending in
//     `mov rax,[rip+disp32]` (the __security_cookie load) cannot be
//     stolen at any length and needs the call-patch route instead -
//     the one plugins/deposits PatchWorldSaveCall and plugins/cities
//     PatchCall already use. See the commented block at the bottom of
//     private_construction.ini.
//
//     Until then the v3.3.4 stack scan is what binds a panel to a
//     building. It works on this English client. It does not fix the
//     "last panel wins" race.
//
// v3.4 from v3.3.4:
//
//   - LOCALIZED ROW PATTERNS. Every phrase this plugin scraped off the
//     panel - "Fund construction", "rubles", "dollars", "entire
//     construction", "current phase", "Workdays", the nine "of X"
//     material tags - is now an ini key (`text_*`) accepting
//     '|'-separated alternatives, matched case-insensitively (ASCII,
//     Latin-1 and Cyrillic folding). A game running any localization
//     can be taught its own words without rebuilding the plugin. The
//     English defaults are compiled in, so an untouched ini behaves
//     exactly as before.
//
//   - ROBUST NUMBER PARSE. Row amounts are the first grouped-digit run
//     anywhere in the row, not just at the front. English rows are
//     count-first ("12 of Concrete"); other languages put the count
//     last ("Бетон: 12") and `_wtoi` read them as zero.
//
//   - DIRECT PANEL-BUILDER HOOK (`panel_builder_rva` /
//     `panel_builder_expect` / `panel_builder_stolen`). The
//     architectural replacement for text-based building binding: an
//     inline post-hook on the construction panel builder (the
//     FUN_140786ac0 panel family) captures the building straight from
//     the window argument (`window+0x240`, the documented field) while
//     the builder runs, so every row it prints resolves to the right
//     building with no stack scan and no English "entire construction"
//     trigger. Optional: the three keys come from the operator's own
//     Ghidra session (the caller-RVA probe below prints the address
//     inside the builder), the prologue bytes are verified before
//     patching, and a mismatch declines the hook and keeps the
//     stack-scan fallback. Forwarding caveat: the detour passes four
//     arguments; a builder that reads a fifth must not use this hook.
//
//   - FUND-ROW CALLER PROBE. Fund-row print sites are now logged once
//     each alongside the existing ENTIRE/ROW caller lines, so one
//     session's log lists every panel-builder call site to pin.
//
// v3.3.4 from v3.3.3:
//   - Mangled-name typo. v3.3.3 dropped one character from the
//     C3D_FONTMANAGER::PrintLeftUnicode import name ("QEAX" instead of
//     "QEAAX"), so FindIatSlot could not find it, PatchIat returned 0,
//     TsmPluginStart refused to install, and the plugin was inactive for
//     the whole session. The NoArg variant was spelled correctly and
//     hooked fine, which is why the failure looks partial in the log.
//     Both names are now printed in the symbol string passed to PatchIat
//     so a future typo is one grep away.
//
// v3.3.3 carried forward, unchanged:
//   - LAG FIX: the panel-row scanner runs only when the per-thread
//     cache is stale (300 ms TTL); the entire-construction row forces
//     the scan and every other row reads the cache.
//   - CURRENCY FILTER: accept_currency gates which figure off the fund
//     row is used; rub is the default.
//   - Caller-RVA diagnostic still logs the panel builder's address
//     once per caller site.

#include "../../src/tesmio_plugin.h"
#include "../../src/game_world.h"
#include "../../src/sim_clock_api.h"
#include "../../src/enterprise_base_api.h"
#include "../../src/ui_overlay_api.h"
#include "../../src/currency_api.h"
#include "../../src/citizen_labor_api.h"
#include "../../src/ui_panels_api.h"
#include "../../src/resource_pricing_api.h"
#include "../../src/price_guards.h"
#include "../../src/persistence_api.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <cstdlib>
#include <wchar.h>

// The two engine text entry points this plugin captures rows from. Named
// as #defines rather than inline literals so a typo is caught once, in
// one place, rather than per call site.
//
// Note QEAAX (__cdecl, no-arg-only) vs QEAAX...ZZ (__cdecl variadic).
// MSVC emits two different manglings and FindIatSlot compares by exact
// string. A one-character difference is enough to make PatchIat return
// zero, which is silent at the call site - it only shows up as a FAILED
// line in tesmioloader.log.
#define SYM_PRINT_NOARG    "?PrintLeftUnicodeNoArg@C3D_FONTMANAGER@@QEAAXPEAVC3D_FONT@@MMKPEB_W@Z"
#define SYM_PRINT_VARIADIC "?PrintLeftUnicode@C3D_FONTMANAGER@@QEAAXPEAVC3D_FONT@@MMKPEB_WZZ"

static const TsmSimClockApi*        g_simClock    = nullptr;
static const TsmEnterpriseBaseApi*  g_enterprise   = nullptr;
// v3.6: ui_panels' widget pass, so the CON privatisation tile (and any
// other registered widget) can draw on THIS plugin's construction-site
// panel - ui_panels' own hooks do not cover the construction builder.
static const TsmUiPanelsApi*        g_panels       = nullptr;
static const TsmUiOverlayApi*       g_ui          = nullptr;
static const TsmResourcePricingApi* g_pricing     = nullptr;
static const TsmPersistenceApi*     g_persistence = nullptr;
static const TsmCurrencyApi*        g_currency    = nullptr;
static const TsmCitizenLaborApi*    g_labor       = nullptr;
static int                          g_persHandle  = -1;

static int    g_enabled          = 1;
static int    g_probe            = 1;
static int    g_showOverlay      = 1;
static int    g_tickIntervalMs   = 250;

static float  g_dailyBuildRate   = 0.03f;
// v3.5.2: static fallbacks aligned with the shipped INI values, so a
// missing key degrades to the documented default rather than a legacy
// figure (wage 3.0, multiplier 1.0, handoff timeout 0 = wait forever).
static float  g_wagePerDay       = 3.0f;
static float  g_wagePerDayUsd    = 13.30f; // USD turnkey default; explicit key wins
static float  g_costMultiplier   = 1.0f;
static float  g_contractorMarkup = 1.0f;
static float  g_contractorOverheadMult = 2.50f;
static double g_minimumCost      = 500.0f;
static double g_minimumCostUsd   = -1.0f;  // USD override; < 0 auto-scales
static float  g_usdToRub         = 60.0f;
static float  g_usdDivisor       = 50.0f;

// v3.4.2 ----------------------------------------------------------------
//
// The engine attaches a building's finished mesh, and drops the
// construction-phase meshes, once it observes `building+0x604` cross
// 1.0, and it does that over the frames around the crossing. A site
// whose whole crossing lands inside one plugin tick - which a large
// fast-forward step produces - hands the engine an already-finished
// float with no frame in which it saw the transition, and the final
// mesh can fail to attach: a finished, functional, invisible building.
//
// max_tick_advance bounds how much progress one tick may commit, so the
// crossing is always its own small step whatever the game speed. It is
// far above the normal rate (0.03/day at 60 s/day is ~0.0001 per 250 ms
// tick), so 1x play is unchanged; it only bites under extreme
// fast-forward. 0 disables the clamp entirely.
//
// probe_completion prints one line on the tick a claimed site's
// progress crosses into "finished", carrying the fields a rendering
// fault would touch - node pointer, bbox pointer, demolished flag - so
// a live session says whether the invisible case differs from the
// visible one.
static float  g_maxTickAdvance   = 0.02f;
static int    g_probeCompletion  = 1;

// Day-delta accumulated across throttled frames. The per-site step is
// bounded by g_maxTickAdvance (a progress fraction), so OnTick consumes at
// most its day-equivalent, g_maxTickAdvance / g_dailyBuildRate, per pass and
// leaves the remainder queued; a world load drops it.
static float  g_pendingDayDelta  = 0.0f;
// Unconsumed day fragments are a smoothing buffer, not a ledger: a long
// stall at high time scale must not accumulate an unbounded backlog that
// later drains for minutes after the player returns to 1x.
static const float kMaxPendingDays = 10.0f;

// v3.4.3 ----------------------------------------------------------------
//
// The completion the engine performs - the finished mesh node, the
// bounding box the renderer culls on, the decal clear - runs when the
// engine's OWN construction carries a site across 1.0, not when this
// plugin's raw write does. So the plugin stops just short of the finish
// and lets the engine take the last step. A width of 1% is far more
// than one tick's advance even at speed, so the site spends real frames
// inside the engine's hands.
//
// If the engine never takes it - domestic construction off, or a site
// the state's builders never reach - handoff_timeout_days bounds the
// wait and the plugin finishes the site itself, exactly as v3.4.2 did.
// finish_handoff_frac = 0 disables the whole mechanism.
static float  g_finishHandoffFrac  = 0.99f;
static float  g_handoffTimeoutDays = 0.0f;   // v3.5.2: match shipped ini (0 = wait forever)

#define PROGRESS_CAP 1.000f

#define MAX_EXCLUDE 16
static int g_excludeTypes[MAX_EXCLUDE];
static int g_excludeTypeCount;

enum CostSource { COST_PANEL = 0, COST_FUND, COST_AUTO, COST_NATIVE };
static int g_costSource = COST_AUTO;

// v3.5.0 - native phase pricing. apply_buy_mult multiplies each
// material's price by the resource record's import buy multiplier
// (+0x8C) - what the border actually charges; 0 prices at the raw
// market price, like the panel source does. g_nativeLayoutOk is set at
// Start by the byte probe; while false the native source refuses to
// walk the phase structure.
static int  g_applyBuyMult   = 1;
static bool g_nativeLayoutOk = false;

enum AcceptCurrency { ACC_RUB = 0, ACC_USD, ACC_EITHER };
static int g_acceptCurrency = ACC_RUB;

static bool g_warnedWrongCurrency = false;
static bool g_warnedCrossRate     = false;

// ---------------------------------------------------------------- localized text patterns (v3.4)
//
// Every row this plugin classifies used to match a hard-coded English
// phrase. The phrases are ini-configurable now, each key accepting
// '|'-separated alternatives, so a localized game can be taught its own
// words. Empty key = the compiled-in English default.

#define MAX_PATTERN_ALT 4
#define MAX_PATTERN_LEN 64

struct Pattern {
    wchar_t alt[MAX_PATTERN_ALT][MAX_PATTERN_LEN];
    int     count;
};

enum PatId {
    PAT_FUND_ROW = 0, PAT_RUBLES, PAT_DOLLARS,
    PAT_ENTIRE, PAT_PHASE, PAT_WORKDAYS,
    PAT_CONCRETE, PAT_GRAVEL, PAT_ASPHALT, PAT_STEEL, PAT_BRICKS,
    PAT_WOOD, PAT_PREFABS, PAT_ECOMPS, PAT_MCOMPS,
    PAT_COUNT
};

static Pattern g_pat[PAT_COUNT];

static const char* kPatIniKeys[PAT_COUNT] = {
    "text_fund_row",     "text_fund_rub",     "text_fund_usd",
    "text_entire_block", "text_phase_block",  "text_workdays",
    "text_mat_concrete", "text_mat_gravel",   "text_mat_asphalt",
    "text_mat_steel",    "text_mat_bricks",   "text_mat_wood",
    "text_mat_prefabs",  "text_mat_ecomps",   "text_mat_mcomps",
};

static const wchar_t* kPatDefaults[PAT_COUNT] = {
    L"Fund construction", L"rubles", L"dollars",
    L"entire construction", L"current phase", L"Workdays",
    L"of Concrete", L"of Gravel", L"of Asphalt", L"of Steel", L"of Bricks",
    L"of Wood", L"of Prefabs", L"of eComponents", L"of mComponents",
};

// Case folding for the scripts the game ships in: ASCII, Latin-1
// accents, Cyrillic. Anything outside those compares case-sensitively.
static wchar_t FoldCase(wchar_t c)
{
    if (c >= L'A' && c <= L'Z')          return c + 32;
    if (c >= 0x00C0 && c <= 0x00DE && c != 0x00D7) return c + 32;
    if (c >= 0x0410 && c <= 0x042F)      return c + 0x20;
    if (c == 0x0401)                     return 0x0451;
    return c;
}

static bool WcsIStr(const wchar_t* hay, const wchar_t* needle)
{
    if (!hay || !needle || !needle[0]) return false;
    for (; *hay; hay++) {
        const wchar_t* h = hay;
        const wchar_t* n = needle;
        while (*h && *n && FoldCase(*h) == FoldCase(*n)) { h++; n++; }
        if (!*n) return true;
    }
    return false;
}

static bool PatMatch(PatId id, const wchar_t* text)
{
    if (!text) return false;
    const Pattern& p = g_pat[id];
    for (int i = 0; i < p.count; i++)
        if (WcsIStr(text, p.alt[i])) return true;
    return false;
}

static void Utf8ToWide(const char* s, wchar_t* out, size_t n)
{
    out[0] = 0;
    if (!s || n == 0) return;
    MultiByteToWideChar(CP_UTF8, 0, s, -1, out, (int)n);
    out[n - 1] = 0;
}

// Splits a `|`-separated ini value into a pattern's alternatives. A
// value with nothing usable leaves the compiled-in default in place.
static void LoadPattern(PatId id, const char* listUtf8)
{
    Pattern& p = g_pat[id];
    p.count = 0;

    const char* s = listUtf8;
    while (s && *s && p.count < MAX_PATTERN_ALT) {
        while (*s == ' ' || *s == '\t') s++;
        const char* end = strchr(s, '|');
        size_t len = end ? (size_t)(end - s) : strlen(s);
        while (len > 0 && (s[len - 1] == ' ' || s[len - 1] == '\t')) len--;

        if (len > 0 && len < 128) {
            char piece[128];
            memcpy(piece, s, len);
            piece[len] = 0;
            Utf8ToWide(piece, p.alt[p.count], MAX_PATTERN_LEN);
            if (p.alt[p.count][0]) p.count++;
        }
        if (!end) break;
        s = end + 1;
    }

    if (p.count == 0) {
        wcsncpy_s(p.alt[0], MAX_PATTERN_LEN, kPatDefaults[id], _TRUNCATE);
        p.count = 1;
    }
}

struct PanelMats {
    int   workdays = 0;
    float concrete = 0.0f;
    float gravel   = 0.0f;
    float asphalt  = 0.0f;
    float steel    = 0.0f;
    float bricks   = 0.0f;
    float boards   = 0.0f;
    float prefabs  = 0.0f;
    float ecomps   = 0.0f;
    float mcomps   = 0.0f;
};

// ---------------------------------------------------------------- per-building capture

struct BuildingCapture {
    PanelMats entire;
    PanelMats lastEntire;
    int       fundRub       = 0;
    int       fundUsd       = 0;
    bool      inEntire      = false;
    DWORD     lastPrintTick = 0;
};

static std::unordered_map<void*, BuildingCapture> g_buildingCaptures;

static __declspec(thread) void* t_cachedPanelBuilding;
static __declspec(thread) DWORD t_cachedPanelTick;

#define PANEL_CACHE_TTL_MS 300

static int SafeBuildingType(const BYTE* b)
{
    if (!ReadablePtr(b + TSM_BLD_TYPEDESC, sizeof(void*))) return -1;
    void* td = *(void**)(b + TSM_BLD_TYPEDESC);
    if (!td || !TsmIsValidGamePtr(td)) return -1;
    if (!ReadablePtr((const BYTE*)td + TSM_TD_TYPE_NUM, sizeof(int))) return -1;
    return *(int*)((const BYTE*)td + TSM_TD_TYPE_NUM);
}

static bool IsValidBuildingWindow(const void* ptr, void** outBuilding)
{
    if (!TsmIsValidGamePtr(ptr) || !ReadablePtr(ptr, 0x260)) return false;

    const BYTE* w = (const BYTE*)ptr;

    if (*(const int*)(w + 0x248) != 0) return false;   // 0 == building window
    if (w[0x01] != 0)                  return false;   // 0 == drawn / visible

    float wx = *(const float*)(w + 0x04);
    float wy = *(const float*)(w + 0x08);
    if (wx < -2000.0f || wx > 10000.0f || wy < -2000.0f || wy > 10000.0f) return false;

    void* b = *(void**)(w + 0x240);
    if (!TsmLooksLikeBuilding(b)) return false;
    if (TsmIsDemolished(b)) return false;

    int type = SafeBuildingType((const BYTE*)b);
    if (type < 0 || type > 500) return false;

    float built = *(const float*)((const BYTE*)b + TSM_BLD_BUILT);
    if (built < 0.0f || built >= 1.0f) return false;

    if (outBuilding) *outBuilding = b;
    return true;
}

static void* FindCurrentPanelBuilding()
{
    void** stackTop = (void**)_AddressOfReturnAddress();

    MEMORY_BASIC_INFORMATION mbi;
    if (!VirtualQuery(stackTop, &mbi, sizeof(mbi))) return nullptr;
    void** stackLimit = (void**)((BYTE*)mbi.BaseAddress + mbi.RegionSize);

    size_t maxQwords = 384;
    if ((size_t)(stackLimit - stackTop) < maxQwords)
        maxQwords = (size_t)(stackLimit - stackTop);

    for (size_t i = 1; i < maxQwords; i++)
    {
        void* candidate = stackTop[i];
        void* b = nullptr;
        if (IsValidBuildingWindow(candidate, &b))
            return b;
    }
    return nullptr;
}

static void LogCallerRvaOnce(const char* tag, void* retAddr, void* b)
{
    static std::unordered_set<void*> loggedRetAddrs;
    EnterCriticalSection(&g_lock);
    if (loggedRetAddrs.count(retAddr) != 0) { LeaveCriticalSection(&g_lock); return; }
    loggedRetAddrs.insert(retAddr);
    LeaveCriticalSection(&g_lock);

    BYTE* r = (BYTE*)retAddr;
    if (r >= g_exeBase && r < g_exeBase + g_exeSize)
    {
        Logf("private_construction  [%s] caller SOVIET64.exe + 0x%zX, building = %p",
             tag, (size_t)(r - g_exeBase), b);
    }
    else
    {
        HMODULE m = NULL;
        char name[MAX_PATH] = "?";
        if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                               GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                               (LPCSTR)r, &m) && m)
        {
            GetModuleFileNameA(m, name, MAX_PATH);
            const char* slash = strrchr(name, '\\');
            Logf("private_construction  [%s] caller %s + 0x%zX (NOT in SOVIET64.exe), building = %p",
                 tag, slash ? slash + 1 : name, (size_t)(r - (BYTE*)m), b);
        }
        else
        {
            Logf("private_construction  [%s] caller %p (unknown module), building = %p",
                 tag, retAddr, b);
        }
    }

    // v3.4: one-time pointer at the discovery workflow. Every address
    // this probe logs is inside a panel builder - the step from a
    // caller address to a hookable function entry is one Ghidra jump.
    static bool s_hinted = false;
    if (!s_hinted) {
        s_hinted = true;
        Logf("private_construction  hint: the address above is INSIDE a panel builder, not "
             "its entry. `python tools\\pe\\pdata.py SOVIET64.exe <that rva>` resolves the "
             "containing function and dumps its prologue; then set panel_builder_rva / "
             "panel_builder_expect / panel_builder_stolen in "
             "plugins\\private_construction.ini");
        Logf("private_construction  hint: stolen bytes must be >= 14, must end on an "
             "instruction boundary, and must be POSITION-INDEPENDENT - they are copied to a "
             "trampoline and run there. A /GS prologue ending in `48 8B 05 <disp32>` "
             "(__security_cookie) cannot be inline-hooked at any length; that one needs a "
             "call patch. See the notes at the bottom of the ini.");
    }
}

// ---------------------------------------------------------------- direct panel-builder capture (v3.4)
//
// The replacement for text-based building binding. When the operator
// supplies the construction panel builder's entry - the
// FUN_140786ac0 panel family, discovered via the caller-RVA probe and
// confirmed in Ghidra - an inline hook posts the window's building
// pointer into the per-thread cache while the builder runs. Every row
// printed during that call then resolves to the right building with no
// stack scan and no "entire construction" trigger.
//
// The building comes from window+0x240 through the same
// IsValidBuildingWindow check the stack scan uses - no offset that was
// not already in this file. The hook verifies the prologue bytes before
// patching and declines on a mismatch, in which case the stack-scan
// fallback stays in force unchanged.
//
// Forwarding caveat: the detour passes four arguments through to the
// original (rcx, rdx, r8, r9). A builder that reads a fifth stack
// argument must not use this hook - check the disassembly first.

#define PB_MAX_EXPECT 32

static size_t g_pbRva       = 0;
static BYTE   g_pbExpect[PB_MAX_EXPECT];
static size_t g_pbExpectLen = 0;
static size_t g_pbStolen    = 0;
static bool   g_pbHooked    = false;

typedef void (*t_PanelBuilder)(void*, void*, void*, void*);
static t_PanelBuilder o_PanelBuilder = nullptr;

static void CapturePanelWindow(void* window)
{
    void* b = nullptr;
    if (!IsValidBuildingWindow(window, &b)) return;
    DWORD now = GetTickCount();
    t_cachedPanelBuilding = b;
    t_cachedPanelTick     = now;
}

static void h_PanelBuilder(void* game, void* window, void* a3, void* a4)
{
    // Capture first, so every row the original prints while building
    // the panel sees the binding.
    __try { CapturePanelWindow(window); }
    __except (EXCEPTION_EXECUTE_HANDLER) {}
    o_PanelBuilder(game, window, a3, a4);

    // v3.6: after the builder has laid the construction panel out, run
    // the registered widget providers on this window - enterprise_base's
    // toolbar-docked CON tile (which registers the site for private
    // construction) lives there. No-ops unless ui_panels v4 is present.
    if (g_panels && TsmUiPanelsHasAppend(g_panels)) {
        __try { g_panels->appendWidgetsToWindow(game, window); }
        __except (EXCEPTION_EXECUTE_HANDLER) {}
    }
}

// Labor-driven workday rate, split by the active currency:
//   RUB: citizen_labor's live default daily wage x contractor overhead
//        (static INI wage only as fallback) - the domestic economy.
//   USD: contractor_wage_per_day_usd directly (default 13.30) - the
//        Western turnkey rate. Never the domestic citizen divisor.
static float EffectiveWorkdayRate(void)
{
    int cur = TsmCurrencyActive(g_currency);
    if (cur == TSM_CURRENCY_USD)
        return g_wagePerDayUsd;       // one documented default (INI 13.30)

    // RUB mode: live citizen-labor default x contractor overhead,
    // with the static INI wage as the only fallback.
    float baseWage = g_wagePerDay;
    if (g_labor && g_labor->getDefaultDailyWage) {
        float live = g_labor->getDefaultDailyWage();
        if (live > 0.0f)
            baseWage = live;
    }
    return baseWage * g_contractorOverheadMult;
}

// ---------------------------------------------------------------- pricing maths

// Why an estimate was refused, when the native phase walk was not the source.
// The claim site logs it the same way it logs g_lastNativeFail.
static const char* g_lastLegacyFail = nullptr;

// One ERROR line per material name. The panel estimate can run every frame
// while a site waits; the same missing price is the same fact.
#define MAX_UNPRICED_MATERIALS 16
static char g_unpricedMaterials[MAX_UNPRICED_MATERIALS][32];
static int  g_unpricedMaterialCount = 0;

static void NoteUnpricedMaterial(const char* name, const char* why)
{
    if (!name || !name[0]) name = "?";
    for (int i = 0; i < g_unpricedMaterialCount; i++)
        if (strcmp(g_unpricedMaterials[i], name) == 0) return;
    if (g_unpricedMaterialCount >= MAX_UNPRICED_MATERIALS) return;
    char* slot = g_unpricedMaterials[g_unpricedMaterialCount++];
    int n = 0;
    for (; n < (int)sizeof(g_unpricedMaterials[0]) - 1 && name[n]; n++)
        slot[n] = name[n];
    slot[n] = 0;
    Logf("private_construction  ERROR UNPRICED  \"%s\" %s", name, why);
}

// Prices one material for the panel-materials estimate. Returns false when
// the material has NO native price: economy direction D33 says the whole
// estimate is refused, never billed partly and never filled from a fallback
// (D8). A zero tonnage is not an error - that material is not in the phase.
static bool PriceOne(const char* name, float tons, double* out)
{
    if (out) *out = 0.0;
    if (!(tons > 0.0f)) return true;
    if (!g_pricing || !g_pricing->getByName || !g_pricing->getPrice) {
        NoteUnpricedMaterial(name, "cannot be priced: resource_pricing is "
                                   "unavailable - estimate refused");
        return false;
    }
    void* r = g_pricing->getByName(name);
    if (!r) {
        NoteUnpricedMaterial(name, "has no resource record - estimate refused");
        return false;
    }
    // v4: price in the active currency so construction costs always come
    // out in the same unit the investment pool is held in.
    const float price = g_pricing->getPrice(r, TsmCurrencyActive(g_currency));
    if (!TsmNativePriceUsable(price)) {
        NoteUnpricedMaterial(name, "has no native price - the whole estimate "
                                   "is refused (D33)");
        return false;
    }
    if (out) *out = (double)tons * (double)price;
    return true;
}

static double CostFromPanelMats(const PanelMats& m)
{
    double material = 0.0;
    double one = 0.0;
    g_lastLegacyFail = nullptr;
#define PANEL_MATERIAL(field, resname) \
    do { \
        if (!PriceOne(resname, m.field, &one)) { \
            g_lastLegacyFail = "unpriced material in the panel estimate"; \
            return 0.0; \
        } \
        material += one; \
    } while (0)
    PANEL_MATERIAL(concrete, "concrete");
    PANEL_MATERIAL(gravel,   "gravel");
    PANEL_MATERIAL(asphalt,  "asphalt");
    PANEL_MATERIAL(steel,    "steel");
    PANEL_MATERIAL(bricks,   "bricks");
    PANEL_MATERIAL(boards,   "boards");
    PANEL_MATERIAL(prefabs,  "prefabpanels");
    PANEL_MATERIAL(ecomps,   "ecomponents");
    PANEL_MATERIAL(mcomps,   "mcomponents");
#undef PANEL_MATERIAL

    // v4: panel-materials path - labour and the floor follow the
    // active currency the same way the native walk does; materials
    // are already active-currency via PriceOne above.
    int cur = TsmCurrencyActive(g_currency);
    float wage = EffectiveWorkdayRate();
    double labour = (double)m.workdays * (double)wage;

    double total = (material + labour)
                 * (double)g_contractorMarkup
                 * (double)g_costMultiplier;
    double minCost = g_minimumCost;
    if (cur == TSM_CURRENCY_USD) {
        minCost = (g_minimumCostUsd >= 0.0) ? g_minimumCostUsd
                                            : g_minimumCost / ((g_usdDivisor >= 1.0f) ? g_usdDivisor : 50.0f);
    }
    if (total < minCost) total = minCost;
    return total;
}

static double FundCostLocked(const BuildingCapture& cap)
{
    // The pool is held in the ACTIVE currency (enterprise_base resolves
    // USD-style pools in USD). The fund row's legs are RUB and USD
    // respectively, so each is converted INTO the pool's unit; the cross
    // rate is usd_to_rub (roubles per dollar). accept_currency still
    // selects which leg is read from the row first.
    int cur = TsmCurrencyActive(g_currency);
    double rate = (g_usdToRub > 0.0f) ? (double)g_usdToRub : 0.0;

    // v: a value expressed in `legCur`, converted to the pool's unit.
    // Returns 0.0 when the cross rate needed for the conversion is unknown.
    auto toPool = [cur, rate](double v, int legCur) -> double {
        if (v <= 0.0) return 0.0;
        if (legCur == cur) return v;                  // already pool units
        if (rate <= 0.0) return 0.0;                  // cannot cross
        return (cur == TSM_CURRENCY_USD) ? v / rate    // RUB -> USD
                                         : v * rate;   // USD -> RUB
    };

    switch (g_acceptCurrency) {
    case ACC_USD: {
        // Prefer the row's dollar figure; fall back to the ruble figure.
        if (cap.fundUsd > 0.0) {
            double v = toPool(cap.fundUsd, TSM_CURRENCY_USD);
            if (v > 0.0) return v;
        }
        if (cap.fundRub > 0.0) {
            double v = toPool(cap.fundRub, TSM_CURRENCY_RUB);
            if (v > 0.0) return v;
            if (!g_warnedCrossRate) {
                g_warnedCrossRate = true;
                Logf("private_construction  WARN  fund row ruble figure (%d RUB) "
                     "needs a cross rate but usd_to_rub is unset - the site waits",
                     cap.fundRub);
            }
        }
        return 0.0;
    }
    case ACC_EITHER: {
        if (cap.fundRub > 0.0) {
            double v = toPool(cap.fundRub, TSM_CURRENCY_RUB);
            if (v > 0.0) return v;
        }
        if (cap.fundUsd > 0.0) {
            double v = toPool(cap.fundUsd, TSM_CURRENCY_USD);
            if (v > 0.0) return v;
        }
        return 0.0;
    }
    case ACC_RUB: {
        // Prefer the row's ruble figure; fall back to the dollar figure.
        if (cap.fundRub > 0.0) {
            double v = toPool(cap.fundRub, TSM_CURRENCY_RUB);
            if (v > 0.0) return v;
        }
        if (cap.fundUsd > 0.0) {
            double v = toPool(cap.fundUsd, TSM_CURRENCY_USD);
            if (v > 0.0) return v;
            if (!g_warnedCrossRate) {
                g_warnedCrossRate = true;
                Logf("private_construction  WARN  fund row dollar figure ($%d) "
                     "needs a cross rate but usd_to_rub is unset - the site waits",
                     cap.fundUsd);
            }
        }
        return 0.0;
    }
    }
    return 0.0;
}

// ---------------------------------------------------------- native phase pricing (v3.5.0)
//
// The game keeps every building type's construction phases on its type
// descriptor (building+0x318): a vector at +0x370/+0x378 of 0x21B8-byte
// phase records, each carrying its own {resource*, float amount} vector
// at +0x08 - every material of the WHOLE construction plus the
// "workers" pseudo-resource for workdays, already computed by the game,
// no UI involved. Two independent witnesses pin the layout for 1.1.1.9:
// FUN_14013FE70 walks it from a placed site (scratch/v3_budget_funcs.c)
// and the rich building tooltip reads it at the two byte sites below.
//
// Layout facts only - the offsets, not any code, are what we take from
// the GPL-3.0 "Construction Market Estimate" plugin, which probes the
// same two sites to gate its overlay.

static bool ProbePhaseLayout(void)
{
    // `mov rcx,[r14+0x378] / sub rcx,[r14+0x370]` and, further down,
    // `add rdi,0x21B8` beside the same pair - together these prove the
    // phase vector's location and stride on this exact executable.
    static const BYTE kPhaseVector[] = {
        0x49,0x8B,0x8E,0x78,0x03,0x00,0x00,
        0x49,0x2B,0x8E,0x70,0x03,0x00,0x00
    };
    static const BYTE kStrideVector[] = {
        0x48,0x81,0xC7,0xB8,0x21,0x00,0x00,
        0x49,0x8B,0x8E,0x78,0x03,0x00,0x00,
        0x49,0x2B,0x8E,0x70,0x03,0x00,0x00
    };
    if (!g_exeBase || !g_exeSize) return false;

    const BYTE* s1 = g_exeBase + 0x7AC315;
    const BYTE* s2 = g_exeBase + 0x7AC4C0;
    if (!ReadablePtr(s1, sizeof(kPhaseVector)) ||
        !ReadablePtr(s2, sizeof(kStrideVector)))
        return false;
    return memcmp(s1, kPhaseVector, sizeof(kPhaseVector)) == 0 &&
           memcmp(s2, kStrideVector, sizeof(kStrideVector)) == 0;
}

// Results of the most recent CostForBuilding call. The claim site in
// RunConstructionPass consumes them immediately after (same tick
// thread) for the one-shot [COST] probe lines - the helpers that turn
// a building pointer into a name live later in this file, past
// CostForBuilding, so the logging stays where the names are.
static double      g_lastNative     = 0.0;
static double      g_lastLegacy     = 0.0;
static const char* g_lastNativeFail = nullptr;

// The phase walk. Returns the site's total cost in pool (ruble) units,
// or 0.0 when native cannot price this building - `*failOut` then names
// the guard that rejected it. Reads game memory plus config only, so it
// is safe under the caller's lock from the tick thread; every pointer is
// range-checked before it is dereferenced and every count is capped
// before it is trusted.
static double NativeCostForBuilding(void* b, const char** failOut)
{
    if (failOut) *failOut = nullptr;
    if (!g_nativeLayoutOk)
        { if (failOut) *failOut = "phase layout probe failed"; return 0.0; }
    if (!b || !TsmIsValidGamePtr(b) || !ReadablePtr(b, TSM_BLD_MIN_READABLE))
        { if (failOut) *failOut = "building unreadable"; return 0.0; }

    void* td = *(void**)((BYTE*)b + TSM_BLD_TYPEDESC);
    if (!td || !TsmIsValidGamePtr(td) ||
        !ReadablePtr((const BYTE*)td + TSM_TD_PHASES_END, sizeof(void*)))
        { if (failOut) *failOut = "type descriptor unreadable"; return 0.0; }

    const BYTE* begin = *(const BYTE**)((const BYTE*)td + TSM_TD_PHASES_BEGIN);
    const BYTE* end   = *(const BYTE**)((const BYTE*)td + TSM_TD_PHASES_END);
    if (!begin || !end || end < begin)
        { if (failOut) *failOut = "no construction phases"; return 0.0; }

    size_t bytes = (size_t)(end - begin);
    if (bytes % TSM_PHASE_STRIDE)
        { if (failOut) *failOut = "phase stride mismatch"; return 0.0; }
    size_t phaseCount = bytes / TSM_PHASE_STRIDE;
    if (phaseCount == 0)
        { if (failOut) *failOut = "no construction phases"; return 0.0; }
    if (phaseCount > 128)
        { if (failOut) *failOut = "phase count out of range"; return 0.0; }
    if (!ReadablePtr(begin, bytes))
        { if (failOut) *failOut = "phase vector unreadable"; return 0.0; }

    double matRub = 0.0, matUsd = 0.0, workdays = 0.0;
    int    matEntries = 0;
    const int cur = TsmCurrencyActive(g_currency);

    for (size_t p = 0; p < phaseCount; p++)
    {
        const BYTE* phase = begin + p * TSM_PHASE_STRIDE;
        const BYTE* cBegin = *(const BYTE**)(phase + TSM_PHASE_COSTS);
        const BYTE* cEnd   = *(const BYTE**)(phase + TSM_PHASE_COSTS + sizeof(void*));
        if (!cBegin && !cEnd) continue;                    // phase with no costs
        if (!cBegin || !cEnd || cEnd < cBegin)
            { if (failOut) *failOut = "phase cost vector invalid"; return 0.0; }

        size_t cBytes = (size_t)(cEnd - cBegin);
        if (cBytes % TSM_COST_STRIDE)
            { if (failOut) *failOut = "cost entry stride mismatch"; return 0.0; }
        size_t costCount = cBytes / TSM_COST_STRIDE;
        if (costCount > 512)
            { if (failOut) *failOut = "cost entry count out of range"; return 0.0; }
        if (costCount && !ReadablePtr(cBegin, cBytes))
            { if (failOut) *failOut = "cost vector unreadable"; return 0.0; }

        for (size_t e = 0; e < costCount; e++)
        {
            const BYTE* entry = cBegin + e * TSM_COST_STRIDE;
            void*  res    = *(void**)(entry + TSM_COST_RESOURCE);
            double amount = (double)*(const float*)(entry + TSM_COST_AMOUNT);

            if (!res || !TsmIsValidGamePtr(res) ||
                !ReadablePtr((const BYTE*)res + TSM_RES_BUY_MULT_USD, sizeof(float)))
                { if (failOut) *failOut = "invalid material entry"; return 0.0; }
            if (!(amount > 0.000001) || amount > 1000000000.0)
                { if (failOut) *failOut = "material amount out of range"; return 0.0; }

            // $COST_WORK rides in the same vector as a "workers" record:
            // workdays, not a border-importable good, so it is priced at
            // our wage below instead of at a market price it has not got.
            char rname[48] = {};
            if (TsmResourceName(res, rname, sizeof(rname)) &&
                _stricmp(rname, "workers") == 0)
            {
                workdays += amount;
                continue;
            }

            // Live native price via the Level 1 service. D8/D33: the
            // resource must have a native price in the ACTIVE currency.
            // If it does not, the whole estimate is refused - no partial
            // bill and no fallback price. The import buy multiplier is a
            // real native field, applied only when it is inside (0, 10].
            const float pRub = g_pricing->getPrice(res, TSM_CURRENCY_RUB);
            const float pUsd = g_pricing->getPrice(res, TSM_CURRENCY_USD);
            const float pActive = (cur == TSM_CURRENCY_USD) ? pUsd : pRub;
            if (!TsmNativePriceUsable(pActive))
            {
                Logf("private_construction  ERROR UNPRICED  construction "
                     "material \"%s\" has no native price - the whole "
                     "estimate is refused (D33)", rname);
                if (failOut) *failOut = "unpriced construction material";
                return 0.0;
            }
            double mRub = 1.0, mUsd = 1.0;
            if (g_applyBuyMult)
            {
                float rawRub = *(const float*)((const BYTE*)res + TSM_RES_BUY_MULT_RUB);
                float rawUsd = *(const float*)((const BYTE*)res + TSM_RES_BUY_MULT_USD);
                if (rawRub > 0.0f && rawRub <= 10.0f) mRub = (double)rawRub;
                if (rawUsd > 0.0f && rawUsd <= 10.0f) mUsd = (double)rawUsd;
            }

            matRub += amount * (double)pRub * mRub;
            matUsd += amount * (double)pUsd * mUsd;
            matEntries++;
        }
    }

    if (!matEntries)
        { if (failOut) *failOut = "construction phases contain no materials"; return 0.0; }

    // v4: the investment pool is denominated in the ACTIVE currency, so
    // both legs cross the same way. USD: materials = the native USD leg,
    // which reads the game's own +0x5C price and its +0xAC import buy
    // multiplier; labour follows citizen_labor's effective default rate
    // (already USD-calibrated) x contractor overhead.
    //
    // $COST_WORK rides in the same vector as a "workers" record:
    // workdays, not a border-importable good, so it is priced at
    // our wage below instead of at a market price it has not got.
    float wage = EffectiveWorkdayRate();
    double labour = workdays * (double)wage;

    double material = (cur == TSM_CURRENCY_USD)
        ? matUsd
        : matRub;

    double total = (material + labour)
                 * (double)g_contractorMarkup
                 * (double)g_costMultiplier;

    // USD floor: divide the RUB minimum by usd_divisor unless an
    // explicit minimum_cost_usd is set.
    double minCost = g_minimumCost;
    if (cur == TSM_CURRENCY_USD) {
        minCost = (g_minimumCostUsd >= 0.0) ? g_minimumCostUsd
                                            : g_minimumCost / ((g_usdDivisor >= 1.0f) ? g_usdDivisor : 50.0f);
    }
    if (total < minCost) total = minCost;

    if (!(total > 0.0) || !(total <= 1.0e15))
        { if (failOut) *failOut = "priced total out of range"; return 0.0; }
    return total;
}

static double CostForBuilding(void* b)
{
    double out = 0.0;
    EnterCriticalSection(&g_lock);

    // v3.5.0: native first. It needs no panel capture at all - the
    // phase vector is on the type descriptor whether or not anyone has
    // opened a window - so with cost_source = auto a site prices on the
    // tick it is tagged. fund/panel below stay as the fallback (and as
    // the whole of cost_source = fund/panel), except after a hard D33
    // UNPRICED refusal.
    double native = 0.0;
    g_lastNativeFail = nullptr;
    g_lastLegacyFail = nullptr;
    if (g_costSource == COST_NATIVE || g_costSource == COST_AUTO)
        native = NativeCostForBuilding(b, &g_lastNativeFail);
    g_lastNative = native;

    // D33: an UNPRICED native estimate is a hard refusal. Do not replace it
    // with a fund row or a panel estimate - a different number is not a fix.
    const bool nativeUnpriced =
        g_lastNativeFail && strcmp(g_lastNativeFail, "unpriced construction material") == 0;

    auto it = g_buildingCaptures.find(b);
    if (it != g_buildingCaptures.end())
    {
        const BuildingCapture& cap = it->second;
        bool acceptFund  = (g_costSource == COST_FUND  || g_costSource == COST_AUTO);
        bool acceptPanel = (g_costSource == COST_PANEL || g_costSource == COST_AUTO);

        if (acceptFund && !nativeUnpriced)
        {
            out = FundCostLocked(cap);

            if (out <= 0.0 && g_acceptCurrency == ACC_RUB &&
                cap.fundRub == 0 && cap.fundUsd > 0 && !g_warnedWrongCurrency)
            {
                g_warnedWrongCurrency = true;
                Logf("private_construction  WARN  the fund row showed a dollar figure "
                     "($%d) but accept_currency is rub - the site will wait for a "
                     "ruble capture. Set accept_currency = usd or either to use it.",
                     cap.fundUsd);
            }
        }

        if (out <= 0.0 && acceptPanel && !nativeUnpriced && cap.lastEntire.workdays > 0)
            out = CostFromPanelMats(cap.lastEntire);
    }
    g_lastLegacy = out;
    LeaveCriticalSection(&g_lock);

    // Native wins when it produced anything; fund/panel otherwise.
    return (native > 0.0) ? native : out;
}

// The first run of digits anywhere in the row, with space/comma/NBSP
// grouping skipped. Rows are count-first in English ("12 of Concrete")
// but count-last in other languages ("Бетон: 12"); the old `_wtoi`
// only understood the first shape.
static int ParseGroupedIntAt(const wchar_t* p)
{
    if (!p) return 0;
    int n = 0;
    for (; *p; p++) {
        if (*p >= L'0' && *p <= L'9') n = n * 10 + (*p - L'0');
        else if (*p == L',' || *p == L' ' || *p == 0x00A0) continue;
        else break;
    }
    return n;
}

static int ParseGroupedInt(const wchar_t* s)
{
    if (!s) return 0;
    while (*s && (*s < L'0' || *s > L'9')) s++;
    return ParseGroupedIntAt(s);
}

static int ParseFundAmount(const wchar_t* s)
{
    const wchar_t* p = wcsrchr(s, L'$');
    if (p) { p++; }
    else {
        p = wcsstr(s, L" - ");
        if (!p) p = wcsstr(s, L" \u2013 ");   // en dash
        if (!p) p = wcsstr(s, L" \u2014 ");   // em dash
        if (p) { p += 3; }
        else   { return ParseGroupedInt(s); } // last resort: first number in the row
    }
    while (*p == L' ' || *p == L'$' || *p == 0x00A0) p++;

    return ParseGroupedIntAt(p);
}

static void* ResolveRowBuilding(const char* tag, void* ret, bool trustedRow)
{
    DWORD now = GetTickCount();

    if (t_cachedPanelBuilding &&
        (now - t_cachedPanelTick) < PANEL_CACHE_TTL_MS)
        return t_cachedPanelBuilding;

    if (!trustedRow)
        return nullptr;

    void* scanned = FindCurrentPanelBuilding();
    if (scanned)
    {
        if (g_probe) LogCallerRvaOnce(tag, ret, scanned);
        t_cachedPanelBuilding = scanned;
        t_cachedPanelTick     = now;
        return scanned;
    }

    if (g_probe) LogCallerRvaOnce(tag, ret, nullptr);
    return nullptr;
}

static void CaptureFund(const wchar_t* text, void* ret)
{
    if (!text) return;
    if (!PatMatch(PAT_FUND_ROW, text)) return;

    bool isRubles  = PatMatch(PAT_RUBLES, text);
    bool isDollars = PatMatch(PAT_DOLLARS, text);
    if (!isRubles && !isDollars) return;

    int n = ParseFundAmount(text);
    if (n <= 0) return;

    void* b = ResolveRowBuilding("FUND_PRINT", ret, false);
    if (g_probe) LogCallerRvaOnce("FUND_PRINT", ret, b);
    if (!b) return;

    DWORD now = GetTickCount();

    EnterCriticalSection(&g_lock);
    BuildingCapture& cap = g_buildingCaptures[b];

    if (cap.lastPrintTick > 0 && (now - cap.lastPrintTick > 300)) {
        cap.fundRub = 0;
        cap.fundUsd = 0;
    }
    cap.lastPrintTick = now;

    if (isRubles)  cap.fundRub = n;
    if (isDollars) cap.fundUsd = n;
    LeaveCriticalSection(&g_lock);
}

struct MatTag { PatId id; float PanelMats::* field; };
static const MatTag kMatTags[] = {
    { PAT_CONCRETE, &PanelMats::concrete },
    { PAT_GRAVEL,   &PanelMats::gravel   },
    { PAT_ASPHALT,  &PanelMats::asphalt  },
    { PAT_STEEL,    &PanelMats::steel    },
    { PAT_BRICKS,   &PanelMats::bricks   },
    { PAT_WOOD,     &PanelMats::boards   },
    { PAT_PREFABS,  &PanelMats::prefabs  },
    { PAT_ECOMPS,   &PanelMats::ecomps   },
    { PAT_MCOMPS,   &PanelMats::mcomps   },
};

static void CaptureRow(const wchar_t* text, void* ret)
{
    if (!text || !text[0]) return;

    bool isEntireRow = PatMatch(PAT_ENTIRE, text);

    bool isConstructionText = isEntireRow ||
        PatMatch(PAT_PHASE, text) || PatMatch(PAT_WORKDAYS, text);

    if (!isConstructionText) {
        for (size_t i = 0; i < sizeof(kMatTags) / sizeof(kMatTags[0]); i++) {
            if (PatMatch(kMatTags[i].id, text)) { isConstructionText = true; break; }
        }
    }
    if (!isConstructionText) return;

    void* b = ResolveRowBuilding(isEntireRow ? "ENTIRE_PRINT" : "ROW_PRINT",
                                 ret, isEntireRow);
    if (g_probe) LogCallerRvaOnce(isEntireRow ? "ENTIRE_PRINT" : "ROW_PRINT", ret, b);
    if (!b) return;

    EnterCriticalSection(&g_lock);
    BuildingCapture& cap = g_buildingCaptures[b];

    if (isEntireRow) {
        cap.lastEntire = cap.entire;
        cap.entire     = PanelMats{};
        cap.inEntire   = true;
        LeaveCriticalSection(&g_lock);
        return;
    }
    if (PatMatch(PAT_PHASE, text)) {
        cap.inEntire = false;
        LeaveCriticalSection(&g_lock);
        return;
    }

    if (!cap.inEntire) {
        LeaveCriticalSection(&g_lock);
        return;
    }

    if (PatMatch(PAT_WORKDAYS, text)) {
        int n = ParseGroupedInt(text);
        if (n > 0) cap.entire.workdays = n;
        LeaveCriticalSection(&g_lock);
        return;
    }

    for (size_t i = 0; i < sizeof(kMatTags) / sizeof(kMatTags[0]); i++) {
        if (PatMatch(kMatTags[i].id, text)) {
            int n = ParseGroupedInt(text);
            if (n > 0) cap.entire.*(kMatTags[i].field) = (float)n;
            break;
        }
    }
    LeaveCriticalSection(&g_lock);
}

typedef void (*t_PrintNoArg)(void* self, void* font, float x, float y,
                             unsigned long colour, const wchar_t* text);
typedef void (*t_Print)(void* self, void* font, float x, float y,
                        unsigned long colour, const wchar_t* fmt, ...);

static t_PrintNoArg o_PrintNoArg = nullptr;
static t_Print      o_Print      = nullptr;

static void h_PrintNoArg(void* self, void* font, float x, float y,
                         unsigned long colour, const wchar_t* text)
{
    void* ret = _ReturnAddress();
    __try { CaptureRow(text, ret); }
    __except (EXCEPTION_EXECUTE_HANDLER) {}

    if (o_PrintNoArg) o_PrintNoArg(self, font, x, y, colour, text);
}

static void h_Print(void* self, void* font, float x, float y,
                    unsigned long colour, const wchar_t* fmt, ...)
{
    wchar_t buf[1024];
    buf[0] = 0;

    if (fmt) {
        if (wcschr(fmt, L'%') == nullptr) {
            wcsncpy_s(buf, _countof(buf), fmt, _TRUNCATE);
        } else {
            va_list ap;
            va_start(ap, fmt);
            _vsnwprintf_s(buf, _countof(buf), _TRUNCATE, fmt, ap);
            va_end(ap);
        }
    }

    void* ret = _ReturnAddress();
    __try {
        CaptureRow(buf, ret);
        CaptureFund(buf, ret);
    } __except (EXCEPTION_EXECUTE_HANDLER) {}

    if (o_Print) o_Print(self, font, x, y, colour, L"%ls", buf);
}

// ---------------------------------------------------------------- state

struct SiteState {
    double totalCost;
    double totalPaid;
    bool   stalled;
    bool   waiting;
    bool   waitLogged;
    DWORD  lastLogAt;

    // v3.4.4: the handoff to the engine's own construction.
    // handoffWaitedDays accumulates in-game days spent parked at the
    // handoff ceiling *without engine progress*; handoffTimedOut latches
    // once it has stalled past handoff_timeout_days, after which this
    // plugin finishes the site itself. handoffLastProgress is the
    // building+0x604 value at the last parked tick - any forward motion
    // resets the clock. handoffLogged stops the "waiting for the engine"
    // line repeating every tick.
    float  handoffWaitedDays = 0.0f;
    float  handoffLastProgress = 0.0f;
    bool   handoffTimedOut   = false;
    bool   handoffLogged     = false;
};

static std::unordered_map<void*, SiteState> g_sites;
static std::vector<void*> g_activeSites;
static bool g_paused = false;

static bool ReadBuildingPosition(void* b, float* outX, float* outZ)
{
    if (!TsmIsValidGamePtr(b) || !ReadablePtr(b, 0x400)) return false;

    // Primary: C3D_NODE is embedded at bld + 0x320 (not a pointer).
    // The translation (position) vector is Row 3 of the 4x4 matrix at
    // +0x30 (X) / +0x38 (Z) — real world coords in hundreds/thousands of
    // metres. Reject unit-vector rows with |x| > 1.5 || |z| > 1.5.
    // Then pointer dereference, then bbox fallback for construction sites.
    const BYTE* nodeDirect = (const BYTE*)b + 0x320;
    if (ReadablePtr(nodeDirect, 0x40)) {
        float x = *(const float*)(nodeDirect + 0x30);
        float z = *(const float*)(nodeDirect + 0x38);
        if (x == x && z == z && (fabsf(x) > 1.5f || fabsf(z) > 1.5f) &&
            fabsf(x) < 100000.0f && fabsf(z) < 100000.0f) {
            *outX = x;
            *outZ = z;
            return true;
        }
    }

    const BYTE* nodePtr = *(const BYTE* const*)((const BYTE*)b + 0x320);
    if (TsmIsValidGamePtr(nodePtr) && ReadablePtr(nodePtr, 0x40)) {
        float x = *(const float*)(nodePtr + 0x30);
        float z = *(const float*)(nodePtr + 0x38);
        if (x == x && z == z && (fabsf(x) > 1.5f || fabsf(z) > 1.5f) &&
            fabsf(x) < 100000.0f && fabsf(z) < 100000.0f) {
            *outX = x;
            *outZ = z;
            return true;
        }
    }

    if (ReadablePtr((const BYTE*)b + TSM_BLD_BBOX, sizeof(void*))) {
        BYTE* bb = *(BYTE**)((const BYTE*)b + TSM_BLD_BBOX);
        if (bb && ReadablePtr(bb + 0x20, 16)) {
            const float* mn = *(const float**)(bb + 0x20);
            if (mn && ReadablePtr(mn, 16)) {
                *outX = mn[1];
                *outZ = mn[3];
                return true;
            }
        }
    }
    return false;
}

static bool IsExcludedType(int type)
{
    for (int i = 0; i < g_excludeTypeCount; i++)
        if (g_excludeTypes[i] == type) return true;
    return false;
}

static void GetSiteName(void* b, wchar_t* out, size_t n)
{
    out[0] = 0;
    if (g_enterprise && (!g_enterprise->getBuildingCustomName(b, out, n) || !out[0])) {
        const char* obj = g_enterprise->getBuildingObjectName(b);
        MultiByteToWideChar(CP_UTF8, 0, obj[0] ? obj : "Site", -1, out, (int)n);
    }
    if (!out[0]) wcscpy_s(out, n, L"Site");
}

static void LogSiteProgress(const SiteState& st, void* b, const char* tag)
{
    wchar_t name[128] = {0};
    GetSiteName(b, name, 128);
    Logf("private_construction  [%s] \"%ls\" total %.2f  paid %.2f  (%.0f%%)",
         tag, name, st.totalCost, st.totalPaid,
         st.totalCost > 0.0 ? (st.totalPaid / st.totalCost) * 100.0 : 0.0);
}

// v3.4.2: the crossing diagnostic. One line on the tick a claimed site's
// progress reaches 1.0, carrying the fields a mesh-visibility fault
// would touch, so a session in which a completed building goes invisible
// says whether it differs from one that renders.
static void LogCompletion(void* b, const SiteState& st, float before,
                          float after, float advance, float dayDelta)
{
    wchar_t name[128] = {0};
    GetSiteName(b, name, 128);

    int type = TsmBuildingType(b);

    // TSM_BLD_NODE is the building's current mesh node - the thing a
    // failed phase-to-final swap leaves pointing at nothing or at a
    // scaffold. TSM_BLD_BBOX is what the renderer culls on. Both are
    // read defensively: this runs under the pass's own SEH, but a null
    // node is exactly the shape of the fault being chased, so a null in
    // the log is the answer, not a reason to bail.
    void* node = nullptr;
    void* bbox = nullptr;
    if (ReadablePtr((BYTE*)b + TSM_BLD_NODE, sizeof(void*)))
        node = *(void**)((BYTE*)b + TSM_BLD_NODE);
    if (ReadablePtr((BYTE*)b + TSM_BLD_BBOX, sizeof(void*)))
        bbox = *(void**)((BYTE*)b + TSM_BLD_BBOX);

    int dem = TsmIsDemolished(b) ? 1 : 0;

    Logf("private_construction  [COMPLETE] \"%ls\" type %d  built %.4f -> %.4f  "
         "(step %.4f, dayDelta %.4f, clamp %s, node %p, bbox %p, demolished %d, paid %.2f)",
         name, type, before, after, advance, dayDelta,
         g_maxTickAdvance > 0.0f ? "on" : "off", node, bbox, dem, st.totalPaid);
}

// ---------------------------------------------------------------- persistence

#define CONST_MAGIC    "TSMCNST1"
#define CONST_VERSION  1u
#define CONST_FILENAME "tesmio_construction.dat"

#pragma pack(push, 1)
struct ConstSaveHeader {
    char     magic[8];
    uint32_t version;
    uint32_t recordCount;
    uint32_t reserved;
};
struct ConstSaveRecord {
    char     objectName[64];
    float    posX;
    float    posZ;
    double   totalCost;
    double   totalPaid;
    uint8_t  waiting;
    uint8_t  stalled;
};
#pragma pack(pop)

static std::vector<ConstSaveRecord> g_pendingSites;

static void SaveSites(const char* dir)
{
    std::vector<ConstSaveRecord> records;
    EnterCriticalSection(&g_lock);
    records.reserve(g_sites.size());

    for (const auto& kv : g_sites) {
        float px = 0.0f, pz = 0.0f;
        ReadBuildingPosition(kv.first, &px, &pz);

        ConstSaveRecord rec = {};
        const char* objName = g_enterprise ? g_enterprise->getBuildingObjectName(kv.first) : "";
        strncpy_s(rec.objectName, sizeof(rec.objectName), objName ? objName : "", _TRUNCATE);
        rec.posX      = px;
        rec.posZ      = pz;
        rec.totalCost = kv.second.totalCost;
        rec.totalPaid = kv.second.totalPaid;
        rec.waiting   = kv.second.waiting ? 1 : 0;
        rec.stalled   = kv.second.stalled ? 1 : 0;
        records.push_back(rec);
    }

    size_t count = records.size();
    size_t total = sizeof(ConstSaveHeader) + count * sizeof(ConstSaveRecord);
    std::vector<BYTE> buf(total);

    ConstSaveHeader hdr;
    memcpy(hdr.magic, CONST_MAGIC, 8);
    hdr.version     = CONST_VERSION;
    hdr.recordCount = (uint32_t)count;
    hdr.reserved    = 0;
    memcpy(buf.data(), &hdr, sizeof(hdr));

    if (count > 0)
        memcpy(buf.data() + sizeof(hdr), records.data(), count * sizeof(ConstSaveRecord));
    LeaveCriticalSection(&g_lock);

    if (g_persistence->writeAtomic(dir, CONST_FILENAME, buf.data(), total))
        Logf("private_construction  saved %zu construction site(s) into %s\\%s",
             count, dir, CONST_FILENAME);
    else
        Logf("private_construction  WARN  could not write %s\\%s", dir, CONST_FILENAME);
}

static void LoadSites(const char* dir)
{
    size_t len = 0;
    void* raw = g_persistence->readFile(dir, CONST_FILENAME, &len);
    if (!raw) {
        EnterCriticalSection(&g_lock);
        g_sites.clear();
        g_pendingSites.clear();
        g_buildingCaptures.clear();
        g_warnedWrongCurrency = false;
        g_warnedCrossRate     = false;
        LeaveCriticalSection(&g_lock);
        Logf("private_construction  no %s in this save - starting empty", CONST_FILENAME);
        return;
    }

    if (len < sizeof(ConstSaveHeader)) {
        Logf("private_construction  %s is too short - ignored", CONST_FILENAME);
        free(raw);
        return;
    }

    ConstSaveHeader hdr;
    memcpy(&hdr, raw, sizeof(hdr));
    if (memcmp(hdr.magic, CONST_MAGIC, 8) != 0 || hdr.version != CONST_VERSION) {
        Logf("private_construction  %s header invalid - ignored", CONST_FILENAME);
        free(raw);
        return;
    }

    size_t want = sizeof(hdr) + (size_t)hdr.recordCount * sizeof(ConstSaveRecord);
    if (want > len) {
        Logf("private_construction  %s claims %u records but is only %zu bytes - ignored",
             CONST_FILENAME, hdr.recordCount, len);
        free(raw);
        return;
    }

    EnterCriticalSection(&g_lock);
    g_sites.clear();
    g_pendingSites.clear();
    g_buildingCaptures.clear();
    g_warnedWrongCurrency = false;
    g_warnedCrossRate     = false;
    g_pendingSites.reserve(hdr.recordCount);

    const BYTE* p = (const BYTE*)raw + sizeof(hdr);
    for (uint32_t i = 0; i < hdr.recordCount; i++) {
        ConstSaveRecord r;
        memcpy(&r, p, sizeof(r));
        p += sizeof(r);
        g_pendingSites.push_back(r);
    }
    LeaveCriticalSection(&g_lock);

    free(raw);
    Logf("private_construction  staged %u construction site(s) from %s\\%s",
         hdr.recordCount, dir, CONST_FILENAME);
}

static void OnSaveLoad(TsmSaveEvent ev, const char* dir, void* /*ud*/)
{
    if (!dir || !g_persistence) return;
    if      (ev == TSM_SAVE_WRITING) SaveSites(dir);
    else if (ev == TSM_SAVE_LOADING) {
        // A fragment accumulated before the load belongs to the old world.
        // OnTick and this callback can run on different threads.
        EnterCriticalSection(&g_lock);
        g_pendingDayDelta = 0.0f;
        LeaveCriticalSection(&g_lock);
        LoadSites(dir);
    }
}

static void MatchPendingSites(TsmBuildingSpan* span)
{
    if (g_pendingSites.empty() || !span || span->count == 0) return;

    std::vector<ConstSaveRecord> stillPending;
    stillPending.reserve(g_pendingSites.size());

    int matched = 0;
    for (const auto& rec : g_pendingSites) {
        void* found = nullptr;
        for (size_t j = 0; j < span->count; j++) {
            BYTE* b = (BYTE*)span->begin[j];
            if (!TsmIsValidGamePtr(b) || !ReadablePtr(b, TSM_BLD_MIN_READABLE) || TsmIsDemolished(b))
                continue;
            if (g_sites.count(b)) continue;

            float px = 0.0f, pz = 0.0f;
            if (!ReadBuildingPosition(b, &px, &pz)) continue;

            if (fabsf(px - rec.posX) < 3.5f && fabsf(pz - rec.posZ) < 3.5f) {
                if (rec.objectName[0] && g_enterprise) {
                    const char* on = g_enterprise->getBuildingObjectName(b);
                    if (on && on[0] && strcmp(on, rec.objectName) != 0) continue;
                }
                found = b;
                break;
            }
        }

        if (found) {
            SiteState st = {};
            st.totalCost  = rec.totalCost;
            st.totalPaid  = rec.totalPaid;
            st.waiting    = rec.waiting != 0;
            st.stalled    = rec.stalled != 0;
            st.waitLogged = false;
            st.lastLogAt  = 0;
            g_sites[found] = st;
            matched++;
        } else {
            stillPending.push_back(rec);
        }
    }

    if (matched > 0 && g_probe) {
        Logf("private_construction  matched %d pending site(s) from save, %zu still pending",
             matched, stillPending.size());
    }

    g_pendingSites.swap(stillPending);
}

// ---------------------------------------------------------------- construction pass

static void RunConstructionPass(float dayDelta)
{
    BYTE* game = TsmGameObject(g_exeBase);
    if (!game || !ReadablePtr(game, TSM_BLD_MIN_READABLE)) return;

    TsmBuildingSpan span;
    if (!TsmGetBuildings(game, &span)) return;

    std::unordered_set<void*> live;
    live.reserve(span.count);

    if (!(dayDelta > 0.0f) || !(dayDelta == dayDelta)) dayDelta = 0.0f;

    double pool = g_enterprise->getPrivateInvestmentPool();
    float progressDelta = g_dailyBuildRate * dayDelta;

    EnterCriticalSection(&g_lock);
    MatchPendingSites(&span);
    g_activeSites.clear();

    for (size_t i = 0; i < span.count; i++) {
        BYTE* b = (BYTE*)span.begin[i];
        if (!TsmIsValidGamePtr(b)) continue;
        if (!ReadablePtr(b, TSM_BLD_MIN_READABLE)) continue;
        if (TsmIsDemolished(b) || TsmIsDecayed(b)) continue;

        int type = TsmBuildingType(b);
        if (type < 0) continue;
        if (IsExcludedType(type)) continue;

        float progress = *(float*)(b + TSM_BLD_BUILT);

        if (progress >= 1.0f) {
            auto it = g_sites.find(b);
            if (it != g_sites.end()) {
                // v3.4.2: the plugin only ever advances a site whose
                // price it has captured, so a site that reaches 1.0
                // with no captured price was finished by something
                // else - normally the game's own "Fund construction
                // using rubles" option, which completes the whole
                // building at once and is expected to look instant.
                // Erasing the entry is right; logging it as FINISHED
                // at 0% read as though the plugin had built it.
                if (g_probe) {
                    if (it->second.totalCost > 0.0) {
                        LogSiteProgress(it->second, b, "FINISHED");
                    } else {
                        wchar_t name[128] = {0};
                        GetSiteName(b, name, 128);
                        Logf("private_construction  [ALREADY-BUILT] \"%ls\" reached 100%% "
                             "without a captured price (the game's own fund-construction "
                             "finishes a building at once) - nothing here for this plugin "
                             "to build", name);
                    }
                }
                g_sites.erase(it);
                g_buildingCaptures.erase(b);
            }
            continue;
        }

        if (!g_enterprise->isBuildingPrivate(b)) continue;

        live.insert(b);

        SiteState& st = g_sites[b];

        if (st.totalCost <= 0.0) {
            double captured = CostForBuilding(b);

            if (captured > 0.0) {
                st.waiting   = false;
                st.totalCost = captured;
                st.totalPaid = captured * (double)progress;
                st.stalled   = false;

                g_buildingCaptures.erase(b);

                if (g_probe) {
                    wchar_t name[128] = {0};
                    GetSiteName(b, name, 128);
                    Logf("private_construction  [CLAIMED] \"%ls\" total %.2f %s (%s)",
                         name, captured, TsmCurrencySymbol(g_currency),
                         g_lastNative > 0.0 ? "native phase pricing" : "panel capture bound to site");
                    // v3.5.0 shadow: native vs fund/panel side by side,
                    // so one session calibrates the native walk.
                    if (g_lastNative > 0.0 && g_lastLegacy > 0.0)
                        Logf("private_construction  [COST] \"%ls\" native %.2f vs fund/panel %.2f (ratio %.3f)",
                             name, g_lastNative, g_lastLegacy,
                             g_lastLegacy > 0.0 ? g_lastNative / g_lastLegacy : 0.0);
                    else if (g_lastNative > 0.0)
                        Logf("private_construction  [COST] \"%ls\" native %.2f %s (no fund/panel capture to compare)",
                             name, g_lastNative, TsmCurrencySymbol(g_currency));
                }
            } else {
                st.waiting = true;
                g_activeSites.push_back(b);

                if (!st.waitLogged) {
                    st.waitLogged = true;
                    if (g_probe) {
                        wchar_t name[128] = {0};
                        GetSiteName(b, name, 128);
                        if (g_lastNativeFail)
                            Logf("private_construction  [WAITING] \"%ls\" native unavailable: %s - open its construction panel once",
                                 name, g_lastNativeFail);
                        else if (g_lastLegacyFail)
                            Logf("private_construction  [WAITING] \"%ls\" estimate refused: %s - no site is claimed",
                                 name, g_lastLegacyFail);
                        else
                            Logf("private_construction  [WAITING] \"%ls\" no captured price yet - open its construction panel once",
                                 name);
                    }
                }
                continue;
            }
        }

        // v3.4.3: decide how far THIS plugin may take the site. With the
        // handoff on it stops at finish_handoff_frac and leaves the last
        // sliver to the engine's own construction, whose completion the
        // engine actually runs (finished mesh, bbox, decal clear). Once a
        // site has sat at that ceiling for handoff_timeout_days this
        // plugin gives up waiting and finishes it itself.
        float effCap = PROGRESS_CAP;
        if (g_finishHandoffFrac > 0.0f &&
            g_finishHandoffFrac < PROGRESS_CAP &&
            !st.handoffTimedOut)
        {
            if (progress >= g_finishHandoffFrac - 0.0001f) {
                // v3.4.4: stall timeout, not wall timeout. The user's
                // session showed the engine CAN take the last sliver (it
                // moved a parked site 0.9976 -> 0.9990) but only after the
                // plugin's own progress writes had already consumed the
                // workday/material state the domestic builders needed, so
                // deliveries stalled and 3-5 days ran out mid-recovery.
                // Any forward motion of building+0x604 resets the clock;
                // only a site the engine truly ignores times out.
                //
                // First parked tick only arms the baseline (a fresh
                // SiteState zeroes handoffLastProgress, so comparing
                // against it would always read as motion once).
                if (!st.handoffLogged) {
                    st.handoffLastProgress = progress;
                } else if (progress > st.handoffLastProgress + 0.000001f) {
                    st.handoffWaitedDays = 0.0f;
                    st.handoffLastProgress = progress;
                } else {
                    st.handoffWaitedDays += dayDelta;
                }

                if (g_handoffTimeoutDays > 0.0f &&
                    st.handoffWaitedDays >= g_handoffTimeoutDays)
                {
                    st.handoffTimedOut = true;
                    if (g_probe) {
                        wchar_t name[128] = {0};
                        GetSiteName(b, name, 128);
                        Logf("private_construction  [HANDOFF-TIMEOUT] \"%ls\" the engine did "
                             "not advance it in %.1f day(s) (stalled at %.4f) - completing it here; "
                             "expect an invisible finish, deliver materials / enable domestic construction instead",
                             name, st.handoffWaitedDays, progress);
                    }
                } else {
                    effCap = g_finishHandoffFrac;
                    if (!st.handoffLogged) {
                        st.handoffLogged = true;
                        st.handoffLastProgress = progress;
                        st.lastLogAt = GetTickCount();
                        if (g_probe) {
                            wchar_t name[128] = {0};
                            GetSiteName(b, name, 128);
                            if (g_handoffTimeoutDays > 0.0f)
                                Logf("private_construction  [HANDOFF] \"%ls\" parked at %.0f%% (now %.4f) - "
                                     "leaving the final step to the engine's construction; "
                                     "timeout %.1f day(s) of no engine progress",
                                     name, g_finishHandoffFrac * 100.0f, progress,
                                     g_handoffTimeoutDays);
                            else
                                Logf("private_construction  [HANDOFF] \"%ls\" parked at %.0f%% (now %.4f) - "
                                     "awaiting state inspection; a domestic construction office must "
                                     "finish the last step (handoff_timeout_days = 0 waits forever)",
                                     name, g_finishHandoffFrac * 100.0f, progress);
                        }
                    } else {
                        // v3.4.4 heartbeat: while parked, say where the
                        // engine has got to, same 15 s cadence as BUILDING.
                        DWORD nowHb = GetTickCount();
                        if (g_probe && nowHb - st.lastLogAt > 15000) {
                            st.lastLogAt = nowHb;
                            wchar_t name[128] = {0};
                            GetSiteName(b, name, 128);
                            if (g_handoffTimeoutDays > 0.0f)
                                Logf("private_construction  [WAITING] \"%ls\" %.4f stalled %.2f/%.1f day(s) - "
                                     "engine holds the last step",
                                     name, progress, st.handoffWaitedDays,
                                     g_handoffTimeoutDays);
                            else
                                Logf("private_construction  [WAITING] \"%ls\" %.4f awaiting state inspection "
                                     "for %.2f day(s) - domestic construction office holds the last step",
                                     name, progress, st.handoffWaitedDays);
                        }
                    }
                }
            }
        }

        float advance = progressDelta;

        // v3.4.2 backstop: bound one tick's step so even a fallback
        // completion (handoff off or timed out) is not one huge jump.
        if (g_maxTickAdvance > 0.0f && advance > g_maxTickAdvance)
            advance = g_maxTickAdvance;

        if (progress + advance > effCap)
            advance = effCap - progress;

        if (advance <= 0.00001f) {
            g_activeSites.push_back(b);
            continue;
        }

        double costThisTick = st.totalCost * (double)advance;
        if (costThisTick < 0.0) costThisTick = 0.0;

        if (pool >= costThisTick) {
            g_enterprise->modifyPrivateInvestmentPool(-costThisTick, "private_construction_work");
            pool -= costThisTick;
            st.totalPaid += costThisTick;

            float before = progress;
            *(float*)(b + TSM_BLD_BUILT) += advance;
            float after = *(float*)(b + TSM_BLD_BUILT);

            bool wasStalled = st.stalled;
            st.stalled = false;

            if (wasStalled && g_probe) {
                wchar_t name[128] = {0};
                GetSiteName(b, name, 128);
                Logf("private_construction  [RESUMED] \"%ls\" pool recovered", name);
            }

            // One line on the tick that carries the site into "finished":
            // the step size, the day delta that produced it, and the
            // fields a failed mesh swap would leave behind.
            if (g_probeCompletion && before < PROGRESS_CAP && after >= PROGRESS_CAP)
                LogCompletion(b, st, before, after, advance, dayDelta);

            DWORD now = GetTickCount();
            if (g_probe && now - st.lastLogAt > 15000) {
                st.lastLogAt = now;
                LogSiteProgress(st, b, "BUILDING");
            }
        } else if (!st.stalled) {
            st.stalled = true;
            if (g_probe) {
                wchar_t name[128] = {0};
                GetSiteName(b, name, 128);
                Logf("private_construction  [STALLED] \"%ls\" needs %.2f but pool has %.2f",
                     name, costThisTick, pool);
            }
        }

        g_activeSites.push_back(b);
    }

    for (auto it = g_sites.begin(); it != g_sites.end(); ) {
        if (live.count(it->first)) ++it;
        else                        it = g_sites.erase(it);
    }

    LeaveCriticalSection(&g_lock);
}

static void OnTick(void* /*ud*/, float /*dt*/, float dayDelta)
{
    // dayDelta is PER FRAME (sim_clock_api.h): the frames this throttle
    // skips must not lose their share of the day. Accumulate every valid
    // frame and hand the whole accumulated delta to the pass that runs.
    if (dayDelta > 0.0f && dayDelta == dayDelta) {
        EnterCriticalSection(&g_lock);
        g_pendingDayDelta += dayDelta;
        if (g_pendingDayDelta > kMaxPendingDays)
            g_pendingDayDelta = kMaxPendingDays;
        LeaveCriticalSection(&g_lock);
    }

    static DWORD lastRun = 0;
    DWORD now = GetTickCount();
    if (now - lastRun < (DWORD)g_tickIntervalMs) return;
    lastRun = now;

    // The pass caps a site's PROGRESS step at g_maxTickAdvance (a 0..1
    // fraction), so the day fragment consumed here is that fraction's
    // day-equivalent. Capping days by the raw fraction made the limit
    // bind at ordinary fast-forward speeds.
    float dayCap = 0.0f;
    if (g_maxTickAdvance > 0.0f && g_dailyBuildRate > 0.0f)
        dayCap = g_maxTickAdvance / g_dailyBuildRate;

    EnterCriticalSection(&g_lock);
    float passDelta = g_pendingDayDelta;
    if (dayCap > 0.0f && passDelta > dayCap) passDelta = dayCap;
    g_pendingDayDelta -= passDelta;
    if (!(g_pendingDayDelta > 0.0f)) g_pendingDayDelta = 0.0f;  // drain + NaN
    LeaveCriticalSection(&g_lock);

    // A faulted pass deliberately drops its fragment: re-running it could
    // double-charge a site that already advanced before the fault.
    __try { RunConstructionPass(passDelta); }
    __except (FaultFilter("private_construction pass", GetExceptionInformation()))
    {
        Logf("private_construction  the pass faulted - skipped this frame");
    }
}

static void OnPause(void* /*ud*/, int paused)
{
    g_paused = paused != 0;
}

static float DrawOverlay(void* /*ud*/, float x, float y, float step)
{
    std::vector<void*> sites;
    EnterCriticalSection(&g_lock);
    sites = g_activeSites;
    LeaveCriticalSection(&g_lock);

    if (sites.empty()) return y;

    const wchar_t* curSym = (TsmCurrencyActive(g_currency) == TSM_CURRENCY_USD)
        ? L"USD" : L"RUB";
    wchar_t buf[256];

    _snwprintf_s(buf, _countof(buf), _TRUNCATE,
                 L"=== PRIVATE CONSTRUCTION (currency: %ls): %zu active site(s) ===",
                 curSym, sites.size());
    g_ui->drawTextOutlined(x, y, 0xFF70C8FF, buf);
    y += step;

    for (size_t i = 0; i < sites.size() && i < 4; i++) {
        void* b = sites[i];
        if (!TsmIsValidGamePtr(b) || !ReadablePtr(b, TSM_BLD_MIN_READABLE))
            continue;

        EnterCriticalSection(&g_lock);
        auto it = g_sites.find(b);
        if (it == g_sites.end()) {
            LeaveCriticalSection(&g_lock);
            continue;
        }
        SiteState copy = it->second;
        LeaveCriticalSection(&g_lock);

        wchar_t name[128] = {0};
        GetSiteName(b, name, 128);

        float progress = *(float*)((BYTE*)b + TSM_BLD_BUILT);

        const wchar_t* statusStr;
        unsigned long  statusColor;
        if (copy.waiting) {
            statusStr = L"WAITING FOR PRICE";
            statusColor = 0xFFFFA040;
        } else if (g_paused) {
            statusStr = L"PAUSED";
            statusColor = 0xFFDCDCDC;
        } else if (copy.stalled) {
            statusStr = L"STALLED - POOL DRY";
            statusColor = 0xFFFF7060;
        } else if (progress >= 1.0f) {
            statusStr = L"COMPLETED";
            statusColor = 0xFFC8A0FF;
        } else if (g_finishHandoffFrac > 0.0f &&
                   g_finishHandoffFrac < PROGRESS_CAP &&
                   !copy.handoffTimedOut &&
                   progress >= g_finishHandoffFrac - 0.0001f) {
            // v3.5.1: parked at the handoff ceiling - the state sends a
            // domestic Construction Office to take the last step (the
            // engine's own construction), framed as the State
            // Acceptance Commission signing off on the site.
            statusStr = L"AWAITING STATE INSPECTION";
            statusColor = 0xFF60D0FF;
        } else {
            statusStr = L"BUILDING";
            statusColor = 0xFF90F090;
        }

        if (copy.waiting) {
            _snwprintf_s(buf, _countof(buf), _TRUNCATE,
                         L"  %ls  %5.1f%%  [%ls]",
                         name, progress * 100.0f, statusStr);
        } else {
            _snwprintf_s(buf, _countof(buf), _TRUNCATE,
                         L"  %ls  %5.1f%%  paid %.0f %ls / %.0f %ls  [%ls]",
                         name, progress * 100.0f,
                         copy.totalPaid, curSym, copy.totalCost, curSym, statusStr);
        }
        g_ui->drawTextOutlined(x, y, statusColor, buf);
        y += step;
    }

    return y;
}

static size_t ParseRva(const char* s)
{
    if (!s || !s[0]) return 0;
    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) s += 2;
    return (size_t)strtoul(s, nullptr, 16);
}

// "4C 8B DC 55 56" -> bytes. Stops at anything that is not a hex pair.
static size_t ParseHexBytes(const char* s, BYTE* out, size_t max)
{
    size_t n = 0;
    while (s && *s && n < max) {
        while (*s == ' ' || *s == ',' || *s == '\t') s++;
        if (!*s) break;

        unsigned v = 0;
        int got = 0;
        for (; got < 2; got++) {
            char c = *s;
            unsigned d;
            if      (c >= '0' && c <= '9') d = (unsigned)(c - '0');
            else if (c >= 'a' && c <= 'f') d = (unsigned)(c - 'a' + 10);
            else if (c >= 'A' && c <= 'F') d = (unsigned)(c - 'A' + 10);
            else break;
            v = (v << 4) | d;
            s++;
        }
        if (got != 2) break;
        out[n++] = (BYTE)v;
    }
    return n;
}

static void ParseExcludeList(const char* s)
{
    g_excludeTypeCount = 0;
    if (!s || !s[0]) {
        g_excludeTypes[g_excludeTypeCount++] = 0;
        g_excludeTypes[g_excludeTypeCount++] = 1;
        g_excludeTypes[g_excludeTypeCount++] = 20;
        g_excludeTypes[g_excludeTypeCount++] = 30;
        return;
    }
    while (*s && g_excludeTypeCount < MAX_EXCLUDE) {
        while (*s == ' ' || *s == ',' || *s == '\t') s++;
        if (!*s) break;
        g_excludeTypes[g_excludeTypeCount++] = atoi(s);
        while (*s && *s != ',') s++;
    }
}

static void ReadSettings(void)
{
    const char* ini = "plugins\\private_construction.ini";
    char v[384];

    g_enabled        = H->configInt(ini, "private_construction", "enabled", 1);
    g_probe          = H->configInt(ini, "private_construction", "probe",   1);
    g_showOverlay    = H->configInt(ini, "private_construction", "show_overlay", 1);
    g_tickIntervalMs = H->configInt(ini, "private_construction", "tick_interval_ms", 250);

    if (H->configString(ini, "private_construction", "daily_build_rate", v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f > 0.0f && f <= 1.0f) g_dailyBuildRate = f;
    }
    if (H->configString(ini, "private_construction", "contractor_wage_per_day", v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f > 0.0f) g_wagePerDay = f;
    }
    if (H->configString(ini, "private_construction", "cost_multiplier", v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f > 0.0f) g_costMultiplier = f;
    }
    if (H->configString(ini, "private_construction", "contractor_markup", v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f > 0.0f) g_contractorMarkup = f;
    }
    // v4: contractor overhead multiplier for the labor-driven workday
    // rate (heavy equipment, cranes, contractor overhead). 2.50 ships
    // the requested default; must be positive to take effect.
    if (H->configString(ini, "private_construction", "contractor_overhead_mult", v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f > 0.0f && f <= 100.0f) g_contractorOverheadMult = f;
    }
    if (H->configString(ini, "private_construction", "minimum_cost", v, sizeof(v), "") && v[0]) {
        double d = atof(v);
        if (d >= 0.0) g_minimumCost = d;
    }
    if (H->configString(ini, "private_construction", "usd_to_rub", v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f > 0.01f) g_usdToRub = f;
    }

    // v4: GOVERNING CURRENCIES. USD is now the active currency with
    // enterprise_base; these keys carry USD-calibrated values (or a
    // divisor) that native and panel pricing use instead of the RUB
    // figures above. An explicit *_usd key always wins over the divisor.
    g_usdDivisor = 50.0f;
    if (H->configString(ini, "private_construction", "usd_divisor",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 1.0f && f <= 1000.0f) g_usdDivisor = f;
    }
    if (H->configString(ini, "private_construction",
                        "contractor_wage_per_day_usd",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f > 0.0f) g_wagePerDayUsd = f;
    }
    if (H->configString(ini, "private_construction",
                        "minimum_cost_usd",
                        v, sizeof(v), "") && v[0]) {
        double d = atof(v);
        if (d >= 0.0) g_minimumCostUsd = d;
    }

    // v3.4.2: the fast-forward guard and the crossing diagnostic. The
    // clamp accepts 0 (off) through 1 (a tick may commit the whole
    // building), so a value outside that is left at the default rather
    // than silently clamped to something the operator did not ask for.
    if (H->configString(ini, "private_construction", "max_tick_advance", v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 0.0f && f <= 1.0f) g_maxTickAdvance = f;
    }
    g_probeCompletion = H->configInt(ini, "private_construction", "probe_completion", 1);

    // v3.4.3: the engine handoff. The fraction must be inside (0,1):
    // 0 disables the handoff, and 1 would mean "hand off nothing".
    if (H->configString(ini, "private_construction", "finish_handoff_frac", v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 0.0f && f < 1.0f) g_finishHandoffFrac = f;
    }
    if (H->configString(ini, "private_construction", "handoff_timeout_days", v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 0.0f) g_handoffTimeoutDays = f;
    }

    if (H->configString(ini, "private_construction", "cost_source", v, sizeof(v), "auto") && v[0]) {
        if      (_stricmp(v, "panel")     == 0 ||
                 _stricmp(v, "materials") == 0 ||
                 _stricmp(v, "workdays")  == 0) g_costSource = COST_PANEL;
        else if (_stricmp(v, "fund")      == 0) g_costSource = COST_FUND;
        else if (_stricmp(v, "native")    == 0) g_costSource = COST_NATIVE;
        else                                    g_costSource = COST_AUTO;
    }

    // v3.5.0: apply the import buy multiplier when pricing natively.
    g_applyBuyMult = H->configInt(ini, "private_construction", "apply_buy_mult", 1);

    if (H->configString(ini, "private_construction", "accept_currency", v, sizeof(v), "rub") && v[0]) {
        if      (_stricmp(v, "usd")    == 0) g_acceptCurrency = ACC_USD;
        else if (_stricmp(v, "either") == 0) g_acceptCurrency = ACC_EITHER;
        else                                 g_acceptCurrency = ACC_RUB;
    }

    if (H->configString(ini, "private_construction", "exclude_types", v, sizeof(v), "") && v[0])
        ParseExcludeList(v);
    else
        ParseExcludeList(nullptr);

    // v3.4: localized row patterns. Empty key = the English default.
    for (int i = 0; i < PAT_COUNT; i++) {
        v[0] = 0;
        H->configString(ini, "private_construction", kPatIniKeys[i], v, sizeof(v), "");
        LoadPattern((PatId)i, v);
    }

    // v3.4: direct panel-builder hook. All three keys together, or
    // none - a partial set is refused at Start with a log line rather
    // than half-installed.
    v[0] = 0;
    if (H->configString(ini, "private_construction", "panel_builder_rva", v, sizeof(v), "") && v[0])
        g_pbRva = ParseRva(v);
    if (g_tsmAddr.usingIniOverrides &&
        (g_pbRva == 0 || g_pbRva == TSM_RVA_DEFAULT_PANEL_CONSTRUCT)) {
        g_pbRva = (size_t)g_tsmAddr.panel_construction;
    }

    v[0] = 0;
    if (H->configString(ini, "private_construction", "panel_builder_expect", v, sizeof(v), "") && v[0])
        g_pbExpectLen = ParseHexBytes(v, g_pbExpect, PB_MAX_EXPECT);

    g_pbStolen = (size_t)H->configInt(ini, "private_construction", "panel_builder_stolen", 0);
    if (g_pbStolen == 0) g_pbStolen = g_pbExpectLen;

    if (g_tickIntervalMs < 50)   g_tickIntervalMs = 50;
    if (g_tickIntervalMs > 5000) g_tickIntervalMs = 5000;
}

extern "C" __declspec(dllexport) unsigned TsmPluginApiVersion(void)
{
    return TSM_API_VERSION;
}

extern "C" __declspec(dllexport) int TsmPluginInit(const TsmHost* host, TsmPluginInfo* info)
{
    TsmBind(host);
    info->name    = "private_construction";
    info->version = "3.6.1";

    ReadSettings();
    if (!g_enabled) {
        Logf("private_construction  disabled");
        return 1;
    }
    return 0;
}

extern "C" __declspec(dllexport) int TsmPluginStart(void)
{
    if (!g_enabled) return 1;
    if (!TsmCheckExeBuild(H, "private_construction")) return 1;

    g_simClock = (const TsmSimClockApi*)H->consume(TSM_SERVICE_SIM_CLOCK, TSM_SIM_CLOCK_VERSION);
    if (!g_simClock) { Logf("private_construction  sim_clock missing"); return 1; }

    g_enterprise = (const TsmEnterpriseBaseApi*)H->consume(TSM_SERVICE_ENTERPRISE_BASE, TSM_ENTERPRISE_BASE_VERSION);
    if (!g_enterprise)
        g_enterprise = (const TsmEnterpriseBaseApi*)H->consume(TSM_SERVICE_ENTERPRISE_BASE, TSM_ENTERPRISE_BASE_VERSION_V7);
    if (!g_enterprise) { Logf("private_construction  enterprise_base missing"); return 1; }

    g_panels = (const TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS, TSM_UI_PANELS_VERSION);
    if (!g_panels) g_panels = (const TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS, TSM_UI_PANELS_VERSION_V5);
    if (!g_panels) g_panels = (const TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS, TSM_UI_PANELS_VERSION_V4);
    if (!g_panels) g_panels = (const TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS, TSM_UI_PANELS_VERSION_V3);
    if (!g_panels) g_panels = (const TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS, TSM_UI_PANELS_VERSION_V2);
    if (!g_panels) g_panels = (const TsmUiPanelsApi*)H->consume(TSM_SERVICE_UI_PANELS, TSM_UI_PANELS_VERSION_V1);

    g_labor = (const TsmCitizenLaborApi*)H->consume(TSM_SERVICE_CITIZEN_LABOR, TSM_CITIZEN_LABOR_VERSION);
    if (!g_labor) g_labor = (const TsmCitizenLaborApi*)H->consume(TSM_SERVICE_CITIZEN_LABOR, TSM_CITIZEN_LABOR_VERSION_V4);
    if (!g_labor) g_labor = (const TsmCitizenLaborApi*)H->consume(TSM_SERVICE_CITIZEN_LABOR, TSM_CITIZEN_LABOR_VERSION_V3);
    if (!g_labor) g_labor = (const TsmCitizenLaborApi*)H->consume(TSM_SERVICE_CITIZEN_LABOR, TSM_CITIZEN_LABOR_VERSION_V2);
    // v4: soft dependency - NULL labor falls back to the static
    // contractor_wage_per_day inside EffectiveWorkdayRate().
    if (!g_labor) Logf("private_construction  WARN  citizen_labor is not installed - using static contractor_wage_per_day");

    // v4: prefer the unified-currency iface; fall back to v3 (RUB-only)
    // routing so a mixed install still prices instead of unloading.
    g_currency = (const TsmCurrencyApi*)H->consume(TSM_SERVICE_CURRENCY, TSM_CURRENCY_VERSION);
    if (!g_currency)
        g_currency = (const TsmCurrencyApi*)H->consume(TSM_SERVICE_CURRENCY, TSM_CURRENCY_VERSION_V3);
    if (!g_currency) { Logf("private_construction  currency is not installed"); return 1; }

    g_pricing = (const TsmResourcePricingApi*)H->consume(TSM_SERVICE_RESOURCE_PRICING, TSM_RESOURCE_PRICING_VERSION);
    if (!g_pricing) g_pricing = (const TsmResourcePricingApi*)H->consume(TSM_SERVICE_RESOURCE_PRICING, TSM_RESOURCE_PRICING_VERSION_V1);
    if (!g_pricing) { Logf("private_construction  resource_pricing missing"); return 1; }

    g_persistence = (const TsmPersistenceApi*)H->consume(TSM_SERVICE_PERSISTENCE, TSM_PERSISTENCE_VERSION);
    if (!g_persistence) {
        Logf("private_construction  WARN  persistence service missing - site state will not survive save/load");
    } else {
        g_persHandle = g_persistence->registerHandler("private_construction", OnSaveLoad, nullptr);
        if (g_persHandle <= 0)
            Logf("private_construction  WARN  persistence refused handler");
    }

    if (g_showOverlay) {
        g_ui = (const TsmUiOverlayApi*)H->consume(TSM_SERVICE_UI_OVERLAY, TSM_UI_OVERLAY_VERSION);
    }

    // The two engine text entry points. Both are QEAAX: __cdecl, no
    // fastcall register use, which is what MSVC emits for a method that
    // is called from C++ variadic and non-variadic contexts in the same
    // translation unit. The non-variadic form ends in @Z; the variadic
    // adds a second Z for the ellipsis. PrintLeftUnicodeNoArg was hooked
    // correctly in every prior build; the variadic was not, and the
    // plugin refused to install as a result.
    bool okA = PatchIat(g_exe, DLL_ENGINE, SYM_PRINT_NOARG,
                        (void*)h_PrintNoArg, (void**)&o_PrintNoArg,
                        "private_construction PrintLeftUnicodeNoArg");
    bool okB = PatchIat(g_exe, DLL_ENGINE, SYM_PRINT_VARIADIC,
                        (void*)h_Print, (void**)&o_Print,
                        "private_construction PrintLeftUnicode");

    if (!okA || !okB) {
        Logf("private_construction  panel capture could not be installed (NoArg=%s, variadic=%s) - refusing to run",
             okA ? "yes" : "no", okB ? "yes" : "no");
        return 1;
    }

    // v3.4: the direct panel-builder hook. Optional - a refusal here
    // costs the language-independent binding and nothing else; the
    // text patterns and the stack-scan fallback keep working.
    if (g_pbRva) {
        // v3.4.1: the old gate read `>= 5`, which is the width of a
        // `jmp rel32`. InstallInlineHook does not write one of those -
        // it writes a 14-byte `jmp qword ptr [rip+0]` plus an absolute
        // address, and refuses anything shorter than 14 outright (see
        // src/tesmioloader.cpp, and docs/07-pitfalls.md for the save
        // crash that put the check there). A 5-byte configuration was
        // therefore accepted here and rejected one call later, which
        // reads in the log as a loader failure rather than as a bad
        // ini.
        //
        // `g_pbStolen <= g_pbExpectLen` closes the other half.
        // InstallInlineHook compares `memcmp(target, expect, stolen)`,
        // so a stolen count larger than the bytes the operator actually
        // supplied reads past them. g_pbExpect is a 32-byte
        // zero-initialised static, so the over-read is in-bounds of
        // this module and cannot fault - but it compares real target
        // bytes against zeroes, fails, and reports "prologue mismatch"
        // for an ini whose expect bytes were correct as far as they
        // went. Catch it here, where the message can say what is
        // actually wrong.
        if (g_pbExpectLen >= 14 && g_pbStolen >= 14 &&
            g_pbStolen <= g_pbExpectLen && g_pbStolen <= PB_MAX_EXPECT) {
            g_pbHooked = InstallInlineHook(g_exeBase + g_pbRva, (void*)h_PanelBuilder,
                                           (void**)&o_PanelBuilder, g_pbExpect,
                                           g_pbStolen, "private_construction panel builder");
            if (g_pbHooked) {
                Logf("private_construction  panel builder hooked at rva 0x%zX (%zu byte(s) stolen) - "
                     "building binding is direct and language-independent",
                     g_pbRva, g_pbStolen);
            } else {
                Logf("private_construction  WARN  panel builder hook at rva 0x%zX refused "
                     "(prologue bytes do not match this exe) - keeping the stack-scan fallback",
                     g_pbRva);
            }
        } else {
            Logf("private_construction  WARN  panel_builder_rva is set but "
                 "panel_builder_expect (%zu) / panel_builder_stolen (%zu) are "
                 "not usable - need >= 14 of each, stolen <= expect, and "
                 "expect <= %d - not hooking",
                 g_pbExpectLen, g_pbStolen, PB_MAX_EXPECT);
        }
    }

    // v3.5.0: pin the construction-phase layout before any native walk
    // may run. A mismatch (game update moved the data) disables the
    // native source for this session only - fund/panel keep working.
    g_nativeLayoutOk = ProbePhaseLayout();
    if (g_nativeLayoutOk) {
        Logf("private_construction  native phase layout verified (typedesc+0x370, "
             "stride 0x21B8, cost vector at phase+0x08)");
    } else if (g_costSource == COST_NATIVE) {
        Logf("private_construction  WARN  native phase layout probe FAILED and cost_source = native - "
             "no source can price a site; set cost_source = auto (recommended) so fund/panel can");
    } else {
        Logf("private_construction  WARN  native phase layout probe FAILED - native pricing disabled, "
             "fund/panel fallback active (open a site's panel once)");
    }

    g_simClock->registerTick(OnTick, nullptr);
    g_simClock->registerPause(OnPause, nullptr);
    if (g_ui) g_ui->registerDrawer(DrawOverlay, nullptr);

    const char* srcName = (g_costSource == COST_PANEL)  ? "panel"
                        : (g_costSource == COST_FUND)   ? "fund"
                        : (g_costSource == COST_NATIVE) ? "native"
                        :                                  "auto";
    const char* curName = (g_acceptCurrency == ACC_USD)    ? "usd"
                        : (g_acceptCurrency == ACC_EITHER) ? "either"
                        :                                    "rub";
    Logf("private_construction  live v3.6.1 (cost_source=%s, accept_currency=%s, currency %s, "
         "native layout %s, %d text pattern(s), builder hook %s, max_tick_advance %.3f, "
         "finish_handoff_frac %.2f, handoff_timeout_days %.1f, probe_completion %d)",
         srcName, curName, TsmCurrencySymbol(g_currency),
         g_nativeLayoutOk ? "ok" : "FAILED", PAT_COUNT,
         g_pbHooked ? "on" : "off",
         g_maxTickAdvance, g_finishHandoffFrac, g_handoffTimeoutDays,
         g_probeCompletion);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID) { return TRUE; }
