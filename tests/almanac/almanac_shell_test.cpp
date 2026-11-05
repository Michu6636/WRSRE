#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>

#include "almanac_ui_api.h"
#include "almanac_shell.h"
#include "almanac_pages.h"
#include "host_contract.h"
#include "host_validation.h"

using namespace almanac;

static AlmanacHostSurface make_host(std::uint64_t frameNumber = 1u,
                                    std::uint64_t generation = 1u,
                                    float dpi = 2.0f) {
    AlmanacHostSurface s{};
    s.capabilities = AlmanacHostCapability_Visibility |
                     AlmanacHostCapability_SelectionValidation |
                     AlmanacHostCapability_Geometry |
                     AlmanacHostCapability_Dpi |
                     AlmanacHostCapability_Clipping |
                     AlmanacHostCapability_Scrolling |
                     AlmanacHostCapability_PointerInput |
                     AlmanacHostCapability_FrameIdentity |
                     AlmanacHostCapability_LifecycleInvalidation |
                     AlmanacHostCapability_FlowLayout;
    s.visible = true;
    s.selected = true;
    s.geometry.dpi = dpi;
    s.geometry.origin_x = 10.0f;
    s.geometry.origin_y = 20.0f;
    s.geometry.extent_width = 100.0f;
    s.geometry.extent_height = 200.0f;
    s.clip.enabled = true;
    s.clip.x = 10.0f;
    s.clip.y = 20.0f;
    s.clip.width = 100.0f;
    s.clip.height = 200.0f;
    s.scroll.can_scroll_y = true;
    s.scroll.max_offset_y = 50.0f;
    s.scroll.offset_y = 5.0f;
    s.pointer.action = AlmanacPointerAction_Down;
    s.pointer.x = 15.0f;
    s.pointer.y = 30.0f;
    s.pointer.in_bounds = true;
    s.pointer.frame_id = frameNumber;
    s.frame.frame_number = frameNumber;
    s.frame.generation = generation;
    s.lifecycle.generation = generation;
    s.lifecycle.invalidated = false;
    return s;
}

static int populate_cb(const AlmanacHostSurface* host, AlmanacPageSnapshot* out, void* userData) {
    (void)host;
    int* counter = static_cast<int*>(userData);
    if (counter) ++(*counter);
    if (!out) return 0;
    out->populated = 1;
    out->available = 1;
    out->coherent = 1;
    out->rowCount = 2;
    std::snprintf(out->rows[0].label, ALMANAC_TEXT_LEN, "Row A");
    std::snprintf(out->rows[0].value, ALMANAC_TEXT_LEN, "10");
    out->rows[0].currency = 0;
    out->rows[0].valueClass = ALMANAC_VALUE_ACTUAL;
    out->rows[0].available = 1;
    std::snprintf(out->rows[1].label, ALMANAC_TEXT_LEN, "Row B");
    std::snprintf(out->rows[1].value, ALMANAC_TEXT_LEN, "20");
    out->rows[1].currency = 1;
    out->rows[1].valueClass = ALMANAC_VALUE_DRY_RUN;
    out->rows[1].available = 1;
    return 1;
}

