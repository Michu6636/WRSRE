#pragma once

#include "almanac_ui_api.h"
#include "host_contract.h"
#include "host_validation.h"

#include <cstddef>
#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct AlmanacShell AlmanacShell;

// Lifecycle.
AlmanacShell* almanac_shell_create(void);
void almanac_shell_destroy(AlmanacShell* shell);

// Provider registration. Returns handle >= 1 on success, 0 on failure/full.
int almanac_shell_register_provider(AlmanacShell* shell,
                                    AlmanacPageId page,
                                    AlmanacPopulatePageCallback populate,
                                    void* userData);
// Attach an action callback to an existing provider. One action provider is
// allowed per page; its userData is the registration userData. Null clears it.
int almanac_shell_set_provider_action(AlmanacShell* shell, int handle,
                                      AlmanacPageActionCallback action);
void almanac_shell_unregister_provider(AlmanacShell* shell, int handle);
std::size_t almanac_shell_provider_count(const AlmanacShell* shell);
int almanac_shell_provider_is_registered(const AlmanacShell* shell, int handle);

// Navigation (internal only; no native tab indices).
int almanac_shell_navigate(AlmanacShell* shell, AlmanacPageId page);
int almanac_shell_go_back(AlmanacShell* shell);
AlmanacPageId almanac_shell_current_page(const AlmanacShell* shell);
AlmanacPageId almanac_shell_previous_page(const AlmanacShell* shell);

// Population (distinct from rendering). Returns 1 if host renderable.
int almanac_shell_populate(AlmanacShell* shell, const almanac::AlmanacHostSurface* host);

// Rendering (display only). Returns 1 if rendered.
int almanac_shell_render(AlmanacShell* shell, const almanac::AlmanacHostSurface* host,
                         AlmanacRenderFrame* outFrame);

// Pointer input. Returns 1 if accepted.
int almanac_shell_handle_pointer(AlmanacShell* shell, const almanac::AlmanacHostSurface* host);

// Read-only state.
const AlmanacPageSnapshot* almanac_shell_page_snapshot(const AlmanacShell* shell,
                                                      AlmanacPageId page);
int almanac_shell_host_renderable(const AlmanacShell* shell);
float almanac_shell_layout_dpi(const AlmanacShell* shell);
float almanac_shell_row_height(const AlmanacShell* shell);
float almanac_shell_scroll_offset_y(const AlmanacShell* shell);
int almanac_shell_last_hit_row(const AlmanacShell* shell);

// Value-class / unit aggregation. Returns number of buckets written.
std::size_t almanac_aggregate_money(const AlmanacMoney* values, std::size_t count,
                                    AlmanacAggregateBucket* out, std::size_t capacity);

#ifdef __cplusplus
}
#endif
