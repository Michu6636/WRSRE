// citizen_retail - the shop-sale loop.
//
// See citizen_retail_api.h for the contract. This file is the
// implementation: walking active shops, inspecting citizen demands,
// verifying store inventory, and billing via the private sector
// interceptor or crediting the state treasury.
//
// v2.9 updates (checkout stub: sub/add rsp, 0x80 imm8 sign-extension - the crash):
//   - BUG 3 (the ACCESS_VIOLATION at stub+0x85): step 3 / step 9 emitted
//     `sub rsp, 0x80` / `add rsp, 0x80` with the imm8 opcode 48 83
//     (sub r/m64, imm8 / add r/m64, imm8). Opcode 83 sign-extends its
//     8-bit immediate, and 0x80 as a signed byte is -128, so the stub
//     actually ran `sub rsp, -128` (RSP UP 128 into the caller's frame;
//     the XMM saves overwrote engine locals) and `add rsp, -128` back,
//     with the gate's own frame landing on the six pushed registers.
//     The pops then restored garbage (rdx=rbx=0 -> fault on
//     movss xmm0,[rdx+rbx+8] at stub+0x85). Fix: emit the imm32 forms
//     48 81 EC 80 00 00 00 / 48 81 C4 80 00 00 00 via EmitRspAdj, which
//     uses imm8 only for n <= 0x7F and imm32 otherwise. Stub grows
//     179 -> 185 bytes (still fits the 256-byte allocation); the jz
//     moves +0x84 -> +0x8A with its rel8 value unchanged (computed
//     dynamically), and the wage-mobility stub (0x70) is unaffected.
//   - Version -> 2.9; save format unchanged.
//
// v2.8 updates (checkout stub: jz rel8 stored over the opcode - the crash - plus wrong mov rcx,r14 encoding):
//   - BUG 1 (the ACCESS_VIOLATION at ...0083): step 11 captured jzInstOff as
//     the offset of the JZ OPCODE (0x74) - `(p - stub) - 1` taken right after
//     emitting the opcode - and later stored the rel8 displacement over that
//     byte instead of over its operand. Bytes 0x83..0x84 became `20 00`: the
//     CPU decoded 0x20 as `AND r/m8, r8` with ModRM 0x00 (mod=00, rm=[rax]) =
//     `AND byte ptr [rax], al`, and since `pop rax` had just restored rax=0
//     the stub crashed writing to address 0 the first time a citizen reached
//     any checkout (blocked or not). Fix: emit opcode+placeholder first, then
//     capture the OPERAND offset (jzRel8Off, one byte later) and store rel8
//     there. The rel8 VALUE (0x20 -> .abort @ stub+0xA5 in v2.8; stub+0xAB
//     in v2.9 after the two imm32 growths) was and remains
//     correct; only the store target was wrong. Stub size unchanged (179) in v2.8.
//   - BUG 2 (silent - interceptor never saw a shop): step 5 emitted
//     `mov rcx, r14` as 49 89 F9, which decodes as `mov r9, rdi` (opcode 89 =
//     mov r/m64,r64 takes the SOURCE in the ModRM reg field; REX.B=1 on 0x49
//     extends the DESTINATION/rm instead). rcx never got the shop pointer, so
//     CheckoutAffordabilityGate saw shop=0, its TsmIsValidGamePtr guard
//     returned 0, and every checkout fell through to citizenToStatePartial -
//     the private-shop interceptor (the whole point of enterprise_base's
//     registration) never ran. Correct encoding is 4C 89 F1 (REX.W|R, reg
//     field = r14, rm = rcx). `mov rdx,rsi` (48 89 F2) and `mov r8,rax`
//     (49 89 C0) audited and correct.
//   - [CHECKOUT STUB] install log now dumps the test/jz/stolen region (expect
//     `45 84 DB 74 20` = test r11b,r11b / jz +0x20 followed by the 18 stolen
//     bytes) and cross-checks the patched jz target against .abort, so a
//     mis-aimed rel8 store is caught at install time instead of at the first
//     private-shop purchase.
//   - Version -> 2.8; save format unchanged.
//
// v2.7 updates (checkout gate result clobbered by pop r11):
//   - The stub saved the gate result (al) into r11b (step 7), but step 10 then
//     executed `pop r11` - the first of the 7 LIFO pops - which restored the
//     caller's original r11 over the stashed result. step 11's
//     `test r11b,r11b` therefore tested ambient r11, not al: the allow/abort
//     branch was a coin flip tied to surrounding code (a non-zero caller r11
//     made every purchase .allow = free goods; a zero r11 made every purchase
//     .abort = retail dead). R11 is dead at the hook point (absent from the
//     register map, docs/Retail_Purchase_Block_Investigation §4), so the fix
//     removes `push r11`/`pop r11` entirely and lets r11b carry the result.
//   - Frame dropped 0x88 -> 0x80 (and sub/add rsp with it): with 6 pushes the
//     old 0x88 frame left the `call` mis-aligned (48 + 0x88 ≡ 8 mod 16); 0x80
//     restores 16-byte alignment (48 + 0x80 ≡ 0). All shadow/XMM slots are
//     unchanged and fit inside 0x80; only the unused 0x80..0x87 pad is gone.
//   - Version -> 2.7; save format unchanged.
//
// v2.6 updates (checkout stub ASLR - same latent bug as the wage-mobility stub):
//   - The .allow/.abort tail addresses were compile-time ABSOLUTE constants
//     0x1401725F0 / 0x14017293A, valid only at the preferred base 0x140000000.
//     The stub embeds them verbatim in the 8-byte slot after a
//     `jmp qword ptr [rip+0]`, so under ASLR (exe base e.g. 0x7FF759C60000)
//     the .allow branch jumped into unmapped memory the moment a citizen
//     bought at a private shop. They are now RVAs resolved from g_exeBase at
//     install time (TSM_CHECKOUT_ALLOW_RVA / TSM_CHECKOUT_ABORT_RVA).
//   - Added a one-shot [CHECKOUT STUB] install diagnostic (stub size, live
//     stub address, resolved allow/abort targets) to tesmioloader.log.
//   - Version -> 2.6; save format unchanged.
//
// v2.5 updates (closing the private-shop underpayment exploit):
//   - citizen_retail API v2: TsmRetailInterceptor now documents three
//     returns - >0 handled, 0 state-charges (unchanged v1 meaning),
//     TSM_RETAIL_INSUFFICIENT (-1) "private shop, citizen cannot pay
//     the full private price, take nothing and abort". The struct
//     layout is unchanged; the provider dual-publishes the v1 iface
//     view under TSM_CITIZEN_RETAIL_VERSION_V1 so an out-of-tree
//     consumer built against the v1 header still resolves the service
//     (registry matches versions exactly), plus the v2 iface.
//   - CheckoutAffordabilityGate no longer reads every non-zero
//     interceptor return as "paid": -1 aborts the dispense (returns 0,
//     shelf untouched, demand unsatisfied, state NOT charged). The
//     check matches -1 exactly so a legitimate 0 still falls through
//     to citizenToStatePartial - a v1 interceptor never emits -1, so a
//     mixed install cannot trip the new branch. enterprise_base, which
//     consumes TSM_CITIZEN_RETAIL_VERSION, only registers its
//     pre-checking OnRetail through the v2 iface it resolved; with an
//     old v1-only citizen_retail the consume fails and no interceptor
//     is installed, leaving the gate's own state-price pre-check +
//     abort path in charge.
//   - Rate-limited probe log [CHECKOUT] insufficient for the abort.
//   - Version -> 2.5; save format unchanged.
//
// v2.4 updates (Native Atomic Checkout Hook - closing the free-goods loophole):
//   - Installs an inline hook at FUN_140171E10+0x1725DE (the shop retail tick's
//     checkout fulfillment gate). Before the engine decrements shelf stock or
//     satisfies citizen demand, the hook checks whether the customer can afford
//     the per-frame dispensed amount (amount = min(shelf, rate x time), priced
//     at getPrice(res, active currency) x market multiplier). If wallet < cost,
//     fulfillment is aborted by jumping to the engine's native skip target
//     0x17293A - shelf stock is untouched, demand remains unsatisfied, and the
//     citizen leaves empty-handed to suffer native hunger/health penalties.
//   - Payment is deducted atomically with the give: on success, the hook
//     deducts via the retail interceptor (private shops) or
//     citizenToStatePartial (state stores), then jumps to 0x1725F0 to let the
//     engine complete the shelf/demand write.
//   - WalkShop's retroactive citizenToStatePartial charge is removed from
//     ProcessOneDemand to prevent double-charging; WalkShop retains SoL demand
//     scaling and market-book demand recording.
//   - Version -> 2.4; save format unchanged.
//
// v2.1 updates (Standard of Living, Task 7 - absolute CPI runway):
//   - The relative P25/P75 wallet percentiles are GONE (the sort, the
//     forEachWallet snapshot and the EMA are all deleted: relative
//     percentiles structurally force 25% of a rich republic into "Low"
//     and can never reach a 0% poverty rate). Citizens are classified
//     against the ONE absolute daily survival basket:
//       cpi    = g_pricing->getCpiDailyBasket(0.0f)   (Level 1-safe;
//                the neutral default daily rent rides inside the provider)
//       runway = wallet / cpi   (days of survival)
//       runway >= runway_affluent_days (30) -> Affluent: meat x1.5,
//                 clothes x1.5, electronics x2.0
//       runway <  runway_low_days (7)      -> Low: meat x0.2, clothes x0.5,
//                 electronics x0.0, and - when happiness < 0.40 - surges
//                 alcohol x1.8 (poverty escapism)
//       otherwise                           -> Middle: baseline 1.0x
//     Destitute (wallet < 1 day of bread) lands inside Low here by
//     construction; the eviction stage itself lives in enterprise_base's
//     CollectResidentialRent (Level 2 - retail must never reach up).
//   - The basket is read once per absolute day (cache) because the walk
//     queries it for every shopper; scaling entry+0x00 stays strictly
//     once per citizen per absolute day (g_solScaledDay dedup) and scales
//     shelf deductions and wallet charges linearly (FUN_140171E10).
//
// v2.2 updates:
//   - Fixed alcohol escapism collision: kind==1 (Food) was grouped with
//     kind==3 (Alcohol) and kind==10 (Drink-alt) in the low-runway
//     escapism multiplier, so impoverished citizens buying bread at
//     grocery stores were charged 1.8x as "alcohol escapism." Escapism
//     now applies to kind==3 and kind==10 only.
//   - Version -> 2.2; save format unchanged.
// //
// v2.0 updates (Macro Supply & Demand, Task 2):
//   - Every retail demand (serviced or turned away at empty shelves)
//     feeds resource_pricing's Republic market order book via
//     TsmPricingRecordDemand, which is how overproduction and glut are
//     visible to the price engine.
//   - The retail price is the base price scaled by the live market
//     multiplier: totalCost = amount * basePrice * marketMult (1.0 when
//     the book is absent or disabled). The private-shop interceptor
//     then applies its own markup on top, exactly as before.
//   - resource_pricing is consumed prefer-v2 with a v1 fallback; when
//     only v1 is present everything degrades to the pre-v2 behaviour.
//
// v1.2 updates:
//   - Unified global currency (v4): the sale is priced in the active
//     currency (TsmCurrencyActive), so a USD-mode save charges the USD
//     leg of the resource price and the treasury routes to +0x580.
//     min_cost scales by usd_divisor in USD mode unless min_cost_usd
//     is set. Currency consume falls back to the v3 iface.
//
// v1.1 updates:
//   - Multi-Demand Basket: Removed premature `break;` in WalkShop so citizens
//     purchasing multiple distinct goods (e.g. food + clothes + electronics)
//     in a single visit are charged for the full basket.
//   - Inventory Sanity Check (ShopHasResourceInStock): Verifies that the shop
//     actually carries the resource in its storage with positive content before
//     charging. Prevents false charges (e.g. billing food in a clothing store
//     or billing customers when shelves are empty).
//   - Expanded Retail Demands: Accepts Kind 1 (Food), Kind 2 (Goods/Clothes/
//     Electronics), and Kind 3 (Alcohol drinks) when stocked in storage.
//   - Defense-in-Depth Memory Verification: Checks full span readability on
//     people and storage vectors.

