// Drawing checks for the Almanac panel, against a real Dear ImGui checkout.
//
// No GPU and no Windows: ImGui is driven headlessly and the emitted vertex count
// is used as the evidence that the panel actually produced geometry rather than
// returning early. This does not test the D3D11 backend, the swap chain or the
// plugin's hooks - only the drawing code the DLL compiles in.
#include "../../plugins/almanac_overlay_dev/almanac_panel_view.h"

#include "imgui.h"

#include <cstdint>
#include <cstdio>

namespace {

int g_failures = 0;

void Check(bool ok, const char* what)
{
    if (!ok) {
        std::printf("FAIL  %s\n", what);
        ++g_failures;
    }
}

// ---------------------------------------------------------------- drawing

// Counts the vertices ImGui produced, which is the cheapest evidence that the
// panel actually emitted geometry rather than returning early.
//
// Two frames are drawn, as in game: the first can legitimately produce less
// because a window has no measured size yet, and a test that only looked at
// frame one would either pass on nothing or fail on that warm-up.
int DrawAndCountVertices(ImVec2 displaySize, bool* reportedShown)
{
    ImGuiContext* context = ImGui::CreateContext();
    ImGui::SetCurrentContext(context);
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    io.DisplaySize = displaySize;
    io.DeltaTime = 1.0f / 60.0f;
    io.Fonts->AddFontDefault();
    unsigned char* pixels = nullptr;
    int width = 0, height = 0;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    if (!io.Fonts->IsBuilt()) {
        // The headless path has no backend to build the atlas; draw calls would
        // assert without it.
        ImGui::DestroyContext(context);
        ImGui::SetCurrentContext(nullptr);
        return -1;
    }
    io.Fonts->SetTexID(static_cast<ImTextureID>(1));

    almanac::PanelModel model;
    model.drawnFrames   = 42;
    model.processId     = 1234;
    model.buildNotice   = "test";
    model.toggleKeyName = "F8";

    bool shown = false;
    int vertices = 0;
    for (int frame = 0; frame < 2; ++frame) {
        ImGui::NewFrame();
        shown = almanac::DrawAlmanacPanel(model);
        ImGui::Render();

        const ImDrawData* data = ImGui::GetDrawData();
        vertices = 0;
        if (data) {
            for (int i = 0; i < data->CmdListsCount; ++i)
                vertices += data->CmdLists[i]->VtxBuffer.Size;
        }
    }
    if (reportedShown) *reportedShown = shown;
    ImGui::DestroyContext(context);
    ImGui::SetCurrentContext(nullptr);
    return vertices;
}

void DrawingTests()
{
    bool shown = false;
    const int normal = DrawAndCountVertices(ImVec2(1920.0f, 1080.0f), &shown);
    if (normal < 0) {
        std::printf("SKIP  drawing (font atlas could not be built)\n");
        return;
    }
    Check(shown, "panel reports itself drawn at 1920x1080");
    Check(normal > 200, "panel emits geometry at 1920x1080");

    const int small = DrawAndCountVertices(ImVec2(640.0f, 480.0f), &shown);
    Check(shown, "panel reports itself drawn at 640x480");
    Check(small > 200, "panel emits geometry at 640x480");

    // Wide but very short: the panel must still draw (clipped) rather than
    // vanish, because a short game window is still a window.
    const int shortSurface = DrawAndCountVertices(ImVec2(1920.0f, 220.0f), &shown);
    Check(shown, "panel draws on a wide, short surface");
    Check(shortSurface > 200, "panel emits geometry on a wide, short surface");

    // Degenerate surface: a minimised or fully collapsed window. Nothing may be
    // drawn and no assertion may fire.
    bool degenerateShown = true;
    const int degenerate = DrawAndCountVertices(ImVec2(0.0f, 0.0f), &degenerateShown);
    Check(!degenerateShown, "panel refuses a 0x0 surface");
    Check(degenerate == 0, "panel emits no geometry for a 0x0 surface");

    // Narrower than the panel can use: refuse rather than draw a single column
    // of clipped text.
    degenerateShown = true;
    const int narrow = DrawAndCountVertices(ImVec2(200.0f, 1080.0f), &degenerateShown);
    Check(!degenerateShown, "panel refuses a 200px-wide surface");
    Check(narrow == 0, "panel emits no geometry at 200px wide");

    // Live Economy tab with State Budget table enabled: verify geometry builds cleanly.
    ImGuiContext* context = ImGui::CreateContext();
    ImGui::SetCurrentContext(context);
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    io.DisplaySize = ImVec2(1280.0f, 720.0f);
    io.DeltaTime = 1.0f / 60.0f;
    io.Fonts->AddFontDefault();
    unsigned char* pixels = nullptr;
    int width = 0, height = 0;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    if (io.Fonts->IsBuilt()) {
        io.Fonts->SetTexID(static_cast<ImTextureID>(1));
        almanac::PanelModel liveModel;
        liveModel.hasLiveData = true;
        liveModel.tabs[0].count = 1;
        std::snprintf(liveModel.tabs[0].rows[0].label, 64, "Treasury (RUB)");
        std::snprintf(liveModel.tabs[0].rows[0].value, 64, "9796000.12 RUB");
        liveModel.budget.hasLedger = true;
        liveModel.budget.closedYear = 1960;
        liveModel.budget.closedMonth = 3;
        liveModel.budget.closedNet = 6549.74;
        liveModel.budget.liveNet = -120.50;
        int tab = 0;
        bool liveShown = false;
        int liveVerts = 0;
        for (int frame = 0; frame < 2; ++frame) {
            ImGui::NewFrame();
            liveShown = almanac::DrawAlmanacPanel(liveModel, nullptr, &tab);
            ImGui::Render();
            const ImDrawData* data = ImGui::GetDrawData();
            liveVerts = 0;
            if (data) {
                for (int i = 0; i < data->CmdListsCount; ++i)
                    liveVerts += data->CmdLists[i]->VtxBuffer.Size;
            }
        }
        Check(liveShown, "live model with state budget draws");
        Check(liveVerts > normal, "live state budget table adds visible geometry");
    }
    ImGui::DestroyContext(context);
    ImGui::SetCurrentContext(nullptr);
}

} // namespace

int main()
{
    DrawingTests();
    if (g_failures) {
        std::printf("%d drawing check(s) failed\n", g_failures);
        return 1;
    }
    std::printf("panel drawing checks passed\n");
    return 0;
}
