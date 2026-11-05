#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>

#include "host_contract.h"
#include "host_validation.h"

using namespace almanac;

struct SyntheticEntry {
    unsigned char active;
    char name[103];
};
static_assert(sizeof(SyntheticEntry) == 0x68, "stride");

static SyntheticEntry make_entry(const char* name, unsigned char active = 1) {
    SyntheticEntry e{};
    e.active = active;
    std::snprintf(e.name, sizeof(e.name), "%s", name);
    return e;
}

int main() {
    // checked arithmetic
    {
        std::size_t out = 0;
        assert(checked_add_size(10, 20, &out) && out == 30);
        assert(!checked_add_size(std::numeric_limits<std::size_t>::max(), 1, &out));
        assert(checked_mul_size(6, 7, &out) && out == 42);
        assert(!checked_mul_size(std::numeric_limits<std::size_t>::max(), 2, &out));
        assert(!checked_add_size(1, 2, nullptr));
    }

    // null/empty vector
    {
        AlmanacNativePanelVector v{};
        assert(!native_panel_vector_is_valid(v));
        assert(validate_native_panel_vector(v).status == AlmanacVectorValidation_NullBegin);

        AlmanacNativePanelVector v2{};
        SyntheticEntry e = make_entry("importexport");
        v2.begin = &e;
        v2.end = &e;
        v2.stride = sizeof(SyntheticEntry);
        v2.max_entries = 8;
        assert(native_panel_vector_is_valid(v2));
        assert(validate_native_panel_vector(v2).count == 0u);
    }

    // reversed bounds
    {
        SyntheticEntry a = make_entry("importexport");
        SyntheticEntry b = make_entry("trade");
        AlmanacNativePanelVector v{};
        v.begin = &b;
        v.end = &a;
        v.stride = sizeof(SyntheticEntry);
        v.max_entries = 8;
        assert(!native_panel_vector_is_valid(v));
        assert(validate_native_panel_vector(v).status == AlmanacVectorValidation_Reversed);
    }

    // overflowing bounds (too many)
    {
        SyntheticEntry entries[4] = { make_entry("importexport"), make_entry("trade"),
                                     make_entry("loanusd"), make_entry("loanrub") };
        AlmanacNativePanelVector v{};
        v.begin = entries;
        v.end = entries + 4;
        v.stride = sizeof(SyntheticEntry);
        v.max_entries = 2;
        assert(!native_panel_vector_is_valid(v));
        assert(validate_native_panel_vector(v).status == AlmanacVectorValidation_TooMany);
    }

    // misaligned selection
    {
        SyntheticEntry entries[2] = { make_entry("importexport"), make_entry("trade") };
        AlmanacNativePanelVector v{};
        v.begin = entries;
        v.end = entries + 2;
        v.stride = sizeof(SyntheticEntry);
        v.max_entries = 8;
        AlmanacNativeSelection s{};
        s.selected = reinterpret_cast<const unsigned char*>(entries) + 1;
        assert(!native_selection_is_member(v, s));
        assert(validate_native_selection(v, s).status == AlmanacSelectionValidation_Misaligned);
    }

    // selection outside vector
    {
        SyntheticEntry entries[2] = { make_entry("importexport"), make_entry("trade") };
        SyntheticEntry outside = make_entry("loanusd");
        AlmanacNativePanelVector v{};
        v.begin = entries;
        v.end = entries + 2;
        v.stride = sizeof(SyntheticEntry);
        v.max_entries = 8;
        AlmanacNativeSelection s{};
        s.selected = &outside;
        assert(!native_selection_is_member(v, s));
        assert(validate_native_selection(v, s).status == AlmanacSelectionValidation_OutOfRange);
    }

    // unknown name
    {
        SyntheticEntry entries[1] = { make_entry("notaknownpanel") };
        AlmanacNativePanelVector v{};
        v.begin = entries;
        v.end = entries + 1;
        v.stride = sizeof(SyntheticEntry);
        v.max_entries = 8;
        AlmanacNativeSelection s{};
        s.selected = entries;
        assert(!native_selection_is_member(v, s));
        assert(validate_native_selection(v, s).status == AlmanacSelectionValidation_UnknownName);
    }

    // unterminated name
    {
        SyntheticEntry e{};
        e.active = 1;
        for (std::size_t i = 0; i < sizeof(e.name); ++i) e.name[i] = 'a';
        // no NUL terminator within capacity
        AlmanacNativePanelVector v{};
        v.begin = &e;
        v.end = &e + 1;
        v.stride = sizeof(SyntheticEntry);
        v.max_entries = 8;
        AlmanacNativeSelection s{};
        s.selected = &e;
        assert(!native_selection_is_member(v, s));
        assert(validate_native_selection(v, s).status == AlmanacSelectionValidation_UnterminatedName);
    }

    // valid membership
    {
        SyntheticEntry entries[3] = { make_entry("importexport"), make_entry("trade"), make_entry("loanrub") };
        AlmanacNativePanelVector v{};
        v.begin = entries;
        v.end = entries + 3;
        v.stride = sizeof(SyntheticEntry);
        v.max_entries = 8;
        AlmanacNativeSelection s{};
        s.selected = entries + 1;
        AlmanacSelectionValidation r = validate_native_selection(v, s);
        assert(r.ok);
        assert(r.index == 1u);
        assert(native_selection_is_member(v, s));
    }

    // bounded name handling
    {
        char buf[8] = "trade";
        std::size_t len = 0;
        assert(validate_bounded_name(buf, 8, &len));
        assert(len == 5u);
        assert(name_is_known_host_panel(buf, 8));

        char bad[4] = { 'a', 'b', 'c', 'd' }; // no NUL
        assert(!validate_bounded_name(bad, 4, &len));
        assert(!name_is_known_host_panel(bad, 4));

        assert(!validate_bounded_name(nullptr, 4, &len));
        assert(!validate_bounded_name(buf, 0, &len));
    }

    // closed host
    {
        AlmanacHostVisibility vis{};
        vis.sidebar_open = false;
        unsigned caps = AlmanacHostCapability_Visibility | AlmanacHostCapability_SelectionValidation;
        assert(!host_is_open(vis, caps));

        SyntheticEntry entries[1] = { make_entry("importexport") };
        AlmanacNativePanelVector v{};
        v.begin = entries;
        v.end = entries + 1;
        v.stride = sizeof(SyntheticEntry);
        v.max_entries = 8;
        AlmanacNativeSelection s{};
        s.selected = entries;
        AlmanacSelectionValidation r = validate_host_selection(vis, v, s, caps);
        assert(!r.ok);
        assert(r.status == AlmanacSelectionValidation_ClosedHost);
    }

    // missing capability disables
    {
        AlmanacHostSurface surface{};
        surface.capabilities = 0u;
        surface.visible = true;
        surface.selected = true;
        surface.geometry.dpi = 1.0f;
        surface.geometry.origin_x = 0.0f;
        surface.geometry.origin_y = 0.0f;
        surface.geometry.extent_width = 10.0f;
        surface.geometry.extent_height = 10.0f;
        surface.frame.generation = 1u;
        surface.lifecycle.generation = 1u;
        assert(!host_surface_can_render(surface));
        assert(!host_surface_can_clip(surface));
        assert(!host_surface_can_scroll(surface));
        assert(!host_surface_can_receive_pointer(surface));
    }

    // lifecycle invalidation
    {
        AlmanacLifecycleState state{};
        state.generation = 1u;
        state.invalidated = false;
        AlmanacFrameIdentity current{};
        current.frame_number = 10u;
        current.generation = 2u;
        assert(lifecycle_invalidated(state, current));
        assert(generation_is_stale(1u, 2u));

        state.invalidated = true;
        state.generation = 2u;
        assert(lifecycle_invalidated(state, current));
        assert(AlmanacLifecycleInvalidated(&state, &current));
    }

    // stale selection surface
    {
        AlmanacHostSurface surface{};
        surface.capabilities = AlmanacHostCapability_Visibility |
                              AlmanacHostCapability_SelectionValidation |
                              AlmanacHostCapability_Geometry |
                              AlmanacHostCapability_Dpi |
                              AlmanacHostCapability_FlowLayout |
                              AlmanacHostCapability_FrameIdentity |
                              AlmanacHostCapability_LifecycleInvalidation;
        surface.visible = true;
        surface.selected = true;
        surface.geometry.dpi = 1.0f;
        surface.geometry.origin_x = 0.0f;
        surface.geometry.origin_y = 0.0f;
        surface.geometry.extent_width = 10.0f;
        surface.geometry.extent_height = 10.0f;
        surface.frame.frame_number = 5u;
        surface.frame.generation = 2u;
        surface.lifecycle.generation = 1u;
        surface.lifecycle.invalidated = false;
        assert(!host_surface_can_render(surface));
    }

    // valid surface render
    {
        AlmanacHostSurface surface{};
        surface.capabilities = AlmanacHostCapability_Visibility |
                              AlmanacHostCapability_SelectionValidation |
                              AlmanacHostCapability_Geometry |
                              AlmanacHostCapability_Dpi |
                              AlmanacHostCapability_FlowLayout |
                              AlmanacHostCapability_FrameIdentity |
                              AlmanacHostCapability_LifecycleInvalidation;
        surface.visible = true;
        surface.selected = true;
        surface.geometry.dpi = 1.0f;
        surface.geometry.origin_x = 0.0f;
        surface.geometry.origin_y = 0.0f;
        surface.geometry.extent_width = 10.0f;
        surface.geometry.extent_height = 10.0f;
        surface.frame.frame_number = 5u;
        surface.frame.generation = 2u;
        surface.lifecycle.generation = 2u;
        surface.lifecycle.invalidated = false;
        assert(host_surface_is_valid(surface));
        assert(host_surface_can_render(surface));

        AlmanacHostSurface missing = surface;
        missing.capabilities &= ~AlmanacHostCapability_Visibility;
        assert(!host_surface_can_render(missing));

        surface.capabilities |= AlmanacHostCapability_PointerInput;
        surface.pointer.action = AlmanacPointerAction_Down;
        surface.pointer.in_bounds = true;
        surface.pointer.frame_id = 4u;
        assert(!host_surface_can_receive_pointer(surface));
        surface.pointer.frame_id = 5u;
        assert(host_surface_can_receive_pointer(surface));
        surface.visible = false;
        assert(!host_surface_can_receive_pointer(surface));
    }

    // null global guards
    {
        assert(!AlmanacNativePanelVectorIsValid(nullptr));
        assert(!AlmanacHostSurfaceCanRender(nullptr));
        assert(AlmanacLifecycleInvalidated(nullptr, nullptr));
    }

    std::printf("ALL TESTS PASSED\n");
    return 0;
}