#include "../../src/tesmio_plugin.h"
#include "../../src/game_world.h"
#include "../../src/sim_clock_api.h"
#include "../../src/currency_api.h"
#include "../../src/resource_pricing_api.h"
#include "../../src/persistence_api.h"
#include "../../src/citizen_retail_api.h"
#include "../../src/citizen_labor_api.h"

#include <unordered_map>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <stdint.h>

static const TsmSimClockApi*        g_simClock    = nullptr;
static const TsmCurrencyApi*        g_currency    = nullptr;
static const TsmResourcePricingApi* g_pricing     = nullptr;
static const TsmPersistenceApi*     g_persistence = nullptr;
static int                          g_persHandle  = -1;
static const TsmCitizenLaborApi*    g_labor       = nullptr;   // v2.3: income flow (wage/transfer)
static float                        g_breadTons   = 0.0020f;   // v2.3: tons of staple bread = 1 day of survival

static int    g_enabled        = 1;
static int    g_probe          = 1;
static int    g_tickIntervalMs = 250;
static float  g_minCost        = 0.01f;
// v4: RUB-denominated base for min_cost; USD-effective value resolves at
// Start (min_cost_usd or usd_divisor).
static float  g_minCostCfg     = 0.01f;
static float  g_usdDivisor     = 50.0f;

// ---------------------------------------------------------------- v2.4 native checkout hook
#define TSM_CHECKOUT_HOOK_RVA   (g_tsmAddr.retail_checkout_hook)
// v2.6: allow/abort are RVAs resolved from g_exeBase at install time. The old
// absolute constants (0x1401725F0 / 0x14017293A) were only valid at the
// preferred base 0x140000000; under ASLR the stub's jmp [rip+0] tails jumped
// into unmapped memory on the first private-shop purchase.
#define TSM_CHECKOUT_ALLOW_RVA  (g_tsmAddr.retail_checkout_allow)
#define TSM_CHECKOUT_ABORT_RVA  (g_tsmAddr.retail_checkout_abort)
#define TSM_CHECKOUT_STOLEN     18u

static const BYTE kCheckoutExpect[TSM_CHECKOUT_STOLEN] = {
    0xF3, 0x0F, 0x10, 0x44, 0x1A, 0x08,
    0x4D, 0x8D, 0x85, 0x20, 0x14, 0x01, 0x00,
    0xF3, 0x41, 0x0F, 0x59, 0xD7,
};

static BYTE*   g_checkoutStub   = nullptr;
static void*   g_checkoutTrampoline = nullptr;
static int     g_checkoutHooked = 0;

// v2.9: emits sub/add rsp, imm. Uses imm8 only when the value fits in a
// SIGNED byte (<= 0x7F); otherwise imm32. imm8 0x80..0xFF sign-extends
// to negative and flips the operation (0x80 -> sub rsp,-128 = add 128).
static void EmitRspAdj(BYTE*& p, bool isSub, uint32_t n)
{
    *p++ = 0x48;
    if (n <= 0x7F) {
        *p++ = 0x83;
        *p++ = isSub ? 0xEC : 0xC4;
        *p++ = (BYTE)n;
    } else {
        *p++ = 0x81;
        *p++ = isSub ? 0xEC : 0xC4;
        memcpy(p, &n, 4); p += 4;
    }
}

#define MAX_SHOP_TYPES 16
static int    g_shopTypes[MAX_SHOP_TYPES];
static int    g_shopTypeCount;

static std::unordered_map<uint64_t, int> g_lastShopDay;

// v2.3 (Task 7): Standard of Living - Flow + Stock evaluation matrix.
//   cpi   = g_pricing->getCpiDailyBasket(0.0f)   (Level 1-safe; neutral
//           default daily rent rides inside the provider)
//   runway = wallet / cpi        (savings stock: days of survival)
//   ppr   = dailyWage / cpi      (income flow: purchasing power ratio)
//   Top-down classification, first match wins:
//     Destitute:  wallet < 1 day of bread AND income <= 0.01f
//     Affluent:   ppr >= 2.0f && runway >= g_runwayAffluentDays (30 d)
//     Middle:     ppr >= 1.0f || runway >= g_runwayLowDays (14 d)
//     Low:        everything else
//   No percentiles. Scaling fires strictly once per citizen per absolute day
//   (g_solScaledDay dedup) so amountNeeded never compounds across ticks.
static int   g_solEnabled           = 1;
static float g_highMeatMult         = 1.5f;
static float g_highClothesMult      = 1.5f;
static float g_highElecMult         = 2.0f;
static float g_lowMeatMult          = 0.2f;
static float g_lowClothesMult       = 0.5f;
static float g_lowElecMult          = 0.0f;
static float g_alcoholEscapismMult  = 1.8f;
static float g_escapismThreshold    = 0.40f;
static float g_runwayAffluentDays   = 30.0f;  // runway_affluent_days (INI)
static float g_runwayLowDays        = 14.0f;  // runway_low_days (INI) - Middle/Low boundary
static std::unordered_map<uint64_t, int> g_solScaledDay;
static int g_solLogCount = 0;
#define SOL_LOG_BUDGET 20

// Demand entry layout (Task 7 SoL, Citizen_SoL_Investigation): inline
// array at person+0x118, stride 0x80, 7 entries; kinds 1 drink / 2 goods
// / 10 drink-alt / 15 food.
#define SOL_DEMAND_STRIDE  0x80
#define SOL_DEMAND_AMOUNT  0x00
#define SOL_DEMAND_KIND    0x08
#define SOL_DEMAND_RES     0x10
#define SOL_DEMAND_MAX     7

// POD-only: SEH-guarded demand entry read. C2712 forbids __try beside
// C++ unwinding, so every fault-guarded engine touch lives in a raw
// locals-only helper like this one.
static int ReadDemandEntryRaw(void* person, int d, float* outAmount,
                              int* outKind, void** outRes)
{
    float amt = 0.0f;
    int kind = -1;
    void* res = nullptr;
    __try {
        if (!TsmIsValidGamePtr(person)) return 0;
        if (!ReadablePtr((BYTE*)person + TSM_PERSON_DEMANDS,
                         TSM_PERSON_SIZE)) return 0;
        BYTE* e = (BYTE*)person + TSM_PERSON_DEMANDS +
                  (size_t)d * SOL_DEMAND_STRIDE;
        if (!ReadablePtr(e, 0x18)) return 0;
        amt  = *(float*)(e + SOL_DEMAND_AMOUNT);
        kind = *(int*)(e + SOL_DEMAND_KIND);
        res  = *(void**)(e + SOL_DEMAND_RES);
    } __except (FaultFilter("citizen_retail SoL demand read",
                            GetExceptionInformation())) {
        return 0;
    }
    if (outAmount) *outAmount = amt;
    if (outKind) *outKind = kind;
    if (outRes) *outRes = res;
    return 1;
}

// POD-only: SEH-guarded amount write-back. Pure engine write; the caller
// owns the scaling math.
static void WriteDemandAmountRaw(void* person, int d, float amount)
{
    __try {
        if (!TsmIsValidGamePtr(person)) return;
        if (!ReadablePtr((BYTE*)person + TSM_PERSON_DEMANDS,
                         TSM_PERSON_SIZE)) return;
        BYTE* e = (BYTE*)person + TSM_PERSON_DEMANDS +
                  (size_t)d * SOL_DEMAND_STRIDE;
        if (!ReadablePtr(e, sizeof(float))) return;
        *(float*)(e + SOL_DEMAND_AMOUNT) = amount;
    } __except (FaultFilter("citizen_retail SoL demand write",
                            GetExceptionInformation())) {
    }
}

