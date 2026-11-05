#include "host_validation.h"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>

namespace almanac {

namespace {

const char* kKnownHostPanels[] = {
    "importexport", "trade", "loanusd", "loanrub",
    "productionconsumption", "warehouse", "various",
    "population", "resources", "crime", "tourism",
    "airplanes", "ships", "vehicles", "list",
    "storage", "conveyor"
};

constexpr std::size_t kKnownHostPanelCount =
    sizeof(kKnownHostPanels) / sizeof(kKnownHostPanels[0]);

bool finite(float v) { return std::isfinite(v); }

} // namespace

bool checked_add_size(std::size_t a, std::size_t b, std::size_t* out) {
    if (out == nullptr) return false;
    if (a > std::numeric_limits<std::size_t>::max() - b) return false;
    *out = a + b;
    return true;
}

bool checked_mul_size(std::size_t a, std::size_t b, std::size_t* out) {
    if (out == nullptr) return false;
    if (a != 0u && b > std::numeric_limits<std::size_t>::max() / a) return false;
    *out = a * b;
    return true;
}

AlmanacVectorValidation validate_native_panel_vector(const AlmanacNativePanelVector& vector) {
    AlmanacVectorValidation result{};
    if (vector.begin == nullptr) {
        result.status = AlmanacVectorValidation_NullBegin;
        return result;
    }
    if (vector.end == nullptr) {
        result.status = AlmanacVectorValidation_NullEnd;
        return result;
    }
    const std::uintptr_t begin = reinterpret_cast<std::uintptr_t>(vector.begin);
    const std::uintptr_t end = reinterpret_cast<std::uintptr_t>(vector.end);
    if (end < begin) {
        result.status = AlmanacVectorValidation_Reversed;
        return result;
    }
    if (vector.stride == 0u) {
        result.status = AlmanacVectorValidation_ZeroStride;
        return result;
    }
    const std::size_t bytes = static_cast<std::size_t>(end - begin);
    if (bytes % vector.stride != 0u) {
        result.status = AlmanacVectorValidation_NotMultiple;
        return result;
    }
    const std::size_t count = bytes / vector.stride;
    std::size_t max_entries = vector.max_entries;
    if (max_entries == 0u) max_entries = 1u << 20;
    if (max_entries > std::numeric_limits<std::size_t>::max() / vector.stride) {
        result.status = AlmanacVectorValidation_Overflow;
        return result;
    }
    if (count > max_entries) {
        result.status = AlmanacVectorValidation_TooMany;
        return result;
    }
    result.ok = true;
    result.status = AlmanacVectorValidation_Ok;
    result.count = count;
    return result;
}

bool native_panel_vector_is_valid(const AlmanacNativePanelVector& vector) {
    return validate_native_panel_vector(vector).ok;
}

bool validate_bounded_name(const char* name, std::size_t capacity, std::size_t* out_length) {
    if (name == nullptr || capacity == 0u) return false;
    for (std::size_t i = 0u; i < capacity; ++i) {
        if (name[i] == '\0') {
            if (out_length != nullptr) *out_length = i;
            return true;
        }
    }
    return false;
}

bool name_is_known_host_panel(const char* name, std::size_t capacity) {
    std::size_t length = 0u;
    if (!validate_bounded_name(name, capacity, &length)) return false;
    if (length == 0u) return false;
    for (std::size_t i = 0u; i < kKnownHostPanelCount; ++i) {
        if (std::strcmp(name, kKnownHostPanels[i]) == 0) return true;
    }
    return false;
}

AlmanacSelectionValidation validate_native_selection(const AlmanacNativePanelVector& vector,
                                                   const AlmanacNativeSelection& selection) {
    AlmanacSelectionValidation result{};
    const AlmanacVectorValidation vector_result = validate_native_panel_vector(vector);
    if (!vector_result.ok) {
        result.status = AlmanacSelectionValidation_VectorInvalid;
        return result;
    }
    if (selection.selected == nullptr) {
        result.status = AlmanacSelectionValidation_NullSelection;
        return result;
    }
    const std::uintptr_t begin = reinterpret_cast<std::uintptr_t>(vector.begin);
    const std::uintptr_t end = reinterpret_cast<std::uintptr_t>(vector.end);
    const std::uintptr_t selected = reinterpret_cast<std::uintptr_t>(selection.selected);
    if (selected < begin || selected >= end) {
        result.status = AlmanacSelectionValidation_OutOfRange;
        return result;
    }
    const std::size_t offset = static_cast<std::size_t>(selected - begin);
    if (offset % vector.stride != 0u) {
        result.status = AlmanacSelectionValidation_Misaligned;
        return result;
    }
    const std::size_t capacity = vector.stride > 1u ? vector.stride - 1u : 0u;
    const char* name = static_cast<const char*>(selection.selected) + 1;
    std::size_t length = 0u;
    if (!validate_bounded_name(name, capacity, &length)) {
        result.status = AlmanacSelectionValidation_UnterminatedName;
        return result;
    }
    if (!name_is_known_host_panel(name, capacity)) {
        result.status = AlmanacSelectionValidation_UnknownName;
        return result;
    }
    result.ok = true;
    result.status = AlmanacSelectionValidation_Ok;
    result.index = offset / vector.stride;
    return result;
}

bool native_selection_is_member(const AlmanacNativePanelVector& vector,
                                const AlmanacNativeSelection& selection) {
    return validate_native_selection(vector, selection).ok;
}

bool host_has_capability(unsigned capabilities, AlmanacHostCapability capability) {
    const unsigned bit = static_cast<unsigned>(capability);
    if (bit == 0u) return false;
    return (capabilities & bit) == bit;
}

bool host_geometry_is_valid(const AlmanacHostGeometry& geometry) {
    if (!finite(geometry.dpi) || geometry.dpi <= 0.0f) return false;
    if (!finite(geometry.origin_x) || !finite(geometry.origin_y)) return false;
    if (!finite(geometry.extent_width) || !finite(geometry.extent_height)) return false;
    if (geometry.extent_width <= 0.0f || geometry.extent_height <= 0.0f) return false;
    return true;
}

bool host_clip_rect_is_valid(const AlmanacHostClipRect& clip) {
    if (!clip.enabled) return true;
    if (!finite(clip.x) || !finite(clip.y)) return false;
    if (!finite(clip.width) || !finite(clip.height)) return false;
    if (clip.width <= 0.0f || clip.height <= 0.0f) return false;
    return true;
}

bool host_scroll_state_is_valid(const AlmanacHostScrollState& scroll) {
    if (!finite(scroll.offset_x) || !finite(scroll.offset_y)) return false;
    if (!finite(scroll.max_offset_x) || !finite(scroll.max_offset_y)) return false;
    if (scroll.max_offset_x < 0.0f || scroll.max_offset_y < 0.0f) return false;
    if (scroll.offset_x < 0.0f || scroll.offset_y < 0.0f) return false;
    if (scroll.offset_x > scroll.max_offset_x + 1e-6f) return false;
    if (scroll.offset_y > scroll.max_offset_y + 1e-6f) return false;
    return true;
}

bool host_pointer_event_is_valid(const AlmanacPointerEvent& event) {
    if (event.action < AlmanacPointerAction_None || event.action > AlmanacPointerAction_Wheel) return false;
    if (!finite(event.x) || !finite(event.y) || !finite(event.wheel_delta)) return false;
    return true;
}

bool host_frame_identity_is_valid(const AlmanacFrameIdentity& identity) {
    return identity.generation != 0u;
}

bool host_lifecycle_is_valid(const AlmanacLifecycleState& state) {
    return state.generation != 0u;
}

bool host_visibility_is_valid(const AlmanacHostVisibility& visibility) {
    if (visibility.almanac_selected && !visibility.trade_family_selected) return false;
    if (visibility.trade_family_selected && !visibility.sidebar_open) return false;
    if (visibility.almanac_selected && !visibility.sidebar_open) return false;
    return true;
}

bool host_is_open(const AlmanacHostVisibility& visibility, unsigned capabilities) {
    if (!host_has_capability(capabilities, AlmanacHostCapability_Visibility)) return false;
    if (!host_visibility_is_valid(visibility)) return false;
    return visibility.sidebar_open;
}

bool frame_identity_equal(const AlmanacFrameIdentity& a, const AlmanacFrameIdentity& b) {
    return a.frame_number == b.frame_number && a.generation == b.generation;
}

bool generation_is_stale(std::uint64_t cached_generation, std::uint64_t current_generation) {
    return cached_generation != current_generation;
}

bool lifecycle_invalidated(const AlmanacLifecycleState& state, const AlmanacFrameIdentity& current) {
    if (state.invalidated) return true;
    if (state.generation != current.generation) return true;
    if (state.invalidation_frame != 0u && state.invalidation_frame <= current.frame_number) return true;
    return false;
}

AlmanacSelectionValidation validate_host_selection(const AlmanacHostVisibility& visibility,
                                                  const AlmanacNativePanelVector& vector,
                                                  const AlmanacNativeSelection& selection,
                                                  unsigned capabilities) {
    AlmanacSelectionValidation result{};
    if (!host_has_capability(capabilities, AlmanacHostCapability_SelectionValidation)) {
        result.status = AlmanacSelectionValidation_MissingCapability;
        return result;
    }
    if (!host_is_open(visibility, capabilities)) {
        result.status = AlmanacSelectionValidation_ClosedHost;
        return result;
    }
    const AlmanacVectorValidation vector_result = validate_native_panel_vector(vector);
    if (!vector_result.ok) {
        result.status = AlmanacSelectionValidation_VectorInvalid;
        return result;
    }
    return validate_native_selection(vector, selection);
}

bool host_surface_is_valid(const AlmanacHostSurface& surface) {
    if (!host_geometry_is_valid(surface.geometry)) return false;
    if (!host_clip_rect_is_valid(surface.clip)) return false;
    if (!host_scroll_state_is_valid(surface.scroll)) return false;
    if (!host_pointer_event_is_valid(surface.pointer)) return false;
    if (!host_frame_identity_is_valid(surface.frame)) return false;
    if (!host_lifecycle_is_valid(surface.lifecycle)) return false;
    return true;
}

bool host_surface_can_render(const AlmanacHostSurface& surface) {
    const unsigned required =
        AlmanacHostCapability_Visibility |
        AlmanacHostCapability_SelectionValidation |
        AlmanacHostCapability_Geometry |
        AlmanacHostCapability_Dpi |
        AlmanacHostCapability_FlowLayout |
        AlmanacHostCapability_FrameIdentity |
        AlmanacHostCapability_LifecycleInvalidation;
    if ((surface.capabilities & required) != required) return false;
    if (!surface.visible || !surface.selected) return false;
    if (!host_geometry_is_valid(surface.geometry)) return false;
    if (!host_frame_identity_is_valid(surface.frame)) return false;
    if (!host_lifecycle_is_valid(surface.lifecycle)) return false;
    if (lifecycle_invalidated(surface.lifecycle, surface.frame)) return false;
    if (surface.clip.enabled && !host_surface_can_clip(surface)) return false;
    if ((surface.scroll.can_scroll_x || surface.scroll.can_scroll_y) &&
        !host_surface_can_scroll(surface)) return false;
    return true;
}

bool host_surface_can_clip(const AlmanacHostSurface& surface) {
    if (!host_has_capability(surface.capabilities, AlmanacHostCapability_Clipping)) return false;
    if (!surface.clip.enabled) return false;
    return host_clip_rect_is_valid(surface.clip);
}

bool host_surface_can_scroll(const AlmanacHostSurface& surface) {
    if (!host_has_capability(surface.capabilities, AlmanacHostCapability_Scrolling)) return false;
    if (!surface.scroll.can_scroll_x && !surface.scroll.can_scroll_y) return false;
    return host_scroll_state_is_valid(surface.scroll);
}

bool host_surface_can_receive_pointer(const AlmanacHostSurface& surface) {
    if (!host_surface_can_render(surface)) return false;
    if (!host_has_capability(surface.capabilities, AlmanacHostCapability_PointerInput)) return false;
    if (!host_pointer_event_is_valid(surface.pointer)) return false;
    if (surface.pointer.action == AlmanacPointerAction_None) return false;
    if (surface.pointer.frame_id != surface.frame.frame_number) return false;
    return surface.pointer.in_bounds;
}

} // namespace almanac

