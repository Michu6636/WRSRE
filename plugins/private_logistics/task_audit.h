#ifndef PRIVATE_LOGISTICS_TASK_AUDIT_H
#define PRIVATE_LOGISTICS_TASK_AUDIT_H
#include <stdint.h>
#include <string.h>

// Read-only value snapshots, no retained game pointers. Caller validates entry
// and cfg spans. Compare float representations, not rounded log strings.
struct AutoTaskAudit {
    unsigned char enabled, loadAll, unloadAll;
    uint32_t loadBits, unloadBits;
    uintptr_t loadBegin, loadEnd, unloadBegin, unloadEnd;
};
inline AutoTaskAudit AutoReadTaskAudit(const void* entry, const void* cfg)
{
    const unsigned char* p = (const unsigned char*)cfg;
    AutoTaskAudit a = {};
    a.enabled = *(const unsigned char*)entry;
    a.loadAll = p[0]; a.unloadAll = p[0xC8];
    memcpy(&a.loadBits, p + 4, 4); memcpy(&a.unloadBits, p + 0xCC, 4);
    memcpy(&a.loadBegin, p + 8, sizeof(uintptr_t));
    memcpy(&a.loadEnd, p + 16, sizeof(uintptr_t));
    memcpy(&a.unloadBegin, p + 0xD0, sizeof(uintptr_t));
    memcpy(&a.unloadEnd, p + 0xD8, sizeof(uintptr_t));
    return a;
}
inline double AutoAuditPercent(uint32_t bits)
{
    float f; memcpy(&f, &bits, sizeof(f)); return (double)f;
}
// 1=activation, 2/4=load/unload wildcard, 8/16=load/unload percentage,
// 32/64=load/unload selector span. Diagnostic only: never drives policy.
inline unsigned AutoTaskAuditChanges(const AutoTaskAudit& a, const AutoTaskAudit& b)
{
    return (a.enabled != b.enabled ? 1u : 0u) |
        (a.loadAll != b.loadAll ? 2u : 0u) |
        (a.unloadAll != b.unloadAll ? 4u : 0u) |
        (a.loadBits != b.loadBits ? 8u : 0u) |
        (a.unloadBits != b.unloadBits ? 16u : 0u) |
        ((a.loadBegin != b.loadBegin || a.loadEnd != b.loadEnd) ? 32u : 0u) |
        ((a.unloadBegin != b.unloadBegin || a.unloadEnd != b.unloadEnd) ? 64u : 0u);
}
#endif
