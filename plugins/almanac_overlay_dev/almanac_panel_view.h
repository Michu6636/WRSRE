// almanac_panel_view.h - the one panel this plugin draws. Declarations only.
//
// The body is original WRSRE code. It reads a plain value struct and emits ImGui
// calls; it cannot open itself, capture input, reach the game or allocate GPU
// resources. Whoever calls it owns the context, the frame and the surface.
#pragma once

namespace almanac {

// Everything the panel displays. Deliberately a value type: no service
// pointers, no game handles, no live repository state. A future live read-only
// Finance feed replaces the numbers here without changing the drawing code.
struct PanelModel {
    const char* title          = "Almanac";
    const char* subtitle       = "Market economy suite";
    const char* dataNotice     = "Synthetic placeholder values - not read from your republic.";
    const char* toggleKeyName  = "F8";
    const char* buildNotice    = "";
    int         drawnFrames    = 0;
    int         processId      = 0;
    bool        autoShow       = true;
};

// Draw the panel. Must be called between ImGui::NewFrame() and
// ImGui::Render(), on the owning thread, with a current context.
//
// The panel is input-free by construction (every window and control carries
// ImGuiWindowFlags_NoInputs / the read-only styles below), so it can be placed
// over the game without stealing clicks or keys from the game or from another
// overlay. Returns true while the panel is still being drawn; false only if the
// viewport is too small to lay out, which the caller treats as "not shown".
bool DrawAlmanacPanel(const PanelModel& model);

} // namespace almanac
