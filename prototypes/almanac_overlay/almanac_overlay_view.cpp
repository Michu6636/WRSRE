#include "almanac_overlay_view.h"
#include "imgui.h"

namespace almanac {
FinanceView SyntheticOverlayView() {
    FinanceSnapshot snapshot{};
    snapshot.activeCurrency = {TSM_CURRENCY_RUB, FinanceAvailability::Available};
    snapshot.treasury = {1234567.0, FinanceAvailability::Available};
    // Other fields deliberately unavailable: no invented policy values.
    return BuildFinanceView(snapshot);
}
OverlayPrototypeActions DrawOverlayPrototype(const FinanceView& view) {
    OverlayPrototypeActions actions{};
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    const ImVec2 available = viewport->WorkSize;
    // Ratios are viewport-relative. Font/style scaling belongs to the backend;
    // do not apply game scale and Windows DPI a second time here.
    ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + available.x * .5f,
                                 viewport->WorkPos.y + available.y * .5f),
                            ImGuiCond_FirstUseEver, ImVec2(.5f, .5f));
    ImGui::SetNextWindowSize(ImVec2(available.x * .55f, available.y * .65f),
                             ImGuiCond_FirstUseEver);
    bool open = true;
    const bool expanded = ImGui::Begin("Almanac — developer prototype###AlmanacPrototype",
                                       &open, ImGuiWindowFlags_NoSavedSettings);
    if (expanded) {
        ImGui::TextUnformatted("SYNTHETIC DATA — not connected to your republic");
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
        if (ImGui::Button("Close")) actions.close = true;
    }
    ImGui::End(); // required even when Begin returns false
    actions.close = actions.close || !open;
    return actions;
}
} // namespace almanac
