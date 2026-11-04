// game_world.h - offsets and safe-read helpers for the game's own objects.
//
// Not a service. A header, included by any plugin that reads the game's
// buildings, people, or resource records, so that the offsets live in one
// file instead of being redeclared in every plugin.

#ifndef TSM_GAME_WORLD_H
#define TSM_GAME_WORLD_H

#include <stdint.h>
#include <stddef.h>
#include "game_addresses.h"

// -----------------------------------------------------------------------------
// The game object
// -----------------------------------------------------------------------------

#define TSM_GO_RVA              (g_tsmAddr.game_object)

#define TSM_GO_DAY              0x590
#define TSM_GO_YEAR             0x594
#define TSM_GO_TIME_OF_DAY      0x59C

#define TSM_GO_TREASURY_USD     0x580
#define TSM_GO_TREASURY_RUB     0x588

#define TSM_GO_BLD_VEC_BEGIN    0x11B08
#define TSM_GO_BLD_VEC_END      0x11B10

#define TSM_GO_RES_VEC          0xC2B0

#define TSM_GO_RES_FOOD         0xC300
#define TSM_GO_RES_MEAT         0xC310
#define TSM_GO_RES_CLOTHES      0xC318
#define TSM_GO_RES_ELETRONICS   0xC320

// -----------------------------------------------------------------------------
// The building object
// -----------------------------------------------------------------------------

#define TSM_BLD_MIN_READABLE    0xC00
#define TSM_BLD_BUILT           0x604
#define TSM_BLD_DEMOLISHED      0xEA8
#define TSM_BLD_TYPEDESC        0x318
#define TSM_BLD_NODE            0x320
#define TSM_BLD_BBOX            0x5D0
#define TSM_BLD_STORAGES_BEGIN  0x970
#define TSM_BLD_STORAGES_END    0x978
#define TSM_BLD_RESIDENTS_BEGIN 0xBC0
#define TSM_BLD_RESIDENTS_END   0xBC8
#define TSM_BLD_PEOPLE_BEGIN    0xBD8
#define TSM_BLD_PEOPLE_END      0xBE0
// Semantic aliases: 0xBC0 holds workers/staff on workplaces (verified by the
// engine's factory tick FUN_1401cf590 and factory panel FUN_140799630);
// 0xBD8 holds visitors/customers inside the building (verified by the shop
// and pub panel builders).
#define TSM_BLD_WORKERS_BEGIN   TSM_BLD_RESIDENTS_BEGIN   // 0xBC0
#define TSM_BLD_WORKERS_END     TSM_BLD_RESIDENTS_END     // 0xBC8
#define TSM_BLD_VISITORS_BEGIN  TSM_BLD_PEOPLE_BEGIN      // 0xBD8
#define TSM_BLD_VISITORS_END    TSM_BLD_PEOPLE_END        // 0xBE0
// 0xDDC (float): Customer service capacity — the maximum number of customers
// that can be attended at once by currently present workers. Verified by the
// shop/pub panel builders: customers = people_at_0xBD8 - (int)*(float*)(bld+0xDDC).
// NOT the maximum worker limit — do not use for staffing calculations.
#define TSM_BLD_CUSTOMER_CAP    0xDDC
// 0xDEC (int): Maximum worker limit on the building instance. Initialized from
// the type descriptor's $WORKERS_NEEDED at construction (never written after).
// Verified by the factory tick FUN_1401cf590 (worker production limit) and
// factory/shop/pub panel builders (hiring gate when == 0). This is the true
// maximum workforce capacity for staffing ratio and UI display.
#define TSM_BLD_MAX_WORKERS     0xDEC
// v8.0 (Tasks 1+3): environmental attractiveness / land value (float,
// 0.0-1.0, updated daily by FUN_1404D5F50 sampling game+0x120A8; valid on
// non-demolished type-2 residences; 19-victoria3 F4/F5), the native rent
// slider (float 0..1, per-building player-adjustable via FUN_14073E950
// GetString(0x995) "%ls > %d %%", bld+0x13E0 = slider/100; v8.3 rent
// grounding), and the native decayed flag (set to 1 by FUN_14013FE70 when
// a building collapses into ruins; the mod-facing "already decayed"
// signal, 19-victoria3 F3).
#define TSM_BLD_ATTRACTIVENESS  0x11B0
#define TSM_BLD_RENT_LEVEL      0x13E0
#define TSM_BLD_DECAYED         0x1198
// v8.3: tag-scan span. 0x580 cut off larger structs; 0x1200 covers the
// full building header while every offset is ReadablePtr-gated.
#define TSM_TAG_SCAN_END        0x1200