// POD-only: SEH-guarded happiness read (person+0x0E8 float).
static float ReadHappinessRaw(void* person, float dflt)
{
    float v = dflt;
    __try {
        if (TsmIsValidGamePtr(person) &&
            ReadablePtr((BYTE*)person + TSM_PERSON_HAPPINESS,
                        sizeof(float))) {
            float r = *(float*)((BYTE*)person + TSM_PERSON_HAPPINESS);
            if (r == r) v = r;
        }
    } __except (FaultFilter("citizen_retail SoL happiness",
                            GetExceptionInformation())) {
        v = dflt;
    }
    return v;
}

// POD-only: SEH-guarded demand-count read (person+0x110 int, cap 7).
static int ReadDemandCountRaw(void* person)
{
    int n = 0;
    __try {
        if (TsmIsValidGamePtr(person) &&
            ReadablePtr((BYTE*)person + TSM_PERSON_DEMAND_N, sizeof(int)))
            n = *(int*)((BYTE*)person + TSM_PERSON_DEMAND_N);
    } __except (FaultFilter("citizen_retail SoL demand count",
                            GetExceptionInformation())) {
        n = 0;
    }
    if (n < 0) n = 0;
    if (n > SOL_DEMAND_MAX) n = SOL_DEMAND_MAX;
    return n;
}

// v2.0 (Task 2): which citizens already had their retail wants recorded
// into the Republic order book on a given absolute day. Read by nothing
// except the demand tap below; never persisted. This dedup is the whole
// reason the demand signal stays sane: the walk re-sees a standing
// citizen every 250 ms tick, and recording every sighting would inflate
// one person's daily bread into hundreds of tons and pin every good at
// the shortage ceiling forever.
static std::unordered_map<uint64_t, int> g_demandDay;

static TsmRetailInterceptor g_interceptor   = nullptr;
static void*                g_interceptorUD = nullptr;

static int RegisterInterceptor(TsmRetailInterceptor cb, void* userData)
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

// ---------------------------------------------------------------- the walk

static inline int GetAbsDay(int day)
{
    int year = g_simClock ? g_simClock->getYear() : -1;
    if (year < 0 || day < 0) return -1;
    return (year * 365) + day;
}

static bool AlreadyShoppedToday(uint64_t id, int day)
{
    int absDay = GetAbsDay(day);
    if (absDay < 0) return false;

    EnterCriticalSection(&g_lock);
    auto it = g_lastShopDay.find(id);
    bool seen = (it != g_lastShopDay.end() && it->second == absDay);
    LeaveCriticalSection(&g_lock);
    return seen;
}

static void MarkShoppedToday(uint64_t id, int day)
{
    int absDay = GetAbsDay(day);
    if (absDay < 0) return;

    EnterCriticalSection(&g_lock);
    g_lastShopDay[id] = absDay;
    LeaveCriticalSection(&g_lock);
}

// Verifies that the building carries `res` in its storage, with no
// requirement on the current on-shelf quantity. Used by the v2.0 demand
// tap: a citizen whose whole venue stocks a resource is a REAL buyer of
// it even when the shelves read 0.0 - those turnaways are what scarcity
// is made of. A demand for a resource the venue never sells is not
// demand, it is background hunger/exercise; do not record it.
static bool ShopStocksResource(BYTE* shop, void* res)
{
    if (!TsmIsValidGamePtr(shop) || !TsmIsValidGamePtr(res)) return false;

    void** storBegin = *(void***)(shop + TSM_BLD_STORAGES_BEGIN);
    void** storEnd   = *(void***)(shop + TSM_BLD_STORAGES_END);
    if (!storBegin || !storEnd || storEnd <= storBegin) return false;
    if (!TsmIsValidGamePtr(storBegin)) return false;
    if (!ReadablePtr(storBegin, sizeof(void*))) return false;

    size_t storBytes = (size_t)((BYTE*)storEnd - (BYTE*)storBegin);
    if (storBytes % TSM_STORAGE_STRIDE) return false;
    size_t storCount = storBytes / TSM_STORAGE_STRIDE;

    if (storCount > 64) storCount = 64;
    if (!ReadablePtr(storBegin, sizeof(void*))) return false;

    for (size_t si = 0; si < storCount; si++) {
        BYTE* storage = (BYTE*)storBegin + si * TSM_STORAGE_STRIDE;
        if (!ReadablePtr(storage, 0x10)) continue;

        void** slotBegin = *(void***)(storage + TSM_ST_SLOTS_BEGIN);
        void** slotEnd   = *(void***)(storage + TSM_ST_SLOTS_END);
        if (!slotBegin || !slotEnd || slotEnd <= slotBegin) continue;
        if (!TsmIsValidGamePtr(slotBegin)) continue;

        size_t slotBytes = (size_t)((BYTE*)slotEnd - (BYTE*)slotBegin);
        if (slotBytes % TSM_SLOT_STRIDE) continue;
        size_t slotCount = slotBytes / TSM_SLOT_STRIDE;
        if (slotCount > 64) slotCount = 64;
        if (!ReadablePtr(slotBegin, sizeof(void*))) continue;

        for (size_t sj = 0; sj < slotCount; sj++) {
            BYTE* slot = (BYTE*)slotBegin + sj * TSM_SLOT_STRIDE;
            void* r = *(void**)(slot + TSM_SLOT_RESOURCE);
            if (r == res) return true;
        }
    }
    return false;
}

// v2.0 (Task 2): per-citizen per-day dedup for the market demand tap.
// Called strictly at most once per (id, resource) per day, so walking the
// same citizen four times a second in a standing crowd cannot fabricate
// demand. The caller holds no lock; these lists are the caller's own.
static int DemandDayKeyed(uint64_t id, void* res)
{
    // Mix the stable citizen id with the resource record pointer into a
    // per-day marker bucket.
    uintptr_t a = (uintptr_t)id;
    uintptr_t b = (uintptr_t)res;
    uint64_t k = (a * 0x9E3779B97F4A7C15ull) ^ (b + 0x9E3779B97F4A7C15ull
                 + (a << 6) + (a >> 2));
    EnterCriticalSection(&g_lock);
    auto it = g_demandDay.find(k);
    int seen = (it != g_demandDay.end());
    LeaveCriticalSection(&g_lock);
    return seen;
}

static void NoteDemandDayKeyed(uint64_t id, void* res)
{
    uintptr_t a = (uintptr_t)id;
    uintptr_t b = (uintptr_t)res;
    uint64_t k = (a * 0x9E3779B97F4A7C15ull) ^ (b + 0x9E3779B97F4A7C15ull
                 + (a << 6) + (a >> 2));
    EnterCriticalSection(&g_lock);
    g_demandDay[k] = 1;
    LeaveCriticalSection(&g_lock);
}

static void ClearDemandDay(void)
{
    EnterCriticalSection(&g_lock);
    g_demandDay.clear();
    LeaveCriticalSection(&g_lock);
}

// Verifies that the building carries `res` in its storage and currently has stock.
static bool ShopHasResourceInStock(BYTE* shop, void* res)
{
    if (!TsmIsValidGamePtr(shop) || !TsmIsValidGamePtr(res)) return false;

    void** storBegin = *(void***)(shop + TSM_BLD_STORAGES_BEGIN);
    void** storEnd   = *(void***)(shop + TSM_BLD_STORAGES_END);
    if (!storBegin || !storEnd || storEnd <= storBegin) return false;
    if (!TsmIsValidGamePtr(storBegin)) return false;

    size_t storBytes = (size_t)((BYTE*)storEnd - (BYTE*)storBegin);
    if (storBytes % TSM_STORAGE_STRIDE) return false;
    size_t storCount = storBytes / TSM_STORAGE_STRIDE;
    if (storCount > 16) storCount = 16;
    if (!ReadablePtr(storBegin, storCount * TSM_STORAGE_STRIDE)) return false;

    for (size_t si = 0; si < storCount; si++) {
        BYTE* storage = (BYTE*)storBegin + si * TSM_STORAGE_STRIDE;
        if (!ReadablePtr(storage, 0x20)) continue;

        void** slotBegin = *(void***)(storage + TSM_ST_SLOTS_BEGIN);
        void** slotEnd   = *(void***)(storage + TSM_ST_SLOTS_END);
        if (!slotBegin || !slotEnd || slotEnd <= slotBegin) continue;
        if (!TsmIsValidGamePtr(slotBegin)) continue;

        size_t slotBytes = (size_t)((BYTE*)slotEnd - (BYTE*)slotBegin);
        if (slotBytes % TSM_SLOT_STRIDE) continue;
        size_t slotCount = slotBytes / TSM_SLOT_STRIDE;
        if (slotCount > 32) slotCount = 32;
        if (!ReadablePtr(slotBegin, slotCount * TSM_SLOT_STRIDE)) continue;

        for (size_t sj = 0; sj < slotCount; sj++) {
            BYTE* slot = (BYTE*)slotBegin + sj * TSM_SLOT_STRIDE;
            void* slotRes = *(void**)(slot + TSM_SLOT_RESOURCE);
            float cnt     = *(float*)(slot + TSM_SLOT_CONTENT);
            if (slotRes == res && cnt > 0.0001f) return true;
        }
    }
    return false;
}

