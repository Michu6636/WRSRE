#pragma once

#include "host_contract.h"

#include <cstddef>
#include <cstdint>

namespace almanac {

bool checked_add_size(std::size_t a, std::size_t b, std::size_t* out);
bool checked_mul_size(std::size_t a, std::size_t b, std::size_t* out);

AlmanacVectorValidation validate_native_panel_vector(const AlmanacNativePanelVector& vector);
bool native_panel_vector_is_valid(const AlmanacNativePanelVector& vector);

AlmanacSelectionValidation validate_native_selection(const AlmanacNativePanelVector& vector,
                                                   const AlmanacNativeSelection& selection);
bool native_selection_is_member(const AlmanacNativePanelVector& vector,
                                const AlmanacNativeSelection& selection);

bool validate_bounded_name(const char* name, std::size_t capacity, std::size_t* out_length);
bool name_is_known_host_panel(const char* name, std::size_t capacity);

bool host_has_capability(unsigned capabilities, AlmanacHostCapability capability);
bool host_geometry_is_valid(const AlmanacHostGeometry& geometry);
bool host_clip_rect_is_valid(const AlmanacHostClipRect& clip);
bool host_scroll_state_is_valid(const AlmanacHostScrollState& scroll);
bool host_pointer_event_is_valid(const AlmanacPointerEvent& event);
bool host_frame_identity_is_valid(const AlmanacFrameIdentity& identity);
bool host_lifecycle_is_valid(const AlmanacLifecycleState& state);
bool host_visibility_is_valid(const AlmanacHostVisibility& visibility);
bool host_is_open(const AlmanacHostVisibility& visibility, unsigned capabilities);

bool frame_identity_equal(const AlmanacFrameIdentity& a, const AlmanacFrameIdentity& b);
bool generation_is_stale(std::uint64_t cached_generation, std::uint64_t current_generation);
bool lifecycle_invalidated(const AlmanacLifecycleState& state, const AlmanacFrameIdentity& current);

AlmanacSelectionValidation validate_host_selection(const AlmanacHostVisibility& visibility,
                                                  const AlmanacNativePanelVector& vector,
                                                  const AlmanacNativeSelection& selection,
                                                  unsigned capabilities);

bool host_surface_is_valid(const AlmanacHostSurface& surface);
bool host_surface_can_render(const AlmanacHostSurface& surface);
bool host_surface_can_clip(const AlmanacHostSurface& surface);
bool host_surface_can_scroll(const AlmanacHostSurface& surface);
bool host_surface_can_receive_pointer(const AlmanacHostSurface& surface);

} // namespace almanac

extern "C" {
bool AlmanacCheckedAddSize(std::size_t a, std::size_t b, std::size_t* out);
bool AlmanacCheckedMulSize(std::size_t a, std::size_t b, std::size_t* out);
bool AlmanacNativePanelVectorIsValid(const almanac::AlmanacNativePanelVector* vector);
bool AlmanacNativeSelectionIsMember(const almanac::AlmanacNativePanelVector* vector,
                                    const almanac::AlmanacNativeSelection* selection);
bool AlmanacValidateBoundedName(const char* name, std::size_t capacity, std::size_t* out_length);
bool AlmanacNameIsKnownHostPanel(const char* name, std::size_t capacity);
bool AlmanacHostSurfaceCanRender(const almanac::AlmanacHostSurface* surface);
bool AlmanacLifecycleInvalidated(const almanac::AlmanacLifecycleState* state,
                                 const almanac::AlmanacFrameIdentity* current);
}