// -----------------------------------------------------------------------------
// Native Cities / NamePoints (14-cities.md)
// -----------------------------------------------------------------------------
// bld + 0x108 = NamePoint* — the native City/Town this building belongs to.
// Verified by two independent witnesses: the building vector walk in the
// city-window head FUN_1407CEF60 (reads bld+0x108 as the city pointer) and
// the resident assignment pass FUN_1401b5420 (writes bld+0x108 from the
// chosen NamePoint). Every placed building carries this pointer; null means
// unassigned (e.g. a just-placed site before the next city tick).
#define TSM_BLD_CITY            0x108   // NamePoint* — the city this building belongs to

#define TSM_NAMEPOINT_POS       0x04    // float x, y, z — city centre
#define TSM_NAMEPOINT_NAME      0x10    // wchar_t[128] — city name
#define TSM_NAMEPOINT_BLD_BEGIN 0x1A0   // vector<Building*> begin
#define TSM_NAMEPOINT_BLD_END   0x1B0   // vector<Building*> end

// -----------------------------------------------------------------------------
// The type descriptor - construction phases (v3.5.0)
//
// Every building type keeps its construction phases here: a std::vector of
// phase records, each phase carrying its own {resource*, amount} vector at
// +0x08. Pinned for 1.1.1.9 by two independent witnesses: the decompile of
// FUN_14013FE70 (scratch/v3_budget_funcs.c reads typedesc+0x370/0x378 with
// stride 0x21B8 straight off building+0x318 of a placed site) and the rich
// tooltip's phase walk at exe+0x7AC315 / +0x7AC4C0.
// -----------------------------------------------------------------------------

#define TSM_TD_TYPE_NUM         0x360
#define TSM_TD_NAME             0x000
#define TSM_TD_PHASES_BEGIN     0x370   // std::vector<Phase> begin
#define TSM_TD_PHASES_END       0x378   // std::vector<Phase> end
#define TSM_PHASE_STRIDE        0x21B8
#define TSM_PHASE_COSTS         0x08    // std::vector<Entry> inside each phase
#define TSM_COST_STRIDE         0x10    // {Resource* +0x00, float amount +0x08}
#define TSM_COST_RESOURCE       0x00
#define TSM_COST_AMOUNT         0x08

// -----------------------------------------------------------------------------
// The Person object
// -----------------------------------------------------------------------------

#define TSM_PERSON_SIZE         0x750
#define TSM_PERSON_ENTITY_ID    0x08
#define TSM_PERSON_SPEED_SEED   0xA4
#define TSM_PERSON_FOREIGN      0xC8
#define TSM_PERSON_AGE          0xD4
// Task 7 (SoL): native happiness float (0..1). Backs the poverty-escapism
// gate; Citizen_SoL_Investigation confirms this is a float, not a ptr.
#define TSM_PERSON_HAPPINESS    0xE8
#define TSM_PERSON_TOURIST      0x71C
// 0x734 (float): "spent on retail this period", written by BOTH the engine
// (FUN_140171E10's fulfillment block: `*(float*)(person+0x734) += fVar20`,
// see scratch/sol_171e10.c) and the currency plugin's WalletTake
// (currency.cpp PERSON_SPENT_OFFSET). Anything that reverses a wallet
// debit must reverse this too, or the citizen's satisfaction accumulator
// (+0x738) and the city's retail turnover drift upward on money that was
// handed back.
#define TSM_PERSON_SPENT        0x734
#define TSM_PERSON_DEMAND_N     0x110
#define TSM_PERSON_DEMANDS      0x118
#define TSM_PERSON_DEMAND_STRIDE 0x80
#define TSM_PERSON_DEMAND_CAP   7

// -----------------------------------------------------------------------------
// The resource record
// -----------------------------------------------------------------------------

#define TSM_RES_STRIDE          832
#define TSM_RES_NAME            0x00
#define TSM_RES_CAPTION         0x40
#define TSM_RES_ICON            0x48
#define TSM_RES_PRICE_RUB       0x58
#define TSM_RES_PRICE_USD       0x5C
#define TSM_RES_BASE_RUB        0x78
#define TSM_RES_BASE_USD        0x7C
#define TSM_RES_BUY_MULT_RUB    0x8C   // import buy multiplier (price x this)
#define TSM_RES_BUY_MULT_USD    0xAC