static int ProcessOneDemand(void* shop, void* person, const BYTE* demand)
{
    int kind = *(const int*)(demand + 0x08);
    // Kind 1 = Food, Kind 2 = Goods (Meat, Clothes, Electronics), Kind 3 = Alcohol (Pub)
    if (kind != 1 && kind != 2 && kind != 3) return 0;

    void* res = *(void**)(demand + 0x10);
    if (!TsmIsValidGamePtr(res)) return 0;

    float amountNeeded = *(const float*)(demand + 0x00);
    if (!(amountNeeded > 0.00001f)) return 0;

    // v2.0 (Task 2): record this citizen want in the Republic market book
    // REGARDLESS of whether this shop has shelves - turned-away unmet
    // demand is exactly what drives scarcity. Record once per
    // (citizen, resource) per absolute day, and only when the venue
    // sells the resource at all: checking stocks is redundant here
    // because ProcessOneDemand is only reachable after WalkShop verified
    // the shop carries the resource, but we re-verify anyway because the
    // demand tap must never run on an unvalidated game pointer triple.
    if (TsmPricingHasMarket(g_pricing) &&
        TsmIsValidGamePtr(shop) && TsmIsValidGamePtr(person)) {
        uint64_t cid = TsmCitizenID(person);
        if (cid != 0 && ShopStocksResource((BYTE*)shop, res) &&
            !DemandDayKeyed(cid, res)) {
            g_pricing->recordDemand(res, (double)amountNeeded);
            NoteDemandDayKeyed(cid, res);
        }
    }

    // Sanity check: Ensure this venue stocks this resource and has stock on hand
    if (!ShopHasResourceInStock((BYTE*)shop, res)) return 0;

    // v4: unified global currency - price in the active currency so
    // wholesale, retail, pub charges and state procurement all transact
    // in the same unit the treasury routes to.
    int cur = TsmCurrencyActive(g_currency);
    float pricePerTon = g_pricing->getPrice(res, cur);
    // v2.0 (Task 2): Victoria-3 market price = base * marketMult.
    // When the Republic book has the multiplier API available, use it;
    // otherwise fall back to a neutral 1.0 (pre-v2 behaviour).
    float marketMult = TsmPricingMarketMult(g_pricing, res);
    float totalCost   = amountNeeded * (pricePerTon * marketMult);
    if (!(totalCost == totalCost) || totalCost < g_minCost)
        totalCost = g_minCost;

    // v2.4: payment is handled natively by the checkout hook at
    // FUN_140171E10+0x1725DE before the engine decrements shelf/demand.
    // WalkShop no longer charges here to avoid double-charging.
    return 1;
}

// v2.4: affordability gate called from the checkout hook stub.
// 4 arguments: shop (RCX), person (RDX), res (R8), amount (XMM3).
// Returns 1 = allow fulfillment (engine gives goods), 0 = abort (jump to skip target).
// SEH-bracketed: any fault fails open (allow) rather than crashing the shop tick.
static int CheckoutAffordabilityGate(void* shop, void* person, void* res, float amount)
{
    __try {
        if (!g_currency || !g_pricing || !res || !person) return 1;
        if (!(amount > 0.00001f)) return 1;   // nothing to give, allow

        // Unit price in active currency scaled by market multiplier.
        int cur = TsmCurrencyActive(g_currency);
        float pricePerTon = g_pricing->getPrice(res, cur);
        float marketMult = TsmPricingMarketMult(g_pricing, res);
        float cost = amount * pricePerTon * marketMult;
        if (!(cost == cost) || cost < 0.0f) cost = 0.0f;

        // Negligible cost: don't bother the citizen, allow the give.
        if (cost < 0.0001f) return 1;

        // Wallet check: can the citizen afford this frame's dispensed amount?
        float wallet = g_currency->getWalletBalance(person);
        if (!(wallet == wallet) || wallet < 0.0f) wallet = 0.0f;

        if (wallet < cost) {
            if (g_probe) {
                static int logged;
                if (++logged <= 20) {
                    char name[32] = "?";
                    TsmResourceName(res, name, sizeof(name));
                    Logf("citizen_retail  [CHECKOUT] blocked: \"%s\" amount=%.4f cost=%.2f wallet=%.2f %s",
                         name, (double)amount, (double)cost, (double)wallet,
                         TsmCurrencySymbol(g_currency));
                }
            }
            return 0;   // abort: shelf untouched, demand unsatisfied
        }

        // Payment: private-shop interceptor first, then state treasury.
        if (g_interceptor) {
            int handled = g_interceptor(shop, person, cost, res, g_interceptorUD);
            // v2.5: ONLY the exact v2 sentinel aborts. 0 keeps its v1
            // meaning ("state charges as usual") and falls through to
            // citizenToStatePartial - never misread as "abort" by a
            // mixed install, because a v1 interceptor documented only
            // {0, non-zero} and can never emit -1.
            if (handled == TSM_RETAIL_INSUFFICIENT) {
                // The interceptor pre-checked the wallet against the full
                // private price (markup + VAT) and took NOTHING. Abort:
                // no state charge, no dispensed goods.
                if (g_probe) {
                    static int logged;
                    if (++logged <= 20) {
                        char name[32] = "?";
                        TsmResourceName(res, name, sizeof(name));
                        Logf("citizen_retail  [CHECKOUT] insufficient: \"%s\" "
                             "amount=%.4f state cost=%.2f wallet=%.2f %s - "
                             "private price unaffordable, nothing charged",
                             name, (double)amount, (double)cost,
                             (double)g_currency->getWalletBalance(person),
                             TsmCurrencySymbol(g_currency));
                    }
                }
                return 0;   // abort: shelf untouched, demand unsatisfied, no money taken
            }
            if (handled != 0) return 1;   // interceptor handled it
        }

        float taken = g_currency->citizenToStatePartial(person, cost, "state_store_sale");
        if (taken < cost - 0.0001f) return 0;   // payment failed, abort
        return 1;   // allow: engine completes shelf/demand write
    }
    __except (FaultFilter("citizen_retail checkout gate", GetExceptionInformation())) {
        return 1;   // fail open on any fault
    }
}

