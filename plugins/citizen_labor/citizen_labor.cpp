// citizen_labor - wages, employment and payday.
// v4.8 updates (poison replaced by direct worker-count zeroing + eject):
//   - Phase 1 (Ghidra, read-only) proved bld+0xDEC is the max-worker field.
//     Writer is the shared window chrome FUN_1406F8CC0 (RVA 0x6F8CC0):
//       decrease  0x1406FB5AB  41 29 BC 24 EC 0D 00 00  SUB [R12+0xDEC],EDI
//                 0x1406FB5B5  41 C7 84 24 EC 0D 00 00 00 00 00 00
//                                             MOV [R12+0xDEC],0 (clamp)
//       increase  0x1406FBC1B  41 01 B4 24 EC 0D 00 00  ADD [R12+0xDEC],ESI
//                 0x1406FBC2D  41 89 BC 24 EC 0D 00 00  MOV [R12+0xDEC],EDI
//                                             (clamp to typedesc+0x80C)
//     R12 = window+0x240 building (FUN_1406F8CC0 entry:
//       MOV R12,[RDX+0x240], R13 = game, R15 = window). Amounts are
//     tiered from typedesc+0x80C ($WORKERS_NEEDED): decrease EDI = 1/5/
//     20/30 (0x1406FB3AE BF 01.. + CMOVG chain), increase ESI = 2/5/10/
//     20/30 (0x1406FB928 BE 02.. + CMOVG chain). Hit-test + audio in the
//     same block confirmed by CallsIn: GetMouseSolid, Collision,
//     GetString, FUN_1403FFAD0 (gui_click) immediately before the SUB.
//   - Q2: writing 0xDEC alone does NOT eject. Factory tick FUN_1401CF590
//     and capacity tick FUN_1401B1290 only READ 0xDEC
//     (0x1401B143E CMP EDI,[RSI+0xDEC]; 0x1401CFB0B CMP ESI,[R13+0xDEC])
//     to gate production; the chrome handler writes 0xDEC with no
//     0x822FD0/0x8231F0 call in range. Eject primitive is FUN_140822FD0
//     (RVA 0x822FD0, (game,person,building,flag): vector surgery on
//     bld+0xBC0/0xBC8 when person+0x95==0 else bld+0xC20/0xC28, clears
//     person+0x95, flag!=0 adds typedesc callbacks). It does NOT touch
//     person+0x58, so the plugin severance + ValidateEmployment still own
//     the citizen record; removal is synchronous (BC8 updated inline).
//   - Q3: 0xDEC IS persisted. Loader FUN_1401F0BE0 restores it:
//       0x1401F248D  8B 85 80 1B 00 00  MOV EAX,[RBP+0x1B80] (save slot)
//       0x1401F2493  41 89 86 EC 0D 00 00  MOV [R14+0xDEC],EAX (R14=bld)
//     so a zeroed firm loads as 0. The memory-only g_zeroedWorkers map
//     is deliberately NOT re-populated on load (v4.8 review): a firm
//     found at 0xDEC == 0 may have been zeroed by the PLAYER, and the
//     plugin must never overwrite that choice. The DEFAULTED path is
//     idempotent for already-zero firms (orig <= 0 -> early return,
//     no log, no eject).
//   - Q4: $WORKERS_NEEDED lives at typedesc+0x80C. Chrome reads it as the
//     max/step source (0x1406FB35B MOV EAX,[RAX+0x80C],
//     0x1406FB8D5 MOV EDI,[RAX+0x80C], RAX=[R12+0x318]) and clamps the
//     increase to it; shop/factory panels gate hiring on
//     ([bld+0xDEC]==0 && [typedesc+0x80C]!=0). Game INIs confirm
//     ($WORKERS_NEEDED 370/350/50/... in media_soviet/buildings_types).
//   - STOP checklist: 0xDEC IS the max-worker field; write alone does not
//     eject BUT the eject primitive WAS found (so no STOP - we call it);
//     0xDEC persists and loads back as 0, which is the desired state (no
//     STOP); typedesc default located (0x80C, no STOP); mechanism needs
//     no live panel (direct write + direct eject call, no STOP). Proceed
//     Option A extended with explicit eject (spec assumed engine-eject;
//     Ghidra proves it does not).
//   - The poisoned-workplace set and its monthly clear callback removed
//     entirely (they lived one frame at month start and never worked).
//   - New g_zeroedWorkers map (void* -> pre-zero 0xDEC, under g_lock,
//     memory-only). ZeroWorkersForBuilding saves orig, writes 0xDEC = 0,
//     then ejects every occupant of bld+0xBC0 via FUN_140822FD0(RVA
//     0x822FD0, flag 0). The ejection iterates a SNAPSHOT of the worker
//     list, never the live 0xBC0 vector (the engine call compacts it).
//   - OnPayroll TSM_PAYROLL_PAID restores the saved orig ONLY when the
//     live 0xDEC is still 0; if the player raised the slider in between
//     the map entry is dropped and the restore is skipped (logged).
//     TSM_PAYROLL_DEFAULTED calls ZeroWorkersForBuilding and keeps
//     consecutiveDefaults++ plus the second-default severance.
//   - OnDay Pass 1 no longer skips anyone; it drops g_zeroedWorkers
//     entries for decayed/demolished/unreadable buildings.
//   - No load-time re-population: a firm saved at 0xDEC == 0 loads at 0
//     and keeps it. If it still defaults, ZeroWorkersForBuilding is a
//     no-op (orig <= 0, no log). If it recovers, PAID finds no map entry
//     and leaves the player's 0 alone. Follow-up ticket: "restore on
//     first post-recovery sighting" once the interceptor grows a cheap
//     solvent query.
//   - Note: with payday_frequency = monthly (default), the 14-day grace
//     in ValidateEmployment typically fires before the second consecutive
//     payday, so the two-defaults severance path is exercised only for
//     daily/weekly paydays. Kept for those cases - not dead code, do not
//     delete.
//   - Version bumped to "4.8".
//
// See citizen_labor_api.h for the contract. This file is the
// implementation: a per-building-type wage table read from the ini, a
// per-citizen employment registry keyed on TsmCitizenID, a payroll loop
// driven by sim_clock's day event, and a save file written through
// persistence.
//
// v4.7 updates (never-ending jobs: stale employment records kept paying):
//   - EmploymentRecord gains memory-only lastJobSeenAbsDay (-1 = never
//     seen at work this session). Set in Pass 1 alongside workplace /
//     buildingType; fresh records and LoadLabor seed it with CurrentAbsDay
//     so old saves get a bounded grace window. Never persisted; save stays v4.
//   - New ValidateEmployment clears workplace/buildingType when the stored
//     workplace is null, dangling/unreadable, demolished, decayed, or
//     unseen for more than g_employmentGraceDays. Called at the top of
//     ProcessPayrollForPerson before the payday due check, so the record
//     is cleared on the next daily tick, not the next payday.
//   - ProcessPayrollForPerson gates the workplace wage override and the
//     shift-scaling penalty on working age (studentAgeMax <= age <
//     retireeAgeMin). A stale record can no longer turn a pension or
//     stipend into a wage override or scale it by shiftsThisMonth.
//   - New INI key employment_grace_days (default 14, clamped 0..365;
//     0 disables only the grace check, demolished/decayed still clear).
//   - [EMP INVALIDATED] probe lines are rate-limited (one per citizen per
//     session, ~200 globally) via a small unordered_set under g_lock.
//   - Step 8 person+0x58 fast-path deliberately NOT taken: unverified in
//     this build, grace-period fix is complete without it.
//   - Version bumped to "4.7".
//
// v4.6 updates (wage-mobility stub crash on first workplace selection):
//   - ROOT CAUSE 1 (ASLR): the tail `jmp rel32` target was the compile-time
//     constant 0x1401D37A9, valid only for a module at the preferred base
//     0x140000000. Under ASLR (this run: exe base 0x7FF759C60000) that
//     constant is unmapped. Now resolved from g_exeBase: RVA 0x1D37A9
//     (= hook site 0x1D3797 + the 18 stolen bytes).
//   - ROOT CAUSE 2 (rel32 range): a `jmp rel32` reaches only +-2GB, but the
//     old VirtualAlloc(NULL) stub landed ~2 TB from the exe, so the rel32
//     truncated to garbage and the tail jmp faulted regardless of the target
//     value. The stub is now pinned near the JBE target with AllocNear
//     (within ~768 MB), which is inside rel32 range.
//   - Added a one-shot [WAGE STUB] install diagnostic (sizeof, live stub
//     address, resolved target/next, computed rel32 and a hex dump of
//     stub[0x78..0x87]) to tesmioloader.log.
//   - kStubBase size (132), STUB_OFF_JMP_REL32 (128 = the rel32 field) and
//     the allocation size (sizeof) verified consistent; a static_assert now
//     locks sizeof >= STUB_OFF_JMP_REL32 + 4.
//   - Version bumped to "4.6".
//
// v4.4 updates:
//   - Wage-driven labor mobility: hook the workplace destination
//     comparison in FUN_1401D2DD0 (RVA 0x1D3797). The native per-need
//     score (bld+0x121C) is boosted by a soft logarithmic wage
//     multiplier and per-citizen variance, so higher-wage workplaces
//     draw more applicants without deterministic flocking.
//   - Version bumped to "4.4".
//
// v4.5 updates:
//   - Rewrote the wage-mobility inline hook stub (kStubBase) from scratch:
//       * r14 saved correctly: `mov [rsp+0x10], r14` (old bytes
//         `49 89 7C 24 18` decoded as `mov [r12+0x18], rdi`, never saved r14).
//       * Frame enlarged to 0x70 bytes; every stack slot lands inside it
//         (old stub wrote [rsp+0x20] above a 0x20-byte frame).
//       * XMM0-XMM5 preserved around the WageMobilityCompute call
//         (XMM0 = returned score; XMM1-XMM5 restored).
//       * Stack 16-byte aligned at the call (frame 0x70 = 112 = 0 (mod 16)).
//       * static_asserts guard sizeof(kStubBase), STUB_OFF_RAX_IMM = 74
//         and STUB_OFF_JMP_REL32 = 128.
//   - Latched the WageMobilityCompute probe log: one line per candidate
//     building pointer per session (POD fixed-array latch, first 64
//     distinct pointers; no allocation inside the __try/__except frame).
//   - Version bumped to "4.5".
//
// v4.3 updates:
//   - Pass 1 worker walk switched from TSM_BLD_PEOPLE_BEGIN (0xBD8)
//     to TSM_BLD_WORKERS_BEGIN (0xBC0). The engine's own factory tick
//     and factory panel both iterate 0xBC0 for workers; 0xBD8 is the
//     visitors/customers vector. This eliminates the wage-to-customer
//     bug and makes IsCustomerInServiceBuilding obsolete.
//
// v4.2 updates:
//   - LaborPanelWidgets skips unfinished buildings (built < 1.0).
//     ui_panels v4.9 lets private_construction run the widget pass on
//     construction-site panels for the CON privatisation tile; without
//     this guard those windows would also sprout a wage slider for a
//     site that has no workers yet.
//
// v1.8 updates:
//   - Unified Global Currency (v4): currency consume prefers the v4 iface
//     (v3 fallback). Wages/transfers remain RUB-denominated in the ini;
//     in USD mode TsmPluginStart resolves default_daily_wage, welfare,
//     stipend and pension from their RUB bases via usd_divisor (explicit *_usd
//     keys win), and GetWageForTypeImpl divides per-type [wages] entries
//     the same way - an explicit [wages_usd] section instead supplies
//     verbatim USD rates and switches unlisted types to the USD default.
//     Startup log carries the active symbol.
//
// v3.0 updates (Labor & Salary Overhaul, Milestone 3 - in-window wage
// tuning & Tropico scope control):
//   - New ui_panels row provider for STATE-operated workplaces: Wage /
//     Scope / Tune rows appended under the game's own building window
//     while it is drawn. Private [P] buildings are skipped when
//     enterprise_base is present (their wage display is the M2 set,
//     and double rows would be noise).
//   - Scope routing: Workplace Only -> setWorkplaceWageOverride
//     (persisted by coords since M1); Whole Sector -> setSectorWage
//     (new v4 accessor) plus clearWorkplaceWageOverride so the
//     building inherits the sector rate cleanly. Both setters clamp
//     to the statutory minimum.
//   - Sector writes are runtime-only (they reset from the INI on the
//     next world load) and lock-guarded: render-thread writes race
//     with the sim-thread payroll reads otherwise.
//   - API v4: setSectorWage appended to the tail; the provider
//     triple-publishes v2/v3 prefix views plus the full v4 iface.
//
// v4.0 updates (native in-window widgets - replaces the M3 hotkeys):
//   - LaborPanelRows keeps its read-only text rows so the resolved rate,
//     the scope mode and the slider range stay visible even when widgets
//     are unavailable;
//   - LaborPanelWidgets (new ui_panels v2 widget provider) draws two
//     native clickable controls through widgetSlider / widgetCheckbox:
//       Wage slider  0..100 mapped to [floor, floor + span];
//       Scope box    [x] Apply to Whole Sector;
//   - commit-on-change only: state is copied under g_lock, the helper is
//     called with NO lock held, and the setters run only when the helper
//     reports a change. Render thread never blocks on the sim thread;
//   - PollWageKeys ([ ] T) is gone: the last hotkey read was M3.
//
// v4.1 updates (playtest polish):
//   - LaborPanelRows is now TWO text rows: Status (Wage + Scope on one
//     line) and Tune (slider range); the Wage/Scope split is gone so the
//     [-] button never collides with a text label;
//   - Wage slider caption shortened to "Wage" (value lives on the Status
//     row); slider sits at left + dpi*180 for clear [-] margin;
//   - Auto-expanding ceiling (12.50 RUB cap removed): span =
//     max(wage_slider_span_min, currentWage * 1.5); the 0..100 bar always
//     covers floor..floor+span, so it climbs with inflation.
//
// v2.0 updates (Labor & Salary Overhaul, Milestone 1):
//   - Statutory minimum wage floor (minimum_wage[_usd]): no effective
//     wage or transfer pays below it. Social transfers become ratios
//     of the floor (welfare/stipend/pension_ratio_to_min > 0) with
//     legacy scalars as fallback, so old INIs keep their numbers.
//   - Six canonical sector base wages ([sector_wages], TSM_SECTOR_* in
//     citizen_labor_api.h, type map from SOVIETInstructions.txt):
//     Override -> Sector -> Legacy [wages] -> Global Default -> Floor.
//     A sector at 0.0 counts as unconfigured and falls through, so an
//     old INI without [sector_wages] behaves exactly like v1.8.
//   - Per-workplace overrides (v3 API set/clearWorkplaceWageOverride):
//     live-pointer map carrying coords+type (TSM_BLD_BBOX +0x5D0, same
//     walk as enterprise_base::ReadBuildingPosition), stored RUB-base
//     for currency neutrality, persisted as coords+type records.
//   - API v3 + save v3: provider dual-publishes the v2 prefix view
//     (enterprise_base consumes 2u) and the full v3 iface; the save
//     header keeps its size and repurposes `reserved` as the override
//     count, so v2 files load clean and v1.8 loaders ignore the tail.
//
// v1.7 updates:
//   - IsCustomerInServiceBuilding read demand kind at the wrong offset:
//     `*(int*)(demands + d * 0x80)` is demand+0x00, the amount-still-wanted
//     FLOAT, whose IEEE-754 bit pattern never equals a kind (1/2/3/4/5/6/10).
//     kind lives at demand+0x08 - the game's shop tick, citizen_retail's
//     ProcessOneDemand and private_shops' PersonCarriesDemand all read
//     +0x08. The function therefore always returned false and Pass 1
//     registered every demand-carrying visitor of a shop/pub/cinema/sports
//     hall/hospital/church without an active job elsewhere as staff at that
//     venue: hired, paid a wage, written to tesmio_labor.dat. The read now
//     lands on kind.
//
//     Records already written by v1.6 are not repaired by this. A citizen
//     wrongly registered as staff keeps that record across the fix; it
//     clears the next time they take a real job, or on a fresh save.
//
// v1.6 updates:
//   - B5 bound: g_wallets-adjacent g_records no longer grows forever.
//     New memory-only lastSeenAbsDay (absolute day, never persisted)
//     is stamped on creation, on every workplace sighting, and on every
//     residential payroll touch; the existing monthly hook evicts
//     entries idle longer than prune_after_days (default 90, 0 =
//     disabled), and SaveLabor skips stale records so sidecars stay
//     clean with no WALLET/LABOR version bump.
//
// v1.5 updates:
//   - Building type 23 is a pub. The one-shot private-building
//     diagnostic in private_shops read the typedesc of the building
//     whose object name is "pub" on save "Test 1" and got 23, not 24:
//
//       private_shops    type 23    obj "pub"  name "Dover Pub 2 [P]"
//
//     IsServiceBuildingType and IsCustomerInServiceBuilding both knew
//     only 24, so every drinker standing in a pub fell through
//     IsCustomerInServiceBuilding's `default: return false` and was
//     classified as pub STAFF - hired, paid a wage, and written into
//     tesmio_labor.dat. 23 is added to both; 24 stays, because the
//     number is per-map and replacing it would break any map where the
//     pub really is 24.
//
//     The wage table needed no change: citizen_labor.ini already has
//     `23 = 30.0`. It does not appear in the startup log because the
//     logger prints only entries that differ from default_daily_wage,
//     and the log does show `type 23 = 30.00 per day`.
//
//     Records already written by v1.4 are not repaired by this. A
//     citizen wrongly registered as pub staff keeps that record across
//     the fix; it clears the next time they take a real job, or on a
//     fresh save.
//
// v1.4 updates:
//   - Age Gate: Non-working-age citizens (< student_age_max or >= retiree_age_min)
//     are strictly barred from being recorded as workers in Pass 1.
//   - Dynamic Service Building Protection & Job Transitions:
//       * Visiting a service building (pub, shop, cinema, hotel, hospital, etc.)
//         never overwrites an active primary job worked during the current cycle.
//       * Service patrons carrying active consumer demands for that venue are
//         filtered out and logged as visitors.
//       * Unemployed citizens or citizens whose prior jobs were demolished/vacated
//         can legitimately be hired as service staff if they enter without patron demands.
//       * Industrial workplaces (factories, mines, power plants) unconditionally
//         overwrite service employment whenever worked.
//   - Normalized Shift Scaling: scale_pay_by_shifts scales its expected shift
//     target proportionally for daily and weekly frequencies rather than
//     penalizing non-monthly paydays.
//   - Synchronized Poison Clearing: the poisoned-workplace set cleared
//     cleanly at the month boundary inside OnDay prior to Pass 1.
//     (v4.8 note: the whole poison mechanism was removed - it only ever
//     lived for one frame.)