// -----------------------------------------------------------------------------
// The storage block
// -----------------------------------------------------------------------------

#define TSM_STORAGE_STRIDE      0xE0
#define TSM_ST_SLOTS_BEGIN      0x00
#define TSM_ST_SLOTS_END        0x08
#define TSM_ST_IMPORT           0x84   // byte: 1 when built from $STORAGE_IMPORT* / $STORAGE_FUEL
#define TSM_ST_EXPORT           0x85   // byte: 1 when built from $STORAGE_EXPORT*
#define TSM_ST_CAPACITY         0x8C
#define TSM_ST_CLASS            0x90

#define TSM_SLOT_STRIDE         0x10
#define TSM_SLOT_RESOURCE       0x00
#define TSM_SLOT_CONTENT        0x08
#define TSM_SLOT_QUALITY        0x0C

// -----------------------------------------------------------------------------
// Inline helpers
// -----------------------------------------------------------------------------

static inline bool TsmIsValidGamePtr(const void* p)
{
    uintptr_t u = (uintptr_t)p;
    return u >= 0x10000 && u <= 0x00007FFFFFFEFFFFULL;
}

static inline const wchar_t* TsmBuildingCityName(const void* b)
{
    if (!b || !TsmIsValidGamePtr(b) || !ReadablePtr(b, TSM_BLD_MIN_READABLE)) return L"";
    void* np = *(void**)((const BYTE*)b + TSM_BLD_CITY);
    if (!np || !TsmIsValidGamePtr(np) || !ReadablePtr(np, 0x110)) return L"";
    return (const wchar_t*)((const BYTE*)np + TSM_NAMEPOINT_NAME);
}

static inline BYTE* TsmGameObject(BYTE* exeBase)
{
    if (!exeBase || !g_tsmAddr.exeVerified || g_tsmAddr.game_object == 0)
        return nullptr;
    return exeBase + g_tsmAddr.game_object;
}

struct TsmBuildingSpan {
    void** begin;
    void** end;
    size_t count;
};

static inline bool TsmGetBuildings(BYTE* game, TsmBuildingSpan* out)
{
    if (!game || !out) return false;
    if (!ReadablePtr(game + TSM_GO_BLD_VEC_BEGIN, 16)) return false;

    out->begin = *(void***)(game + TSM_GO_BLD_VEC_BEGIN);
    out->end   = *(void***)(game + TSM_GO_BLD_VEC_END);
    if (!out->begin || !out->end || out->end <= out->begin) return false;

    size_t n = (size_t)(out->end - out->begin);
    if (n > 25000) return false;
    out->count = n;
    return true;
}

static inline void* TsmShopGood(BYTE* game, int which)
{
    if (!game || !ReadablePtr(game + TSM_GO_RES_FOOD, 0x28)) return 0;
    switch (which) {
        case 0:  return *(void**)(game + TSM_GO_RES_FOOD);
        case 1:  return *(void**)(game + TSM_GO_RES_MEAT);
        case 2:  return *(void**)(game + TSM_GO_RES_CLOTHES);
        default: return *(void**)(game + TSM_GO_RES_ELETRONICS);
    }
}

static inline bool TsmIsFinished(const void* b)
{
    return *(const float*)((const BYTE*)b + TSM_BLD_BUILT) >= 1.0f;
}

static inline bool TsmIsDemolished(const void* b)
{
    return *(const BYTE*)((const BYTE*)b + TSM_BLD_DEMOLISHED) != 0;
}

// v8.0 (Task 3): true while the engine's own decayed/ruined flag is set on
// the building (native FUN_14013FE70 sets bld+0x1198 = 1; engine-persisted
// in the save). The field is a BYTE: the dispatcher routes on
// *(char*)(bld+0x1198) == 0 (scratch/d_dispatcher.txt:563), and the v8.7
// probe session proved the upper three bytes are UNRELATED adjacent data -
// live, occupied dwellings carried int reads 268700160 / -256 / -1061158912
// (0xC0C0C0C0 float bits) / 1048576 while the routing byte stayed 0. The
// old int read here made the registration pass latch those live dwellings
// as "Abandoned (Ruins)" right after privatisation. Never faults.
static inline bool TsmIsDecayed(const void* b)
{
    if (!TsmIsValidGamePtr(b)) return false;
    if (!ReadablePtr((const BYTE*)b + TSM_BLD_DECAYED, sizeof(BYTE))) return false;
    return *(const BYTE*)((const BYTE*)b + TSM_BLD_DECAYED) != 0;
}

