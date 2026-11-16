// SPDX-License-Identifier: GPL-3.0-only
// Extracted from NavySeal5/wrsr-Earthworks src/ui/overlay.cpp.
// Upstream file credits RepublicMP (E3418 and contributors), commit
// 630775724abd5e3346eb59f9c66e2873128d74a4, for input/overlay techniques.
// Changes: isolated style-only function; no native hooks, textures or input.
#include "panel_style.h"
#include "imgui.h"
namespace almanac {
void ApplyEarthworksPanelStyle() {
    ImGui::StyleColorsLight();
    auto& style=ImGui::GetStyle();
    const ImVec4 ink{0.22f,0.16f,0.12f,1.0f},paper{0.91f,0.85f,0.74f,1.0f};
    const ImVec4 paperDark{0.76f,0.65f,0.49f,1.0f},oxblood{0.40f,0.10f,0.09f,1.0f};
    // Keep control text on the fixed dark-ink color; selected/pressed surfaces
    // stay light enough for it to remain readable.
    const ImVec4 red{0.82f,0.57f,0.48f,1.0f},redLight{0.89f,0.70f,0.60f,1.0f};
    style.Colors[ImGuiCol_Text]=ink;
    style.Colors[ImGuiCol_TextDisabled]={0.43f,0.36f,0.28f,1.0f};
    style.Colors[ImGuiCol_WindowBg]=paper;
    style.Colors[ImGuiCol_ChildBg]={0.88f,0.81f,0.69f,0.55f};
    style.Colors[ImGuiCol_Border]={0.36f,0.25f,0.16f,0.85f};
    style.Colors[ImGuiCol_FrameBg]={0.96f,0.92f,0.83f,1.0f};
    style.Colors[ImGuiCol_FrameBgHovered]={1.0f,0.97f,0.89f,1.0f};
    style.Colors[ImGuiCol_FrameBgActive]={0.99f,0.91f,0.76f,1.0f};
    style.Colors[ImGuiCol_TitleBg]=oxblood;
    style.Colors[ImGuiCol_TitleBgActive]=red;
    style.Colors[ImGuiCol_TitleBgCollapsed]=oxblood;
    style.Colors[ImGuiCol_Button]=paperDark;
    style.Colors[ImGuiCol_ButtonHovered]=redLight;
    style.Colors[ImGuiCol_ButtonActive]=red;
    style.Colors[ImGuiCol_Header]=paperDark;
    style.Colors[ImGuiCol_HeaderHovered]=redLight;
    style.Colors[ImGuiCol_HeaderActive]=red;
    style.Colors[ImGuiCol_Tab]=paperDark;
    style.Colors[ImGuiCol_TabHovered]=redLight;
    style.Colors[ImGuiCol_TabSelected]=red;
    style.Colors[ImGuiCol_TabSelectedOverline]=oxblood;
    style.Colors[ImGuiCol_TabDimmed]=paperDark;
    style.Colors[ImGuiCol_TabDimmedSelected]=paper;
    style.Colors[ImGuiCol_CheckMark]=oxblood;
    style.Colors[ImGuiCol_SliderGrab]=red;
    style.Colors[ImGuiCol_SliderGrabActive]=oxblood;
    style.Colors[ImGuiCol_Separator]={0.45f,0.32f,0.21f,0.55f};
    style.Colors[ImGuiCol_ScrollbarBg]={0.67f,0.56f,0.42f,0.30f};
    style.Colors[ImGuiCol_ScrollbarGrab]=paperDark;
    style.Colors[ImGuiCol_ScrollbarGrabHovered]=redLight;
    style.Colors[ImGuiCol_ScrollbarGrabActive]=red;
    style.WindowPadding={14,12}; style.FramePadding={10,5}; style.ItemSpacing={8,7};
    style.WindowRounding=12; style.ChildRounding=8; style.FrameRounding=6; style.TabRounding=6; style.GrabRounding=5;
    style.WindowBorderSize=1; style.FrameBorderSize=1;
}
} // namespace almanac
