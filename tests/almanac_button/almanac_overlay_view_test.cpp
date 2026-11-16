// Headless Dear ImGui smoke test: no OS window, GPU, hook or game.
#include "almanac_overlay_view.h"
#include "imgui.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#define CHECK(x) do { if (!(x)) { std::fprintf(stderr,"FAIL %d: %s\n",__LINE__,#x); std::exit(1); } } while (0)
int main() {
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