// v8.0 (Task 1): the residence's land-value scalar (0.0-1.0), or the
// neutral 0.5 fallback when the field is unreadable or out of range.
// Never faults; safe from panel builders on the render thread.
static inline float TsmAttractiveness(const void* b)
{
    float dflt = 0.5f;
    if (!TsmIsValidGamePtr(b)) return dflt;
    if (!ReadablePtr((const BYTE*)b + TSM_BLD_ATTRACTIVENESS, sizeof(float)))
        return dflt;
    float v = *(const float*)((const BYTE*)b + TSM_BLD_ATTRACTIVENESS);
    if (!(v == v) || v < 0.0f || v > 1.0f) return dflt;
    return v;
}

// v8.10 (2026-10-18): BOTH reads gated. The old body trusted
// TsmIsValidGamePtr(), which is a RANGE check, not a readability check -
// the exact class of bug v8.4 burned 14,707 C0000005s on when it
// dereferenced float bit-patterns read out of 8-byte slots as pointers.
// The failure this hardens against is concrete: when a PERSON object is
// handed in by mistake (ui_panels running the widget pass on the citizen
// panel before v6.6), `person + 0x318` is not a type descriptor - it is
// the FIFTH demand record (0x118 + 4 * 0x80 = 0x318), i.e. the float pair
// {remaining, rate}. A float pair whose high dword is zero passes the
// range check (0x000000003F800000 is "in range") and the following
// `*(int*)(td + 0x360)` reads unmapped low memory.
// Never faults: every read is range-checked AND readability-checked, and
// the resulting type number is sanity-bounded.
#define TSM_TD_MAX_TYPE         4096   // type numbers are small; anything
                                       // bigger is not a descriptor
static inline int TsmBuildingType(const void* b)
{
    if (!b || !TsmIsValidGamePtr(b)) return -1;
    if (!ReadablePtr((const BYTE*)b + TSM_BLD_TYPEDESC, sizeof(void*)))
        return -1;
    void* td = *(void**)((const BYTE*)b + TSM_BLD_TYPEDESC);
    if (!td || !TsmIsValidGamePtr(td)) return -1;
    if (!ReadablePtr((const BYTE*)td + TSM_TD_TYPE_NUM, sizeof(int)))
        return -1;
    int t = *(const int*)((const BYTE*)td + TSM_TD_TYPE_NUM);
    if (t < 0 || t > TSM_TD_MAX_TYPE) return -1;
    return t;
}

// v8.10: "is this actually a Building?" - the discriminator ui_panels'
// window+0x240 slot cannot give us on its own, because the SAME offset
// holds a Person* on the citizen panel (kind 2, builder 0x7CEEB0).
//
// A Building carries a type descriptor at +0x318 whose own +0x360 is a
// small type number, and the descriptor is a large object (>= 0x380, see
// TSM_TD_PHASES_END). A Person* fails at least one of those: the demand
// array it aliases at +0x318 either has no valid 64-bit pointer in it, or
// points at memory too short to hold a descriptor. Cheap, lock-free,
// fault-free, safe on the render thread.
static inline bool TsmLooksLikeBuilding(const void* b)
{
    if (!b || !TsmIsValidGamePtr(b)) return false;
    if (!ReadablePtr(b, TSM_BLD_MIN_READABLE)) return false;
    if (!ReadablePtr((const BYTE*)b + TSM_BLD_TYPEDESC, sizeof(void*)))
        return false;
    void* td = *(void**)((const BYTE*)b + TSM_BLD_TYPEDESC);
    if (!td || !TsmIsValidGamePtr(td)) return false;
    if (!ReadablePtr(td, TSM_TD_PHASES_END + sizeof(void*))) return false;
    return TsmBuildingType(b) >= 0;
}

static inline bool TsmResourceName(const void* rec, char* out, size_t n)
{
    if (!rec || !out || n < 2) return false;
    return SafeReadStr((const BYTE*)rec + TSM_RES_NAME, out, n);
}

static inline float TsmResourceLivePrice(const void* rec, int currency)
{
    if (!rec) return 0.0f;
    const BYTE* p = (const BYTE*)rec;
    return (currency == 1)
        ? *(const float*)(p + TSM_RES_PRICE_USD)
        : *(const float*)(p + TSM_RES_PRICE_RUB);
}

#endif // TSM_GAME_WORLD_H