extern "C" {

bool AlmanacCheckedAddSize(std::size_t a, std::size_t b, std::size_t* out) {
    return almanac::checked_add_size(a, b, out);
}

bool AlmanacCheckedMulSize(std::size_t a, std::size_t b, std::size_t* out) {
    return almanac::checked_mul_size(a, b, out);
}

bool AlmanacNativePanelVectorIsValid(const almanac::AlmanacNativePanelVector* vector) {
    if (vector == nullptr) return false;
    return almanac::native_panel_vector_is_valid(*vector);
}

bool AlmanacNativeSelectionIsMember(const almanac::AlmanacNativePanelVector* vector,
                                    const almanac::AlmanacNativeSelection* selection) {
    if (vector == nullptr || selection == nullptr) return false;
    return almanac::native_selection_is_member(*vector, *selection);
}

bool AlmanacValidateBoundedName(const char* name, std::size_t capacity, std::size_t* out_length) {
    return almanac::validate_bounded_name(name, capacity, out_length);
}

bool AlmanacNameIsKnownHostPanel(const char* name, std::size_t capacity) {
    return almanac::name_is_known_host_panel(name, capacity);
}

bool AlmanacHostSurfaceCanRender(const almanac::AlmanacHostSurface* surface) {
    if (surface == nullptr) return false;
    return almanac::host_surface_can_render(*surface);
}

bool AlmanacLifecycleInvalidated(const almanac::AlmanacLifecycleState* state,
                                 const almanac::AlmanacFrameIdentity* current) {
    if (state == nullptr || current == nullptr) return true;
    return almanac::lifecycle_invalidated(*state, *current);
}

}
