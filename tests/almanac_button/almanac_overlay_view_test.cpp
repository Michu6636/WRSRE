// Headless Dear ImGui smoke test: no OS window, GPU, hook or game.
#include "almanac_overlay_view.h"
#include "earthworks/panel_style.h"
#include "imgui.h"
#include "imgui_internal.h" // tests inspect flags/bounds; production uses public API only
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#define CHECK(x) do { if (!(x)) { std::fprintf(stderr,"FAIL %d: %s\n",__LINE__,#x); std::exit(1); } } while (0)
static void PanelStyle() {
    auto* first = ImGui::CreateContext();
    ImGui::GetStyle().WindowPadding = ImVec2(3, 4);
    auto* second = ImGui::CreateContext();
    ImGui::SetCurrentContext(second);
    almanac::ApplyEarthworksPanelStyle();
    CHECK(ImGui::GetCurrentContext() == second);
    CHECK(ImGui::GetStyle().WindowPadding.x == 14);
    CHECK(ImGui::GetStyle().WindowPadding.y == 12);
    CHECK(std::fabs(ImGui::GetStyle().Colors[ImGuiCol_WindowBg].x - .91f) < .001f);
    almanac::ApplyEarthworksPanelStyle(); // absolute values; no repeated scaling
    CHECK(ImGui::GetStyle().WindowPadding.x == 14);
    ImGui::SetCurrentContext(first);
    CHECK(ImGui::GetStyle().WindowPadding.x == 3);
    CHECK(ImGui::GetStyle().WindowPadding.y == 4);
    ImGui::DestroyContext(second);
    ImGui::DestroyContext(first);
}
static void ReadOnlyDrawing() {
    ImGui::CreateContext();
    auto& io = ImGui::GetIO();
    io.IniFilename = nullptr; io.LogFilename = nullptr;
    io.DeltaTime = 1.0f / 60.0f;
    almanac::ApplyEarthworksPanelStyle();
    // Dedicated draw-only context: no platform input backend or global keyboard
    // navigation. ImGui's fallback debug window can otherwise claim Tab itself.
    io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard;
    unsigned char* pixels = nullptr; int width = 0, height = 0;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    const auto view = almanac::SyntheticOverlayView();
    const ImVec2 sizes[] = {{1280,720}, {800,600}, {1920,1080}, {320,240}};
    for (const auto size : sizes) {
        io.DisplaySize = size;
        for (int frame = 0; frame < 4; ++frame) {
            io.AddMousePosEvent(size.x * .5f, size.y * .5f);
            io.AddMouseButtonEvent(0, (frame % 2) != 0);
            io.AddKeyEvent(ImGuiKey_Tab, (frame % 2) != 0);
            ImGui::NewFrame();
            almanac::DrawOverlayReadOnlyPrototype(view);
            auto* window = ImGui::FindWindowByName("Almanac — draw only###AlmanacReadOnlyPrototype");
            CHECK(window && window->Active && !window->HasCloseButton);
            CHECK((window->Flags & ImGuiWindowFlags_NoInputs) == ImGuiWindowFlags_NoInputs);
            CHECK(window->Flags & ImGuiWindowFlags_NoMove);
            CHECK(window->Flags & ImGuiWindowFlags_NoResize);
            CHECK(window->Flags & ImGuiWindowFlags_NoFocusOnAppearing);
            CHECK(std::fabs(window->Size.x - std::floor(size.x * .55f)) < 2);
            CHECK(std::fabs(window->Size.y - std::floor(size.y * .65f)) < 2);
            CHECK(window->Pos.x >= 0 && window->Pos.y >= 0);
            CHECK(window->Pos.x + window->Size.x <= size.x + 1);
            CHECK(window->Pos.y + window->Size.y <= size.y + 1);
            ImGui::Render();
            if (io.WantCaptureMouse || io.WantCaptureKeyboard || io.WantTextInput)
                std::fprintf(stderr, "capture at %.0fx%.0f frame %d: mouse=%d keyboard=%d text=%d\n",
                    size.x, size.y, frame, io.WantCaptureMouse, io.WantCaptureKeyboard, io.WantTextInput);
            CHECK(!io.WantCaptureMouse && !io.WantCaptureKeyboard && !io.WantTextInput);
            if (frame > 0) CHECK(ImGui::GetDrawData()->TotalVtxCount > 0);
        }
    }
    // A minimized/tiny viewport omits drawing instead of retaining stale bounds.
    const ImVec2 hiddenSizes[] = {{0,0}, {1,1}};
    for (const auto size : hiddenSizes) {
        io.DisplaySize = size;
        ImGui::NewFrame();
        almanac::DrawOverlayReadOnlyPrototype(view);
        auto* window = ImGui::FindWindowByName("Almanac — draw only###AlmanacReadOnlyPrototype");
        CHECK(window && window->LastFrameActive != ImGui::GetFrameCount());
        ImGui::Render();
        CHECK(ImGui::GetDrawData()->TotalVtxCount == 0);
        CHECK(!io.WantCaptureMouse && !io.WantCaptureKeyboard);
    }
    ImGui::DestroyContext();
}
int main() {
    PanelStyle();
    ReadOnlyDrawing();
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto& io = ImGui::GetIO();
    io.IniFilename = nullptr; io.LogFilename = nullptr;
    io.DeltaTime = 1.0f / 60.0f;
    unsigned char* pixels = nullptr; int width = 0, height = 0;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    CHECK(pixels && width && height);
    auto view = almanac::SyntheticOverlayView();
    CHECK(std::strstr(view.rows[0].value, "RUB"));
    CHECK(std::strstr(view.rows[1].value, "Unavailable"));
    const ImVec2 sizes[] = {{1280,720}, {1920,1080}, {800,600}};
    for (const auto size : sizes) {
        io.DisplaySize = size;
        for (int frame = 0; frame < 3; ++frame) {
            ImGui::NewFrame();
            CHECK(!almanac::DrawOverlayPrototype(view).close);
            ImGui::Render();
            CHECK(ImGui::GetDrawData() && ImGui::GetDrawData()->Valid);
            if (frame > 0) CHECK(ImGui::GetDrawData()->TotalVtxCount > 0);
        }
    }
    ImGui::DestroyContext();
    std::puts("headless overlay drawing tests passed");
}