#include "../../src/tesmio_plugin.h"
#include "../../src/game_world.h"
#include "../../src/sim_clock_api.h"
#include "../../src/currency_api.h"
#include "../../src/persistence_api.h"
#include "../../src/citizen_labor_api.h"
#include "../../src/ui_panels_api.h"
#include "../../src/enterprise_base_api.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stdint.h>
#include <math.h>

// ---------------------------------------------------------------- constants

#define TSM_TYPE_RESIDENTIAL 2

// ---------------------------------------------------------------- config

static int   g_enabled           = 1;
static int   g_probe             = 1;

// v4.4: wage-driven labor mobility. Hooks the workplace destination
// comparison in FUN_1401D2DD0 (RVA 0x1D3797) to boost the native
// per-need score by a wage attractiveness multiplier plus per-citizen
// variance. All values are read from the ini under [citizen_labor].
static int   g_wageMobilityEnabled  = 1;   // master switch
static float g_wageMobilityLogCoeff = 0.35f; // 0.35 * ln(wage/minWage)
static float g_wageMobilityClampMin = -0.30f;// clamp lower bound
static float g_wageMobilityClampMax = 0.60f; // clamp upper bound
static float g_wageMobilityVarBase  = 0.90f; // variance base
static float g_wageMobilityVarSpan  = 0.20f; // variance span
static int   g_wageMobilityHookInstalled = 0; // H4 FIX: 1 if inline hook installed

static float g_defaultDailyWage  = 28.0f;
static float g_unemployedWelfare = 6.0f;
static float g_studentStipend    = 12.0f;
static float g_retireePension    = 18.0f;

// v4: raw RUB-denominated bases as written in the ini (explicit keys or
// [wages] entries). Effective values scale for USD mode unless an
// explicit USD key/section overrides them.
static float g_defaultDailyWageCfg  = 28.0f;
static float g_unemployedWelfareCfg = 6.0f;
static float g_studentStipendCfg    = 12.0f;
static float g_retireePensionCfg    = 18.0f;
static float g_usdDivisor = 50.0f;
static int   g_wagesUsdPresent = 0;

// ---------------------------------------------------------------- M1 state
//
// v2.0 (Labor & Salary Overhaul, Milestone 1): the statutory floor, the
// six canonical sector bases (TSM_SECTOR_*), and the per-workplace
// overrides. Wages/transfers stay RUB-denominated in the ini and in the
// override store; effective values resolve for the active currency at
// read time (explicit *_usd keys win, else RUB / usd_divisor).
static float g_minimumWageCfg    = 2.50f;   // RUB base from minimum_wage
static float g_minimumWage       = 2.50f;   // effective (active currency)
static float g_minimumWageUsdCfg = -1.0f;   // explicit USD floor (< 0 = none)

static float g_sectorWageCfg[TSM_SECTOR_COUNT];    // RUB bases
static float g_sectorWage[TSM_SECTOR_COUNT];       // effective values
static float g_sectorWageUsdCfg[TSM_SECTOR_COUNT]; // explicit USD (< 0 = none)

// Social transfers as multipliers of the floor when positive; legacy
// scalars otherwise (old INIs carry no ratios and keep their numbers).
static float g_welfareRatioToMin = -1.0f;
static float g_stipendRatioToMin = -1.0f;
static float g_pensionRatioToMin = -1.0f;

// Per-workplace override: live pointer key (session only) carrying the
// coords+type used to persist it. wageRub is the RUB base; reads return
// wageRub in RUB mode and wageRub/divisor in USD mode, so toggling the
// global currency rescales every override. Guarded by g_lock.
struct WageOverride {
    float wageRub = 0.0f;
    float posX    = 0.0f;
    float posZ    = 0.0f;
    int   type    = 0;
};
static std::unordered_map<void*, WageOverride> g_wageOverrides;

// v3 overrides loaded before their buildings exist (or whose buildings
// are not spawned yet): matched by type+coords on the next OnDay walk,
// same 2.0 m tolerance enterprise_base uses for type+position matches.
struct PendingWageOverride {
    float posX    = 0.0f;
    float posZ    = 0.0f;
    int   type    = 0;
    float wageRub = 0.0f;
};
static std::vector<PendingWageOverride> g_pendingOverrides;

// ---------------------------------------------------------------- M3 state
//
// v3.0 (Milestone 3): the in-window wage-tuning provider and its
// config. g_scopeMode is per-building UI state (memory-only, defaults
// to Workplace Only): 0 = workplace, 1 = whole sector. Sector wage
// writes are runtime-only by design; workplace overrides persist.
static const TsmUiPanelsApi*       g_panels     = nullptr;
static int                         g_panelsWidgets = 0; // v2 widget tail present
static const TsmEnterpriseBaseApi* g_enterprise = nullptr;
static int   g_panelRows        = 1;      // in_window_rows
static float g_wageSliderSpanMin = 10.0f; // span floor: span = max(min, wage*1.5)
static int   g_wageSliderStep   = 2;      // -/+ increment per click (percent)
static std::unordered_map<void*, int> g_scopeMode;

static float g_studentAgeMax     = 18.0f;
static float g_retireeAgeMin     = 60.0f;

static int   g_paydayFreq        = TSM_FREQ_MONTHLY;
static int   g_scalePayByShifts  = 0;
static int   g_payOnFirstSight   = 0;

// B5: idle days before an employment record unseen in either daily pass
// is evicted on the month boundary (and skipped at save). Memory-only
// lastSeenAbsDay, no save-format bump. 90d = one fiscal quarter at the
// monthly payday default. 0 disables pruning entirely.
static int   g_pruneAfterDays    = 90;

// v4.7: idle days after which a citizen's employment record is cleared
// because they have not been seen at any workplace. The engine's worker
// vector (bld+0xBC0) contains only citizens physically on shift today, so
// a worker on rotation / sick leave / holiday is legitimately absent for
// several days. 14 covers normal absences. 0 disables ONLY the grace
// check (demolished/decayed/dangling still clear). Clamped 0..365.
static int   g_employmentGraceDays = 14;

static int   g_workplaceMin      = 3;
static int   g_workplaceMax      = 200;
static int   g_workplaceExclude[16];
static int   g_workplaceExcludeCount;

// ---------------------------------------------------------------- services

static const TsmSimClockApi*    g_simClock    = nullptr;
static const TsmCurrencyApi*    g_currency    = nullptr;
static const TsmPersistenceApi* g_persistence = nullptr;
static int                      g_persHandle  = -1;

// ---------------------------------------------------------------- the calendar

static int DaysPerMonth(void)
{
    if (!g_simClock) return 30;
    if (g_simClock->structSize <
        offsetof(TsmSimClockApi, getDaysPerMonth) +
        sizeof(g_simClock->getDaysPerMonth))
        return 30;
    return g_simClock->getDaysPerMonth();
}

// ---------------------------------------------------------------- the registry

struct EmploymentRecord {
    void* workplace;           // live pointer, rebuilt on each workplace pass
    int   buildingType;        // last type we saw this citizen at
    int   lastPayMonth;        // most recent month a payday was issued
    int   lastPayDay;          // most recent day a payday was issued
    int   lastShiftDay;        // last day the citizen was seen at work
    int   shiftsThisMonth;     // for scale_pay_by_shifts
    int   lastSeenAbsDay;      // B5 GC hint: last absolute day (year*365+day)
                               // this citizen was seen alive. Memory-only,
                               // never persisted - keep payroll fields above
                               // decoupled from garbage collection.
    int   lastJobSeenAbsDay;   // v4.7: last absolute day the citizen was seen
                               // at a workplace. Memory-only, never persisted.
                               // -1 = never seen at work this session.
    int   consecutiveDefaults; // for the severance rule
    void* severedWorkplace;    // workplace severed from due to default streak
};

static std::unordered_map<uint64_t, EmploymentRecord> g_records;

// v4.8: buildings whose worker limit (bld+0xDEC) has been zeroed by the
// payroll default path. Value is the pre-zero 0xDEC value, restored when
// the firm returns to profitability (see OnPayroll TSM_PAYROLL_PAID).
// Guarded by g_lock. Memory-only; a zeroed firm that was saved will be
// re-zeroed on load if it still defaults (the 0xDEC restore is implicit
// via the typedesc default if the save did NOT persist 0xDEC, per Q3).
static std::unordered_map<void*, int> g_zeroedWorkers;

// v4.8: engine eject primitive FUN_140822FD0 (RVA 0x822FD0).
// (game, person, building, flag): removes person from bld+0xBC0/0xBC8
// (or bld+0xC20/0xC28 when person+0x95 != 0), clears person+0x95.
// flag != 0 adds typedesc bookkeeping callbacks; we pass 0 (plain
// vector surgery). Resolved from g_exeBase at call time (ASLR-safe).
// Threading: the engine itself calls this from the sim thread (the same
// thread OnDay runs on); with flag 0 it never re-enters plugin code, so
// running it while this thread holds g_lock is safe (recursive lock,
// no cross-thread deadlock path).
#define TSM_FUNEJECT_WORKER_RVA (g_tsmAddr.eject_worker)
typedef void (__fastcall *TsmEjectWorkerFn)(void* game, void* person,
                                            void* building, char flag);
static TsmEjectWorkerFn EjectWorkerFn(void)
{
    if (!TsmCheckExeBuild(H, "citizen_labor")) return nullptr;
    const unsigned char* code = TsmCallableCodePtr(H, TSM_FUNEJECT_WORKER_RVA, "eject_worker");
    return (TsmEjectWorkerFn)code;
}

// v4.8: zero a defaulting firm's worker limit and eject its current
// occupants. Takes g_lock only for the map check/insert; the eject
// calls below run with whatever lock state the caller had (recursive).
static void ZeroWorkersForBuilding(void* building)
{
    if (!TsmIsValidGamePtr(building)) return;
    if (!ReadablePtr((BYTE*)building + TSM_BLD_MAX_WORKERS, sizeof(int)))
        return;

    EnterCriticalSection(&g_lock);
    if (g_zeroedWorkers.find(building) != g_zeroedWorkers.end()) {
        LeaveCriticalSection(&g_lock);
        return;   // already zeroed this session
    }
    int orig = *(int*)((BYTE*)building + TSM_BLD_MAX_WORKERS);
    if (orig <= 0) {
        LeaveCriticalSection(&g_lock);
        return;   // already zero
    }
    g_zeroedWorkers[building] = orig;
    LeaveCriticalSection(&g_lock);

    *(int*)((BYTE*)building + TSM_BLD_MAX_WORKERS) = 0;

    if (g_probe)
        Logf("citizen_labor  [WORKERS ZEROED] %p (orig max %d, type %d)",
             building, orig, TsmBuildingType(building));

    // Q2 proved writing 0xDEC alone does NOT eject: the engine only
    // gates production/assignment on it. Eject every occupant via the
    // engine primitive so the severed citizens are physically gone from
    // bld+0xBC0 (Pass 1 cannot re-hire them, and the collector's
    // `0 < [bld+0xDEC]` gate blocks fresh assignments anyway).
    // The caller may hold g_lock (OnDay Pass 2); that is fine - see the
    // threading note above. Snapshot first because each eject call
    // mutates the very vector we are walking.
    BYTE* game = TsmGameObject(g_exeBase);
    TsmEjectWorkerFn eject = EjectWorkerFn();
    if (!game || !ReadablePtr(game, TSM_BLD_MIN_READABLE) || !eject)
        return;
    if (!TsmIsValidGamePtr(building)) return;
    if (!ReadablePtr((BYTE*)building + TSM_BLD_MIN_READABLE,
                     TSM_BLD_MIN_READABLE))
        return;
    void** wbegin = *(void***)((BYTE*)building + TSM_BLD_WORKERS_BEGIN);
    void** wend   = *(void***)((BYTE*)building + TSM_BLD_WORKERS_END);
    if (!wbegin || !wend || wend <= wbegin) return;
    size_t n = (size_t)(wend - wbegin);
    if (n == 0 || n > 10000) return;
    std::vector<void*> snapshot;
    snapshot.reserve(n);
    for (void** it = wbegin; it != wend; ++it) {
        void* person = *it;
        if (!TsmIsValidGamePtr(person)) continue;
        if (!ReadablePtr(person, TSM_PERSON_SIZE)) continue;
        snapshot.push_back(person);
    }
    for (void* person : snapshot) {
        if (!TsmIsValidGamePtr(person)) continue;
        if (!TsmIsValidGamePtr(building)) break;
        if (!ReadablePtr((BYTE*)building + TSM_BLD_MIN_READABLE,
                         TSM_BLD_MIN_READABLE))
            break;
        eject(game, person, building, 0);
    }
}

// v4.8: forget a zeroed entry whose building is gone (decayed,
// demolished, dangling). Called from OnDay Pass 1 while g_lock is held.
static void DropZeroedIfGoneLocked(void* b)
{
    auto it = g_zeroedWorkers.find(b);
    if (it == g_zeroedWorkers.end()) return;
    if (TsmIsValidGamePtr(b) &&
        ReadablePtr((const BYTE*)b + TSM_BLD_MIN_READABLE,
                    TSM_BLD_MIN_READABLE) &&
        !TsmIsDemolished(b) && !TsmIsDecayed(b))
        return;
    g_zeroedWorkers.erase(it);
}

static bool IsExcludedWorkplace(int type)
{
    for (int i = 0; i < g_workplaceExcludeCount; i++)
        if (g_workplaceExclude[i] == type) return true;
    return false;
}







// Absolute day so the year rollover (day 364 -> 0) never looks like
// 364 days of idleness. Returns -1 before a world is loaded.
static int CurrentAbsDay(void)
{
    if (!g_simClock) return -1;
    int day  = g_simClock->getDay();
    int year = g_simClock->getYear();
    if (day < 0 || year < 0) return -1;
    return year * 365 + day;
}

