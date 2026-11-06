#include "almanac_shell.h"
#include "almanac_pages.h"

#include <algorithm>
#include <cmath>
#include <cstring>

namespace {
constexpr std::size_t kMaxProviders = ALMANAC_MAX_PROVIDERS;
} // namespace

struct ProviderEntry {

    int handle = 0;
    int valid = 0;
    AlmanacPageId page = ALMANAC_PAGE_OVERVIEW;
    AlmanacPopulatePageCallback populate = nullptr;
    AlmanacPageActionCallback action = nullptr;
    void* userData = nullptr;
};

struct AlmanacShell {
    AlmanacPageId currentPage = ALMANAC_PAGE_OVERVIEW;
    AlmanacPageId previousPage = ALMANAC_PAGE_OVERVIEW;
    int hasPrevious = 0;

    ProviderEntry providers[kMaxProviders];
    std::size_t providerCount = 0;
    int nextHandle = 1;

    AlmanacPageSnapshot snapshots[ALMANAC_PAGE_COUNT];

    std::uint64_t hostGeneration = 0u;
    int hostRenderable = 0;
    float layoutDpi = 1.0f;
    float rowHeight = 35.0f;
    float scrollOffsetY = 0.0f;
    int lastHitRow = -1;
    AlmanacRenderFrame lastFrame{};
};

