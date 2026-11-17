// almanac_panel_view.cpp - drawing for the single Almanac panel. Original code.
//
// Layout, in order down the window:
//
//   header      title, subtitle, and the key that hides/reopens the panel
//   notice      a plain warning that the numbers are placeholders
//   tabs        Economy / Labour / Logistics - fixed labels, no state read from
//               anywhere, so the panel shows the shape of the finished product
//   body        one labelled table per tab with synthetic values
//   footer      frame/process readout, which is the cheapest proof in a
//               screenshot that this is being drawn live rather than cached
//
// Input: none. Every window is NoInputs, no title bar control is interactive,
// and the tab bar is a plain group of buttons drawn TextDisabled-style rather
// than a real tab bar, so no focus, hover, selection or IME reaches ImGui and
// nothing is written back to io.WantCaptureMouse/Keyboard. That is what makes it
// safe to draw over a live game: the game keeps every click and every keypress.
#include "almanac_panel_view.h"

#include "imgui.h"

#include <cstdio>

namespace almanac {
namespace {

// Window flags for the non-interactive launcher button (clicks are handled
// directly by WndProc hit-testing against the published rect).
constexpr ImGuiWindowFlags kLauncherFlags =
    ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings |
    ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar |
    ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoNav |
    ImGuiWindowFlags_NoFocusOnAppearing |
    ImGuiWindowFlags_NoBringToFrontOnFocus |
    ImGuiWindowFlags_NoTitleBar;

// Window flags for the interactive Almanac panel (Milestone 2): draggable,
// resizable, scrollable, with clickable tabs.
constexpr ImGuiWindowFlags kInteractivePanelFlags =
    ImGuiWindowFlags_NoSavedSettings |
    ImGuiWindowFlags_NoCollapse |
    ImGuiWindowFlags_NoNav |
    ImGuiWindowFlags_NoFocusOnAppearing |
    ImGuiWindowFlags_NoBringToFrontOnFocus;

// Paper and ink, close to the game's own panel palette. Text colours are ImVec4
// for TextColored; the two that also paint geometry keep their packed form.
const ImVec4 kInk   = ImVec4(0.22f, 0.17f, 0.12f, 1.00f);
const ImVec4 kQuiet = ImVec4(0.42f, 0.35f, 0.27f, 1.00f);
const ImVec4 kWarn  = ImVec4(0.55f, 0.18f, 0.12f, 1.00f);
const ImVec4 kLive  = ImVec4(0.16f, 0.42f, 0.20f, 1.00f);
const ImU32  kRule  = IM_COL32(0x38, 0x2B, 0x1E, 0x55);

struct Row {
    const char* label;
    const char* value;
};

// Synthetic sample rows. Static storage, never written, one array per tab.
// The numbers stand in for the live Finance/labour/logistics feeds that are not
// connected yet; they are labels with plausible values, not derived results.
const Row kEconomyRows[] = {
    {"Treasury (RUB)",             "1,234,567.00"},
    {"Active currency",            "RUB"},
    {"VAT",                        "20% (enabled)"},
    {"Profit tax",                 "15% (enabled)"},
    {"Property tax per enterprise", "250.00 (land value)"},
    {"Citizen wallets opened",     "18,402"},
    {"Average household balance",  "412.60"},
};
const Row kLabourRows[] = {
    {"Employed citizens",           "23,118"},
    {"Vacancies",                   "1,744"},
    {"Average wage",                "1,182.40 / month"},
    {"Skill premium (skilled)",     "+18%"},
    {"Payroll remittance (M)",      "27.30"},
    {"Mobility last quarter",       "612 moved"},
};
const Row kLogisticsRows[] = {
    {"Private distribution offices", "34"},
    {"Trips settled today",          "1,209"},
    {"Fuel burn (M litres)",         "0.42"},
    {"B2B counterparty invoices",    "76 open"},
    {"Foreign restock share",        "10% target"},
    {"Shipments in transit",         "158"},
};

const char* const kTabNames[] = {"Economy", "Labour", "Logistics"};
const Row* const   kTabRows[]  = {kEconomyRows, kLabourRows, kLogisticsRows};
const int          kTabRowCounts[] = {
    static_cast<int>(sizeof(kEconomyRows) / sizeof(kEconomyRows[0])),
    static_cast<int>(sizeof(kLabourRows) / sizeof(kLabourRows[0])),
    static_cast<int>(sizeof(kLogisticsRows) / sizeof(kLogisticsRows[0])),
};
constexpr int kTabCount = 3;

void Rule()
{
    const ImVec2 p = ImGui::GetCursorScreenPos();
    const float  w = ImGui::GetContentRegionAvail().x;
    ImGui::GetWindowDrawList()->AddLine(
        p, ImVec2(p.x + w, p.y), kRule, 1.0f);
    ImGui::Dummy(ImVec2(w, 1.0f));
}

// Interactive tab strip: clicking any tab switches the active table.
void DrawTabStrip(int& activeTab)
{
    if (activeTab < 0 || activeTab >= kTabCount) activeTab = 0;
    ImDrawList* draw = ImGui::GetWindowDrawList();
    for (int i = 0; i < kTabCount; ++i) {
        if (i) ImGui::SameLine(0.0f, 6.0f);
        const ImVec2 at   = ImGui::GetCursorScreenPos();
        const ImVec2 size = ImGui::CalcTextSize(kTabNames[i]);
        const ImVec2 pad(12.0f, 5.0f);
        const ImVec2 btnSize(size.x + pad.x * 2.0f, size.y + pad.y * 2.0f);

        char btnId[48];
        std::snprintf(btnId, sizeof(btnId), "##almanac_tab_%d", i);
        if (ImGui::InvisibleButton(btnId, btnSize)) {
            activeTab = i;
        }
        const bool selected = (i == activeTab);
        const bool hovered  = ImGui::IsItemHovered();

        const ImU32 fillCol = selected
            ? IM_COL32(0x66, 0x1A, 0x17, 0xFF)
            : (hovered ? IM_COL32(0xDF, 0xCF, 0xAF, 0xFF)
                       : IM_COL32(0xD3, 0xC1, 0x9F, 0xFF));
        const ImU32 borderCol = selected || hovered
            ? IM_COL32(0x66, 0x1A, 0x17, 0xDD)
            : kRule;
        const ImVec4 textCol = selected
            ? ImVec4(0.96f, 0.91f, 0.82f, 1.00f)
            : (hovered ? kInk : kQuiet);

        draw->AddRectFilled(
            at, ImVec2(at.x + btnSize.x, at.y + btnSize.y), fillCol, 4.0f);
        draw->AddRect(
            at, ImVec2(at.x + btnSize.x, at.y + btnSize.y), borderCol, 4.0f);
        draw->AddText(
            ImVec2(at.x + pad.x, at.y + pad.y),
            ImGui::ColorConvertFloat4ToU32(textCol), kTabNames[i]);
    }
}

void DrawTable(const Row* rows, int count)
{
    if (!ImGui::BeginTable("##almanac_rows", 2,
            ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerH |
            ImGuiTableFlags_SizingStretchProp)) {
        return;
    }
    ImGui::TableSetupColumn("Item", ImGuiTableColumnFlags_WidthStretch, 1.6f);
    ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch, 1.0f);
    for (int i = 0; i < count; ++i) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextColored(kInk, "%s", rows[i].label);
        ImGui::TableSetColumnIndex(1);
        ImGui::TextColored(kQuiet, "%s", rows[i].value);
    }
    ImGui::EndTable();
}

