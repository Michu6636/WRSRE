// almanac_ui_api.h - versioned mod-owned Almanac UI shell/provider contract.
//
// This header is the contract consumed by Almanac page providers and by the
// future integrator (worktree 06). It exposes no native tab indices and no
// raw game pointers. All cross-DLL structures are fixed-capacity and
// append-only: new fields are added at the end of structures and guarded by
// structSize / capability checks.

#ifndef TSM_ALMANAC_UI_API_H
#define TSM_ALMANAC_UI_API_H

#include <cstddef>
#include <cstdint>

#ifdef __cplusplus
// Forward declaration keeps this shared header independent of the Almanac
// plugin's private include directory. almanac_shell.h supplies the full type.
namespace almanac { struct AlmanacHostSurface; }
extern "C" {
#endif

// ---------------------------------------------------------------------------
// Service identity and versioning.
// ---------------------------------------------------------------------------
#define TSM_SERVICE_ALMANAC_UI "almanac_ui"

#define ALMANAC_UI_API_VERSION 1u
#define ALMANAC_UI_API_VERSION_V1 1u

// ---------------------------------------------------------------------------
// Fixed capacities. These bound every structure that may cross a DLL
// boundary; providers must never exceed them.
// ---------------------------------------------------------------------------
constexpr std::size_t ALMANAC_MAX_PAGES = 7u;
constexpr std::size_t ALMANAC_MAX_PROVIDERS = 32u;
constexpr std::size_t ALMANAC_MAX_ROWS = 64u;
constexpr std::size_t ALMANAC_TEXT_LEN = 128u;
constexpr std::size_t ALMANAC_UNIT_LEN = 16u;
constexpr std::size_t ALMANAC_CURRENCY_LEN = 8u;

// ---------------------------------------------------------------------------
// Stable mod-owned page IDs. These are NOT native tab indices.
// ---------------------------------------------------------------------------
enum AlmanacPageId {
    ALMANAC_PAGE_OVERVIEW = 0,
    ALMANAC_PAGE_FISCAL_POLICY = 1,
    ALMANAC_PAGE_MONEY_FLOW = 2,
    ALMANAC_PAGE_PRIVATE_ECONOMY = 3,
    ALMANAC_PAGE_PRICES_CITIZENS = 4,
    ALMANAC_PAGE_LOGISTICS_PRODUCTION = 5,
    ALMANAC_PAGE_CAUSATION = 6,
    ALMANAC_PAGE_COUNT = 7,
    ALMANAC_PAGE_INVALID = 0x7fffffff
};

// Compatibility aliases for common naming variants.
#define ALMANAC_PAGE_OVERVIEW_ID ALMANAC_PAGE_OVERVIEW
typedef AlmanacPageId AlmanacPage;

// ---------------------------------------------------------------------------
// Value classes. Structural: never aggregate across different classes.
// ---------------------------------------------------------------------------
enum AlmanacValueClass {
    ALMANAC_VALUE_ACTUAL = 0,
    ALMANAC_VALUE_DRY_RUN = 1,
    ALMANAC_VALUE_ESTIMATED = 2,
    ALMANAC_VALUE_COUNT = 3,
    ALMANAC_VALUE_INVALID = -1
};

// ---------------------------------------------------------------------------
// Fixed-capacity view-model structures.
// ---------------------------------------------------------------------------
struct AlmanacMoney {
    double amount = 0.0;
    int currency = -1;                 // -1 = not a currency; 0 = RUB, 1 = USD
    char unit[ALMANAC_UNIT_LEN] = {0};
    AlmanacValueClass valueClass = ALMANAC_VALUE_ACTUAL;
    int available = 0;
    int stale = 0;
};

struct AlmanacDisplayRow {
    char label[ALMANAC_TEXT_LEN] = {0};
    char value[ALMANAC_TEXT_LEN] = {0};
    char unit[ALMANAC_UNIT_LEN] = {0};
    int currency = -1;
    AlmanacValueClass valueClass = ALMANAC_VALUE_ACTUAL;
    int available = 0;
    int stale = 0;
    std::uint64_t updatedAt = 0u;

    // Optional command metadata. Read-only rows leave these at zero. The
    // shell dispatches actionId only for an interactive, enabled row.
    unsigned actionId = 0u;
    int interactive = 0;
    int enabled = 0;
};

struct AlmanacPageSnapshot {
    AlmanacPageId page = ALMANAC_PAGE_OVERVIEW;
    std::uint64_t hostGeneration = 0u;
    std::uint64_t frameNumber = 0u;
    int populated = 0;
    int available = 0;
    int stale = 0;
    int coherent = 1;
    std::size_t rowCount = 0u;
    AlmanacDisplayRow rows[ALMANAC_MAX_ROWS];
    char emptyState[ALMANAC_TEXT_LEN] = {0};
    char title[ALMANAC_TEXT_LEN] = {0};
};

struct AlmanacAggregateBucket {
    AlmanacValueClass valueClass = ALMANAC_VALUE_ACTUAL;
    int currency = -1;
    char unit[ALMANAC_UNIT_LEN] = {0};
    double total = 0.0;
    std::size_t count = 0u;
    int truncated = 0;
};

struct AlmanacRenderFrame {
    AlmanacPageId page = ALMANAC_PAGE_OVERVIEW;
    int rendered = 0;
    std::size_t rowCount = 0u;
    float dpi = 0.0f;
    float rowHeight = 0.0f;
    float originX = 0.0f;
    float originY = 0.0f;
    float extentWidth = 0.0f;
    float extentHeight = 0.0f;
    int clipped = 0;
    float clipX = 0.0f;
    float clipY = 0.0f;
    float clipWidth = 0.0f;
    float clipHeight = 0.0f;
    int scrolled = 0;
    float scrollOffsetY = 0.0f;
};

// ---------------------------------------------------------------------------
// Callback types. Providers populate bounded snapshots only; they must not
// retain locks or pointers after return.
// ---------------------------------------------------------------------------
typedef int (*AlmanacPopulatePageCallback)(const almanac::AlmanacHostSurface* host,
                                          AlmanacPageSnapshot* out,
                                          void* userData);

typedef void (*AlmanacPageActionCallback)(AlmanacPageId page,
                                         unsigned actionId,
                                         void* userData);

// ---------------------------------------------------------------------------
// Versioned API struct. Append-only: new members go at the end; consumers
// guard with structSize checks.
// ---------------------------------------------------------------------------
struct AlmanacUiApi {
    unsigned structSize = 0u;
    unsigned apiVersion = ALMANAC_UI_API_VERSION;

    // Factory / lifecycle (optional; may be null in a particular host).
    // If null, callers use the direct functions in almanac_shell.h.
    void* reserved = nullptr;
};

static inline int AlmanacUiApiHasShell(const AlmanacUiApi* api) {
    return api != nullptr &&
           api->structSize >= sizeof(AlmanacUiApi) &&
           api->apiVersion >= ALMANAC_UI_API_VERSION;
}

#ifdef __cplusplus
}
#endif

#endif // TSM_ALMANAC_UI_API_H
