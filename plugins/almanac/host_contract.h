#pragma once

#include <cstddef>
#include <cstdint>

namespace almanac {

enum AlmanacHostCapability : unsigned {
    AlmanacHostCapability_None = 0u,
    AlmanacHostCapability_Visibility = 1u << 0,
    AlmanacHostCapability_SelectionValidation = 1u << 1,
    AlmanacHostCapability_Geometry = 1u << 2,
    AlmanacHostCapability_Dpi = 1u << 3,
    AlmanacHostCapability_Clipping = 1u << 4,
    AlmanacHostCapability_Scrolling = 1u << 5,
    AlmanacHostCapability_PointerInput = 1u << 6,
    AlmanacHostCapability_KeyboardInput = 1u << 7,
    AlmanacHostCapability_FrameIdentity = 1u << 8,
    AlmanacHostCapability_LifecycleInvalidation = 1u << 9,
    AlmanacHostCapability_FlowLayout = 1u << 10,
    AlmanacHostCapability_All = 0x7fffffffu
};

struct AlmanacHostGeometry {
    float dpi = 0.0f;
    float origin_x = 0.0f;
    float origin_y = 0.0f;
    float extent_width = 0.0f;
    float extent_height = 0.0f;
};

struct AlmanacHostClipRect {
    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;
    bool enabled = false;
};

struct AlmanacHostScrollState {
    float offset_x = 0.0f;
    float offset_y = 0.0f;
    float max_offset_x = 0.0f;
    float max_offset_y = 0.0f;
    bool can_scroll_x = false;
    bool can_scroll_y = false;
};

enum AlmanacPointerAction {
    AlmanacPointerAction_None = 0,
    AlmanacPointerAction_Move = 1,
    AlmanacPointerAction_Down = 2,
    AlmanacPointerAction_Up = 3,
    AlmanacPointerAction_Wheel = 4
};

struct AlmanacPointerEvent {
    AlmanacPointerAction action = AlmanacPointerAction_None;
    float x = 0.0f;
    float y = 0.0f;
    float wheel_delta = 0.0f;
    unsigned buttons = 0u;
    bool in_bounds = false;
    std::uint64_t frame_id = 0u;
};

struct AlmanacFrameIdentity {
    std::uint64_t frame_number = 0u;
    std::uint64_t generation = 0u;
};

struct AlmanacLifecycleState {
    std::uint64_t generation = 0u;
    bool invalidated = false;
    std::uint64_t invalidation_frame = 0u;
};

struct AlmanacHostVisibility {
    bool sidebar_open = false;
    bool trade_family_selected = false;
    bool almanac_selected = false;
};

struct AlmanacNativePanelVector {
    const void* begin = nullptr;
    const void* end = nullptr;
    std::size_t stride = 0u;
    std::size_t max_entries = 0u;
};

struct AlmanacNativeSelection {
    const void* selected = nullptr;
};

enum AlmanacVectorValidationStatus {
    AlmanacVectorValidation_Ok = 0,
    AlmanacVectorValidation_NullBegin = 1,
    AlmanacVectorValidation_NullEnd = 2,
    AlmanacVectorValidation_Reversed = 3,
    AlmanacVectorValidation_ZeroStride = 4,
    AlmanacVectorValidation_NotMultiple = 5,
    AlmanacVectorValidation_TooMany = 6,
    AlmanacVectorValidation_Overflow = 7
};

struct AlmanacVectorValidation {
    bool ok = false;
    AlmanacVectorValidationStatus status = AlmanacVectorValidation_NullBegin;
    std::size_t count = 0u;
};

enum AlmanacSelectionValidationStatus {
    AlmanacSelectionValidation_Ok = 0,
    AlmanacSelectionValidation_VectorInvalid = 1,
    AlmanacSelectionValidation_NullSelection = 2,
    AlmanacSelectionValidation_OutOfRange = 3,
    AlmanacSelectionValidation_Misaligned = 4,
    AlmanacSelectionValidation_UnknownName = 5,
    AlmanacSelectionValidation_UnterminatedName = 6,
    AlmanacSelectionValidation_ClosedHost = 7,
    AlmanacSelectionValidation_MissingCapability = 8,
    AlmanacSelectionValidation_StaleGeneration = 9
};

struct AlmanacSelectionValidation {
    bool ok = false;
    AlmanacSelectionValidationStatus status = AlmanacSelectionValidation_VectorInvalid;
    std::size_t index = 0u;
};

struct AlmanacHostSurface {
    unsigned capabilities = 0u;
    bool visible = false;
    bool selected = false;
    AlmanacHostGeometry geometry{};
    AlmanacHostClipRect clip{};
    AlmanacHostScrollState scroll{};
    AlmanacPointerEvent pointer{};
    AlmanacFrameIdentity frame{};
    AlmanacLifecycleState lifecycle{};
};

} // namespace almanac
