#ifndef PRIVATE_LOGISTICS_SITE_PERMISSIONS_H
#define PRIVATE_LOGISTICS_SITE_PERMISSIONS_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>

// Directions are from the truck's perspective. No game pointers retained here.
enum { AUTO_ALLOW_LOAD = 1, AUTO_ALLOW_UNLOAD = 2, AUTO_ALLOW_BOTH = 3 };

inline unsigned AutoSiteCapabilities(int type, const char* asset)
{
    // Explicit asset exception: observed warehouse can report shop type 3.
    // Do not use substring "store"/"warehouse" guesses for Workshop assets.
    const char* name = asset ? asset : "";
    for (const char* p = name; *p; ++p)
        if (*p == '/' || *p == '\\') name = p + 1;
    if (strcmp(name, "warehouse_covered_combined") == 0 ||
        strcmp(name, "warehouse_open") == 0) return AUTO_ALLOW_BOTH;
    if (type == 5 || type == 6) return AUTO_ALLOW_BOTH; // storage / factory
    switch (type) {
        // Known retail/service consumer families; never loading sources.
        case 3: case 4: case 10: case 23: case 24: case 25: case 26: case 28:
            return AUTO_ALLOW_UNLOAD;
        default: return 0; // unknown, infrastructure, customs, DO: no guessed role
    }
}

// Native labelled widget polarity established by the user's v0.18 game test:
// raw 0 draws CHECKED; raw 1 draws UNCHECKED. Keep it at the UI boundary;
// saved permission bits and TaskCfg directions retain their logical meaning.
inline unsigned AutoNativeCheckboxValue(bool allowed) { return allowed ? 0u : 1u; }
inline bool AutoNativeCheckboxChecked(unsigned raw) { return raw == 0; }

inline bool AutoPreserveManualCustoms(int type) { return type == 20; }

// Entry activation is separate from direction selectors. Appending an OFF
// entry is not a functioning company connection. Request native activation;
// the engine must still validate roads, vehicle compatibility and orders.
inline bool AutoApplyEntryActivation(void* entry, unsigned directions)
{
    unsigned char* enabled = (unsigned char*)entry;
    const unsigned char wanted = (directions & AUTO_ALLOW_BOTH) ? 1 : 0;
    if (*enabled == wanted) return false;
    *enabled = wanted;
    return true;
}

inline unsigned AutoEffectiveDirections(bool isPrivate, unsigned permissions,
                                        unsigned capabilities)
{
    return (isPrivate ? capabilities : (permissions & capabilities)) & AUTO_ALLOW_BOTH;
}

inline unsigned char AutoMigratePermission(unsigned char value, bool legacy)
{
    // PLSTOR17: 1=LOAD, 2=explicit OFF. Never turn old OFF into UNLOAD.
    if (legacy) return value == 1 ? AUTO_ALLOW_LOAD : 0;
    return value <= AUTO_ALLOW_BOTH ? value : 0;
}

inline unsigned char AutoPermissionToggle(unsigned mask, unsigned direction,
                                           unsigned checked, unsigned capabilities)
{
    return (unsigned char)((checked ? (mask | direction) : (mask & ~direction)) & capabilities);
}

// Company policy bounds (not permission bits): keep source thresholds below
// the native 90% ceiling and use a positive destination replenishment trigger.
inline int AutoClampSourcePercent(int value) { return value < 0 ? 0 : (value > 90 ? 90 : value); }
inline int AutoClampDestinationPercent(int value) { return value < 10 ? 10 : (value > 100 ? 100 : value); }

// The caller validates cfg and both native vector spans before calling.
// Deny = no wildcard AND empty resource selector list, not just threshold=0.
// Keep allocation begin/capacity untouched: native code still owns the vector.
// The private DO owns its task resource selectors; native storage/resource and
// reachability rules remain the planner's responsibility.
inline bool AutoApplyTaskDirections(void* cfg, unsigned directions,
                                    int sourcePercent = 10, int destinationPercent = 30)
{
    unsigned char* p = (unsigned char*)cfg;
    bool changed = false;
    for (unsigned side = 0; side != 2; ++side) {
        const size_t offset = side ? 0xC8 : 0;
        const bool allowed = (directions & (side ? AUTO_ALLOW_UNLOAD : AUTO_ALLOW_LOAD)) != 0;
        const unsigned char all = allowed ? 1 : 0;
        if (p[offset] != all) { p[offset] = all; changed = true; }
        // A denied direction is enforced by wildcard OFF plus an empty list,
        // NOT its percentage. Native UI normalizes disabled unload to 10%.
        // Rewriting that unused value to zero on every day needlessly marks
        // the office dirty and rearms its countdown. Preserve it verbatim.
        if (allowed) {
            const int threshold = side ? AutoClampDestinationPercent(destinationPercent) : AutoClampSourcePercent(sourcePercent);
            const float percentage = (float)threshold / 100.0f;
            float previous;
            memcpy(&previous, p + offset + 4, sizeof(previous));
            if (previous != percentage) {
                memcpy(p + offset + 4, &percentage, sizeof(percentage)); changed = true;
            }
        }
        if (!allowed) {
            uintptr_t begin, end;
            memcpy(&begin, p + offset + 8, sizeof(begin));
            memcpy(&end, p + offset + 16, sizeof(end));
            if (end != begin) {
                memcpy(p + offset + 16, &begin, sizeof(begin)); changed = true;
            }
        }
    }
    return changed;
}
#endif