void DrawLiveTable(const TabData& data)
{
    if (data.count <= 0) {
        ImGui::TextColored(kQuiet, "No live service data available for this tab.");
        return;
    }
    if (!ImGui::BeginTable("##almanac_rows", 2,
            ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerH |
            ImGuiTableFlags_SizingStretchProp)) {
        return;
    }
    ImGui::TableSetupColumn("Item", ImGuiTableColumnFlags_WidthStretch, 1.6f);
    ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch, 1.0f);
    for (int i = 0; i < data.count; ++i) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextColored(kInk, "%s", data.rows[i].label);
        ImGui::TableSetColumnIndex(1);
        ImGui::TextColored(kQuiet, "%s", data.rows[i].value);
    }
    ImGui::EndTable();
}

} // namespace

LauncherRect DrawAlmanacLauncherButton(bool panelOpen, bool hovered,
                                       const char* toggleKeyName)
{
    LauncherRect rect{};
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    if (!viewport) return rect;

    const ImVec2 origin = viewport->WorkPos;
    const ImVec2 extent = viewport->WorkSize;
    if (!(extent.x > 240.0f) || !(extent.y > 120.0f)) return rect;

    const char* key = (toggleKeyName && toggleKeyName[0]) ? toggleKeyName : "F9";
    char label[64];
    std::snprintf(label, sizeof(label), "%s (%s)",
                  panelOpen ? "Almanac [Open]" : "Almanac", key);

    const ImVec2 textSize = ImGui::CalcTextSize(label);
    const float  padX     = 14.0f;
    const float  padY     = 6.0f;
    const float  btnW     = textSize.x + padX * 2.0f;
    const float  btnH     = textSize.y + padY * 2.0f;
    const ImVec2 pos(origin.x + 24.0f, origin.y + 64.0f);

    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(btnW, btnH), ImGuiCond_Always);

    const ImVec4 bg = panelOpen
        ? (hovered ? ImVec4(0.56f, 0.17f, 0.13f, 0.98f)
                   : ImVec4(0.46f, 0.12f, 0.10f, 0.96f))
        : (hovered ? ImVec4(0.95f, 0.90f, 0.79f, 0.98f)
                   : ImVec4(0.91f, 0.85f, 0.74f, 0.95f));
    const ImVec4 border = hovered
        ? ImVec4(0.65f, 0.22f, 0.16f, 1.00f)
        : ImVec4(0.36f, 0.25f, 0.16f, 0.90f);
    const ImVec4 textCol = panelOpen
        ? ImVec4(0.96f, 0.91f, 0.82f, 1.00f)
        : kInk;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(1.0f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 6.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, hovered ? 1.5f : 1.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, bg);
    ImGui::PushStyleColor(ImGuiCol_Border, border);

    if (ImGui::Begin("##almanac_launcher_btn", nullptr, kLauncherFlags)) {
        ImGui::SetCursorPos(ImVec2(padX, padY));
        ImGui::TextColored(textCol, "%s", label);
        rect.left   = pos.x;
        rect.top    = pos.y;
        rect.right  = pos.x + btnW;
        rect.bottom = pos.y + btnH;
        rect.valid  = true;
    }
    ImGui::End();

    ImGui::PopStyleColor(2);
    ImGui::PopStyleVar(4);
    return rect;
}