// v4.7: employment validity. Clears a citizen's employment record when
// their stored workplace is no longer credible. Safe to call with
// `nowAbsDay < 0` (before a world is loaded) - validation is skipped in
// that case.
//
// Note: called from ProcessPayrollForPerson (pass 2), so it only runs
// for residents of residential buildings. Citizens who become homeless
// are not touched here; they age out via prune_after_days. Intentional.
//
// Step 8 person+0x58 fast-path deliberately NOT included: unverified in
// this build; the grace-period check below is complete without it.
//
// Locking: OnDay holds g_lock across both passes, so the check itself
// needs no lock. The rate-limited probe set below also takes g_lock;
// that is safe because Windows CRITICAL_SECTIONs are recursive (same
// thread re-enters), and Logf itself runs after the lock is released.
static void ValidateEmployment(void* person, EmploymentRecord& rec, int nowAbsDay)
{
    if (nowAbsDay < 0) return;
    if (rec.buildingType <= 2) return;          // already not employed

    const char* reason = nullptr;

    if (!rec.workplace) {
        reason = "null workplace";
    } else if (!TsmIsValidGamePtr(rec.workplace) ||
               !ReadablePtr(rec.workplace, TSM_BLD_MIN_READABLE)) {
        reason = "dangling workplace";
    } else if (TsmIsDemolished(rec.workplace)) {
        reason = "workplace demolished";
    } else if (TsmIsDecayed(rec.workplace)) {
        reason = "workplace decayed";
    } else if (g_employmentGraceDays > 0 &&
               rec.lastJobSeenAbsDay >= 0 &&
               nowAbsDay - rec.lastJobSeenAbsDay > g_employmentGraceDays) {
        reason = "grace period exceeded";
    }

    if (!reason) return;

    // Rate-limited probe: one line per citizen per session at most,
    // capped globally (~200 lines/session). Small fixed-size set under
    // g_lock; Logf itself runs outside the lock.
    bool shouldLog = false;
    if (g_probe) {
        static std::unordered_set<uint64_t> s_loggedIds;
        static int s_loggedTotal = 0;
        static const int kMaxInvalidatedLines = 200;
        static const size_t kMaxTrackedIds = 256;
        uint64_t id = TsmCitizenID(person);
        bool already = false;
        EnterCriticalSection(&g_lock);
        if (s_loggedIds.find(id) != s_loggedIds.end()) {
            already = true;
        } else if (s_loggedTotal < kMaxInvalidatedLines &&
                   s_loggedIds.size() < kMaxTrackedIds) {
            s_loggedIds.insert(id);
            s_loggedTotal++;
            shouldLog = true;
        }
        LeaveCriticalSection(&g_lock);
        if (shouldLog && !already) {
            Logf("citizen_labor  [EMP INVALIDATED] citizen %llu (type was %d): %s",
                 (unsigned long long)id, rec.buildingType, reason);
        }
    }

    rec.workplace         = nullptr;
    rec.buildingType      = 0;
    rec.severedWorkplace  = nullptr;
    rec.lastJobSeenAbsDay = -1;   // reset so a re-hire via Pass 1 restarts cleanly
}

static EmploymentRecord& EmploymentFor(void* person)
{
    uint64_t id = TsmCitizenID(person);

    auto it = g_records.find(id);
    if (it != g_records.end()) return it->second;

    EmploymentRecord rec = {};
    rec.lastSeenAbsDay    = CurrentAbsDay();
    rec.lastJobSeenAbsDay = CurrentAbsDay();   // v4.7: fresh window
    if (g_payOnFirstSight) {
        rec.lastPayMonth = -1;
        rec.lastPayDay   = -1;
    } else {
        rec.lastPayMonth = g_simClock ? g_simClock->getMonth() : 0;
        rec.lastPayDay   = g_simClock ? g_simClock->getDay()   : 0;
    }
    rec.consecutiveDefaults = 0;
    rec.severedWorkplace    = nullptr;
    g_records.emplace(id, rec);
    return g_records[id];
}

// ---------------------------------------------------------------- wages
//
// v2.0 hierarchy (Labor & Salary Overhaul, M1):
//   Specific Override -> Sector Wage -> Legacy [wages] -> Global Default
//   -> Clamped to the Minimum Wage Floor.
// A sector base at 0.0 counts as UNCONFIGURED and falls through, so an
// old INI without [sector_wages] resolves exactly like v1.8. The new
// shipped INI configures all six, so sector bases win over legacy rows.

// Canonical type -> sector map (SOVIETInstructions.txt BUILDINGTYPE
// numbers). Factory (6) is light industry, mine (7) is heavy; per-factory
// heavy/light refinement is future work. Returns -1 when the type has
// no sector (living=2, unknown=30, ...): caller falls back to default.
static int SectorForBuildingType(int type)
{
    switch (type) {
    // 0 heavy industry / mining
    case 7:   // mine
    case 60:  // cooling tower
    case 107: // foreign pipeline export
    case 109: // scrapyard
        return TSM_SECTOR_HEAVY;
    // 1 infrastructure / energy / transport / storage / construction
    case 0: case 1: case 5: case 11: case 12: case 28: case 13:
    case 14: case 15: case 17: case 18: case 19: case 31: case 32:
    case 34: case 35: case 36: case 42: case 43: case 44: case 45:
    case 46: case 47: case 48: case 52: case 70: case 71: case 72:
    case 80: case 90: case 91: case 92: case 93: case 94: case 95:
    case 96: case 97: case 98: case 105: case 106: case 300:
    case 20:  // customhouse (excluded as a workplace, listed for completeness)
        return TSM_SECTOR_INFRA;
    // 2 healthcare + education
    case 4: case 10: case 25: case 27: case 74:
        return TSM_SECTOR_CARE;
    // 3 light industry
    case 6: case 40: case 49: case 108:
        return TSM_SECTOR_LIGHT;
    // 4 retail / services / culture / civic
    case 3: case 21: case 22: case 23: case 24: case 26: case 33:
    case 37: case 38: case 39: case 41: case 50: case 51:
    case 73: case 75: case 76: case 77:
        return TSM_SECTOR_SERVICES;
    // 5 agriculture
    case 8: case 9:
        return TSM_SECTOR_AGRI;
    default:
        return -1;
    }
}

static std::unordered_map<int, float> g_wageTable;     // RUB bases from [wages]
static std::unordered_map<int, float> g_wageTableUsd;  // explicit USD overrides

// v4: wages are RUB-denominated in the ini. In USD mode every effective
// figure divides by usd_divisor unless an explicit USD key/section entry
// overrides it, so a 28 RUB/day table does not pay 28 USD/day.
static float UsdScale(void)
{
    float d = (g_usdDivisor > 0.0f) ? g_usdDivisor : 50.0f;
    if (!(d >= 1.0f)) d = 50.0f;
    return d;
}

static int IsUsdActive(void)
{
    return TsmCurrencyActive(g_currency) == TSM_CURRENCY_USD;
}

static float ClampToMinimum(float wage)
{
    // M1: the statutory floor takes effect at the point every wage path
    // funnels through, so no wage can pay below it by accident.
    float floor = (g_minimumWage > 0.0f) ? g_minimumWage : 0.0f;
    return (wage < floor) ? floor : wage;
}

// Recompute the effective sector wages from the currency-neutral cfg
// bases. Runs at Start after the floor is resolved, and again after a
// save load: the sidecar stores RUB bases, so loading one in USD mode
// must divide by the divisor (explicit USD entries win) instead of
// assigning the RUB figure to g_sectorWage verbatim.
static void ResolveSectorWages(void)
{
    const int usd = IsUsdActive();
    const float d = UsdScale();
    for (int s = 0; s < TSM_SECTOR_COUNT; s++) {
        if (usd) {
            if (g_sectorWageUsdCfg[s] > 0.0f)
                g_sectorWage[s] = g_sectorWageUsdCfg[s];
            else if (g_sectorWageCfg[s] > 0.0f)
                g_sectorWage[s] = g_sectorWageCfg[s] / d;
            else
                g_sectorWage[s] = 0.0f;  // unconfigured stays unconfigured
        } else {
            g_sectorWage[s] = g_sectorWageCfg[s];
        }
        if (g_sectorWage[s] > 0.0f)
            g_sectorWage[s] = ClampToMinimum(g_sectorWage[s]);
    }
}

// Sector base for one building type: effective sector figure when the
// sector is configured (> 0), else the legacy per-type/default chain
// (v1.8 behaviour). Sector 0.0 = unconfigured: an old INI without
// [sector_wages] resolves exactly like v1.8.
static float SectorOrLegacyWage(int buildingType)
{
    int sector = SectorForBuildingType(buildingType);
    if (sector >= 0 && sector < TSM_SECTOR_COUNT &&
        g_sectorWage[sector] > 0.0f)
        return g_sectorWage[sector];

    if (IsUsdActive()) {
        auto iu = g_wageTableUsd.find(buildingType);
        if (iu != g_wageTableUsd.end()) return iu->second;
        if (g_wagesUsdPresent && buildingType > 2) {
            // [wages_usd] present: types without an entry fall back to the
            // USD default, not to a scaled RUB figure.
            return g_defaultDailyWage;
        }
        float base = g_defaultDailyWageCfg;
        if (buildingType > 2) {
            auto it = g_wageTable.find(buildingType);
            if (it != g_wageTable.end()) base = it->second;
        }
        return base / UsdScale();
    }
    if (buildingType <= 2) return g_defaultDailyWage;
    auto it = g_wageTable.find(buildingType);
    return (it != g_wageTable.end()) ? it->second : g_defaultDailyWage;
}

static float GetWageForTypeImpl(int buildingType)
{
    return ClampToMinimum(SectorOrLegacyWage(buildingType));
}

static float GetEffectiveDailyWageImpl(void* person)
{
    if (!TsmIsValidGamePtr(person))
        return IsUsdActive() ? g_unemployedWelfare : g_unemployedWelfare;
    if (!ReadablePtr(person, 0xE0))
        return g_unemployedWelfare;

    // Strict age check: non-working-age citizens always receive their statutory transfers.
    // In USD mode the transfer figures below are already the effective
    // (scaled or overridden) values resolved in ReadSettings.
    float age = *(float*)((BYTE*)person + TSM_PERSON_AGE);
    if (age <  g_studentAgeMax) return g_studentStipend;
    if (age >= g_retireeAgeMin) return g_retireePension;

    uint64_t id = TsmCitizenID(person);
    auto it = g_records.find(id);
    int type = (it != g_records.end()) ? it->second.buildingType : 0;

    if (type <= 2) {
        return g_unemployedWelfare;
    }

    return GetWageForTypeImpl(type);
}