namespace {
void copy_text(char* dst, std::size_t cap, const char* src) {

    if (!dst || cap == 0) return;
    if (!src) { dst[0] = '\0'; return; }
    std::size_t i = 0;
    for (; i + 1 < cap && src[i] != '\0'; ++i) dst[i] = src[i];
    dst[i] = '\0';
}

void reset_snapshot(AlmanacPageSnapshot& snap, AlmanacPageId page, std::uint64_t gen, std::uint64_t frame) {
    snap = AlmanacPageSnapshot{};
    snap.page = page;
    snap.hostGeneration = gen;
    snap.frameNumber = frame;
    snap.populated = 0;
    snap.available = 0;
    snap.stale = 0;
    snap.coherent = 1;
    snap.rowCount = 0;
    copy_text(snap.title, ALMANAC_TEXT_LEN, almanac_page_title(page));
    copy_text(snap.emptyState, ALMANAC_TEXT_LEN, almanac_page_default_empty_state(page));
}

float clampf(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

} // namespace

extern "C" AlmanacShell* almanac_shell_create(void) {
    AlmanacShell* shell = new AlmanacShell();
    for (std::size_t i = 0; i < ALMANAC_PAGE_COUNT; ++i) {
        reset_snapshot(shell->snapshots[i], static_cast<AlmanacPageId>(i), 0u, 0u);
        shell->snapshots[i].populated = 1;
    }
    shell->currentPage = ALMANAC_PAGE_OVERVIEW;
    shell->previousPage = ALMANAC_PAGE_OVERVIEW;
    return shell;
}

extern "C" void almanac_shell_destroy(AlmanacShell* shell) {
    delete shell;
}

extern "C" int almanac_shell_register_provider(AlmanacShell* shell,
                                              AlmanacPageId page,
                                              AlmanacPopulatePageCallback populate,
                                              void* userData) {
    if (!shell || !populate) return 0;
    if (page < 0 || page >= ALMANAC_PAGE_COUNT) return 0;
    AlmanacShell* s = shell;
    for (std::size_t i = 0; i < kMaxProviders; ++i) {
        if (!s->providers[i].valid) {
            s->providers[i].handle = s->nextHandle++;
            if (s->nextHandle <= 0) s->nextHandle = 1;
            s->providers[i].valid = 1;
            s->providers[i].page = page;
            s->providers[i].populate = populate;
            s->providers[i].userData = userData;
            ++s->providerCount;
            return s->providers[i].handle;
        }
    }
    return 0;
}

extern "C" int almanac_shell_set_provider_action(AlmanacShell* shell, int handle,
                                                 AlmanacPageActionCallback action) {
    if (!shell || handle <= 0) return 0;
    ProviderEntry* target = nullptr;
    for (std::size_t i = 0; i < kMaxProviders; ++i) {
        ProviderEntry& pe = shell->providers[i];
        if (pe.valid && pe.handle == handle) target = &pe;
    }
    if (!target) return 0;
    if (action) {
        for (std::size_t i = 0; i < kMaxProviders; ++i) {
            const ProviderEntry& pe = shell->providers[i];
            if (pe.valid && pe.handle != handle && pe.page == target->page && pe.action) {
                return 0; // never fan one command out to multiple economic providers
            }
        }
    }
    target->action = action;
    return 1;
}

extern "C" void almanac_shell_unregister_provider(AlmanacShell* shell, int handle) {
    if (!shell || handle <= 0) return;
    AlmanacShell* s = shell;
    for (std::size_t i = 0; i < kMaxProviders; ++i) {
        if (s->providers[i].valid && s->providers[i].handle == handle) {
            s->providers[i] = ProviderEntry{};
            if (s->providerCount > 0) --s->providerCount;
            return;
        }
    }
}

extern "C" std::size_t almanac_shell_provider_count(const AlmanacShell* shell) {
    if (!shell) return 0;
    return shell->providerCount;
}

extern "C" int almanac_shell_provider_is_registered(const AlmanacShell* shell, int handle) {
    if (!shell || handle <= 0) return 0;
    const AlmanacShell* s = shell;
    for (std::size_t i = 0; i < kMaxProviders; ++i) {
        if (s->providers[i].valid && s->providers[i].handle == handle) return 1;
    }
    return 0;
}

extern "C" int almanac_shell_navigate(AlmanacShell* shell, AlmanacPageId page) {
    if (!shell) return 0;
    if (page < 0 || page >= ALMANAC_PAGE_COUNT) return 0;
    AlmanacShell* s = shell;
    if (page == s->currentPage) return 1;
    s->previousPage = s->currentPage;
    s->hasPrevious = 1;
    s->currentPage = page;
    return 1;
}

extern "C" int almanac_shell_go_back(AlmanacShell* shell) {
    if (!shell) return 0;
    AlmanacShell* s = shell;
    if (s->hasPrevious && s->previousPage != s->currentPage) {
        s->currentPage = s->previousPage;
        s->previousPage = ALMANAC_PAGE_OVERVIEW;
        s->hasPrevious = 0;
    } else {
        s->currentPage = ALMANAC_PAGE_OVERVIEW;
        s->previousPage = ALMANAC_PAGE_OVERVIEW;
        s->hasPrevious = 0;
    }
    return 1;
}

extern "C" AlmanacPageId almanac_shell_current_page(const AlmanacShell* shell) {
    if (!shell) return ALMANAC_PAGE_OVERVIEW;
    return shell->currentPage;
}

extern "C" AlmanacPageId almanac_shell_previous_page(const AlmanacShell* shell) {
    if (!shell) return ALMANAC_PAGE_OVERVIEW;
    const AlmanacShell* s = shell;
    return s->hasPrevious ? s->previousPage : ALMANAC_PAGE_OVERVIEW;
}

extern "C" int almanac_shell_populate(AlmanacShell* shell, const almanac::AlmanacHostSurface* host) {
    if (!shell) return 0;
    AlmanacShell* s = shell;

    if (!host || !host_surface_can_render(*host)) {
        s->hostRenderable = 0;
        s->hostGeneration = 0;
        for (std::size_t i = 0; i < ALMANAC_PAGE_COUNT; ++i) {
            reset_snapshot(s->snapshots[i], static_cast<AlmanacPageId>(i), 0u, 0u);
            s->snapshots[i].populated = 0;
            s->snapshots[i].available = 0;
        }
        return 0;
    }

    const std::uint64_t newGen = host->lifecycle.generation;
    if (s->hostGeneration != 0u && s->hostGeneration != newGen) {
        for (std::size_t i = 0; i < ALMANAC_PAGE_COUNT; ++i) {
            reset_snapshot(s->snapshots[i], static_cast<AlmanacPageId>(i), newGen, host->frame.frame_number);
            s->snapshots[i].populated = 0;
            s->snapshots[i].available = 0;
        }
    }
    s->hostGeneration = newGen;
    s->hostRenderable = 1;
    s->layoutDpi = host->geometry.dpi;
    s->rowHeight = host->geometry.dpi * 35.0f;
    s->scrollOffsetY = clampf(host->scroll.offset_y, 0.0f, host->scroll.max_offset_y);

    const AlmanacPageId page = s->currentPage;
    AlmanacPageSnapshot& snap = s->snapshots[page];
    reset_snapshot(snap, page, newGen, host->frame.frame_number);

    int anyProvider = 0;
    int success = 0;
    for (std::size_t i = 0; i < kMaxProviders; ++i) {
        ProviderEntry& pe = s->providers[i];
        if (!pe.valid || pe.page != page || !pe.populate) continue;
        anyProvider = 1;
        AlmanacPageSnapshot temp{};
        reset_snapshot(temp, page, newGen, host->frame.frame_number);
        temp.populated = 1;
        if (pe.populate(host, &temp, pe.userData)) {
            if (temp.coherent && temp.rowCount <= ALMANAC_MAX_ROWS) {
                // reset_snapshot seeds a default empty state on both snapshots.
                // A provider-specific state may replace that default; later
                // providers cannot overwrite the first provider-specific state.
                if (temp.emptyState[0] != '\0') {
                    const char* defaultEmpty = almanac_page_default_empty_state(page);
                    const int destinationIsDefault =
                        std::strcmp(snap.emptyState, defaultEmpty) == 0;
                    const int providerIsDefault =
                        std::strcmp(temp.emptyState, defaultEmpty) == 0;
                    if (success == 0 || (destinationIsDefault && !providerIsDefault)) {
                        copy_text(snap.emptyState, ALMANAC_TEXT_LEN, temp.emptyState);
                    }
                }
                ++success;
                snap.available = snap.available || temp.available;
                snap.stale = snap.stale || temp.stale;
                for (std::size_t r = 0; r < temp.rowCount && snap.rowCount < ALMANAC_MAX_ROWS; ++r) {
                    snap.rows[snap.rowCount++] = temp.rows[r];
                }
            }
        }
    }

    if (!anyProvider) {
        snap.populated = 1;
        snap.available = 0;
    } else {
        snap.populated = (success > 0) ? 1 : 0;
        if (snap.rowCount == 0 && !snap.available) {
            // keep empty state
        }
    }
    if (snap.emptyState[0] == '\0') {
        copy_text(snap.emptyState, ALMANAC_TEXT_LEN, almanac_page_default_empty_state(page));
    }
    return 1;
}

extern "C" int almanac_shell_render(AlmanacShell* shell, const almanac::AlmanacHostSurface* host,
                                    AlmanacRenderFrame* outFrame) {
    if (!shell) return 0;
    AlmanacShell* s = shell;
    AlmanacRenderFrame frame{};
    frame.page = s->currentPage;

    if (!host || !host_surface_can_render(*host)) {
        s->hostRenderable = 0;
        s->hostGeneration = 0;
        for (std::size_t i = 0; i < ALMANAC_PAGE_COUNT; ++i) {
            reset_snapshot(s->snapshots[i], static_cast<AlmanacPageId>(i), 0u, 0u);
            s->snapshots[i].populated = 0;
        }
        s->lastFrame = frame;
        if (outFrame) *outFrame = frame;
        return 0;
    }

    const std::uint64_t newGen = host->lifecycle.generation;
    if (s->hostGeneration != 0u && s->hostGeneration != newGen) {
        for (std::size_t i = 0; i < ALMANAC_PAGE_COUNT; ++i) {
            reset_snapshot(s->snapshots[i], static_cast<AlmanacPageId>(i),
                           newGen, host->frame.frame_number);
            s->snapshots[i].populated = 0;
        }
        s->hostGeneration = newGen;
    }
    const AlmanacPageSnapshot& current = s->snapshots[s->currentPage];
    if (!current.populated || current.hostGeneration != newGen) {
        s->hostRenderable = 0;
        s->lastFrame = frame;
        if (outFrame) *outFrame = frame;
        return 0;
    }

    s->hostGeneration = newGen;
    s->hostRenderable = 1;
    s->layoutDpi = host->geometry.dpi;
    s->rowHeight = host->geometry.dpi * 35.0f;
    s->scrollOffsetY = clampf(host->scroll.offset_y, 0.0f, host->scroll.max_offset_y);

    frame.rendered = 1;
    frame.rowCount = s->snapshots[s->currentPage].rowCount;
    frame.dpi = host->geometry.dpi;
    frame.rowHeight = s->rowHeight;
    frame.originX = host->geometry.origin_x;
    frame.originY = host->geometry.origin_y;
    frame.extentWidth = host->geometry.extent_width;
    frame.extentHeight = host->geometry.extent_height;

    const int canClip = host_surface_can_clip(*host) && host->clip.enabled;
    if (canClip) {
        frame.clipped = 1;
        frame.clipX = host->clip.x;
        frame.clipY = host->clip.y;
        frame.clipWidth = host->clip.width;
        frame.clipHeight = host->clip.height;
    } else {
        frame.clipped = 0;
    }

    const int canScroll = host_surface_can_scroll(*host);
    if (canScroll && (host->scroll.can_scroll_x || host->scroll.can_scroll_y)) {
        frame.scrolled = 1;
        frame.scrollOffsetY = s->scrollOffsetY;
    } else {
        frame.scrolled = 0;
        frame.scrollOffsetY = 0.0f;
    }

    s->lastFrame = frame;
    if (outFrame) *outFrame = frame;
    return 1;
}

extern "C" int almanac_shell_handle_pointer(AlmanacShell* shell, const almanac::AlmanacHostSurface* host) {
    if (!shell) return 0;
    AlmanacShell* s = shell;
    s->lastHitRow = -1;
    if (!host || !host_surface_can_receive_pointer(*host)) return 0;

    // Clipping and hit-testing must agree.
    const float px = host->pointer.x;
    const float py = host->pointer.y;
    if (host->clip.enabled) {
        if (!host_surface_can_clip(*host)) return 0;
        if (px < host->clip.x || py < host->clip.y ||
            px >= host->clip.x + host->clip.width ||
            py >= host->clip.y + host->clip.height) {
            s->lastHitRow = -1;
            return 0;
        }
    }

    const AlmanacPageSnapshot& snap = s->snapshots[s->currentPage];
    if (!snap.populated || snap.hostGeneration != host->lifecycle.generation) {
        s->lastHitRow = -1;
        return 0;
    }

    const float ox = host->geometry.origin_x;
    const float oy = host->geometry.origin_y;
    const float ew = host->geometry.extent_width;
    const float eh = host->geometry.extent_height;
    const float visibleX = px - ox;
    const float visibleY = py - oy;
    if (visibleX < 0.0f || visibleY < 0.0f || visibleX >= ew || visibleY >= eh) {
        s->lastHitRow = -1;
        return 0;
    }

    // Panel bounds use the visible coordinate. Only content hit-testing adds
    // the current host's vertical scroll offset.
    s->layoutDpi = host->geometry.dpi;
    s->rowHeight = host->geometry.dpi * 35.0f;
    s->scrollOffsetY = (host->scroll.can_scroll_y && host_surface_can_scroll(*host))
        ? clampf(host->scroll.offset_y, 0.0f, host->scroll.max_offset_y)
        : 0.0f;
    const float contentY = visibleY + s->scrollOffsetY;
    const float rh = s->rowHeight > 0.0f ? s->rowHeight : 1.0f;
    const int row = static_cast<int>(contentY / rh);
    if (row < 0 || static_cast<std::size_t>(row) >= snap.rowCount) {
        s->lastHitRow = -1;
        return 0;
    }
    const AlmanacDisplayRow& hit = snap.rows[static_cast<std::size_t>(row)];
    if (!hit.interactive || !hit.enabled) {
        s->lastHitRow = -1;
        return 0;
    }
    s->lastHitRow = row;
    if (host->pointer.action == almanac::AlmanacPointerAction_Down && hit.actionId != 0u) {
        for (std::size_t i = 0; i < kMaxProviders; ++i) {
            ProviderEntry& pe = s->providers[i];
            if (!pe.valid || pe.page != s->currentPage || !pe.action) continue;
            pe.action(s->currentPage, hit.actionId, pe.userData);
            break; // one action provider per page; never duplicate a command
        }
    }
    return 1;
}

extern "C" const AlmanacPageSnapshot* almanac_shell_page_snapshot(const AlmanacShell* shell,
                                                                 AlmanacPageId page) {
    if (!shell || page < 0 || page >= ALMANAC_PAGE_COUNT) return nullptr;
    const AlmanacShell* s = shell;
    return &s->snapshots[page];
}

extern "C" int almanac_shell_host_renderable(const AlmanacShell* shell) {
    if (!shell) return 0;
    return shell->hostRenderable;
}

extern "C" float almanac_shell_layout_dpi(const AlmanacShell* shell) {
    if (!shell) return 0.0f;
    return shell->layoutDpi;
}

extern "C" float almanac_shell_row_height(const AlmanacShell* shell) {
    if (!shell) return 0.0f;
    return shell->rowHeight;
}

extern "C" float almanac_shell_scroll_offset_y(const AlmanacShell* shell) {
    if (!shell) return 0.0f;
    return shell->scrollOffsetY;
}

extern "C" int almanac_shell_last_hit_row(const AlmanacShell* shell) {
    if (!shell) return -1;
    return shell->lastHitRow;
}

extern "C" std::size_t almanac_aggregate_money(const AlmanacMoney* values, std::size_t count,
                                               AlmanacAggregateBucket* out, std::size_t capacity) {
    if (!values || !out || capacity == 0) return 0;
    std::size_t used = 0;
    bool truncated = false;
    for (std::size_t i = 0; i < count; ++i) {
        const AlmanacMoney& v = values[i];
        if (!v.available) continue;
        if (v.valueClass < ALMANAC_VALUE_ACTUAL ||
            v.valueClass > ALMANAC_VALUE_ESTIMATED ||
            !std::isfinite(v.amount)) {
            truncated = true;
            continue;
        }
        // find existing bucket with identical class/currency/unit
        std::size_t idx = used;
        int found = 0;
        for (std::size_t b = 0; b < used; ++b) {
            if (out[b].valueClass == v.valueClass && out[b].currency == v.currency &&
                std::strncmp(out[b].unit, v.unit, ALMANAC_UNIT_LEN) == 0) {
                idx = b;
                found = 1;
                break;
            }
        }
        if (!found) {
            if (used >= capacity) {
                truncated = true;
                continue;
            }
            idx = used++;
            out[idx].valueClass = v.valueClass;
            out[idx].currency = v.currency;
            copy_text(out[idx].unit, ALMANAC_UNIT_LEN, v.unit);
            out[idx].total = 0.0;
            out[idx].count = 0;
            out[idx].truncated = 0;
        }
        out[idx].total += v.amount;
        ++out[idx].count;
        if (!std::isfinite(out[idx].total)) {
            out[idx].total = 0.0;
            out[idx].truncated = 1;
            truncated = true;
        }
    }
    if (truncated) {
        for (std::size_t i = 0; i < used; ++i) out[i].truncated = 1;
    }
    return used;
}