bool DrawAlmanacPanel(const PanelModel& model, LauncherRect* outRect, int* activeTab)
{
    if (outRect) *outRect = LauncherRect{};
    int localTab = 0;
    int& tab = activeTab ? *activeTab : localTab;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    if (!viewport) return false;

    const ImVec2 origin = viewport->WorkPos;
    const ImVec2 extent = viewport->WorkSize;

    // Refuse nonsense geometry rather than drawing a degenerated window. A
    // minimised or zero-sized surface reports 0 here; nothing else is assumed.
    // Only X must be able to hold the window: on a wide but very short surface a
    // clipped panel is better than a hidden one, and the host test pins that.
    if (!(extent.x > 240.0f)) return false;

    // Width is a fraction of the surface, floored so it stays readable on a
    // small window and capped so it never covers the whole game view.
    float width = extent.x * 0.46f;
    if (width < 520.0f) width = 520.0f;
    if (width > extent.x - 32.0f) width = extent.x - 32.0f;

    // Height is derived from the content, not from ImGuiWindowFlags_
    // AlwaysAutoResize: an auto-resizing window has no valid size on its first
    // frame and draws nothing, which is exactly the "panel does not appear"
    // symptom this plugin exists to fix. The estimate below is computed from the
    // live style and font metrics; the body child absorbs any difference.
    const ImGuiStyle& style = ImGui::GetStyle();
    const float line    = ImGui::GetTextLineHeight();
    const float frame   = ImGui::GetFrameHeight();
    const float gap     = style.ItemSpacing.y;
    const float padding = style.WindowPadding.y;
    const int   rows    = kTabRowCounts[0];
    // Terms top to bottom, in the order DrawAlmanacPanel emits them below. The
    // child gets the leftover, so an error here shows as padding at the bottom
    // of the body rather than as a clipped footer.
    float height = padding * 2.0f                              // window padding
                 + frame                                       // title bar
                 + gap + line                                  // subtitle line
                 + gap + 1.0f                                  // rule
                 + gap + line                                  // data notice
                 + gap                                         // Spacing() after it
                 + line + 8.0f                                 // tab strip
                 + gap + 1.0f                                  // rule under tabs
                 + static_cast<float>(rows) * (line + gap)     // table rows
                 + line                                        // note under the table
                 + 1.0f + gap + line                           // footer rule + line
                 + gap;                                        // child gap allowance

    float maxHeight = extent.y - 128.0f;
    if (maxHeight < 120.0f) maxHeight = 120.0f;
    if (height > maxHeight) height = maxHeight;
    if (width < 200.0f) width = 200.0f;

    // Initial placement top-left below the launcher button; user may drag and
    // resize freely afterwards.
    ImGui::SetNextWindowPos(ImVec2(origin.x + 24.0f, origin.y + 104.0f),
                            ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_FirstUseEver);
    const float maxW = (extent.x - 32.0f > 420.0f) ? (extent.x - 32.0f) : 420.0f;
    const float maxH = (extent.y - 48.0f > 240.0f) ? (extent.y - 48.0f) : 240.0f;
    ImGui::SetNextWindowSizeConstraints(ImVec2(420.0f, 240.0f), ImVec2(maxW, maxH));

    // ###  separates the visible title from the window's identity, so changing
    // the visible text never creates a new window.
    char title[128];
    std::snprintf(title, sizeof(title), "%s###almanac_panel", model.title);

    // ImGui draws the title in ImGuiCol_Text. The title bar is dark oxblood, so
    // the light text colour is pushed for the call that draws it and popped
    // before any content: the body keeps the dark ink colour.
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.95f, 0.90f, 0.80f, 1.00f));
    const bool open = ImGui::Begin(title, nullptr, kInteractivePanelFlags);
    ImGui::PopStyleColor();

    bool shown = false;
    if (open) {
        shown = true;
        if (outRect) {
            const ImVec2 wPos  = ImGui::GetWindowPos();
            const ImVec2 wSize = ImGui::GetWindowSize();
            outRect->left   = wPos.x;
            outRect->top    = wPos.y;
            outRect->right  = wPos.x + wSize.x;
            outRect->bottom = wPos.y + wSize.y;
            outRect->valid  = true;
        }

        ImGui::TextColored(kInk, "%s", model.subtitle);
        ImGui::SameLine();
        ImGui::TextColored(kQuiet, "  |  %s to hide / reopen",
                           model.toggleKeyName);
        Rule();

        ImGui::TextColored(model.hasLiveData ? kLive : kWarn,
                           "%s", model.dataNotice);
        ImGui::Spacing();

        DrawTabStrip(tab);
        ImGui::Spacing();
        Rule();

        const float footer = gap + 1.0f + gap + line;
        if (ImGui::BeginChild("##almanac_body", ImVec2(0.0f, -footer),
                              ImGuiChildFlags_None,
                              ImGuiWindowFlags_None)) {
            if (model.hasLiveData) {
                DrawLiveTable(model.tabs[tab]);
            } else {
                DrawTable(kTabRows[tab], kTabRowCounts[tab]);
            }
        }
        ImGui::EndChild();

        Rule();
        ImGui::TextColored(kQuiet,
                           "frame %d   pid %d   %s%s",
                           model.drawnFrames, model.processId,
                           model.autoShow ? "auto-shown once, " : "",
                           model.buildNotice ? model.buildNotice : "");
    }
    ImGui::End();
    return shown;
}

} // namespace almanac