// v2.4: installs the 18-byte inline hook at the checkout fulfillment gate.
static bool InstallCheckoutHook(void)
{
    if (!g_exeBase || !TsmCheckExeBuild(H, "citizen_retail") ||
        !TSM_CHECKOUT_HOOK_RVA || !TSM_CHECKOUT_ALLOW_RVA || !TSM_CHECKOUT_ABORT_RVA)
        return false;

    void* target = (void*)(g_exeBase + TSM_CHECKOUT_HOOK_RVA);

    g_checkoutStub = (BYTE*)VirtualAlloc(NULL, 256,
        MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!g_checkoutStub) {
        Logf("citizen_retail  CHECKOUT hook FAILED (VirtualAlloc)");
        return false;
    }

    BYTE* p = g_checkoutStub;
    void* gateAddr = (void*)&CheckoutAffordabilityGate;
    uintptr_t allowAddr = (uintptr_t)(g_exeBase + TSM_CHECKOUT_ALLOW_RVA);
    uintptr_t abortAddr = (uintptr_t)(g_exeBase + TSM_CHECKOUT_ABORT_RVA);

    // --- 1. Compute amount = min([rdx+rbx+0x8], xmm2 * xmm15) ---
    // movss xmm0, dword ptr [rdx + rbx + 0x8]
    *p++ = 0xF3; *p++ = 0x0F; *p++ = 0x10; *p++ = 0x44; *p++ = 0x1A; *p++ = 0x08;
    // movaps xmm3, xmm2
    *p++ = 0x0F; *p++ = 0x28; *p++ = 0xDA;
    // mulss xmm3, xmm15
    *p++ = 0xF3; *p++ = 0x41; *p++ = 0x0F; *p++ = 0x59; *p++ = 0xDF;
    // minss xmm3, xmm0
    *p++ = 0xF3; *p++ = 0x0F; *p++ = 0x5D; *p++ = 0xD8;

    // --- 2. Save all volatile GPRs ---
    *p++ = 0x50;                                                        // push rax
    *p++ = 0x51;                                                        // push rcx
    *p++ = 0x52;                                                        // push rdx
    *p++ = 0x41; *p++ = 0x50;                                          // push r8
    *p++ = 0x41; *p++ = 0x51;                                          // push r9
    *p++ = 0x41; *p++ = 0x52;                                          // push r10
    // v2.7: push r11 is deliberately OMITTED. r11 is dead at the hook point
    // (absent from the register map, docs/Retail_Purchase_Block_Investigation
    //  §4) and is reserved below to carry the gate result (al) across the
    // pops, so step 11's `test r11b,r11b` reads the real return value. The old
    // push r11 / pop r11 pair restored the caller's r11 over the stashed
    // result, making the allow/abort branch depend on ambient r11.

    // --- 3. Allocate stack for XMM saves + shadow space ---
    // RSP at hook entry is 0 mod 16 (deep inside FUN_140171E10, ~0x7CE past entry).
    // v2.7: 6 pushes (r11 removed) = 48 bytes. 48 + X ≡ 0 (mod 16)  ->  X ≡ 0
    // (mod 16). Chosen X = 0x80:  48 + 0x80 = 0xB0 = 176,  176 % 16 == 0  OK.
    // (The v2.6 7-push frame used X = 0x88; dropping a push drops the required
    //  frame by 8 to keep the `call` 16-byte aligned.)
    // Frame layout (RSP after sub rsp, 0x80):
    //   [rsp+0x00..0x1F] shadow space (32 bytes; callee may spill here - stub never writes)
    //   [rsp+0x20]  xmm0
    //   [rsp+0x30]  xmm1
    //   [rsp+0x40]  xmm2
    //   [rsp+0x50]  xmm3
    //   [rsp+0x60]  xmm4
    //   [rsp+0x70]  xmm5           (ends exactly at frame top 0x80)
    // At CALL: RSP = entry - 0x30 - 0x80 = entry - 0xB0, still 0 mod 16.
    // v2.9: imm32 form via EmitRspAdj (48 81 EC 80 00 00 00). The old
    // 48 83 EC 80 sign-extended 0x80 to -128 and moved RSP the wrong way.
    EmitRspAdj(p, true, 0x80);                                              // sub rsp, 0x80

    // --- 4. Save volatile XMMs (movups = unaligned, no fault) ---
    *p++ = 0x0F; *p++ = 0x11; *p++ = 0x44; *p++ = 0x24; *p++ = 0x20;   // movups [rsp+0x20], xmm0
    *p++ = 0x0F; *p++ = 0x11; *p++ = 0x4C; *p++ = 0x24; *p++ = 0x30;   // movups [rsp+0x30], xmm1
    *p++ = 0x0F; *p++ = 0x11; *p++ = 0x54; *p++ = 0x24; *p++ = 0x40;   // movups [rsp+0x40], xmm2
    *p++ = 0x0F; *p++ = 0x11; *p++ = 0x5C; *p++ = 0x24; *p++ = 0x50;   // movups [rsp+0x50], xmm3
    *p++ = 0x0F; *p++ = 0x11; *p++ = 0x64; *p++ = 0x24; *p++ = 0x60;   // movups [rsp+0x60], xmm4
    *p++ = 0x0F; *p++ = 0x11; *p++ = 0x6C; *p++ = 0x24; *p++ = 0x70;   // movups [rsp+0x70], xmm5

    // --- 5. Set up x64 call arguments ---
    // mov rcx, r14   (Arg 1: shop)
    // v2.8 BUG 2 fix: was 49 89 F9 = `mov r9, rdi` (opcode 89 puts the SOURCE
    // in reg; REX.B on 0x49 extends the DESTINATION). r14 is the SOURCE and
    // needs REX.R, so the REX byte is 4C and ModRM = 11 110 001 (reg=r14,
    // rm=rcx). Without this, rcx stayed 0 and the gate's validity guard
    // rejected every call - the private-shop interceptor never ran.
    *p++ = 0x4C; *p++ = 0x89; *p++ = 0xF1;
    // mov rdx, rsi   (Arg 2: person)
    *p++ = 0x48; *p++ = 0x89; *p++ = 0xF2;
    // mov r8, rax    (Arg 3: res)
    *p++ = 0x49; *p++ = 0x89; *p++ = 0xC0;

    // --- 6. Call CheckoutAffordabilityGate ---
    *p++ = 0x48; *p++ = 0xB8;                                            // mov rax, imm64 (gate addr)
    memcpy(p, &gateAddr, 8); p += 8;
    *p++ = 0xFF; *p++ = 0xD0;                                            // call rax

    // --- 7. Save gate result (al -> r11b) ---
    *p++ = 0x41; *p++ = 0x88; *p++ = 0xC3;                              // mov r11b, al

    // --- 8. Restore volatile XMMs ---
    *p++ = 0x0F; *p++ = 0x10; *p++ = 0x44; *p++ = 0x24; *p++ = 0x20;   // movups xmm0, [rsp+0x20]
    *p++ = 0x0F; *p++ = 0x10; *p++ = 0x4C; *p++ = 0x24; *p++ = 0x30;   // movups xmm1, [rsp+0x30]
    *p++ = 0x0F; *p++ = 0x10; *p++ = 0x54; *p++ = 0x24; *p++ = 0x40;   // movups xmm2, [rsp+0x40]
    *p++ = 0x0F; *p++ = 0x10; *p++ = 0x5C; *p++ = 0x24; *p++ = 0x50;   // movups xmm3, [rsp+0x50]
    *p++ = 0x0F; *p++ = 0x10; *p++ = 0x64; *p++ = 0x24; *p++ = 0x60;   // movups xmm4, [rsp+0x60]
    *p++ = 0x0F; *p++ = 0x10; *p++ = 0x6C; *p++ = 0x24; *p++ = 0x70;   // movups xmm5, [rsp+0x70]

    // --- 9. Deallocate stack ---
    // v2.9: imm32 form via EmitRspAdj (48 81 C4 80 00 00 00); see step 3.
    EmitRspAdj(p, false, 0x80);                                             // add rsp, 0x80

    // --- 10. Restore volatile GPRs ---
    // v2.7: pop r11 is deliberately OMITTED (paired with the removed push r11)
    // so r11b still holds the gate result for step 11. Net stack effect stays
    // zero: 6 pushes / 6 pops.
    *p++ = 0x41; *p++ = 0x5A;                                          // pop r10
    *p++ = 0x41; *p++ = 0x59;                                          // pop r9
    *p++ = 0x41; *p++ = 0x58;                                          // pop r8
    *p++ = 0x5A;                                                        // pop rdx
    *p++ = 0x59;                                                        // pop rcx
    *p++ = 0x58;                                                        // pop rax

    // --- 11. Check gate result and branch ---
    *p++ = 0x45; *p++ = 0x84; *p++ = 0xDB;                              // test r11b, r11b
    *p++ = 0x74;                                                        // jz .abort (opcode)
    *p++ = 0x00;                                                        // rel8 placeholder
    // v2.8 BUG 1 fix: capture the OPERAND offset (the byte just emitted,
    // i.e. the rel8), NOT the opcode. The v2.7 code captured (p - stub) - 1
    // right after the 0x74 opcode, then stored rel8 over that slot - turning
    // `jz` into `20 00` = `AND byte ptr [rax], al`, which crashed writing to
    // [rax] (rax = 0 after the pops) on the first checkout.
    size_t jzRel8Off = (size_t)(p - g_checkoutStub) - 1;

    // === .allow: re-execute the 18 stolen bytes, then jump to exeBase+0x1725F0 ===
    memcpy(p, kCheckoutExpect, TSM_CHECKOUT_STOLEN); p += TSM_CHECKOUT_STOLEN;
    // jmp qword ptr [rip+0]
    *p++ = 0xFF; *p++ = 0x25; *p++ = 0x00; *p++ = 0x00; *p++ = 0x00; *p++ = 0x00;
    memcpy(p, &allowAddr, 8); p += 8;

    // === .abort: jump to exeBase+0x17293A (engine skips to next demand/customer) ===
    *p++ = 0xFF; *p++ = 0x25; *p++ = 0x00; *p++ = 0x00; *p++ = 0x00; *p++ = 0x00;
    memcpy(p, &abortAddr, 8); p += 8;

    // Patch jz rel8: target = .abort (the jmp [rip+0] instruction before abortAddr)
    size_t jzNextOff = jzRel8Off + 1;                                   // first byte after the 2-byte jz
    size_t abortLabelOff = (size_t)(p - g_checkoutStub) - 14;
    int8_t rel8 = (int8_t)((intptr_t)(g_checkoutStub + abortLabelOff) -
                           (intptr_t)(g_checkoutStub + jzNextOff));
    g_checkoutStub[jzRel8Off] = rel8;   // v2.8 BUG 1 fix: store into the OPERAND, not the opcode

    FlushInstructionCache(GetCurrentProcess(), g_checkoutStub,
                          (SIZE_T)(p - g_checkoutStub));

    // static_assert equivalent: stub must fit in the 256-byte allocation.
    // Expected size: 185 bytes (v2.8: 179 + 3 (sub rsp imm8->imm32)
    // + 3 (add rsp imm8->imm32); v2.7: 183 - 2 (push r11) - 2 (pop r11)).
    size_t stubSize = (size_t)(p - g_checkoutStub);
    if (stubSize > 256) {
        Logf("citizen_retail  CHECKOUT hook FAILED (stub %zu bytes > 256)",
             stubSize);
        VirtualFree(g_checkoutStub, 0, MEM_RELEASE);
        g_checkoutStub = nullptr;
        return false;
    }

    if (!InstallInlineHook(target, g_checkoutStub,
                           &g_checkoutTrampoline, kCheckoutExpect,
                           TSM_CHECKOUT_STOLEN, "citizen_retail checkout"))
    {
        VirtualFree(g_checkoutStub, 0, MEM_RELEASE);
        g_checkoutStub = nullptr;
        return false;
    }

    g_checkoutHooked = 1;
    Logf("citizen_retail  [CHECKOUT STUB] size=%zu stub=%p allow=%p abort=%p",
         (size_t)(p - g_checkoutStub), (void*)g_checkoutStub,
         (void*)allowAddr, (void*)abortAddr);
    // v2.8: dump the test/jz/stolen region so a mis-aimed rel8 store is
    // visible at install time. Expected in v2.9: `45 84 DB 74 20` (test
    // r11b,r11b / jz +0x20) at jz@+0x8A, followed by the 18 stolen bytes.
    // (v2.8: jz@+0x84.) If the byte BEFORE the rel8
    // operand ever equals the rel8 value (i.e. `20 00` instead of `74 20`),
    // the store is aimed at the opcode again.
    {
        size_t dumpOff = jzRel8Off - 4;                 // test r11b,r11b (3) + jz opcode (1)
        size_t dumpLen = 4 + 1 + TSM_CHECKOUT_STOLEN;   // test + jz + rel8 + stolen
        char hex[3 * 32 + 1]; int o = 0;
        for (size_t i = 0; i < dumpLen; i++)
            o += _snprintf_s(hex + o, sizeof(hex) - o, _TRUNCATE, "%02X ",
                             g_checkoutStub[dumpOff + i]);
        Logf("citizen_retail  [CHECKOUT STUB] jz/stolen[+0x%zX..+0x%zX]=%s",
             dumpOff, dumpOff + dumpLen - 1, hex);
    }
    // v2.8: cross-check the patched branch: target as computed from the rel8
    // must equal the .abort label offset. Disagreement = installer bug.
    Logf("citizen_retail  [CHECKOUT STUB] jz@+0x%zX rel8=0x%02X target=stub+0x%zX "
         "(expect .abort @ stub+0x%zX)",
         jzRel8Off, (unsigned)(uint8_t)rel8,
         (size_t)((intptr_t)jzRel8Off + 1 + (int)rel8), abortLabelOff);
    Logf("citizen_retail  checkout hook installed at RVA 0x%06X (atomic gate+charge)",
         TSM_CHECKOUT_HOOK_RVA);
    return true;
}

// v2.1 (Task 7): forward declaration. The runway engine (CPI cache +
// scaling) is defined after the walk section for narrative flow, but
// WalkShop calls ApplySolScaling - declared here so the use resolves.
static void ApplySolScaling(void* person, int absDay, float wallet);

