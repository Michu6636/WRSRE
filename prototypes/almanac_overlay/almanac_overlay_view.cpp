#include "almanac_overlay_view.h"
#include "imgui.h"
#include <cmath>

namespace almanac {
FinanceView SyntheticOverlayView() {
    FinanceSnapshot snapshot{};
    snapshot.activeCurrency = {TSM_CURRENCY_RUB, FinanceAvailability::Available};
    snapshot.treasury = {1234567.0, FinanceAvailability::Available};
    // Other fields deliberately unavailable: no invented policy values.
    return BuildFinanceView(snapshot);
}
static OverlayPrototypeActions DrawView(const FinanceView& view, bool interactive) {
    OverlayPrototypeActions actions{};
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    const ImVec2 available = viewport->WorkSize;
    // A minimized/invalid viewport must not retain a visible diagnostic window.
    if (!std::isfinite(available.x) || !std::isfinite(available.y) ||
        !std::isfinite(viewport->WorkPos.x) || !std::isfinite(viewport->WorkPos.y) ||
        available.x * .55f < ImGui::GetStyle().WindowMinSize.x ||
        available.y * .65f < ImGui::GetStyle().WindowMinSize.y) return actions;
    const ImGuiCond placement = interactive ? ImGuiCond_FirstUseEver : ImGuiCond_Always;
    // Ratios are viewport-relative. Font/style scaling belongs to the backend;
    // do not apply game scale and Windows DPI a second time here.
    ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + available.x * .5f,
                                 viewport->WorkPos.y + available.y * .5f),
                            placement, ImVec2(.5f, .5f));
    ImGui::SetNextWindowSize(ImVec2(available.x * .55f, available.y * .65f), placement);
    bool open = true;
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoSavedSettings;
    if (!interactive) flags |= ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoBringToFrontOnFocus;
    const char* title = interactive ? "Almanac — developer prototype###AlmanacPrototype"
                                   : "Almanac — draw only###AlmanacReadOnlyPrototype";
    const bool expanded = ImGui::Begin(title, interactive ? &open : nullptr, flags);
    if (expanded) {
        ImGui::TextWrapped("SYNTHETIC DATA — not connected to your republic");
        ImGui::Separator();
        ImGui::TextUnformatted("Finance");
        if (ImGui::BeginTable("FinanceRows", 2,
                ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_RowBg |
                ImGuiTableFlags_SizingStretchProp)) {
            ImGui::TableSetupColumn("Policy", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableHeadersRow();
            for (const auto& row : view.rows) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::TextUnformatted(row.label ? row.label : "Unavailable");
                ImGui::TableSetColumnIndex(1);
                ImGui::TextUnformatted(row.value);
            }
            ImGui::EndTable();
        }
        ImGui::Separator();
        ImGui::TextWrapped("Editing is disabled in this read-only prototype.");
        if (view.note) ImGui::TextWrapped("%s", view.note);
        if (interactive && ImGui::Button("Close")) actions.close = true;
    }
    ImGui::End(); // required even when Begin returns false
    actions.close = actions.close || !open;
    return actions;
}
OverlayPrototypeActions DrawOverlayPrototype(const FinanceView& view) {
    return DrawView(view, true);
}
void DrawOverlayReadOnlyPrototype(const FinanceView& view) {
    DrawView(view, false);
}
} // namespace almanac