int main() {
    // default page
    {
        AlmanacShell* sh = almanac_shell_create();
        assert(sh);
        assert(almanac_shell_current_page(sh) == ALMANAC_PAGE_OVERVIEW);
        almanac_shell_destroy(sh);
    }

    // stable page IDs count
    {
        assert(ALMANAC_PAGE_COUNT == 7);
        assert(ALMANAC_PAGE_OVERVIEW == 0);
        assert(ALMANAC_PAGE_CAUSATION == 6);
    }

    // navigation and back
    {
        AlmanacShell* sh = almanac_shell_create();
        assert(almanac_shell_navigate(sh, ALMANAC_PAGE_FISCAL_POLICY));
        assert(almanac_shell_current_page(sh) == ALMANAC_PAGE_FISCAL_POLICY);
        assert(almanac_shell_go_back(sh));
        assert(almanac_shell_current_page(sh) == ALMANAC_PAGE_OVERVIEW);
        // back at root stays overview
        assert(almanac_shell_go_back(sh));
        assert(almanac_shell_current_page(sh) == ALMANAC_PAGE_OVERVIEW);
        almanac_shell_destroy(sh);
    }

    // invalid page IDs
    {
        AlmanacShell* sh = almanac_shell_create();
        assert(!almanac_shell_navigate(sh, ALMANAC_PAGE_INVALID));
        assert(!almanac_shell_navigate(sh, (AlmanacPageId)99));
        assert(almanac_shell_current_page(sh) == ALMANAC_PAGE_OVERVIEW);
        almanac_shell_destroy(sh);
    }

    // provider capacity limits
    {
        AlmanacShell* sh = almanac_shell_create();
        int dummy = 0;
        int handles[ALMANAC_MAX_PROVIDERS + 2];
        for (std::size_t i = 0; i < ALMANAC_MAX_PROVIDERS; ++i) {
            handles[i] = almanac_shell_register_provider(sh, ALMANAC_PAGE_OVERVIEW, populate_cb, &dummy);
            assert(handles[i] >= 1);
        }
        assert(almanac_shell_provider_count(sh) == ALMANAC_MAX_PROVIDERS);
        int extra = almanac_shell_register_provider(sh, ALMANAC_PAGE_OVERVIEW, populate_cb, &dummy);
        assert(extra == 0);
        almanac_shell_destroy(sh);
    }

    // provider registration/removal and callback lifetime
    {
        AlmanacShell* sh = almanac_shell_create();
        int counter = 0;
        int h = almanac_shell_register_provider(sh, ALMANAC_PAGE_OVERVIEW, populate_cb, &counter);
        assert(h >= 1);
        AlmanacHostSurface host = make_host();
        assert(almanac_shell_populate(sh, &host));
        const AlmanacPageSnapshot* snap = almanac_shell_page_snapshot(sh, ALMANAC_PAGE_OVERVIEW);
        assert(snap && snap->rowCount == 2);
        assert(counter == 1);
        // unregister then populate should not call
        almanac_shell_unregister_provider(sh, h);
        assert(!almanac_shell_provider_is_registered(sh, h));
        int before = counter;
        assert(almanac_shell_populate(sh, &host));
        assert(counter == before);
        almanac_shell_destroy(sh);
    }

    // empty/unavailable pages without fake values
    {
        AlmanacShell* sh = almanac_shell_create();
        AlmanacHostSurface host = make_host();
        assert(almanac_shell_populate(sh, &host));
        const AlmanacPageSnapshot* snap = almanac_shell_page_snapshot(sh, ALMANAC_PAGE_MONEY_FLOW);
        assert(snap);
        assert(snap->rowCount == 0);
        assert(!snap->available);
        assert(snap->emptyState[0] != '\0');
        almanac_shell_destroy(sh);
    }

    // host capabilities: render requires all
    {
        AlmanacShell* sh = almanac_shell_create();
        AlmanacHostSurface host = make_host();
        // remove geometry capability
        host.capabilities &= ~AlmanacHostCapability_Geometry;
        assert(!host_surface_can_render(host));
        assert(!almanac_shell_populate(sh, &host));
        assert(!almanac_shell_render(sh, &host, nullptr));
        assert(!almanac_shell_host_renderable(sh));
        almanac_shell_destroy(sh);
    }

    // enabled but invalid/unsupported clipping prevents render
    {
        AlmanacHostSurface host = make_host();
        host.clip.enabled = true;
        host.clip.width = 0.0f; // invalid
        assert(!host_surface_can_clip(host));
        assert(!host_surface_can_render(host));
    }

    // enabled but unsupported scrolling: if scroll flags set but no cap, render fails
    {
        AlmanacHostSurface host = make_host();
        host.capabilities &= ~AlmanacHostCapability_Scrolling;
        host.scroll.can_scroll_y = true;
        assert(!host_surface_can_scroll(host));
        assert(!host_surface_can_render(host));
    }

    // scroll bounds
    {
        AlmanacHostSurface host = make_host();
        host.scroll.offset_y = 1000.0f; // out of bounds
        assert(!host_scroll_state_is_valid(host.scroll));
        // renderable check fails due scroll state
        assert(!host_surface_can_render(host));
    }

    // DPI changes rescale
    {
        AlmanacShell* sh = almanac_shell_create();
        AlmanacHostSurface host1 = make_host(1, 1, 1.0f);
        almanac_shell_render(sh, &host1, nullptr);
        assert(almanac_shell_row_height(sh) == 35.0f);
        AlmanacHostSurface host2 = make_host(2, 2, 2.0f);
        almanac_shell_render(sh, &host2, nullptr);
        assert(almanac_shell_row_height(sh) == 70.0f);
        almanac_shell_destroy(sh);
    }

    // clipping and hit-testing agreement
    {
        AlmanacShell* sh = almanac_shell_create();
        int counter = 0;
        almanac_shell_register_provider(sh, ALMANAC_PAGE_OVERVIEW, populate_cb, &counter);
        AlmanacHostSurface host = make_host();
        almanac_shell_populate(sh, &host);
        // pointer inside clip
        host.pointer.x = 15.0f; host.pointer.y = 30.0f; host.pointer.in_bounds = true;
        assert(almanac_shell_handle_pointer(sh, &host));
        // pointer outside clip
        host.pointer.x = 200.0f; host.pointer.y = 30.0f;
        assert(!almanac_shell_handle_pointer(sh, &host));
        almanac_shell_destroy(sh);
    }

    // rejection of clipped/out-of-bounds/empty/hidden/stale-frame input
    {
        AlmanacShell* sh = almanac_shell_create();
        int counter = 0;
        almanac_shell_register_provider(sh, ALMANAC_PAGE_OVERVIEW, populate_cb, &counter);
        AlmanacHostSurface host = make_host();
        almanac_shell_populate(sh, &host);
        // empty action
        AlmanacHostSurface h2 = host;
        h2.pointer.action = AlmanacPointerAction_None;
        assert(!almanac_shell_handle_pointer(sh, &h2));
        // hidden
        AlmanacHostSurface h3 = host;
        h3.visible = false;
        assert(!host_surface_can_render(h3));
        // stale frame
        AlmanacHostSurface h4 = host;
        h4.pointer.frame_id = host.frame.frame_number + 1;
        assert(!host_surface_can_receive_pointer(h4));
        assert(!almanac_shell_handle_pointer(sh, &h4));
        // out-of-bounds pointer
        AlmanacHostSurface h5 = host;
        h5.pointer.x = -5.0f;
        h5.pointer.y = 30.0f;
        assert(!almanac_shell_handle_pointer(sh, &h5));
        almanac_shell_destroy(sh);
    }

    // host disappearance clears state
    {
        AlmanacShell* sh = almanac_shell_create();
        int counter = 0;
        almanac_shell_register_provider(sh, ALMANAC_PAGE_OVERVIEW, populate_cb, &counter);
        AlmanacHostSurface host = make_host();
        almanac_shell_populate(sh, &host);
        assert(almanac_shell_host_renderable(sh));
        // host disappears
        AlmanacHostSurface gone{};
        assert(!almanac_shell_populate(sh, &gone));
        assert(!almanac_shell_host_renderable(sh));
        const AlmanacPageSnapshot* snap = almanac_shell_page_snapshot(sh, ALMANAC_PAGE_OVERVIEW);
        assert(snap && snap->rowCount == 0);
        almanac_shell_destroy(sh);
    }

    // generation invalidation
    {
        AlmanacShell* sh = almanac_shell_create();
        int counter = 0;
        almanac_shell_register_provider(sh, ALMANAC_PAGE_OVERVIEW, populate_cb, &counter);
        AlmanacHostSurface host1 = make_host(1, 1, 2.0f);
        almanac_shell_populate(sh, &host1);
        AlmanacHostSurface host2 = make_host(2, 2, 2.0f); // new generation
        assert(almanac_shell_populate(sh, &host2));
        // snapshot generation updated
        const AlmanacPageSnapshot* snap = almanac_shell_page_snapshot(sh, ALMANAC_PAGE_OVERVIEW);
        assert(snap && snap->hostGeneration == 2u);
        almanac_shell_destroy(sh);
    }

    // coherent bounded snapshots: row count capped
    {
        // provider returns too many rows -> rejected
        auto bad_cb = [](const AlmanacHostSurface*, AlmanacPageSnapshot* out, void*) -> int {
            if (!out) return 0;
            out->populated = 1;
            out->available = 1;
            out->coherent = 1;
            out->rowCount = ALMANAC_MAX_ROWS + 5;
            return 1;
        };
        AlmanacShell* sh = almanac_shell_create();
        almanac_shell_register_provider(sh, ALMANAC_PAGE_OVERVIEW, bad_cb, nullptr);
        AlmanacHostSurface host = make_host();
        almanac_shell_populate(sh, &host);
        const AlmanacPageSnapshot* snap = almanac_shell_page_snapshot(sh, ALMANAC_PAGE_OVERVIEW);
        assert(snap && snap->rowCount == 0);
        almanac_shell_destroy(sh);
    }

    // ACTUAL/DRY_RUN/ESTIMATED and currency/unit separation
    {
        AlmanacMoney vals[4]{};
        vals[0].amount = 10; vals[0].currency = 0; std::strcpy(vals[0].unit, "RUB"); vals[0].valueClass = ALMANAC_VALUE_ACTUAL; vals[0].available = 1;
        vals[1].amount = 20; vals[1].currency = 0; std::strcpy(vals[1].unit, "RUB"); vals[1].valueClass = ALMANAC_VALUE_ACTUAL; vals[1].available = 1;
        vals[2].amount = 5; vals[2].currency = 0; std::strcpy(vals[2].unit, "RUB"); vals[2].valueClass = ALMANAC_VALUE_DRY_RUN; vals[2].available = 1;
        vals[3].amount = 7; vals[3].currency = 1; std::strcpy(vals[3].unit, "USD"); vals[3].valueClass = ALMANAC_VALUE_ACTUAL; vals[3].available = 1;
        AlmanacAggregateBucket buckets[8];
        std::size_t n = almanac_aggregate_money(vals, 4, buckets, 8);
        // 3 distinct buckets: ACTUAL RUB, DRY_RUN RUB, ACTUAL USD
        assert(n == 3);
        // find ACTUAL RUB total 30
        int found = 0;
        for (std::size_t i = 0; i < n; ++i) {
            if (buckets[i].valueClass == ALMANAC_VALUE_ACTUAL && buckets[i].currency == 0) {
                assert(buckets[i].total == 30.0);
                ++found;
            }
        }
        assert(found == 1);
    }

    // value class names
    {
        assert(std::strcmp(almanac_value_class_name(ALMANAC_VALUE_ACTUAL), "ACTUAL") == 0);
        assert(std::strcmp(almanac_value_class_name(ALMANAC_VALUE_DRY_RUN), "DRY_RUN") == 0);
        assert(std::strcmp(almanac_value_class_name(ALMANAC_VALUE_ESTIMATED), "ESTIMATED") == 0);
    }

    // page titles
    {
        assert(std::strcmp(almanac_page_title(ALMANAC_PAGE_OVERVIEW), "Overview") == 0);
        assert(std::strcmp(almanac_page_title(ALMANAC_PAGE_CAUSATION), "Causation") == 0);
    }

    std::printf("ALL TESTS PASSED\n");
    return 0;
}