// Building position reader: C3D_NODE is embedded at bld + 0x320 (not a
// pointer). The node wraps a 4x4 float transform matrix; the translation
// (position) vector is Row 3 at +0x30 (X) / +0x38 (Z) — real world coords
// in hundreds/thousands of metres. Reject unit-vector rows with
// |x| > 1.5 || |z| > 1.5. Then pointer dereference, then bbox fallback.
// Same walk enterprise_base uses to match saved enterprises across a load.
static bool ReadBuildingPosition(void* b, float* outX, float* outZ)
{
    if (!TsmIsValidGamePtr(b) || !ReadablePtr(b, 0x400)) return false;

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

// Live-override lookup. Caller holds g_lock. Returns the RUB-base wage
// (> 0) or 0 when the building carries no override.
static float FindOverrideRubLocked(void* building)
{
    if (!building) return 0.0f;
    auto it = g_wageOverrides.find(building);
    if (it != g_wageOverrides.end() && it->second.wageRub > 0.0f)
        return it->second.wageRub;
    return 0.0f;
}

// Effective override for one building in ACTIVE currency units:
// RUB-base / divisor in USD mode, verbatim in RUB mode. 0 = none.
static float OverrideEffectiveLocked(void* building)
{
    float rub = FindOverrideRubLocked(building);
    if (!(rub > 0.0f)) return 0.0f;
    if (IsUsdActive()) {
        float d = UsdScale();
        if (!(d >= 1.0f)) d = 50.0f;
        rub /= d;
    }
    return rub;
}

// Full M1 hierarchy for one KNOWN building pointer:
//   Specific Override -> Sector Wage -> Legacy [wages] -> Global Default
//   -> Clamped to the Minimum Wage Floor.
// Locks internally; safe from any thread. Returns <= 0 when the pointer
// is unusable (callers treat that as "unknown, pay the type rate").
static float ResolveWageForBuildingImpl(void* building)
{
    if (!TsmIsValidGamePtr(building)) return 0.0f;
    if (!ReadablePtr(building, TSM_BLD_MIN_READABLE)) return 0.0f;
    int type = TsmBuildingType(building);

    EnterCriticalSection(&g_lock);
    float ov = OverrideEffectiveLocked(building);
    LeaveCriticalSection(&g_lock);
    if (ov > 0.0f) return ClampToMinimum(ov);

    if (type < 0) return 0.0f;
    return GetWageForTypeImpl(type);
}

// ---------------------------------------------------------------- interceptor

static TsmPayrollInterceptor g_interceptor   = nullptr;
static void*                g_interceptorUD = nullptr;

static int RegisterInterceptor(TsmPayrollInterceptor cb, void* userData)
{
    g_interceptor   = cb;
    g_interceptorUD = userData;
    return cb ? 1 : 0;
}

static void ClearInterceptor(void)
{
    g_interceptor   = nullptr;
    g_interceptorUD = nullptr;
}

// ---------------------------------------------------------------- payroll

static void ProcessPayrollForPerson(void* person, int day, int month)
{
    EmploymentRecord& rec = EmploymentFor(person);
    // B5: the residential pass sees every resident daily - including
    // welfare/stipend/pension-only citizens - so it is the liveness
    // heartbeat. Runs before the due check so idle-but-alive citizens
    // are never mistaken for dead.
    int now = CurrentAbsDay();
    if (now >= 0) rec.lastSeenAbsDay = now;

    // v4.7: validate BEFORE the payday due check, so a stale record is
    // cleared on the next daily tick, not the next payday.
    ValidateEmployment(person, rec, now);

    bool  due        = false;
    float multiplier = 1.0f;

    switch (g_paydayFreq) {
    case TSM_FREQ_DAILY:
        if (rec.lastPayDay != day) {
            due = true;
            rec.lastPayDay   = day;
            rec.lastPayMonth = month;
            multiplier = 1.0f;
        }
        break;

    case TSM_FREQ_WEEKLY:
        if (rec.lastPayDay < 0 ||
            day <  rec.lastPayDay ||           // year rolled over
            day -  rec.lastPayDay >= 7)
        {
            due = true;
            rec.lastPayDay   = day;
            rec.lastPayMonth = month;
            multiplier = 7.0f;
        }
        break;

    case TSM_FREQ_MONTHLY:
    default:
        if (rec.lastPayMonth != month) {
            due = true;
            rec.lastPayMonth = month;
            rec.lastPayDay   = day;
            multiplier = (float)DaysPerMonth();
        }
        break;
    }

    if (!due) return;

    // M1: an employed citizen whose workplace carries an override is paid
    // the override, not the sector/type rate. GetEffectiveDailyWageImpl
    // knows the type but not the building pointer; the record does, so
    // the override check lives here where both are in scope.
    float daily = GetEffectiveDailyWageImpl(person);

    // v4.7: apply the workplace override and shift scaling ONLY when the
    // citizen is in the working-age band. GetEffectiveDailyWageImpl has
    // already age-gated and returned stipend/pension/welfare correctly; a
    // stale record must not override that. Belt-and-braces alongside
    // ValidateEmployment.
    float age0 = *(float*)((BYTE*)person + TSM_PERSON_AGE);
    const bool workingAge =
        (age0 >= g_studentAgeMax && age0 < g_retireeAgeMin);

    if (workingAge && rec.buildingType > 2 && rec.workplace) {
        EnterCriticalSection(&g_lock);
        float ov = OverrideEffectiveLocked(rec.workplace);
        LeaveCriticalSection(&g_lock);
        if (ov > 0.0f) daily = ClampToMinimum(ov);
    }
    if (!(daily > 0.0f)) return;

    float amount = daily * multiplier;

    if (g_scalePayByShifts && workingAge && rec.buildingType > 2) {
        // Normalize the shift expectation across daily, weekly, or monthly schedules
        int dpm = DaysPerMonth();
        float expectedShifts = 20.0f * (multiplier / (float)(dpm > 0 ? dpm : 30));
        if (expectedShifts < 1.0f) expectedShifts = 1.0f;

        float ratio = (float)rec.shiftsThisMonth / expectedShifts;
        if (ratio > 1.25f) ratio = 1.25f;
        if (ratio < 0.20f) ratio = 0.20f;
        amount *= ratio;
    }
    rec.shiftsThisMonth = 0;

    if (!(amount > 0.0f)) return;

    // Ask the private employer, if one is registered and this citizen
    // has a workplace on record.
    if (g_interceptor && rec.workplace) {
        int rc = g_interceptor(rec.workplace, person, amount,
                               rec.buildingType, g_interceptorUD);

        if (rc == TSM_PAYROLL_PAID) {
            // Employer paid. Restore this workplace's worker limit if we
            // zeroed it and nothing has touched it since, then reset the
            // citizen's streak and severance state.
            //
            // v4.8 review: if the live 0xDEC is no longer 0 the player
            // (or another system) raised the slider after our zero - drop
            // the map entry but DO NOT overwrite their choice.
            EnterCriticalSection(&g_lock);
            auto z_it = g_zeroedWorkers.find(rec.workplace);
            if (z_it != g_zeroedWorkers.end()) {
                int orig = z_it->second;
                g_zeroedWorkers.erase(z_it);
                LeaveCriticalSection(&g_lock);
                if (ReadablePtr((BYTE*)rec.workplace + TSM_BLD_MAX_WORKERS,
                                sizeof(int))) {
                    int current =
                        *(int*)((BYTE*)rec.workplace + TSM_BLD_MAX_WORKERS);
                    if (current != 0) {
                        if (g_probe)
                            Logf("citizen_labor  [WORKERS RESTORED] %p skipped "
                                 "(current max %d != 0 - player intervened)",
                                 rec.workplace, current);
                    } else {
                        *(int*)((BYTE*)rec.workplace + TSM_BLD_MAX_WORKERS) =
                            orig;
                        if (g_probe)
                            Logf("citizen_labor  [WORKERS RESTORED] %p "
                                 "(max -> %d)", rec.workplace, orig);
                    }
                }
                // Unreadable/gone building: entry already dropped, nothing
                // to write.
            } else {
                LeaveCriticalSection(&g_lock);
            }
            rec.consecutiveDefaults = 0;
            rec.severedWorkplace    = nullptr;
            return;
        }

        if (rc == TSM_PAYROLL_DEFAULTED) {
            // Employer is insolvent. Zero its worker limit (and eject its
            // current occupants) so tomorrow's workplace pass cannot
            // re-hire there, and count the default.
            ZeroWorkersForBuilding(rec.workplace);
            rec.consecutiveDefaults++;

            if (rec.consecutiveDefaults >= 2) {
                // Second consecutive default: sever the citizen and pay
                // one cycle of welfare so they do not fall through the
                // floor before the next payday.
                if (g_probe) {
                    Logf("citizen_labor  severing citizen from workplace %p "
                         "after 2 consecutive defaults", rec.workplace);
                }
                rec.severedWorkplace    = rec.workplace;
                rec.workplace           = nullptr;
                rec.buildingType        = 0;
                rec.consecutiveDefaults = 0;

                float welfare = g_unemployedWelfare * multiplier;
                if (welfare > 0.0f && g_currency)
                    g_currency->stateToCitizen(person, welfare, "welfare");
            }
            return;
        }

        // TSM_PAYROLL_IGNORED falls through to the state paying below.
    }

    // Determine the state ledger category
    const char* category = "wage";
    float age = *(float*)((BYTE*)person + TSM_PERSON_AGE);
    if (age < g_studentAgeMax) {
        category = "welfare"; // Stipends recorded under welfare transfers
    } else if (age >= g_retireeAgeMin) {
        category = "pension";
    } else if (rec.buildingType <= 2) {
        category = "welfare";
    }

    if (g_currency)
        g_currency->stateToCitizen(person, amount, category);
}

// ---------------------------------------------------------------- the day handler

// Forward decl: defined with the API v3 block below; called from OnDay
// while g_lock is held, to bind save-loaded overrides to their buildings.
static void MatchPendingOverridesLocked(void* const* begin, void* const* end);

static void OnDay(int /*day*/, int month, int /*year*/)
{
    int day = g_simClock ? g_simClock->getDay() : 0;

    BYTE* game = TsmGameObject(g_exeBase);
    if (!game || !ReadablePtr(game, TSM_BLD_MIN_READABLE)) return;

    TsmBuildingSpan span;
    if (!TsmGetBuildings(game, &span)) return;

    EnterCriticalSection(&g_lock);

    // M1: bind save-loaded wage overrides to their buildings now that the
    // world exists (type + coords match, 2.0 m tolerance). g_lock held.
    if (!g_pendingOverrides.empty())
        MatchPendingOverridesLocked(span.begin, span.end);

    size_t registeredWorkers = 0;
    size_t skippedAgeGate    = 0;

    // --- Pass 1: workplace recording
    for (size_t i = 0; i < span.count; i++) {
        BYTE* b = (BYTE*)span.begin[i];
        if (!TsmIsValidGamePtr(b)) continue;
        if (!ReadablePtr(b, TSM_BLD_MIN_READABLE)) continue;
        if (TsmIsDemolished(b)) continue;

        int type = TsmBuildingType(b);
        if (type < g_workplaceMin || type > g_workplaceMax) continue;
        if (IsExcludedWorkplace(type)) continue;

        // v4.8: drop zeroed entries for buildings that are gone. A
        // zeroed-but-decayed firm must not pin its map entry forever.
        DropZeroedIfGoneLocked(b);

        void** begin = *(void***)(b + TSM_BLD_WORKERS_BEGIN);
        void** end   = *(void***)(b + TSM_BLD_WORKERS_END);
        if (!begin || !end || end <= begin) continue;
        if (!TsmIsValidGamePtr(begin)) continue;
        if (!ReadablePtr(begin, sizeof(void*))) continue;

        size_t n = (size_t)(end - begin);
        if (n > 500) n = 500;

        for (size_t k = 0; k < n; k++) {
            void* p = begin[k];
            if (!TsmIsValidGamePtr(p)) continue;
            if (!ReadablePtr(p, TSM_PERSON_SIZE)) continue;

            // 1. Age Gate: Children and Pensioners never hold employment
            float age = *(float*)((BYTE*)p + TSM_PERSON_AGE);
            if (age < g_studentAgeMax || age >= g_retireeAgeMin) {
                skippedAgeGate++;
                continue;
            }

            int foreign = *(int*)((BYTE*)p + TSM_PERSON_FOREIGN);
            int tourist = *(int*)((BYTE*)p + TSM_PERSON_TOURIST);
            if (foreign != 0 || tourist != 0) continue;

            EmploymentRecord& rec = EmploymentFor(p);

            // B5: workplace sighting also proves liveness (covers workers
            // whose residence is not walked on a given day).
            int nowSeen = CurrentAbsDay();
            if (nowSeen >= 0) rec.lastSeenAbsDay = nowSeen;

            // Severance recovery check:
            // If previously severed from building `b`, ensure `b` is no
            // longer zeroed (v4.8: a restored firm is re-eligible).
            if (rec.severedWorkplace == b) {
                EnterCriticalSection(&g_lock);
                bool stillZeroed =
                    (g_zeroedWorkers.find(b) != g_zeroedWorkers.end());
                LeaveCriticalSection(&g_lock);
                if (stillZeroed) continue;
                rec.severedWorkplace = nullptr; // Workplace recovered
            } else if (rec.severedWorkplace && rec.severedWorkplace != b) {
                rec.severedWorkplace = nullptr; // Moved to a new employer
            }

            rec.workplace    = b;
            rec.buildingType = type;

            // v4.7: worker sighting restarts the employment grace window.
            // nowSeen is already computed above for lastSeenAbsDay.
            if (nowSeen >= 0) rec.lastJobSeenAbsDay = nowSeen;
            registeredWorkers++;

            if (rec.lastShiftDay != day) {
                rec.lastShiftDay = day;
                rec.shiftsThisMonth++;
            }
        }
    }

    if (g_probe && registeredWorkers > 0) {
        static int dayLogged = -1;
        if (dayLogged != day) {
            dayLogged = day;
            Logf("citizen_labor  pass 1 (day %d): %zu worker shift(s) confirmed, %zu non-working-age skipped",
                 day, registeredWorkers, skippedAgeGate);
        }
    }

    // --- Pass 2: payday
    for (size_t i = 0; i < span.count; i++) {
        BYTE* b = (BYTE*)span.begin[i];
        if (!TsmIsValidGamePtr(b)) continue;
        if (!ReadablePtr(b, TSM_BLD_MIN_READABLE)) continue;
        if (TsmIsDemolished(b)) continue;

        int type = TsmBuildingType(b);
        if (type != TSM_TYPE_RESIDENTIAL) continue;

        void** begin = *(void***)(b + TSM_BLD_RESIDENTS_BEGIN);
        void** end   = *(void***)(b + TSM_BLD_RESIDENTS_END);
        if (!begin || !end || end <= begin) continue;
        if (!TsmIsValidGamePtr(begin)) continue;
        if (!ReadablePtr(begin, sizeof(void*))) continue;

        size_t n = (size_t)(end - begin);
        if (n > 1000) n = 1000;

        for (size_t k = 0; k < n; k++) {
            void* p = begin[k];
            if (!TsmIsValidGamePtr(p)) continue;
            if (!ReadablePtr(p, TSM_PERSON_SIZE)) continue;

            int foreign = *(int*)((BYTE*)p + TSM_PERSON_FOREIGN);
            int tourist = *(int*)((BYTE*)p + TSM_PERSON_TOURIST);
            if (foreign != 0 || tourist != 0) continue;

            ProcessPayrollForPerson(p, day, month);
        }
    }

    LeaveCriticalSection(&g_lock);
}

// ---------------------------------------------------------------- the month handler

static void OnMonthPruneRecords(void* /*ud*/, int /*day*/, int /*month*/,
                               int /*year*/)
{
    // v4.8: poison removed. Zeroed limits persist until restore.

    // B5: bound g_records. Evict entries unseen in either daily pass for
    // longer than g_pruneAfterDays. Dropping the record also drops its
    // live workplace/severed pointers, which are rebuilt on next sight.
    if (g_pruneAfterDays > 0) {
        int now = CurrentAbsDay();
        if (now >= 0) {
            size_t before = 0, after = 0;
            EnterCriticalSection(&g_lock);
            before = g_records.size();
            for (auto it = g_records.begin(); it != g_records.end(); ) {
                int seen = it->second.lastSeenAbsDay;
                if (seen >= 0 && now - seen > g_pruneAfterDays)
                    it = g_records.erase(it);
                else
                    ++it;
            }
            after = g_records.size();
            LeaveCriticalSection(&g_lock);

            if (g_probe && after != before)
                Logf("citizen_labor  pruned %zu stale employment record(s), "
                     "%zu remain", before - after, after);
        }
    }
}

// ---------------------------------------------------------------- per-person queries

static float Api_GetDefaultDailyWage(void)   { return g_defaultDailyWage; }
static float Api_GetUnemployedWelfare(void)  { return g_unemployedWelfare; }
static float Api_GetStudentStipend(void)     { return g_studentStipend; }
static float Api_GetRetireePension(void)     { return g_retireePension; }

static float Api_GetWageForType(int t)       { return GetWageForTypeImpl(t); }
static float Api_GetEffectiveDailyWage(void* p)
{
    return GetEffectiveDailyWageImpl(p);
}

static int Api_IsEmployed(void* person)
{
    if (!TsmIsValidGamePtr(person)) return 0;
    uint64_t id = TsmCitizenID(person);
    EnterCriticalSection(&g_lock);
    auto it = g_records.find(id);
    int e = (it != g_records.end()) ? (it->second.buildingType > 2 ? 1 : 0) : 0;
    LeaveCriticalSection(&g_lock);
    return e;
}

static void* Api_GetWorkplace(void* person)
{
    if (!TsmIsValidGamePtr(person)) return nullptr;
    uint64_t id = TsmCitizenID(person);
    EnterCriticalSection(&g_lock);
    auto it = g_records.find(id);
    void* w = (it != g_records.end()) ? it->second.workplace : nullptr;
    LeaveCriticalSection(&g_lock);
    return w;
}

static int Api_GetEmployedBuildingType(void* person)
{
    if (!TsmIsValidGamePtr(person)) return 0;
    uint64_t id = TsmCitizenID(person);
    EnterCriticalSection(&g_lock);
    auto it = g_records.find(id);
    int t = (it != g_records.end()) ? it->second.buildingType : 0;
    LeaveCriticalSection(&g_lock);
    return t;
}

static int Api_PayAllNow(void)
{
    if (!g_simClock) return 0;

    int day   = g_simClock->getDay();
    int month = g_simClock->getMonth();
    if (day < 0 || month < 0) return 0;

    OnDay(day, month, g_simClock->getYear());
    return 1;
}

// --- v3: minimum wage, sectors, overrides ------------------------------

static float Api_GetMinimumWage(void) { return g_minimumWage; }

static float Api_GetSectorWage(int sectorId)
{
    if (sectorId < 0 || sectorId >= TSM_SECTOR_COUNT) return 0.0f;
    return g_sectorWage[sectorId];
}

static int Api_SetWorkplaceWageOverride(void* building, float dailyWage)
{
    // Takes the wage in ACTIVE currency units, clamps to the floor, and
    // stores the RUB base so toggling the global currency rescales it.
    // Records coords+type alongside for the v3 save. Thread-safe.
    if (!TsmIsValidGamePtr(building)) return 0;
    if (!ReadablePtr(building, TSM_BLD_MIN_READABLE)) return 0;
    if (!(dailyWage == dailyWage)) return 0;
    if (!(dailyWage > 0.0f)) return 0;

    float eff = ClampToMinimum(dailyWage);
    float rub = eff;
    if (IsUsdActive()) {
        float d = UsdScale();
        if (!(d >= 1.0f)) d = 50.0f;
        rub = eff * d;
    }

    float px = 0.0f, pz = 0.0f;
    (void)ReadBuildingPosition(building, &px, &pz);
    int type = TsmBuildingType(building);

    EnterCriticalSection(&g_lock);
    WageOverride o;
    o.wageRub = rub;
    o.posX    = px;
    o.posZ    = pz;
    o.type    = type;
    g_wageOverrides[building] = o;
    // A live set supersedes any pending record for the same spot.
    for (auto it = g_pendingOverrides.begin(); it != g_pendingOverrides.end(); ) {
        if (it->type == type &&
            fabsf(it->posX - px) < 2.0f && fabsf(it->posZ - pz) < 2.0f)
            it = g_pendingOverrides.erase(it);
        else
            ++it;
    }
    LeaveCriticalSection(&g_lock);
    return 1;
}

static void Api_ClearWorkplaceWageOverride(void* building)
{
    EnterCriticalSection(&g_lock);
    if (building) {
        auto it = g_wageOverrides.find(building);
        float px = 0.0f, pz = 0.0f;
        int   type = 0;
        if (it != g_wageOverrides.end()) {
            px = it->second.posX; pz = it->second.posZ; type = it->second.type;
            g_wageOverrides.erase(it);
        } else if (TsmIsValidGamePtr(building) &&
                   ReadablePtr(building, TSM_BLD_MIN_READABLE)) {
            (void)ReadBuildingPosition(building, &px, &pz);
            type = TsmBuildingType(building);
        }
        // Also drop pending records for the same spot (clear-before-spawn).
        for (auto jt = g_pendingOverrides.begin(); jt != g_pendingOverrides.end(); ) {
            if (jt->type == type &&
                fabsf(jt->posX - px) < 2.0f && fabsf(jt->posZ - pz) < 2.0f)
                jt = g_pendingOverrides.erase(jt);
            else
                ++jt;
        }
    }
    LeaveCriticalSection(&g_lock);
}

static float Api_GetEffectiveWageForBuilding(void* building)
{
    return ResolveWageForBuildingImpl(building);
}

// M3: runtime sector wage write (v4). Effective, active-currency
// units; the active-currency cfg keeps the value so it survives the
// next currency resolution pass. Lock-guarded: render-thread writes
// here race with the sim-thread payroll reads of g_sectorWage.
// Runtime-only: nothing writes it to the save, so a world load resets
// sectors from the INI (workplace overrides persist - M1).
static int Api_SetSectorWage(int sectorId, float wage)
{
    if (sectorId < 0 || sectorId >= TSM_SECTOR_COUNT) return 0;
    if (!(wage == wage) || !(wage > 0.0f)) return 0;

    float w = ClampToMinimum(wage);
    EnterCriticalSection(&g_lock);
    g_sectorWage[sectorId] = w;
    if (IsUsdActive()) g_sectorWageUsdCfg[sectorId] = w;
    else               g_sectorWageCfg[sectorId]    = w;
    LeaveCriticalSection(&g_lock);

    if (g_probe)
        Logf("citizen_labor  [SECTOR WAGE] sector %d (%s) -> %.2f %s",
             sectorId,
             (sectorId >= 0 && sectorId < TSM_SECTOR_COUNT)
                 ? "ok" : "?", w, TsmCurrencySymbol(g_currency));
    return 1;
}

static int Api_IsWageMobilityHookInstalled(void) { return g_wageMobilityHookInstalled; } // H4 FIX

// Pending v3 overrides (loaded before their buildings spawned): walk the
// live building list once per day and bind by type+coords. Runs inside
// OnDay under g_lock; the walk reuses the span OnDay already holds.
static void MatchPendingOverridesLocked(void* const* begin, void* const* end)
{
    if (g_pendingOverrides.empty() || !begin || !end || end <= begin) return;

    std::vector<PendingWageOverride> still;
    still.reserve(g_pendingOverrides.size());

    for (const auto& pend : g_pendingOverrides) {
        void* hit = nullptr;
        for (void* const* it = begin; it != end; ++it) {
            BYTE* b = (BYTE*)*it;
            if (!TsmIsValidGamePtr(b)) continue;
            if (!ReadablePtr(b, TSM_BLD_MIN_READABLE)) continue;
            if (TsmIsDemolished(b)) continue;
            if (TsmBuildingType(b) != pend.type) continue;
            float px = 0.0f, pz = 0.0f;
            if (!ReadBuildingPosition(b, &px, &pz)) continue;
            if (fabsf(px - pend.posX) < 2.0f && fabsf(pz - pend.posZ) < 2.0f) {
                hit = b;
                break;
            }
        }
        if (hit) {
            WageOverride o;
            o.wageRub = pend.wageRub;
            o.posX    = pend.posX;
            o.posZ    = pend.posZ;
            o.type    = pend.type;
            g_wageOverrides[hit] = o;
        } else {
            still.push_back(pend);
        }
    }
    g_pendingOverrides.swap(still);
}

// ---------------------------------------------------------------- persistence

#define LABOR_MAGIC          "TSMLABR1"
#define LABOR_VERSION        4u
#define LABOR_VERSION_V3     3u
#define LABOR_VERSION_V2     2u
#define LABOR_VERSION_LEGACY 1u
#define LABOR_FILENAME       "tesmio_labor.dat"

#pragma pack(push, 1)
struct LaborSaveHeader {
    char     magic[8];
    uint32_t version;
    uint32_t recordSize;
    uint32_t count;
    // M1: v3 repurposes this as the override count. Same header size, so
    // a v1.8 (v2) loader reads citizen records and ignores the tail.
    uint32_t overrideCount;
};
struct LaborSaveRecord {
    uint64_t citizenId;
    int32_t  buildingType;
    int32_t  lastPayMonth;
    int32_t  lastPayDay;
    int32_t  shiftsThisMonth;
    int32_t  consecutiveDefaults; // v2 only
};
// M1: one per workplace override, coords+type keyed (session pointers
// are never persisted). wageRub is the RUB base (currency-neutral).
struct LaborOverrideRecord {
    float   posX;
    float   posZ;
    int32_t type;
    float   wageRub;
};
#pragma pack(pop)

static void SaveLabor(const char* dir)
{
    std::vector<BYTE> buf;
    EnterCriticalSection(&g_lock);

    // B5 backstop: skip stale records at pack time (no erase under the
    // save-thread lock - the monthly pass does the erasing). Sidecars
    // on disk still get cleaned up, format unchanged.
    int now = CurrentAbsDay();
    std::vector<std::pair<uint64_t, EmploymentRecord>> live;
    live.reserve(g_records.size());
    for (const auto& kv : g_records) {
        int seen = kv.second.lastSeenAbsDay;
        if (g_pruneAfterDays > 0 && now >= 0 && seen >= 0 &&
            now - seen > g_pruneAfterDays)
            continue;
        live.emplace_back(kv.first, kv.second);
    }

    std::vector<LaborOverrideRecord> ovs;
    ovs.reserve(g_wageOverrides.size() + g_pendingOverrides.size());
    for (const auto& kv : g_wageOverrides) {
        if (!(kv.second.wageRub > 0.0f)) continue;
        LaborOverrideRecord o;
        o.posX    = kv.second.posX;
        o.posZ    = kv.second.posZ;
        o.type    = kv.second.type;
        o.wageRub = kv.second.wageRub;
        ovs.push_back(o);
    }
    for (const auto& pend : g_pendingOverrides) {
        if (!(pend.wageRub > 0.0f)) continue;
        LaborOverrideRecord o;
        o.posX    = pend.posX;
        o.posZ    = pend.posZ;
        o.type    = pend.type;
        o.wageRub = pend.wageRub;
        ovs.push_back(o);
    }

    size_t count = live.size();
    size_t total = sizeof(LaborSaveHeader) + count * sizeof(LaborSaveRecord) +
                   ovs.size() * sizeof(LaborOverrideRecord) +
                   TSM_SECTOR_COUNT * sizeof(float);
    buf.resize(total);

    LaborSaveHeader hdr;
    memcpy(hdr.magic, LABOR_MAGIC, 8);
    hdr.version       = LABOR_VERSION;
    hdr.recordSize    = (uint32_t)sizeof(LaborSaveRecord);
    hdr.count         = (uint32_t)count;
    hdr.overrideCount = (uint32_t)ovs.size();
    memcpy(buf.data(), &hdr, sizeof(hdr));

    BYTE* p = buf.data() + sizeof(hdr);
    for (const auto& kv : live) {
        LaborSaveRecord r;
        r.citizenId           = kv.first;
        r.buildingType        = kv.second.buildingType;
        r.lastPayMonth        = kv.second.lastPayMonth;
        r.lastPayDay          = kv.second.lastPayDay;
        r.shiftsThisMonth     = kv.second.shiftsThisMonth;
        r.consecutiveDefaults = kv.second.consecutiveDefaults;
        memcpy(p, &r, sizeof(r));
        p += sizeof(r);
    }
    for (const auto& o : ovs) {
        memcpy(p, &o, sizeof(o));
        p += sizeof(o);
    }
    // v4: sector base wages (RUB bases, currency-neutral).
    for (int s = 0; s < TSM_SECTOR_COUNT; s++) {
        memcpy(p, &g_sectorWageCfg[s], sizeof(float));
        p += sizeof(float);
    }
    LeaveCriticalSection(&g_lock);

    if (g_persistence->writeAtomic(dir, LABOR_FILENAME, buf.data(), total))
        Logf("citizen_labor  saved %zu employment record(s) + %zu wage override(s) into %s\\%s",
             count, ovs.size(), dir, LABOR_FILENAME);
    else
        Logf("citizen_labor  WARN  could not write %s\\%s",
             dir, LABOR_FILENAME);
}

static void ReadSectorWages(const char* ini);   // settings, defined below

// A sidecar that cannot be used is treated exactly like a missing one:
// no employment records, overrides or runtime sector wages from the
// previous world may survive into the newly loaded one. Caller holds
// g_lock.
static void ResetLoadedStateLocked(void)
{
    g_records.clear();
    g_zeroedWorkers.clear();
    g_wageOverrides.clear();
    g_pendingOverrides.clear();
    ReadSectorWages("plugins\\citizen_labor.ini");
    ResolveSectorWages();
}

static void LoadLabor(const char* dir)
{
    size_t len = 0;
    void* raw = g_persistence->readFile(dir, LABOR_FILENAME, &len);
    if (!raw) {
        EnterCriticalSection(&g_lock);
        ResetLoadedStateLocked();
        LeaveCriticalSection(&g_lock);

        Logf("citizen_labor  no %s in this save - starting with an empty registry",
             LABOR_FILENAME);
        return;
    }

    if (len < sizeof(LaborSaveHeader)) {
        EnterCriticalSection(&g_lock);
        ResetLoadedStateLocked();
        LeaveCriticalSection(&g_lock);
        Logf("citizen_labor  %s too short - ignored", LABOR_FILENAME);
        free(raw);
        return;
    }

    LaborSaveHeader hdr;
    memcpy(&hdr, raw, sizeof(hdr));

    size_t recordSize = 0;
    if (hdr.version == LABOR_VERSION || hdr.version == LABOR_VERSION_V3 ||
        hdr.version == LABOR_VERSION_V2) {
        recordSize = sizeof(LaborSaveRecord);
    } else if (hdr.version == LABOR_VERSION_LEGACY) {
        recordSize = offsetof(LaborSaveRecord, consecutiveDefaults);
    } else {
        Logf("citizen_labor  %s version %u, expected %u..%u - ignored",
             LABOR_FILENAME, hdr.version, LABOR_VERSION_LEGACY, LABOR_VERSION);
        EnterCriticalSection(&g_lock);
        ResetLoadedStateLocked();
        LeaveCriticalSection(&g_lock);
        free(raw);
        return;
    }

    if (memcmp(hdr.magic, LABOR_MAGIC, 8) != 0 ||
        hdr.recordSize != recordSize) {
        Logf("citizen_labor  %s header invalid - ignored", LABOR_FILENAME);
        EnterCriticalSection(&g_lock);
        ResetLoadedStateLocked();
        LeaveCriticalSection(&g_lock);
        free(raw);
        return;
    }

    size_t want = sizeof(hdr) + (size_t)hdr.count * recordSize;
    if (want > len) {
        Logf("citizen_labor  %s claims %u records but is only %zu bytes - ignored",
             LABOR_FILENAME, hdr.count, len);
        EnterCriticalSection(&g_lock);
        ResetLoadedStateLocked();
        LeaveCriticalSection(&g_lock);
        free(raw);
        return;
    }

    // M1: v3 appends override records after the citizen records. v2 files
    // carry 0 in the repurposed field (old saver wrote reserved = 0) and
    // parse exactly like before; a short v3 tail is tolerated.
    uint32_t ovCount = 0;
    const BYTE* ovBase = nullptr;
    bool ovTailOk = true;
    if (hdr.version == LABOR_VERSION) {
        ovCount = hdr.overrideCount;
        size_t ovWant = want + (size_t)ovCount * sizeof(LaborOverrideRecord);
        if (ovWant > len) {
            Logf("citizen_labor  %s override tail truncated (%u claimed) - loading citizens only",
                 LABOR_FILENAME, ovCount);
            // The sector-wage tail sits AFTER the override records, so a
            // truncated override block makes its offset unknowable too.
            // Reading at raw+want would interpret override bytes as wages.
            ovCount = 0;
            ovTailOk = false;
        } else if (ovCount > 0) {
            ovBase = (const BYTE*)raw + want;
        }
    }

    EnterCriticalSection(&g_lock);
    g_records.clear();
    g_zeroedWorkers.clear();
    g_wageOverrides.clear();
    g_pendingOverrides.clear();
    // A world load resets runtime sector wages from the INI. The v4 tail
    // below then overwrites the saved RUB bases (a zero there means
    // unconfigured and must stay zero, not inherit a since-edited INI).
    ReadSectorWages("plugins\\citizen_labor.ini");
    g_records.reserve(hdr.count);

    // B5: loaded survivors get a fresh lease - stamp now (memory-only,
    // no format change) so a long-lived save is not pruned on arrival.
    int now = CurrentAbsDay();
    const BYTE* p = (const BYTE*)raw + sizeof(hdr);
    for (uint32_t i = 0; i < hdr.count; i++) {
        LaborSaveRecord r = {};
        memcpy(&r, p, recordSize);
        p += recordSize;

        EmploymentRecord rec = {};
        rec.workplace           = nullptr;
        rec.buildingType        = r.buildingType;
        rec.lastPayMonth        = r.lastPayMonth;
        rec.lastPayDay          = r.lastPayDay;
        rec.shiftsThisMonth     = r.shiftsThisMonth;
        rec.lastSeenAbsDay      = now;
        // v4.7: bounded grace after load. A save written by a buggy build
        // may carry a stale buildingType for a citizen who no longer works
        // anywhere; stamping now gives them a 14-day window. Unseen at a
        // workplace in that window -> ValidateEmployment clears the record.
        rec.lastJobSeenAbsDay   = now;
        rec.consecutiveDefaults = (hdr.version == LABOR_VERSION ||
                                   hdr.version == LABOR_VERSION_V2)
                                  ? r.consecutiveDefaults : 0;
        rec.severedWorkplace    = nullptr;
        g_records.emplace(r.citizenId, rec);
    }
    size_t loaded = g_records.size();

    if (ovBase && ovCount > 0) {
        g_pendingOverrides.reserve(ovCount);
        for (uint32_t i = 0; i < ovCount; i++) {
            LaborOverrideRecord o = {};
            memcpy(&o, ovBase + (size_t)i * sizeof(o), sizeof(o));
            if (!(o.wageRub > 0.0f)) continue;
            PendingWageOverride pend;
            pend.posX    = o.posX;
            pend.posZ    = o.posZ;
            pend.type    = o.type;
            pend.wageRub = o.wageRub;
            g_pendingOverrides.push_back(pend);
        }
    }
    // v4: sector base wages are stored after the override records.
    if (hdr.version >= LABOR_VERSION && ovTailOk) {
        const BYTE* secBase = ovBase ? ovBase + (size_t)ovCount * sizeof(LaborOverrideRecord)
                                     : (const BYTE*)raw + want;
        size_t secWant = (size_t)(secBase - (const BYTE*)raw) + TSM_SECTOR_COUNT * sizeof(float);
        if (secWant <= len) {
            for (int s = 0; s < TSM_SECTOR_COUNT; s++) {
                float w = 0.0f;
                memcpy(&w, secBase + (size_t)s * sizeof(float), sizeof(float));
                // 0 = unconfigured; reject NaN/inf and absurd values,
                // then assign even a zero so the save is authoritative.
                if (!(w >= 0.0f) || !(w <= 1.0e6f)) w = 0.0f;
                g_sectorWageCfg[s] = w;
            }
        }
    }
    // The sidecar stores RUB bases; resolve for the ACTIVE currency now.
    // Start already ran, so nothing later would fix a USD-mode load.
    ResolveSectorWages();
    size_t loadedOv = g_pendingOverrides.size();
    LeaveCriticalSection(&g_lock);

    free(raw);
    Logf("citizen_labor  loaded %zu employment record(s) + %zu wage override(s) (format v%u) from %s\\%s",
         loaded, loadedOv, hdr.version, dir, LABOR_FILENAME);
}

static void OnSaveLoad(TsmSaveEvent ev, const char* dir, void* /*ud*/)
{
    if (!dir || !g_persistence) return;
    if (ev == TSM_SAVE_WRITING) SaveLabor(dir);
    else if (ev == TSM_SAVE_LOADING) LoadLabor(dir);
}

// ---------------------------------------------------------------- settings

static int ParseFrequency(const char* s)
{
    if (!s || !s[0]) return TSM_FREQ_MONTHLY;
    if (g_simClock) return (int)g_simClock->parseFrequency(
        s, (TsmScheduleFrequency)g_paydayFreq);
    if (_stricmp(s, "instant")   == 0) return TSM_FREQ_INSTANT;
    if (_stricmp(s, "daily")     == 0) return TSM_FREQ_DAILY;
    if (_stricmp(s, "weekly")    == 0) return TSM_FREQ_WEEKLY;
    if (_stricmp(s, "monthly")   == 0) return TSM_FREQ_MONTHLY;
    if (_stricmp(s, "quarterly") == 0) return TSM_FREQ_QUARTERLY;
    if (_stricmp(s, "yearly")    == 0) return TSM_FREQ_YEARLY;
    return TSM_FREQ_MONTHLY;
}

static void ParseExcludeList(const char* s)
{
    g_workplaceExcludeCount = 0;
    if (!s || !s[0]) return;
    while (*s && g_workplaceExcludeCount < 16) {
        while (*s == ' ' || *s == ',' || *s == '\t') s++;
        if (!*s) break;
        g_workplaceExclude[g_workplaceExcludeCount++] = atoi(s);
        while (*s && *s != ',') s++;
    }
}

static void ReadWageTable(const char* ini)
{
    char key[16];
    char val[32];
    g_wageTable.clear();
    for (int t = g_workplaceMin; t <= g_workplaceMax; t++) {
        if (IsExcludedWorkplace(t)) continue;
        _snprintf_s(key, sizeof(key), _TRUNCATE, "%d", t);
        DWORD n = H->configString(ini, "wages", key, val, sizeof(val), "");
        if (n == 0 || !val[0]) continue;
        float w = (float)atof(val);
        if (w > 0.0f && w == w) g_wageTable[t] = w;
    }
    // v4: explicit USD per-type overrides. Presence of the section
    // switches unlisted types to the USD default (see GetWageForTypeImpl).
    g_wageTableUsd.clear();
    g_wagesUsdPresent = 0;
    for (int t = g_workplaceMin; t <= g_workplaceMax; t++) {
        if (IsExcludedWorkplace(t)) continue;
        _snprintf_s(key, sizeof(key), _TRUNCATE, "%d", t);
        DWORD n = H->configString(ini, "wages_usd", key, val, sizeof(val), "");
        if (n == 0 || !val[0]) continue;
        float w = (float)atof(val);
        if (w > 0.0f && w == w) {
            g_wageTableUsd[t] = w;
            g_wagesUsdPresent = 1;
        }
    }
    // A [wages_usd] section with no parseable entries still counts as
    // present only when the section exists; probe via a sentinel read.
    if (!g_wagesUsdPresent) {
        char probe[8] = {0};
        DWORD n = H->configString(ini, "wages_usd", "__present__",
                                  probe, sizeof(probe), "");
        (void)n;
    }
}

static void ReadSectorWages(const char* ini)
{
    // M1: six canonical sector bases. RUB bases in [sector_wages] by
    // sector name; explicit USD in [sector_wages_usd]. 0.0 / missing =
    // unconfigured (falls through to the legacy type/default chain, so
    // old INIs behave like v1.8). Effective values resolve at Start.
    static const char* kSectorKeys[TSM_SECTOR_COUNT] = {
        "heavy_industry", "infrastructure", "healthcare_education",
        "light_industry", "services_retail", "agriculture"
    };
    char val[64];
    for (int s = 0; s < TSM_SECTOR_COUNT; s++) {
        g_sectorWageCfg[s]    = 0.0f;
        g_sectorWage[s]       = 0.0f;
        g_sectorWageUsdCfg[s] = -1.0f;
        if (H->configString(ini, "sector_wages", kSectorKeys[s],
                            val, sizeof(val), "") && val[0]) {
            float w = (float)atof(val);
            if (w > 0.0f && w == w && w <= 1.0e6f) g_sectorWageCfg[s] = w;
        }
        if (H->configString(ini, "sector_wages_usd", kSectorKeys[s],
                            val, sizeof(val), "") && val[0]) {
            float w = (float)atof(val);
            if (w > 0.0f && w == w && w <= 1.0e6f) g_sectorWageUsdCfg[s] = w;
        }
    }
}

static void ReadSettings(void)
{
    const char* ini = "plugins\\citizen_labor.ini";
    char v[128];

    g_enabled = H->configInt(ini, "citizen_labor", "enabled", 1);
    g_probe   = H->configInt(ini, "citizen_labor", "probe",   1);

    // v4.4: wage-driven labor mobility config.
    g_wageMobilityEnabled = H->configInt(ini, "citizen_labor", "wage_mobility_enabled", 1);
    if (H->configString(ini, "citizen_labor", "wage_mobility_log_coeff", v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 0.0f && f <= 2.0f && f == f) g_wageMobilityLogCoeff = f;
    }
    if (H->configString(ini, "citizen_labor", "wage_mobility_clamp_min", v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= -1.0f && f <= 1.0f && f == f) g_wageMobilityClampMin = f;
    }
    if (H->configString(ini, "citizen_labor", "wage_mobility_clamp_max", v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 0.0f && f <= 2.0f && f == f) g_wageMobilityClampMax = f;
    }
    if (H->configString(ini, "citizen_labor", "wage_mobility_var_base", v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 0.0f && f <= 1.0f && f == f) g_wageMobilityVarBase = f;
    }
    if (H->configString(ini, "citizen_labor", "wage_mobility_var_span", v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 0.0f && f <= 1.0f && f == f) g_wageMobilityVarSpan = f;
    }
    // sanity: clamp min must be < clamp max
    if (g_wageMobilityClampMin >= g_wageMobilityClampMax) {
        g_wageMobilityClampMin = -0.30f;
        g_wageMobilityClampMax =  0.60f;
    }

    // v4.1: auto-expanding slider ceiling. wage_slider_span_min is the span
    // floor (default 10.0); the live span is max(min, currentWage * 1.5),
    // so the 12.50 RUB hard ceiling is gone. The M3 wage_adjust_step key
    // is still accepted as a min alias (span = step x 100).
    g_panelRows = H->configInt(ini, "citizen_labor", "in_window_rows", 1);
    g_wageSliderSpanMin = 10.0f;
    if (H->configString(ini, "citizen_labor", "wage_slider_span_min",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 1.0f && f <= 100.0f && f == f) g_wageSliderSpanMin = f;
    } else if (H->configString(ini, "citizen_labor", "wage_slider_span",
                               v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 1.0f && f <= 100.0f && f == f) g_wageSliderSpanMin = f;
    } else if (H->configString(ini, "citizen_labor", "wage_adjust_step",
                               v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 0.01f && f <= 10.0f && f == f)
            g_wageSliderSpanMin = f * 100.0f;
        if (g_wageSliderSpanMin < 1.0f)   g_wageSliderSpanMin = 1.0f;
        if (g_wageSliderSpanMin > 100.0f) g_wageSliderSpanMin = 100.0f;
    }
    g_wageSliderStep = H->configInt(ini, "citizen_labor", "wage_slider_step", 2);
    if (g_wageSliderStep < 1)  g_wageSliderStep = 1;
    if (g_wageSliderStep > 50) g_wageSliderStep = 50;

    g_defaultDailyWageCfg  = 28.0f;
    g_unemployedWelfareCfg = 6.0f;
    g_studentStipendCfg    = 12.0f;
    g_retireePensionCfg    = 18.0f;
    // M1: the statutory floor (RUB base) + optional explicit USD floor.
    g_minimumWageCfg    = 2.50f;
    g_minimumWageUsdCfg = -1.0f;
    // M1: transfer ratios to the floor. Negative = absent (legacy scalar
    // path, so old INIs keep their numbers); >= 0 = ratio path.
    g_welfareRatioToMin = -1.0f;
    g_stipendRatioToMin = -1.0f;
    g_pensionRatioToMin = -1.0f;
    if (H->configString(ini, "citizen_labor", "default_daily_wage",
                        v, sizeof(v), "") && v[0])
        g_defaultDailyWageCfg = (float)atof(v);
    if (H->configString(ini, "citizen_labor", "unemployed_welfare",
                        v, sizeof(v), "") && v[0])
        g_unemployedWelfareCfg = (float)atof(v);
    if (H->configString(ini, "citizen_labor", "student_stipend",
                        v, sizeof(v), "") && v[0])
        g_studentStipendCfg = (float)atof(v);
    if (H->configString(ini, "citizen_labor", "retiree_pension",
                        v, sizeof(v), "") && v[0])
        g_retireePensionCfg = (float)atof(v);
    if (H->configString(ini, "citizen_labor", "minimum_wage",
                        v, sizeof(v), "") && v[0]) {
        float w = (float)atof(v);
        if (w > 0.0f && w == w) g_minimumWageCfg = w;
    }
    if (H->configString(ini, "citizen_labor", "minimum_wage_usd",
                        v, sizeof(v), "") && v[0]) {
        float w = (float)atof(v);
        if (w > 0.0f && w == w) g_minimumWageUsdCfg = w;
    }
    if (H->configString(ini, "citizen_labor", "welfare_ratio_to_min",
                        v, sizeof(v), "") && v[0]) {
        float r = (float)atof(v);
        if (r >= 0.0f && r == r) g_welfareRatioToMin = r;
    }
    if (H->configString(ini, "citizen_labor", "stipend_ratio_to_min",
                        v, sizeof(v), "") && v[0]) {
        float r = (float)atof(v);
        if (r >= 0.0f && r == r) g_stipendRatioToMin = r;
    }
    if (H->configString(ini, "citizen_labor", "pension_ratio_to_min",
                        v, sizeof(v), "") && v[0]) {
        float r = (float)atof(v);
        if (r >= 0.0f && r == r) g_pensionRatioToMin = r;
    }

    // Start RUB-effective; ResolveUsdScales at Start (once currency is
    // consumable) applies explicit USD keys or usd_divisor.
    g_defaultDailyWage  = g_defaultDailyWageCfg;
    g_unemployedWelfare = g_unemployedWelfareCfg;
    g_studentStipend    = g_studentStipendCfg;
    g_retireePension    = g_retireePensionCfg;
    // M1: floor + sector bases start RUB-effective too.
    g_minimumWage = g_minimumWageCfg;
    for (int s = 0; s < TSM_SECTOR_COUNT; s++)
        g_sectorWage[s] = g_sectorWageCfg[s];
    g_usdDivisor = 50.0f;
    if (H->configString(ini, "citizen_labor", "usd_divisor",
                        v, sizeof(v), "") && v[0]) {
        float d = (float)atof(v);
        if (d >= 1.0f && d <= 1000.0f && d == d) g_usdDivisor = d;
    }

    if (H->configString(ini, "citizen_labor", "student_age_max",
                        v, sizeof(v), "") && v[0])
        g_studentAgeMax = (float)atof(v);
    if (H->configString(ini, "citizen_labor", "retiree_age_min",
                        v, sizeof(v), "") && v[0])
        g_retireeAgeMin = (float)atof(v);

    if (H->configString(ini, "citizen_labor", "payday_frequency",
                        v, sizeof(v), "monthly") && v[0])
        g_paydayFreq = ParseFrequency(v);

    g_scalePayByShifts = H->configInt(ini, "citizen_labor",
                                      "scale_pay_by_shifts", 0);
    g_payOnFirstSight  = H->configInt(ini, "citizen_labor",
                                      "pay_on_first_sight", 0);

    // B5: 90 days = one fiscal quarter at the monthly payday default.
    // Touched by both daily passes, so live citizens never go stale.
    // 0 disables pruning entirely.
    g_pruneAfterDays   = H->configInt(ini, "citizen_labor",
                                      "prune_after_days", 90);
    if (g_pruneAfterDays < 0)    g_pruneAfterDays = 0;
    if (g_pruneAfterDays > 3650) g_pruneAfterDays = 3650;

    // v4.7: idle days after which a citizen's employment record is cleared
    // because they have not been seen at any workplace. Same configInt
    // pattern as prune_after_days. 0 disables ONLY the grace check
    // (demolished/decayed/dangling still clear).
    g_employmentGraceDays = H->configInt(ini, "citizen_labor",
                                         "employment_grace_days", 14);
    if (g_employmentGraceDays < 0)   g_employmentGraceDays = 0;
    if (g_employmentGraceDays > 365) g_employmentGraceDays = 365;

    if (H->configString(ini, "citizen_labor", "workplace_min",
                        v, sizeof(v), "3") && v[0])
        g_workplaceMin = atoi(v);
    if (H->configString(ini, "citizen_labor", "workplace_max",
                        v, sizeof(v), "200") && v[0])
        g_workplaceMax = atoi(v);
    if (H->configString(ini, "citizen_labor", "workplace_exclude",
                        v, sizeof(v), "20,30") && v[0])
        ParseExcludeList(v);

    if (g_workplaceMin < 0)   g_workplaceMin = 0;
    if (g_workplaceMax < g_workplaceMin) g_workplaceMax = g_workplaceMin;
    if (g_workplaceMax > 400) g_workplaceMax = 400;

    ReadWageTable(ini);
    ReadSectorWages(ini);
}

// =====================================================================
// v4.4: Wage-driven labor mobility hook
// =====================================================================

#define TSM_HOOK_LABOR_DEST_RVA   (g_tsmAddr.labor_mobility_hook)
#define TSM_HOOK_LABOR_DEST_ADDR  (g_exeBase + TSM_HOOK_LABOR_DEST_RVA)
// v4.6: the tail jmp's JBE target is RVA 0x1D37A9 (= hook site 0x1D3797 +
// the 18 stolen bytes = the instruction right after the stolen COMISS). It
// MUST be resolved from g_exeBase at install time - the old compile-time
// constant 0x1401D37A9 only held for a module loaded at the preferred base
// 0x140000000; under ASLR (real base e.g. 0x7FF759C60000) that constant is
// an unmapped address and the tail jmp faults. Computed exactly the way the
// hook site itself is.
#define TSM_HOOK_JBE_RVA          (g_tsmAddr.labor_mobility_jbe)
#define TSM_HOOK_JBE_TARGET       ((uintptr_t)(g_exeBase + TSM_HOOK_JBE_RVA))
#define TSM_HOOK_STOLEN           18u

static const BYTE kHookExpect[TSM_HOOK_STOLEN] = {
    0xF3, 0x0F, 0x10, 0x84, 0x8A, 0x1C, 0x12, 0x00, 0x00,
    0x41, 0x0F, 0x2F, 0x84, 0x8E, 0x1C, 0x12, 0x00, 0x00
};

extern "C" float __declspec(noinline) WageMobilityCompute(
    void* candidate, void* person, void* homeBld, int needIdx)
{
    __try {
        if (!g_wageMobilityEnabled) {
            if (ReadablePtr(candidate, 0x1220))
                return *(float*)((BYTE*)candidate + 0x121C + (size_t)needIdx * 4);
            return 0.0f;
        }
        if (!ReadablePtr(candidate, 0x1220)) return 0.0f;
        float nativeScore = *(float*)((BYTE*)candidate + 0x121C + (size_t)needIdx * 4);
        if (!(nativeScore == nativeScore) || !(nativeScore > 0.0f)) return nativeScore;
        if (!ReadablePtr(homeBld, 0x1220)) return nativeScore;
        float homeScore = *(float*)((BYTE*)homeBld + 0x121C + (size_t)needIdx * 4);
        if (!(homeScore == homeScore) || !(homeScore > 0.0f)) return nativeScore;

        float candWage = ResolveWageForBuildingImpl(candidate);
        if (!(candWage > 0.0f)) return nativeScore;
        float minWage = g_minimumWage;
        if (!(minWage > 0.0f)) return nativeScore;

        float ratio = candWage / minWage;
        float logRatio = logf(ratio);
        float wageMult = 1.0f + __max(g_wageMobilityClampMin,
                           __min(g_wageMobilityLogCoeff * logRatio,
                                 g_wageMobilityClampMax));

        float seed = 0.5f;
        if (ReadablePtr(person, 0xA8)) {
            seed = *(float*)((BYTE*)person + TSM_PERSON_SPEED_SEED);
            seed = fmodf(fabsf(seed), 1.0f);
        }
        float personalVariance = g_wageMobilityVarBase +
                                 g_wageMobilityVarSpan * seed;

        float effectiveScore = nativeScore * wageMult * personalVariance;

        if (g_probe) {
            // Latched probe log: one line per distinct candidate building
            // pointer per session. POD fixed-array latch (no allocation, no
            // C++ unwind) so it stays safe inside this __try/__except frame.
            // First 64 distinct pointers are logged; after that the stub
            // stays silent for the rest of the session.
            static void*  g_loggedMobilityCands[64] = {};
            static size_t g_loggedMobilityCount = 0;
            bool          alreadyLogged = false;
            for (size_t i = 0; i < g_loggedMobilityCount; i++) {
                if (g_loggedMobilityCands[i] == candidate) {
                    alreadyLogged = true;
                    break;
                }
            }
            if (!alreadyLogged && g_loggedMobilityCount < 64) {
                g_loggedMobilityCands[g_loggedMobilityCount++] = candidate;
                alreadyLogged = true;
            }
            if (alreadyLogged) {
                Logf("citizen_labor [WAGE MOBILITY] cand=%p wage=%.2f mult=%.2f "
                     "var=%.2f score=%.4f->%.4f",
                     candidate, candWage, wageMult, personalVariance,
                     nativeScore, effectiveScore);
            }
        }
        return effectiveScore;
    }
    __except (FaultFilter("citizen_labor wage mobility compute",
                          GetExceptionInformation())) {
        return 0.0f;
    }
}
//
// -------------------------------------------------------------------
// Assembly stub (raw x64 machine code, allocated executable).
//
// Layout (offsets from stub base). Frame = 0x70 bytes (112), a multiple of
// 16, so RSP stays 16-byte aligned at the `call rax`. Alignment arithmetic:
// the hook is entered by an inline jmp (not a call), so RSP is already
// 16-aligned at stub entry (MSVC invariant at any instruction boundary);
// sub rsp, 0x70 keeps it 16-aligned because 0x70 % 16 == 0; the Win64 ABI
// requires RSP % 16 == 0 before a call, so the callee sees RSP % 16 == 8
// after the return address is pushed.
//
//   0x00: sub rsp, 0x70          ; allocate frame (preserves 16-byte alignment)
//   0x04: mov [rsp+0x00], rdx    ; save candidate (RDX)
//   0x09: mov [rsp+0x08], rsi    ; save RSI = person   (hook-site registers:
//                                  RDX=candidate, RCX=needIdx, R14=homeBld,
//                                  RSI=person)
//   0x0E: mov [rsp+0x10], r14    ; save R14 = homeBld  (callee-saved, never
//                                  clobbered by the stub; the stolen COMISS
//                                  below reads [r14+rcx*4+0x121c])
//   0x13: mov [rsp+0x18], rcx    ; save needIdx (RCX)
//   0x18: mov rdx, [rsp+0x00]    ; RDX = candidate (scratch)
//   0x1D: mov rcx, rdx           ; RCX = candidate   -> arg 1
//   0x20: mov rdx, [rsp+0x08]    ; RDX = saved RSI   -> arg 2 = person
//   0x25: mov r8,  [rsp+0x10]    ; R8  = saved R14   -> arg 3 = homeBld
//   0x2A: mov r9,  [rsp+0x18]    ; R9  = needIdx     -> arg 4
//   0x2F: movaps [rsp+0x20], xmm1; save volatile XMM1..XMM5 (one 16-byte
//   0x34: movaps [rsp+0x30], xmm2;   slot each, all inside the frame)
//   0x39: movaps [rsp+0x40], xmm3;
//   0x3E: movaps [rsp+0x50], xmm4;
//   0x43: movaps [rsp+0x60], xmm5;
//   0x48: mov rax, imm64         ; address of WageMobilityCompute (patched)
//   0x52: call rax               ; XMM0 = returned boosted score (intended)
//   0x54: movaps xmm1, [rsp+0x20]; restore XMM1..XMM5 (XMM0 left as return)
//   0x59: movaps xmm2, [rsp+0x30];
//   0x5E: movaps xmm3, [rsp+0x40];
//   0x63: movaps xmm4, [rsp+0x50];
//   0x68: movaps xmm5, [rsp+0x60];
//   0x6D: mov rcx, [rsp+0x18]    ; restore needIdx for the stolen COMISS
//   0x72: add rsp, 0x70          ; deallocate frame
//   0x76: comiss xmm0,[r14+rcx*4+0x121c]  ; stolen COMISS (R14=homeBld intact)
//   0x7F: jmp rel32              ; to 0x1401D37A9 (native JBE) (patched)
// -------------------------------------------------------------------
static BYTE*   g_laborMobilityStub   = nullptr;
static void*   g_laborMobilityTrampoline = nullptr;

static const BYTE kStubBase[] = {
    0x48, 0x83, 0xEC, 0x70,                       // sub rsp, 0x70
    0x48, 0x89, 0x54, 0x24, 0x00,                 // mov [rsp+0x00], rdx   ; save candidate
    0x48, 0x89, 0x74, 0x24, 0x08,                 // mov [rsp+0x08], rsi   ; save RSI = person
    0x4C, 0x89, 0x74, 0x24, 0x10,                 // mov [rsp+0x10], r14   ; save R14 = homeBld
    0x48, 0x89, 0x4C, 0x24, 0x18,                 // mov [rsp+0x18], rcx   ; save needIdx
    0x48, 0x8B, 0x54, 0x24, 0x00,                 // mov rdx, [rsp+0x00]   ; RDX = candidate (scratch)
    0x48, 0x89, 0xC9,                             // mov rcx, rdx        ; RCX = candidate (arg 1)
    0x48, 0x8B, 0x54, 0x24, 0x08,                 // mov rdx, [rsp+0x08]   ; RDX = saved RSI (arg 2)
    0x4C, 0x8B, 0x44, 0x24, 0x10,                 // mov r8, [rsp+0x10]    ; R8 = saved R14 (arg 3)
    0x4C, 0x8B, 0x4C, 0x24, 0x18,                 // mov r9, [rsp+0x18]    ; R9 = needIdx (arg 4)
    0x0F, 0x29, 0x4C, 0x24, 0x20,                 // movaps [rsp+0x20], xmm1
    0x0F, 0x29, 0x54, 0x24, 0x30,                 // movaps [rsp+0x30], xmm2
    0x0F, 0x29, 0x5C, 0x24, 0x40,                 // movaps [rsp+0x40], xmm3
    0x0F, 0x29, 0x64, 0x24, 0x50,                 // movaps [rsp+0x50], xmm4
    0x0F, 0x29, 0x6C, 0x24, 0x60,                 // movaps [rsp+0x60], xmm5
    0x48, 0xB8, 0xAA, 0xAA, 0xAA, 0xAA,           // mov rax, imm64 (patched at STUB_OFF_RAX_IMM)
    0xAA, 0xAA, 0xAA, 0xAA,
    0xFF, 0xD0,                                   // call rax
    0x0F, 0x28, 0x4C, 0x24, 0x20,                 // movaps xmm1, [rsp+0x20]
    0x0F, 0x28, 0x54, 0x24, 0x30,                 // movaps xmm2, [rsp+0x30]
    0x0F, 0x28, 0x5C, 0x24, 0x40,                 // movaps xmm3, [rsp+0x40]
    0x0F, 0x28, 0x64, 0x24, 0x50,                 // movaps xmm4, [rsp+0x50]
    0x0F, 0x28, 0x6C, 0x24, 0x60,                 // movaps xmm5, [rsp+0x60]
    0x48, 0x8B, 0x4C, 0x24, 0x18,                 // mov rcx, [rsp+0x18]   ; RCX = needIdx
    0x48, 0x83, 0xC4, 0x70,                       // add rsp, 0x70
    0x41, 0x0F, 0x2F, 0x84, 0x8E, 0x1C, 0x12, 0x00, 0x00,  // comiss xmm0,[r14+rcx*4+0x121c]
    0xE9, 0xBB, 0xBB, 0xBB, 0xBB                  // jmp rel32 (patched at STUB_OFF_JMP_REL32)
};

#define STUB_OFF_RAX_IMM   74u   // offset of imm64 field in `mov rax, imm64`
#define STUB_OFF_JMP_REL32 128u  // offset of rel32 field in `jmp rel32`

static_assert(sizeof(kStubBase) == 132u,
              "kStubBase size changed - re-verify the frame layout and the "
              "two patch offsets below");
static_assert(STUB_OFF_RAX_IMM == 74u,
              "STUB_OFF_RAX_IMM must point at the imm64 field of `mov rax, imm64`");
static_assert(STUB_OFF_JMP_REL32 == 128u,
              "STUB_OFF_JMP_REL32 must point at the rel32 field of `jmp rel32`");
// v4.6: lock the tail-jmp patch to the allocation - the rel32 field is 4 bytes
// at STUB_OFF_JMP_REL32 and the `jmp rel32` (opcode + rel32) is the last
// instruction, so the array must hold all 4 rel32 bytes and the memcpy that
// copies it into the AllocNear'd page (size = sizeof) can never run past the
// end. This is the alignment the crash report asked to verify.
static_assert(sizeof(kStubBase) >= STUB_OFF_JMP_REL32 + 4u,
              "kStubBase must hold the full 4-byte rel32 field of the tail jmp");

static bool InstallLaborMobilityHook(void)
{
    if (!g_exeBase || !TsmCheckExeBuild(H, "citizen_labor") ||
        !TSM_HOOK_LABOR_DEST_RVA || !TSM_HOOK_JBE_RVA)
        return false;

    // v4.6: the tail `jmp rel32` (kStubBase +0x7F) can only reach +-2GB. The
    // JBE target lives in the exe (~0x7FF7_....); a VirtualAlloc(NULL) stub
    // lands ~2 TB away and the rel32 truncates to garbage, so the tail jmp
    // faulted on the first workplace selection. AllocNear pins the stub within
    // ~768 MB of the target (the loader walks allocation granularity outward
    // from the anchor), which is well inside rel32 range.
    g_laborMobilityStub = AllocNear(g_exeBase + TSM_HOOK_JBE_RVA,
                                    sizeof(kStubBase));
    if (!g_laborMobilityStub) {
        Logf("citizen_labor  WAGE MOBILITY hook FAILED (AllocNear - no page "
             "within rel32 range of the JBE target) — private_market_wages will use static INI wages");
        g_wageMobilityHookInstalled = 0;
        return false;
    }

    memcpy(g_laborMobilityStub, kStubBase, sizeof(kStubBase));

    void* computeAddr = (void*)&WageMobilityCompute;
    memcpy(g_laborMobilityStub + STUB_OFF_RAX_IMM, &computeAddr, 8);

    uintptr_t jmpTarget = TSM_HOOK_JBE_TARGET;
    uintptr_t jmpNext   = (uintptr_t)(g_laborMobilityStub + sizeof(kStubBase));
    int32_t   rel32     = (int32_t)(jmpTarget - jmpNext);
    memcpy(g_laborMobilityStub + STUB_OFF_JMP_REL32, &rel32, 4);

    FlushInstructionCache(GetCurrentProcess(), g_laborMobilityStub, sizeof(kStubBase));

    // v4.6: one-shot install diagnostic. Proves the resolved tail-jump target
    // and next-instruction address against the live allocation, and dumps the
    // tail bytes so a bad rel32 or a fall-off-the-end is readable straight
    // from the log: stub[0x78..0x7E] = stolen COMISS tail, [0x7F..0x83] =
    // patched jmp rel32, [0x84..0x87] = page bytes past the 132-byte stub.
    {
        char hex[3 * 16 + 1]; int o = 0;
        for (int i = 0x78; i <= 0x87 && o < (int)sizeof(hex) - 4; i++)
            o += _snprintf_s(hex + o, sizeof(hex) - o, _TRUNCATE, "%02X ",
                             g_laborMobilityStub[i]);
        Logf("citizen_labor  [WAGE STUB] size=%u stub=%p target=%p next=%p rel32=%d",
             (unsigned)sizeof(kStubBase), (void*)g_laborMobilityStub,
             (void*)jmpTarget, (void*)jmpNext, (int)rel32);
        Logf("citizen_labor  [WAGE STUB] expect target=exeBase+0x%X (%p)  "
             "tail[0x78..0x87]=%s",
             (unsigned)TSM_HOOK_JBE_RVA, (void*)jmpTarget, hex);
    }

    if (!InstallInlineHook(TSM_HOOK_LABOR_DEST_ADDR, g_laborMobilityStub,
                           &g_laborMobilityTrampoline, kHookExpect,
                           TSM_HOOK_STOLEN, "citizen_labor wage mobility"))
    {
        VirtualFree(g_laborMobilityStub, 0, MEM_RELEASE);
        g_laborMobilityStub = nullptr;
        g_wageMobilityHookInstalled = 0;
        Logf("citizen_labor  WAGE MOBILITY hook FAILED (InstallInlineHook) — static INI wages only");
        return false;
    }

    g_wageMobilityHookInstalled = 1;
    Logf("citizen_labor  wage mobility hook installed at RVA 0x%06X",
         TSM_HOOK_LABOR_DEST_RVA);
    return true;
}
// In-window wage tuning with Tropico-style scope control. Two providers
// on the ui_panels v2 contract, both render-thread, both gated to
// STATE-operated workplaces (private [P] buildings keep the M2 wage rows
// and are skipped here):
//
//   LaborPanelRows    three read-only text rows: the resolved Wage, the
//                     Scope mode, and the Tune hint documenting the slider
//                     range. Visible even when widgets are unavailable.
//   LaborPanelWidgets two native clickable controls:
//                     wage slider 0..100 -> [floor, floor + span],
//                     scope checkbox -> Whole Sector vs Workplace Only.
//
// Workplace writes go to setWorkplaceWageOverride (persisted by
// coordinates in tesmio_labor.dat); sector writes go to setSectorWage
// and also clear this building's own override so it inherits the
// sector rate cleanly. Both setters clamp to the statutory minimum.
// Sector writes are runtime-only - they reset from citizen_labor.ini
// on the next world load.

static const wchar_t* kSectorNames[TSM_SECTOR_COUNT] = {
    L"Heavy Industry", L"Infrastructure", L"Health/Education",
    L"Light Industry", L"Retail/Services", L"Agriculture"
};

static void PushLaborRow(TsmPanelRows* out, const wchar_t* label,
                         const wchar_t* value)
{
    if (out->count >= out->capacity || out->count >= TSM_PANEL_MAX_ROWS)
        return;
    TsmPanelRow* r = &out->rows[out->count++];
    wcsncpy_s(r->label, TSM_PANEL_LABEL, label, _TRUNCATE);
    wcsncpy_s(r->value, TSM_PANEL_VALUE, value, _TRUNCATE);
    r->colour = 0;   // the hook's default label colour
}

// Render-thread gate shared by the text rows and the widgets: state
// workplaces only. Private [P] buildings keep enterprise_base's own wage
// rows (M2); City Hall (type 39) belongs to the taxes fiscal panel.
// Returns the building type, or -1 when this window gets no labor rows.
static int LaborWindowType(void* building)
{
    if (!g_panelRows || !building) return -1;
    if (!TsmIsValidGamePtr(building) ||
        !ReadablePtr(building, TSM_BLD_MIN_READABLE)) return -1;
    if (TsmIsDemolished(building)) return -1;

    int type = TsmBuildingType(building);
    if (type < g_workplaceMin || type > g_workplaceMax) return -1;
    if (IsExcludedWorkplace(type)) return -1;
    if (type == 39) return -1;   // City Hall: the taxes panel owns it

    if (g_enterprise) {
        TsmEnterpriseInfo info;
        memset(&info, 0, sizeof(info));
        info.structSize = sizeof(TsmEnterpriseInfo);
        if (g_enterprise->describeEnterprise(building, &info) &&
            info.isPrivate)
            return -1;
    }
    return type;
}

// Slider mapping: 0..100 -> [floor, floor + span] in active-currency
// units. Floor is the live statutory minimum; span auto-expands with the
// current wage (v4.1: no hard ceiling): span = max(spanMin, wage * 1.5).
static float WageSliderFloor(void)
{
    float f = g_minimumWage;
    if (!(f > 0.0f) || !(f == f)) f = 2.50f;
    return f;
}

static float WageSliderSpan(float wage)
{
    float m = (g_wageSliderSpanMin > 1.0f) ? g_wageSliderSpanMin : 10.0f;
    float want = wage * 1.5f;
    if (!(want == want)) want = m;
    if (want < m) want = m;
    if (want > 100000.0f) want = 100000.0f;
    return want;
}

static unsigned WageToSlider(float wage, float span)
{
    float f = WageSliderFloor();
    if (!(span > 0.0f) || !(span == span)) span = 10.0f;
    float p = (wage - f) * 100.0f / span;
    if (!(p == p)) p = 0.0f;
    if (p < 0.0f)   p = 0.0f;
    if (p > 100.0f) p = 100.0f;
    return (unsigned)(p + 0.5f);
}

static float SliderToWage(unsigned pct, float span)
{
    if (pct > 100) pct = 100;
    if (!(span > 0.0f) || !(span == span)) span = 10.0f;
    return WageSliderFloor() + span * (float)pct / 100.0f;
}

static void PollWageKeys(void* building)
{
    // v4: hotkeys removed - the wage slider + scope checkbox own
    // interaction now. Kept as a no-op stub so the diff stays reviewable;
    // LaborPanelRows no longer calls it.
    (void)building;
    return;
}

static void LaborPanelRows(void* building, TsmPanelRows* out, void* /*ud*/)
{
    if (!out) return;
    int type = LaborWindowType(building);
    if (type < 0) return;

    int sector = SectorForBuildingType(type);
    const wchar_t* sym =
        (TsmCurrencyActive(g_currency) == TSM_CURRENCY_USD) ? L"USD" : L"RUB";

    float eff = 0.0f;
    int mode = 0;
    EnterCriticalSection(&g_lock);
    auto mit  = g_scopeMode.find(building);
    mode      = (mit != g_scopeMode.end()) ? mit->second : 0;
    LeaveCriticalSection(&g_lock);

    eff = ResolveWageForBuildingImpl(building);
    if (!(eff > 0.0f)) eff = GetWageForTypeImpl(type);

    wchar_t v[TSM_PANEL_VALUE];

    // v4.1: ONE status row - Wage + Scope together, so no text label sits
    // under the slider's [-] button. Format:
    //   Wage: X.XX [SYM] (Scope: Workplace / Sector)
    const wchar_t* scopeName;
    if (mode == 1 && sector >= 0 && sector < TSM_SECTOR_COUNT)
        scopeName = L"Sector";
    else
        scopeName = L"Workplace";
    _snwprintf_s(v, _countof(v), _TRUNCATE,
                 L"Wage: %.2f %ls (Scope: %ls)", eff, sym, scopeName);
    PushLaborRow(out, L"Status", v);

    float span = WageSliderSpan(eff);
    _snwprintf_s(v, _countof(v), _TRUNCATE,
                 L"Slider %.2f-%.2f %ls", WageSliderFloor(),
                 WageSliderFloor() + span, sym);
    PushLaborRow(out, L"Tune", v);
}

// v4 widget provider: the native wage slider + scope checkbox. Runs on
// the render thread inside the ui_panels post-hook, AFTER the text rows.
// Commit-on-change only: copies under g_lock, calls the helper with NO
// lock held, commits through the M1/v4 setters on change.
static void LaborPanelWidgets(void* building, void* /*ud*/)
{
    if (!g_panelsWidgets || !g_panels) return;
    int type = LaborWindowType(building);
    if (type < 0) return;
    // v4.2: construction-site panels (run through ui_panels v4's
    // appendWidgetsToWindow for the CON tile) are unfinished buildings -
    // no workers, no wages, so no slider there. The pass runs on those
    // windows only since v4.9; this keeps them vanilla.
    if (ReadablePtr((BYTE*)building + TSM_BLD_BUILT, sizeof(float)) &&
        *(const float*)((BYTE*)building + TSM_BLD_BUILT) < 1.0f)
        return;
    int sector = SectorForBuildingType(type);

    float eff = ResolveWageForBuildingImpl(building);
    if (!(eff > 0.0f)) eff = GetWageForTypeImpl(type);
    int mode = 0;
    EnterCriticalSection(&g_lock);
    auto mit = g_scopeMode.find(building);
    mode = (mit != g_scopeMode.end() && mit->second != 0) ? 1 : 0;
    LeaveCriticalSection(&g_lock);

    unsigned pct = WageToSlider(eff, WageSliderSpan(eff));
    if (g_panels->widgetSlider(L"Wage", &pct, g_wageSliderStep, 1)) {
        float next = SliderToWage(pct, WageSliderSpan(eff));
        if (mode == 1 && sector >= 0) {
            Api_SetSectorWage(sector, next);
            Api_ClearWorkplaceWageOverride(building);
        } else {
            Api_SetWorkplaceWageOverride(building, next);
        }
        if (g_probe)
            Logf("citizen_labor  [WIDGET WAGE] %p (type %d) -> %.2f %s (%s)",
                 building, type, next, TsmCurrencySymbol(g_currency),
                 (mode == 1 && sector >= 0) ? "sector" : "workplace");
    }

    if (sector < 0) return;   // unmapped type: no sector to scope to
    // v5.0: the engine's native o_Checkbox renders the visual checkmark
    // inversely to numeric 1u - a box that visually reads [X] carries sq==0.
    // Invert so [X] on screen means Sector mode (1):
    unsigned sq = (mode == 1) ? 0u : 1u;
    if (g_panels->widgetCheckbox(&sq, L"Apply to Whole Sector", 0)) {
        // sq == 0 is rendered as checked -> Whole Sector; sq != 0 is unchecked -> Workplace Only
        int nm = (sq == 0) ? 1 : 0;
        EnterCriticalSection(&g_lock);
        g_scopeMode[building] = nm;
        LeaveCriticalSection(&g_lock);
        if (g_probe)
            Logf("citizen_labor  [WIDGET SCOPE] %p (type %d) -> %s", building,
                 type, nm ? "Whole Sector" : "Workplace Only");
    }
}

// ---------------------------------------------------------------- the API

static const TsmCitizenLaborApi kApi = {
    sizeof(TsmCitizenLaborApi),
    Api_GetDefaultDailyWage,
    Api_GetUnemployedWelfare,
    Api_GetStudentStipend,
    Api_GetRetireePension,
    Api_GetWageForType,
    Api_GetEffectiveDailyWage,
    Api_IsEmployed,
    Api_GetWorkplace,
    Api_GetEmployedBuildingType,
    RegisterInterceptor,
    ClearInterceptor,
    Api_PayAllNow,
    // --- v3 tail (M1): floor, sectors, overrides
    Api_GetMinimumWage,
    Api_GetSectorWage,
    Api_SetWorkplaceWageOverride,
    Api_ClearWorkplaceWageOverride,
    Api_GetEffectiveWageForBuilding,
    // --- v4 tail (M3): runtime sector wage write
    Api_SetSectorWage,
    // --- v5 tail: wage mobility hook status (H4 FIX)
    Api_IsWageMobilityHookInstalled,
};

// ---------------------------------------------------------------- lifecycle

extern "C" __declspec(dllexport) unsigned TsmPluginApiVersion(void)
{
    return TSM_API_VERSION;
}

extern "C" __declspec(dllexport) int TsmPluginInit(const TsmHost* host,
                                                   TsmPluginInfo* info)
{
    TsmBind(host);
    info->name    = "citizen_labor";
    info->version = "4.9"; // H4 FIX: wage mobility hook status v5

    ReadSettings();
    if (!g_enabled) {
        Logf("citizen_labor  disabled");
        return 1;
    }

    g_records.reserve(8192);

    // M1 ABI: the registry matches versions exactly, so publish both the
    // v3 iface (full struct) and a v2 view (prefix only, truncated
    // structSize). enterprise_base consumes 2u and older
    // private_construction builds consume TSM_CITIZEN_LABOR_VERSION from
    // their own headers - both keep reading exactly the prefix they
    // expect; v3 consumers get the floor/sector/override tail.
    // M3 ABI: the registry matches versions exactly, so publish the v2
    // prefix (enterprise_base), the v3 prefix (pre-M3 consumers) and
    // the full v4 iface. Every view carries at least its own tail; a
    // consumer reading past its version's structSize gets nothing,
    // which is the whole point.
    static TsmCitizenLaborApi kApiV2, kApiV3, kApiV4;
    memcpy(&kApiV2, &kApi, sizeof(kApiV2));
    kApiV2.structSize =
        (unsigned)offsetof(TsmCitizenLaborApi, getMinimumWage);
    memcpy(&kApiV3, &kApi, sizeof(kApiV3));
    kApiV3.structSize =
        (unsigned)offsetof(TsmCitizenLaborApi, setSectorWage);
    memcpy(&kApiV4, &kApi, sizeof(kApiV4));
    kApiV4.structSize =
        (unsigned)offsetof(TsmCitizenLaborApi, isWageMobilityHookInstalled);
    H->provide(TSM_SERVICE_CITIZEN_LABOR,
               TSM_CITIZEN_LABOR_VERSION_V2, &kApiV2);
    H->provide(TSM_SERVICE_CITIZEN_LABOR,
               TSM_CITIZEN_LABOR_VERSION_V3, &kApiV3);
    H->provide(TSM_SERVICE_CITIZEN_LABOR,
               TSM_CITIZEN_LABOR_VERSION_V4, &kApiV4);
    H->provide(TSM_SERVICE_CITIZEN_LABOR,
               TSM_CITIZEN_LABOR_VERSION, &kApi);
    return 0;
}

extern "C" __declspec(dllexport) int TsmPluginStart(void)
{
    if (!g_enabled) return 1;

    g_simClock = (const TsmSimClockApi*)
        H->consume(TSM_SERVICE_SIM_CLOCK, TSM_SIM_CLOCK_VERSION);
    if (!g_simClock) {
        Logf("citizen_labor  sim_clock is not installed - payroll will not run.");
        return 1;
    }

    // v4: prefer the unified-currency iface; fall back to v3 (RUB-only
    // routing) so payroll still runs in a mixed install.
    g_currency = (const TsmCurrencyApi*)
        H->consume(TSM_SERVICE_CURRENCY, TSM_CURRENCY_VERSION);
    if (!g_currency)
        g_currency = (const TsmCurrencyApi*)
            H->consume(TSM_SERVICE_CURRENCY, TSM_CURRENCY_VERSION_V3);
    if (!g_currency) {
        Logf("citizen_labor  currency is not installed - wages cannot be paid.");
        return 1;
    }

    // M1 + v4 currency calibration. Runs at Start (not Init) because the
    // active currency is only known once the service can be consumed.
    // Order matters: the floor resolves FIRST, then transfers derive
    // from it (ratio path) or from their own explicit keys/divisor,
    // then sectors resolve and clamp to the floor. Explicit *_usd keys
    // always win; otherwise USD-effective figures divide RUB by divisor.
    {
        const char* ini = "plugins\\citizen_labor.ini";
        char v[64];
        int usd = (TsmCurrencyActive(g_currency) == TSM_CURRENCY_USD);
        float d = UsdScale();

        // --- floor
        if (usd) {
            if (g_minimumWageUsdCfg > 0.0f)
                g_minimumWage = g_minimumWageUsdCfg;
            else
                g_minimumWage = g_minimumWageCfg / d;
        } else {
            g_minimumWage = g_minimumWageCfg;
        }
        if (!(g_minimumWage > 0.0f)) g_minimumWage = g_minimumWageCfg;

        // --- default wage
        if (usd) {
            if (H->configString(ini, "citizen_labor", "default_daily_wage_usd",
                                v, sizeof(v), "") && v[0]) {
                float u = (float)atof(v);
                if (u > 0.0f && u == u) g_defaultDailyWage = u;
            } else {
                g_defaultDailyWage = g_defaultDailyWageCfg / d;
            }
        }
        g_defaultDailyWage = ClampToMinimum(g_defaultDailyWage);

        // --- transfers: ratio-to-floor when configured, else explicit
        // USD key, else scaled/divided legacy scalar. Floor clamp last.
        if (g_welfareRatioToMin >= 0.0f) {
            g_unemployedWelfare = g_welfareRatioToMin * g_minimumWage;
        } else if (usd) {
            if (H->configString(ini, "citizen_labor", "unemployed_welfare_usd",
                                v, sizeof(v), "") && v[0]) {
                float u = (float)atof(v);
                if (u > 0.0f && u == u) g_unemployedWelfare = u;
            } else {
                g_unemployedWelfare = g_unemployedWelfareCfg / d;
            }
        }
        if (g_stipendRatioToMin >= 0.0f) {
            g_studentStipend = g_stipendRatioToMin * g_minimumWage;
        } else if (usd) {
            if (H->configString(ini, "citizen_labor", "student_stipend_usd",
                                v, sizeof(v), "") && v[0]) {
                float u = (float)atof(v);
                if (u > 0.0f && u == u) g_studentStipend = u;
            } else {
                g_studentStipend = g_studentStipendCfg / d;
            }
        }
        if (g_pensionRatioToMin >= 0.0f) {
            g_retireePension = g_pensionRatioToMin * g_minimumWage;
        } else if (usd) {
            if (H->configString(ini, "citizen_labor", "retiree_pension_usd",
                                v, sizeof(v), "") && v[0]) {
                float u = (float)atof(v);
                if (u > 0.0f && u == u) g_retireePension = u;
            } else {
                g_retireePension = g_retireePensionCfg / d;
            }
        }
        // Transfers are deliberately NOT clamped to the floor: the spec
        // defines them as FRACTIONS of the minimum wage (welfare 40%,
        // stipend 60%, pension 80%), i.e. legitimately below it. Only
        // wages are floored.

        // --- sectors: explicit USD wins, else scaled RUB; clamp to floor.
        ResolveSectorWages();
    }

    g_persistence = (const TsmPersistenceApi*)
        H->consume(TSM_SERVICE_PERSISTENCE, TSM_PERSISTENCE_VERSION);
    if (!g_persistence)
        Logf("citizen_labor  WARN  no persistence service - employment state will not survive a save/load.");
    else {
        g_persHandle = g_persistence->registerHandler(
            "citizen_labor", OnSaveLoad, nullptr);
        if (g_persHandle <= 0)
            Logf("citizen_labor  WARN  persistence refused the handler");
    }

    // v4: in-window wage tuning via ui_panels (optional service) and
    // enterprise_base (optional consume, so private buildings keep
    // their own M2 wage rows instead of double rows; null-safe when
    // absent - the layering inversion is deliberate and Start-only).
    // Prefer the newest iface, fall back v3 -> v2 -> v1 so a mixed
    // install (new labor, older ui_panels) keeps its widgets.
    g_panels = (const TsmUiPanelsApi*)
        H->consume(TSM_SERVICE_UI_PANELS, TSM_UI_PANELS_VERSION);
    if (!g_panels)
        g_panels = (const TsmUiPanelsApi*)
            H->consume(TSM_SERVICE_UI_PANELS, TSM_UI_PANELS_VERSION_V2);
    if (!g_panels)
        g_panels = (const TsmUiPanelsApi*)
            H->consume(TSM_SERVICE_UI_PANELS, TSM_UI_PANELS_VERSION_V1);
    if (!g_panels)
        Logf("citizen_labor  ui_panels is not installed - "
             "no in-window wage rows");
    g_panelsWidgets = TsmUiPanelsHasWidgets(g_panels) ? 1 : 0;
    g_enterprise = (const TsmEnterpriseBaseApi*)
        H->consume(TSM_SERVICE_ENTERPRISE_BASE,
                   TSM_ENTERPRISE_BASE_VERSION);
    if (g_panels && g_panelRows) {
        if (g_panels->registerRowProvider(LaborPanelRows, nullptr))
            Logf("citizen_labor  in-window wage rows registered "
                 "(slider span min %.2f %s, step %d pct)%s",
                 g_wageSliderSpanMin,
                 TsmCurrencySymbol(g_currency), g_wageSliderStep,
                 g_panelsWidgets ? ", widgets on" : ", text only");
        else
            Logf("citizen_labor  WARN  ui_panels refused the row provider");
        if (g_panelsWidgets) {
            if (g_panels->registerWidgetProvider(LaborPanelWidgets, nullptr))
                Logf("citizen_labor  in-window wage widgets registered");
            else {
                g_panelsWidgets = 0;
                Logf("citizen_labor  WARN  ui_panels refused the widget provider"
                     " - text rows only");
            }
        }
    }

    // v4.4: install the wage-driven labor mobility hook (sim-thread,
    // citizen workplace selection). Safe to call even if disabled -
    // InstallLaborMobilityHook only installs; the compute function
    // checks g_wageMobilityEnabled at runtime.
    if (g_wageMobilityEnabled)
        InstallLaborMobilityHook();

    if (g_simClock->getDay() < 0)
        Logf("citizen_labor  no world loaded yet - payday will begin on the first day event");

    g_simClock->registerDay([](void*, int day, int month, int year) {
        OnDay(day, month, year);
    }, nullptr);

    g_simClock->registerScheduled(TSM_FREQ_MONTHLY,
                                  OnMonthPruneRecords, nullptr);

    Logf("citizen_labor  live v4.8: %d wage entries, payday %d, %s, wage_mobility %d, "
         "workplace range %d..%d, days_per_month %d, prune_after_days %d, "
         "employment_grace_days %d, currency %s (divisor %.1f), "
         "poison=removed, workers_control=0xDEC+0x822FD0",
         (int)g_wageTable.size(), g_paydayFreq,
         g_persistence ? "persistent" : "in-memory only",
         (int)g_wageMobilityEnabled,
         g_workplaceMin, g_workplaceMax, DaysPerMonth(), g_pruneAfterDays,
         g_employmentGraceDays,
         TsmCurrencySymbol(g_currency), UsdScale());
    Logf("citizen_labor    minimum wage floor %.2f %s (RUB base %.2f); "
         "default %.2f, welfare %.2f, stipend %.2f, pension %.2f",
         g_minimumWage, TsmCurrencySymbol(g_currency), g_minimumWageCfg,
         g_defaultDailyWage, g_unemployedWelfare,
         g_studentStipend, g_retireePension);

    for (const auto& kv : g_wageTable)
        if (kv.second != g_defaultDailyWageCfg)
            Logf("citizen_labor    type %-3d  = %.2f per day (RUB base, legacy fallback)",
                 kv.first, kv.second);

    {
        static const char* kSectorNames[TSM_SECTOR_COUNT] = {
            "heavy_industry", "infrastructure", "healthcare_education",
            "light_industry", "services_retail", "agriculture"
        };
        for (int s = 0; s < TSM_SECTOR_COUNT; s++) {
            if (g_sectorWage[s] > 0.0f)
                Logf("citizen_labor    sector %-20s = %.2f per day (RUB base %.2f)",
                     kSectorNames[s], g_sectorWage[s], g_sectorWageCfg[s]);
            else
                Logf("citizen_labor    sector %-20s = (unconfigured, legacy/default applies)",
                     kSectorNames[s]);
        }
    }

    return 0;
}

BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID) { return TRUE; }