static void WalkShop(BYTE* shop, int day)
{
    void** begin = *(void***)(shop + TSM_BLD_PEOPLE_BEGIN);
    void** end   = *(void***)(shop + TSM_BLD_PEOPLE_END);
    if (!begin || !end || end <= begin) return;
    if (!TsmIsValidGamePtr(begin)) return;

    size_t n = (size_t)(end - begin);
    if (n > 500) n = 500;
    if (!ReadablePtr(begin, n * sizeof(void*))) return;

    for (size_t i = 0; i < n; i++) {
        void* person = begin[i];
        if (!TsmIsValidGamePtr(person)) continue;
        if (!ReadablePtr(person, TSM_PERSON_SIZE)) continue;

        int foreign = *(int*)((BYTE*)person + TSM_PERSON_FOREIGN);
        int tourist = *(int*)((BYTE*)person + TSM_PERSON_TOURIST);
        if (foreign != 0 || tourist != 0) continue;

        uint64_t id = TsmCitizenID(person);
        if (!id) continue;

        // v2.0 (Task 7): induced demand scaling BEFORE the day-gate skip
        // and BEFORE the basket reads amountNeeded. The dedup map
        // (g_solScaledDay) fires strictly once per absolute day, so a
        // citizen marked at another shop earlier still scales exactly
        // once; checkout itself stays gated on AlreadyShoppedToday.
        if (g_solEnabled && g_currency && g_currency->getWalletBalance) {
            int absDay = GetAbsDay(day);
            auto st = g_solScaledDay.find(id);
            if (st == g_solScaledDay.end() || st->second != absDay) {
                float wallet = g_currency->getWalletBalance(person);
                if (!(wallet == wallet) || wallet < 0.0f) wallet = 0.0f;
                ApplySolScaling(person, absDay, wallet);
            }
        }

        if (AlreadyShoppedToday(id, day)) continue;

        int demandCount = *(int*)((BYTE*)person + TSM_PERSON_DEMAND_N);
        if (demandCount <= 0) continue;
        if (demandCount > TSM_PERSON_DEMAND_CAP)
            demandCount = TSM_PERSON_DEMAND_CAP;

        const BYTE* demands = (const BYTE*)person + TSM_PERSON_DEMANDS;
        bool anyCharged = false;

        // Process full basket: charge for all servable demands stocked by this shop
        for (int d = 0; d < demandCount; d++) {
            const BYTE* demand = demands + (size_t)d * TSM_PERSON_DEMAND_STRIDE;
            if (ProcessOneDemand(shop, person, demand)) {
                anyCharged = true;
            }
        }

        if (anyCharged) {
            MarkShoppedToday(id, day);
        }
    }
}

// v2.1 (Task 7): the ONE absolute daily basket cost, cached per absolute
// day. The walk asks every shopper to classify, and the provider's
// basket takes the market-book lock internally, so the read is hoisted:
// first shopper of the day queries resource_pricing v3, everyone else
// reuses the answer. injectedDailyRent = 0 - the Level 1 provider's
// neutral default daily rent rides along (retail must not reach into
// enterprise_base for rent). The helper guards the v3 tail via
// TsmPricingHasCpi and falls back to the documented ~2.15 RUB/day when
// the provider is a v2 build. POD-only: no __try needed (the provider
// fault-guards its own reads; the helpers are pure float math).
static float SolCpiDaily(int absDay)
{
    static int   s_cpiDay = -1;
    static float s_cpi     = 0.0f;
    if (absDay == s_cpiDay && s_cpi > 0.0f) return s_cpi;
    float c = TsmPricingCpiBasket(g_pricing, 0.0f);
    if (!(c == c) || !(c > 0.0f)) c = 2.15f;
    s_cpi    = c;
    s_cpiDay = absDay;
    return c;
}

// v2.3 (Task 7): one day of staple bread = P_food x marketMult x g_breadTons
// (0.0020 t default), in the ACTIVE currency. Mirrors enterprise_base's
// destitute bread-day so the retail threshold stays in step with the CPI
// basket. Returns 0.0f when pricing or the food record is unavailable -
// then no citizen can be Destitute on price data alone (they fall through
// to Low/Middle instead).
static float DestituteBreadCost(void)
{
    if (!g_pricing || !g_pricing->getFood || !g_pricing->getPrice) return 0.0f;
    int cur = TsmCurrencyActive(g_currency);
    void* food = g_pricing->getFood();
    if (!food) return 0.0f;
    float pFood = g_pricing->getPrice(food, cur);
    float mult  = TsmPricingMarketMult(g_pricing, food);
    float cost  = pFood * mult * g_breadTons;
    if (!(cost == cost) || cost <= 0.0f) return 0.0f;
    return cost;
}

// v2.3 (Task 7): per-citizen induced demand scaling, strictly once per
// absolute day (g_solScaledDay dedup). Classification is the top-down
// Economic Class Matrix: Flow (daily wage / transfer / CPI) + Stock
// (wallet / CPI). First match wins:
//   runway >= runway_affluent_days -> Affluent: meat/clothes x1.5,
//             electronics x2.0;
//   runway >= runway_low_days (14 d) OR ppr >= 1.0f -> Middle: baseline,
//             no demand write;
//   wallet < 1 day of bread AND income <= 0.01f -> Destitute:
//             meat/clothes/electronics x 0.0 (food untouched);
//   everyone else -> Low: meat x0.2, clothes x0.5, electronics x0.0;
//             alcohol (kind 3 || kind 10) x1.8 when happiness < escapismThreshold.
static void ApplySolScaling(void* person, int absDay, float wallet)
{
    if (!g_solEnabled || !g_pricing) return;
    if (!TsmIsValidGamePtr(person)) return;

    float cpi = SolCpiDaily(absDay);
    if (!(cpi > 0.0f)) return;

    float runway = wallet / cpi;             // days of survival (stock)
    if (!(runway == runway) || runway < 0.0f) runway = 0.0f;

    // --- Income flow (daily wage / transfer) --------------------------------
    float dailyWage = 0.0f;
    if (g_labor && g_labor->getEffectiveDailyWage) {
        dailyWage = g_labor->getEffectiveDailyWage(person);
        if (!(dailyWage == dailyWage) || dailyWage < 0.0f) dailyWage = 0.0f;
    }
    float ppr = (cpi > 0.0f) ? (dailyWage / cpi) : 0.0f;   // purchasing power ratio

    // --- Destitute bread cost (1 day of staple bread) -----------------------
    float breadCost = DestituteBreadCost();

    // --- Top-down classification (first match wins) -------------------------
    int isDestitute = (wallet < breadCost) && (dailyWage <= 0.01f);
    int isAffluent  = (ppr >= 2.0f) && (runway >= g_runwayAffluentDays);
    int isMiddle    = (ppr >= 1.0f) || (runway >= g_runwayLowDays);
    int isLow       = !isDestitute && !isAffluent && !isMiddle;

    if (isMiddle) {
        // Middle: baseline basket, no demand write. Mark scaled for the day.
        g_solScaledDay[TsmCitizenID(person)] = absDay;
        return;
    }

    const char* stratum = isDestitute ? "Destitute" :
                          isAffluent  ? "Affluent" : "Low";

    float happy = 1.0f;
    int needHappy = isLow;   // only Low checks escapism

    void* meat = nullptr;
    void* clothes = nullptr;
    void* elec = nullptr;
    // Getter records are the v1 prefix every provider carries (the consume
    // falls back through v2/v1, and the engine records are world-independent),
    // so only null function pointers are guarded - no version check needed.
    if (g_pricing->getMeat) meat = g_pricing->getMeat();
    if (g_pricing->getClothes) clothes = g_pricing->getClothes();
    if (g_pricing->getEletronics) elec = g_pricing->getEletronics();

    int n = ReadDemandCountRaw(person);
    for (int d = 0; d < n; d++) {
        float amount = 0.0f;
        int kind = -1;
        void* res = nullptr;
        if (!ReadDemandEntryRaw(person, d, &amount, &kind, &res)) continue;
        if (!(amount == amount) || amount <= 0.0f) continue;

        float mult = 1.0f;
        const char* what = nullptr;

        if (isDestitute) {
            // Survival crisis: zero out non-staple goods. Food (kind 1)
            // is the staple - never scaled for destitute.
            if (kind == 1) continue;                       // food: staple, untouched
            if (res && meat && res == meat) { mult = 0.0f; what = "meat"; }
            else if (res && clothes && res == clothes) { mult = 0.0f; what = "clothes"; }
            else if (res && elec && res == elec) { mult = 0.0f; what = "electronics"; }
            else continue;                                 // other non-staple: leave alone
        } else if (isAffluent) {
            if (res && meat && res == meat) { mult = g_highMeatMult; what = "meat"; }
            else if (res && clothes && res == clothes) { mult = g_highClothesMult; what = "clothes"; }
            else if (res && elec && res == elec) { mult = g_highElecMult; what = "electronics"; }
            else continue;
        } else {   // isLow
            if (res && meat && res == meat) { mult = g_lowMeatMult; what = "meat"; }
            else if (res && clothes && res == clothes) { mult = g_lowClothesMult; what = "clothes"; }
            else if (res && elec && res == elec) { mult = g_lowElecMult; what = "electronics"; }
            else if (kind == 3 || kind == 10) {
                if (needHappy) {
                    happy = ReadHappinessRaw(person, 1.0f);
                    needHappy = 0;
                }
                if (happy < g_escapismThreshold) {
                    mult = g_alcoholEscapismMult;
                    what = "alcohol(escapism)";
                } else continue;
            } else continue;
        }

        if (!what || !(mult >= 0.0f) || mult == 1.0f) continue;

        float scaled = amount * mult;
        if (!(scaled == scaled) || scaled < 0.0f) scaled = 0.0f;
        if (scaled > 1e9f) scaled = 1e9f;
        WriteDemandAmountRaw(person, d, scaled);

        if (g_probe && g_solLogCount < SOL_LOG_BUDGET) {
            g_solLogCount++;
            uint64_t cid = TsmCitizenID(person);
            Logf("citizen_retail  [SOL] citizen %llu runway %.1f d "
                 "(wallet %.2f / cpi %.2f, PPR %.2f, Stratum: %s) scaled demand: "
                 "%s %.2f -> %.2f",
                 cid, (double)runway, (double)wallet, (double)cpi,
                 (double)ppr, stratum, what, (double)amount, (double)scaled);
        }
    }

    g_solScaledDay[TsmCitizenID(person)] = absDay;
}

static void RunShopWalk(void)
{
    if (!g_simClock || !g_currency || !g_pricing) return;

    static DWORD lastWalk = 0;
    DWORD now = GetTickCount();
    if (now - lastWalk < (DWORD)g_tickIntervalMs) return;
    lastWalk = now;

    int day = g_simClock->getDay();
    if (day < 0) return;

    // Daily in-memory prune to keep the maps bounded. The demand-day
    // markers are deliberately NOT aged here by value - the entire set is
    // the current calendar day cohort, so a day rollover clears it whole.
    static int lastPruneAbsDay = -1;
    int currentAbsDay = GetAbsDay(day);
    if (currentAbsDay > 0 && currentAbsDay != lastPruneAbsDay) {
        lastPruneAbsDay = currentAbsDay;
        EnterCriticalSection(&g_lock);
        for (auto it = g_lastShopDay.begin(); it != g_lastShopDay.end(); ) {
            if (currentAbsDay - it->second > 2) {
                it = g_lastShopDay.erase(it);
            } else {
                ++it;
            }
        }
        // v2.0 (Task 7): the once-per-day SoL dedup ages the same way -
        // yesterday's cohort may never scale again, so anything older
        // than the 2-day window leaves with the shop markers.
        for (auto it = g_solScaledDay.begin();
             it != g_solScaledDay.end(); ) {
            if (currentAbsDay - it->second > 2) {
                it = g_solScaledDay.erase(it);
            } else {
                ++it;
            }
        }
        g_demandDay.clear();
        LeaveCriticalSection(&g_lock);
    }

    BYTE* game = TsmGameObject(g_exeBase);
    if (!game || !ReadablePtr(game, TSM_BLD_MIN_READABLE)) return;

    TsmBuildingSpan span;
    if (!TsmGetBuildings(game, &span)) return;

    for (size_t i = 0; i < span.count; i++) {
        BYTE* b = (BYTE*)span.begin[i];
        if (!TsmIsValidGamePtr(b)) continue;
        if (!ReadablePtr(b, TSM_BLD_MIN_READABLE)) continue;
        if (TsmIsDemolished(b)) continue;

        int type = TsmBuildingType(b);
        bool isShop = false;
        for (int k = 0; k < g_shopTypeCount; k++)
            if (g_shopTypes[k] == type) { isShop = true; break; }
        if (!isShop) continue;

        __try { WalkShop(b, day); }
        __except (FaultFilter("citizen_retail shop walk", GetExceptionInformation()))
        {
            Logf("citizen_retail  a shop at type %d faulted during walk", type);
        }
    }
}

// ---------------------------------------------------------------- persistence

#define RETAIL_MAGIC    "TSMRETL1"
#define RETAIL_VERSION  1u
#define RETAIL_FILENAME "tesmio_retail.dat"

#pragma pack(push, 1)
struct RetailSaveHeader {
    char     magic[8];
    uint32_t version;
    uint32_t recordSize;
    uint32_t count;
    uint32_t reserved;
};
struct RetailSaveRecord {
    uint64_t citizenId;
    int32_t  lastShopDay;
};
#pragma pack(pop)

static void SaveRetail(const char* dir)
{
    std::vector<RetailSaveRecord> records;
    EnterCriticalSection(&g_lock);
    records.reserve(g_lastShopDay.size());
    int currentAbsDay = GetAbsDay(g_simClock ? g_simClock->getDay() : 0);
    for (const auto& kv : g_lastShopDay) {
        if (currentAbsDay > 0 && currentAbsDay - kv.second > 2) continue;
        RetailSaveRecord r;
        r.citizenId   = kv.first;
        r.lastShopDay = kv.second;
        records.push_back(r);
    }
    LeaveCriticalSection(&g_lock);

    size_t count = records.size();
    size_t total = sizeof(RetailSaveHeader) + count * sizeof(RetailSaveRecord);
    std::vector<BYTE> buf(total);

    RetailSaveHeader hdr;
    memcpy(hdr.magic, RETAIL_MAGIC, 8);
    hdr.version    = RETAIL_VERSION;
    hdr.recordSize = (uint32_t)sizeof(RetailSaveRecord);
    hdr.count      = (uint32_t)count;
    hdr.reserved   = 0;
    memcpy(buf.data(), &hdr, sizeof(hdr));

    if (count > 0)
        memcpy(buf.data() + sizeof(hdr), records.data(), count * sizeof(RetailSaveRecord));

    if (g_persistence->writeAtomic(dir, RETAIL_FILENAME, buf.data(), total))
        Logf("citizen_retail  saved %zu shopping record(s) into %s\\%s",
             count, dir, RETAIL_FILENAME);
    else
        Logf("citizen_retail  WARN  could not write %s\\%s", dir, RETAIL_FILENAME);
}

static void LoadRetail(const char* dir)
{
    size_t len = 0;
    void* raw = g_persistence->readFile(dir, RETAIL_FILENAME, &len);
    if (!raw) {
        EnterCriticalSection(&g_lock);
        g_lastShopDay.clear();
        LeaveCriticalSection(&g_lock);
        Logf("citizen_retail  no %s in this save - starting empty", RETAIL_FILENAME);
        return;
    }

    if (len < sizeof(RetailSaveHeader)) {
        Logf("citizen_retail  %s too short - ignored", RETAIL_FILENAME);
        free(raw);
        return;
    }

    RetailSaveHeader hdr;
    memcpy(&hdr, raw, sizeof(hdr));
    if (memcmp(hdr.magic, RETAIL_MAGIC, 8) != 0 ||
        hdr.version    != RETAIL_VERSION ||
        hdr.recordSize != sizeof(RetailSaveRecord))
    {
        Logf("citizen_retail  %s version %u, expected %u - ignored",
             RETAIL_FILENAME, hdr.version, RETAIL_VERSION);
        free(raw);
        return;
    }

    size_t want = sizeof(hdr) + (size_t)hdr.count * sizeof(RetailSaveRecord);
    if (want > len) {
        Logf("citizen_retail  %s claims %u records but is only %zu bytes",
             RETAIL_FILENAME, hdr.count, len);
        free(raw);
        return;
    }

    EnterCriticalSection(&g_lock);
    g_lastShopDay.clear();
    g_lastShopDay.reserve(hdr.count);
    const BYTE* p = (const BYTE*)raw + sizeof(hdr);
    for (uint32_t i = 0; i < hdr.count; i++) {
        RetailSaveRecord r;
        memcpy(&r, p, sizeof(r));
        p += sizeof(r);
        g_lastShopDay.emplace(r.citizenId, r.lastShopDay);
    }
    size_t loaded = g_lastShopDay.size();
    LeaveCriticalSection(&g_lock);

    free(raw);
    Logf("citizen_retail  loaded %zu shopping record(s) from %s\\%s",
         loaded, dir, RETAIL_FILENAME);
}

static void OnSaveLoad(TsmSaveEvent ev, const char* dir, void* /*ud*/)
{
    if (!dir || !g_persistence) return;
    if      (ev == TSM_SAVE_WRITING) SaveRetail(dir);
    else if (ev == TSM_SAVE_LOADING) LoadRetail(dir);
}

static void ParseShopTypes(const char* s)
{
    g_shopTypeCount = 0;
    if (!s || !s[0]) {
        g_shopTypes[g_shopTypeCount++] = 3;
        return;
    }
    while (*s && g_shopTypeCount < MAX_SHOP_TYPES) {
        while (*s == ' ' || *s == ',' || *s == '\t') s++;
        if (!*s) break;
        g_shopTypes[g_shopTypeCount++] = atoi(s);
        while (*s && *s != ',') s++;
    }
    if (g_shopTypeCount == 0) g_shopTypes[g_shopTypeCount++] = 3;
}

static void ReadSettings(void)
{
    const char* ini = "plugins\\citizen_retail.ini";
    char v[64];

    g_enabled        = H->configInt(ini, "citizen_retail", "enabled", 1);
    g_probe          = H->configInt(ini, "citizen_retail", "probe",   1);
    g_tickIntervalMs = H->configInt(ini, "citizen_retail", "tick_interval_ms", 250);

    g_minCostCfg = g_minCost;
    if (H->configString(ini, "citizen_retail", "min_cost", v, sizeof(v), "") && v[0]) {
        float m = (float)atof(v);
        if (m >= 0.0f && m == m) g_minCostCfg = m;
    }
    g_minCost = g_minCostCfg;
    // v4: RUB->USD divisor for min_cost unless min_cost_usd overrides it.
    if (H->configString(ini, "citizen_retail", "usd_divisor", v, sizeof(v), "") && v[0]) {
        float d = (float)atof(v);
        if (d >= 1.0f && d <= 1000.0f && d == d) g_usdDivisor = d;
    }

    if (H->configString(ini, "citizen_retail", "shop_types", v, sizeof(v), "") && v[0])
        ParseShopTypes(v);
    else
        ParseShopTypes(nullptr);

    // v2.0 (Task 7): [standard_of_living] wealth strata & induced demand.
    g_solEnabled = H->configInt(ini, "standard_of_living",
                                "sol_enabled", 1);
    if (H->configString(ini, "standard_of_living", "high_meat_mult",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 0.0f && f <= 10.0f && f == f) g_highMeatMult = f;
    }
    if (H->configString(ini, "standard_of_living", "high_clothes_mult",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 0.0f && f <= 10.0f && f == f) g_highClothesMult = f;
    }
    if (H->configString(ini, "standard_of_living", "high_electronics_mult",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 0.0f && f <= 10.0f && f == f) g_highElecMult = f;
    }
    if (H->configString(ini, "standard_of_living", "low_meat_mult",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 0.0f && f <= 10.0f && f == f) g_lowMeatMult = f;
    }
    if (H->configString(ini, "standard_of_living", "low_clothes_mult",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 0.0f && f <= 10.0f && f == f) g_lowClothesMult = f;
    }
    if (H->configString(ini, "standard_of_living", "low_electronics_mult",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 0.0f && f <= 10.0f && f == f) g_lowElecMult = f;
    }
    if (H->configString(ini, "standard_of_living",
                        "low_alcohol_escapism_mult",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 0.0f && f <= 10.0f && f == f) g_alcoholEscapismMult = f;
    }
    if (H->configString(ini, "standard_of_living",
                        "escapism_happiness_threshold",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 0.0f && f <= 1.0f && f == f) g_escapismThreshold = f;
    }
    // v2.1 (Task 7): runway thresholds replace the retired p25/p75 keys.
    // Affluent at >= runway_affluent_days of survival; Low below
    // runway_low_days; everything between is Middle (no write). The old
    // p25_default_rub / p75_default_rub keys are no longer read.
    g_runwayAffluentDays = 30.0f;
    if (H->configString(ini, "standard_of_living", "runway_affluent_days",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f > 0.0f && f <= 10000.0f && f == f) g_runwayAffluentDays = f;
    }
    g_runwayLowDays = 14.0f;
    if (H->configString(ini, "standard_of_living", "runway_low_days",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f >= 0.0f && f <= 1000.0f && f == f) g_runwayLowDays = f;
    }
    // v2.3 (Task 7): tons of staple bread = 1 day of survival for the
    // Destitute bread-cost threshold (P_food x marketMult x bread_tons).
    g_breadTons = 0.0020f;
    if (H->configString(ini, "standard_of_living", "bread_tons",
                        v, sizeof(v), "") && v[0]) {
        float f = (float)atof(v);
        if (f > 0.0f && f <= 1.0f && f == f) g_breadTons = f;
    }
    if (g_runwayLowDays > g_runwayAffluentDays)
        g_runwayLowDays = g_runwayAffluentDays;

    if (g_tickIntervalMs < 50)     g_tickIntervalMs = 50;
    if (g_tickIntervalMs > 5000)   g_tickIntervalMs = 5000;
}

static const TsmCitizenRetailApi kApi = {
    sizeof(TsmCitizenRetailApi),
    RegisterInterceptor,
    ClearInterceptor,
};

extern "C" __declspec(dllexport) unsigned TsmPluginApiVersion(void)
{
    return TSM_API_VERSION;
}

extern "C" __declspec(dllexport) int TsmPluginInit(const TsmHost* host, TsmPluginInfo* info)
{
    TsmBind(host);
    info->name    = "citizen_retail";
    info->version = "2.9";

    ReadSettings();
    if (!g_enabled) {
        Logf("citizen_retail  disabled");
        return 1;
    }

    // v2.5 ABI: the interceptor return contract gained
    // TSM_RETAIL_INSUFFICIENT (-1). The struct layout is unchanged
    // (structSize + 2 fn ptrs, same bytes), so the v1 prefix view is
    // the same struct - it is published under version 1u because the
    // registry matches versions exactly and an out-of-tree consumer
    // built against the v1 header must still resolve the service.
    H->provide(TSM_SERVICE_CITIZEN_RETAIL, TSM_CITIZEN_RETAIL_VERSION_V1, &kApi);
    H->provide(TSM_SERVICE_CITIZEN_RETAIL, TSM_CITIZEN_RETAIL_VERSION, &kApi);
    return 0;
}

extern "C" __declspec(dllexport) int TsmPluginStart(void)
{
    if (!g_enabled) return 1;

    g_simClock = (const TsmSimClockApi*)H->consume(TSM_SERVICE_SIM_CLOCK, TSM_SIM_CLOCK_VERSION);
    if (!g_simClock) {
        Logf("citizen_retail  sim_clock is not installed - cannot run");
        return 1;
    }

    // v4: prefer the unified-currency iface; fall back to v3 (RUB-only
    // routing) so a mixed install still charges instead of unloading.
    g_currency = (const TsmCurrencyApi*)H->consume(TSM_SERVICE_CURRENCY, TSM_CURRENCY_VERSION);
    if (!g_currency)
        g_currency = (const TsmCurrencyApi*)H->consume(TSM_SERVICE_CURRENCY, TSM_CURRENCY_VERSION_V3);
    if (!g_currency) {
        Logf("citizen_retail  currency is not installed - cannot charge");
        return 1;
    }

    g_pricing = (const TsmResourcePricingApi*)H->consume(TSM_SERVICE_RESOURCE_PRICING, TSM_RESOURCE_PRICING_VERSION);
    if (!g_pricing)
        g_pricing = (const TsmResourcePricingApi*)H->consume(TSM_SERVICE_RESOURCE_PRICING, TSM_RESOURCE_PRICING_VERSION_V2);
    if (!g_pricing)
        g_pricing = (const TsmResourcePricingApi*)H->consume(TSM_SERVICE_RESOURCE_PRICING, TSM_RESOURCE_PRICING_VERSION_V1);
    if (!g_pricing) {
        Logf("citizen_retail  resource_pricing is not installed - cannot price");
        return 1;
    }
    if (g_probe) {
        if (TsmPricingHasMarket(g_pricing))
            Logf("citizen_retail  Republic market book available - retail "
                 "demands feed it, prices carry the live multiplier");
        else
            Logf("citizen_retail  Republic market book unavailable (v1 pricing) "
                 "- retail prices use base prices");
        // v2.1 (Task 7): confirm the runway engine's basket source.
        if (TsmPricingHasCpi(g_pricing))
            Logf("citizen_retail  CPI runway engine available (pricing v3)");
        else
            Logf("citizen_retail  WARN  pricing has no CPI tail (pre-v3) - "
                 "runway degrades to the 2.15 default basket");
    }

    // v2.3: income flow (daily wage / transfer) for the SoL matrix.
    // Prefer v4, fall back to v2 (getEffectiveDailyWage lives in the v2
    // prefix). NULL labor degrades to runway-only classification.
    g_labor = (const TsmCitizenLaborApi*)H->consume(TSM_SERVICE_CITIZEN_LABOR, TSM_CITIZEN_LABOR_VERSION);
    if (!g_labor)
        g_labor = (const TsmCitizenLaborApi*)H->consume(TSM_SERVICE_CITIZEN_LABOR, TSM_CITIZEN_LABOR_VERSION_V2);
    if (g_probe) {
        if (g_labor)
            Logf("citizen_retail  citizen_labor available - flow + stock SoL matrix live");
        else
            Logf("citizen_retail  WARN  citizen_labor missing - runway-only SoL (no income flow)");
    }

    // v4 USD scale: the sale floor divides the RUB min_cost by
    // usd_divisor unless min_cost_usd is set explicitly.
    if (TsmCurrencyActive(g_currency) == TSM_CURRENCY_USD) {
        const char* ini = "plugins\\citizen_retail.ini";
        char u[32];
        if (H->configString(ini, "citizen_retail", "min_cost_usd",
                            u, sizeof(u), "") && u[0]) {
            float m = (float)atof(u);
            if (m >= 0.0f && m == m) g_minCost = m;
        } else {
            float d = (g_usdDivisor >= 1.0f) ? g_usdDivisor : 50.0f;
            g_minCost = g_minCostCfg / d;
        }
        // v2.1 (Task 7): no benchmark scaling needed - the CPI basket
        // already answers in the active currency (pricing divides its
        // fallbacks by usd_divisor in USD mode).
    }

    // v2.4: install the native atomic checkout hook at FUN_140171E10+0x1725DE.
    // Requires g_currency and g_pricing (both consumed above).
    if (!InstallCheckoutHook()) {
        Logf("citizen_retail  WARN  checkout hook failed to install - "
             "retail sales remain unguarded (free goods)");
    }

    g_persistence = (const TsmPersistenceApi*)H->consume(TSM_SERVICE_PERSISTENCE, TSM_PERSISTENCE_VERSION);
    if (!g_persistence) {
        Logf("citizen_retail  WARN  no persistence service");
    } else {
        g_persHandle = g_persistence->registerHandler("citizen_retail", OnSaveLoad, nullptr);
        if (g_persHandle <= 0)
            Logf("citizen_retail  WARN  persistence refused the handler");
    }

    g_simClock->registerTick([](void*, float, float) { RunShopWalk(); }, nullptr);
    // v2.1 (Task 7): the monthly benchmark hook is gone with the
    // percentiles - the CPI basket is queried live per absolute day by
    // SolCpiDaily inside the walk, so there is nothing to precompute.

    Logf("citizen_retail  live v2.9: interval %d ms, minimum charge %.2f %s, %d shop type(s)",
         g_tickIntervalMs, g_minCost, TsmCurrencySymbol(g_currency), g_shopTypeCount);
    Logf("citizen_retail  SoL flow+stock: %s, Affluent PPR>=2.0 && runway>=%.0f d, "
         "Middle PPR>=1.0 || runway>=%.0f d, Low < %.0f d, bread %.4f t%s",
         g_solEnabled ? "on" : "off",
         (double)g_runwayAffluentDays, (double)g_runwayLowDays,
         (double)g_runwayLowDays,
         (double)g_breadTons,
         TsmPricingHasCpi(g_pricing) ? " v3 CPI" : ", default 2.15");
    return 0;
}

BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID) { return TRUE; }