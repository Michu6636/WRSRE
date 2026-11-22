// almanac_panel_view.cpp - drawing for the single Almanac panel. Original code.
//
// Layout, in order down the window:
//
//   header      title, subtitle, and the key that hides/reopens the panel
//   notice      live-data state, or the reason it is unavailable
//   tabs        Economy / Labour / Logistics / Enterprises
//   body        live rows when a service is connected; otherwise a full-panel
//               "NO DATA: <service> unavailable" in warning colour. There are
//               no sample, synthetic or placeholder values anywhere (D8).
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

// There are no sample rows. Economy, Labour and Logistics show live rows when
// their services are present and "NO DATA" when they are not. The height
// estimate below still assumes a table-shaped body so the window does not jump
// when live data appears; it is a layout constant, not a value on screen.
const char* const kTabNames[]        = {"Economy", "Labour", "Logistics", "Enterprises"};
const char* const kTabServiceNames[] = {"economy", "labour", "logistics", nullptr};
constexpr int kTabCount      = 4;
constexpr int kEmptyDataRows = 7;

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

// One unmistakable line, in warning colour, when a service is absent. This is
// the whole body: never a sample table and never a plausible-looking number.
void DrawNoData(const char* service)
{
    ImGui::TextColored(kWarn, "NO DATA: %s unavailable", service);
    ImGui::Spacing();
    ImGui::TextColored(kQuiet,
        "The service is not installed, not loaded yet, or has not produced a value.");
}

void DrawLiveTable(const TabData& data, const char* service)
{
    if (data.count <= 0) {
        DrawNoData(service);
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

void DrawStateBudgetSection(const StateBudgetData& b, const char* sym)
{
    if (!b.hasLedger) return;
    const char* currency = (sym && sym[0]) ? sym : "RUB";

    ImGui::Spacing();
    Rule();
    ImGui::TextColored(kInk, "State Budget (%s)", currency);
    if (b.closedYear <= 0) {
        ImGui::SameLine();
        ImGui::TextColored(kQuiet, "  - awaiting first month close");
    }
    ImGui::Spacing();

    char closedHdr[40];
    if (b.closedYear > 0) {
        std::snprintf(closedHdr, sizeof(closedHdr), "Closed %d-%02d",
                      b.closedYear, b.closedMonth);
    } else {
        std::snprintf(closedHdr, sizeof(closedHdr), "Last Closed");
    }

    if (!ImGui::BeginTable("##almanac_state_budget", 3,
            ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerH |
            ImGuiTableFlags_SizingStretchProp)) {
        return;
    }
    ImGui::TableSetupColumn("Budget Line", ImGuiTableColumnFlags_WidthStretch, 1.5f);
    ImGui::TableSetupColumn(closedHdr,     ImGuiTableColumnFlags_WidthStretch, 1.0f);
    ImGui::TableSetupColumn("Live Month",  ImGuiTableColumnFlags_WidthStretch, 1.0f);
    ImGui::TableHeadersRow();

    auto fmtMoney = [](char* buf, size_t n, double val, bool valid) {
        if (!valid) {
            std::snprintf(buf, n, "-");
            return;
        }
        std::snprintf(buf, n, "%+.2f", val);
    };

    struct BudgetLine {
        const char* label;
        double      closedVal;
        double      liveVal;
        bool        isNet;
    };
    const BudgetLine lines[] = {
        {"Monthly Net",              b.closedNet,       b.liveNet,       true },
        {"Wages / Pensions / Welf.", b.closedWages,     b.liveWages,     false},
        {"State Store Sales",        b.closedSales,     b.liveSales,     false},
        {"Wholesale B2B",            b.closedWholesale, b.liveWholesale, false},
        {"Corporate Taxes",          b.closedTaxes,     b.liveTaxes,     false},
        {"Other / Procurement",      b.closedOther,     b.liveOther,     false},
    };

    const bool hasClosed = (b.closedYear > 0);
    for (const BudgetLine& row : lines) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextColored(kInk, "%s", row.label);

        char cBuf[32], lBuf[32];
        fmtMoney(cBuf, sizeof(cBuf), row.closedVal, hasClosed);
        fmtMoney(lBuf, sizeof(lBuf), row.liveVal,   true);

        const ImVec4 cCol = !hasClosed
            ? kQuiet
            : (row.isNet ? (row.closedVal >= 0.0 ? kLive : kWarn) : kQuiet);
        const ImVec4 lCol = row.isNet
            ? (row.liveVal >= 0.0 ? kLive : kWarn)
            : kQuiet;

        ImGui::TableSetColumnIndex(1);
        ImGui::TextColored(cCol, "%s", cBuf);
        ImGui::TableSetColumnIndex(2);
        ImGui::TextColored(lCol, "%s", lBuf);
    }
    ImGui::EndTable();
}

void DrawEconomyControls(const ControlState& ctrl, PanelAction* outAction)
{
    if (!ctrl.hasTaxControls && !ctrl.hasBankControl) return;

    ImGui::Spacing();
    Rule();
    ImGui::TextColored(kInk, "Fiscal Policy Controls");
    ImGui::Spacing();

    auto emit = [outAction](PanelAction::Kind k, int idx, bool flg, float val) {
        if (outAction && outAction->kind == PanelAction::Kind::None) {
            outAction->kind  = k;
            outAction->index = idx;
            outAction->flag  = flg;
            outAction->value = val;
        }
    };

    if (ctrl.hasTaxControls) {
        // VAT
        ImGui::AlignTextToFramePadding();
        ImGui::TextColored(kInk, "VAT (%.0f%%)", ctrl.vatRate * 100.0f);
        ImGui::SameLine(180.0f);
        if (ImGui::SmallButton(ctrl.vatEnabled ? "ON##vat" : "OFF##vat")) {
            emit(PanelAction::Kind::SetTaxEnabled, 0, !ctrl.vatEnabled, 0.0f);
        }
        ImGui::SameLine();
        if (ImGui::SmallButton("-2%##vat")) {
            float v = ctrl.vatRate - 0.02f;
            if (v < 0.0f) v = 0.0f;
            emit(PanelAction::Kind::SetTaxRate, 0, false, v);
        }
        ImGui::SameLine();
        if (ImGui::SmallButton("+2%##vat")) {
            float v = ctrl.vatRate + 0.02f;
            if (v > 1.0f) v = 1.0f;
            emit(PanelAction::Kind::SetTaxRate, 0, false, v);
        }

        // Profit Tax
        ImGui::AlignTextToFramePadding();
        ImGui::TextColored(kInk, "Profit Tax (%.0f%%)", ctrl.profitRate * 100.0f);
        ImGui::SameLine(180.0f);
        if (ImGui::SmallButton(ctrl.profitEnabled ? "ON##prof" : "OFF##prof")) {
            emit(PanelAction::Kind::SetTaxEnabled, 1, !ctrl.profitEnabled, 0.0f);
        }
        ImGui::SameLine();
        if (ImGui::SmallButton("-2%##prof")) {
            float v = ctrl.profitRate - 0.02f;
            if (v < 0.0f) v = 0.0f;
            emit(PanelAction::Kind::SetTaxRate, 1, false, v);
        }
        ImGui::SameLine();
        if (ImGui::SmallButton("+2%##prof")) {
            float v = ctrl.profitRate + 0.02f;
            if (v > 1.0f) v = 1.0f;
            emit(PanelAction::Kind::SetTaxRate, 1, false, v);
        }

        // Property Tax
        ImGui::AlignTextToFramePadding();
        ImGui::TextColored(kInk, "Property (%.0f %s)", ctrl.propertyBase, ctrl.currencySymbol);
        ImGui::SameLine(180.0f);
        if (ImGui::SmallButton(ctrl.propertyEnabled ? "ON##prop" : "OFF##prop")) {
            emit(PanelAction::Kind::SetTaxEnabled, 2, !ctrl.propertyEnabled, 0.0f);
        }
        ImGui::SameLine();
        if (ImGui::SmallButton("-25##prop")) {
            float v = ctrl.propertyBase - 25.0f;
            if (v < 0.0f) v = 0.0f;
            emit(PanelAction::Kind::SetTaxRate, 2, false, v);
        }
        ImGui::SameLine();
        if (ImGui::SmallButton("+25##prop")) {
            emit(PanelAction::Kind::SetTaxRate, 2, false, ctrl.propertyBase + 25.0f);
        }

        ImGui::Spacing();
        if (ImGui::SmallButton("Settle Accrued Taxes Now")) {
            emit(PanelAction::Kind::SettleTaxesNow, 0, false, 0.0f);
        }
    }

    if (ctrl.hasBankControl) {
        ImGui::Spacing();
        ImGui::AlignTextToFramePadding();
        ImGui::TextColored(kInk, "Bank Rate (%.0f%%)", ctrl.bankRate * 100.0);
        ImGui::SameLine(180.0f);
        if (ImGui::SmallButton("-1%##bank")) {
            float v = static_cast<float>(ctrl.bankRate) - 0.01f;
            if (v < 0.0f) v = 0.0f;
            emit(PanelAction::Kind::SetBankRate, 0, false, v);
        }
        ImGui::SameLine();
        if (ImGui::SmallButton("+1%##bank")) {
            float v = static_cast<float>(ctrl.bankRate) + 0.01f;
            if (v > 0.25f) v = 0.25f;
            emit(PanelAction::Kind::SetBankRate, 0, false, v);
        }
    }
}

void DrawLabourControls(const ControlState& ctrl, PanelAction* outAction)
{
    if (!ctrl.hasSectorWages) return;

    ImGui::Spacing();
    Rule();
    ImGui::TextColored(kInk, "Sector Wage Controls (min %.2f %s/day)",
                       ctrl.minWage, ctrl.currencySymbol);
    ImGui::Spacing();

    static const char* const kSectorLabels[6] = {
        "Heavy industry", "Infrastructure", "Care & Education",
        "Light industry", "Services",       "Agriculture",
    };

    for (int s = 0; s < 6; ++s) {
        if (!(ctrl.sectorWages[s] > 0.0f)) continue;
        ImGui::PushID(s);
        ImGui::AlignTextToFramePadding();
        ImGui::TextColored(kInk, "%s (%.2f)", kSectorLabels[s], ctrl.sectorWages[s]);
        ImGui::SameLine(200.0f);
        if (ImGui::SmallButton("-0.20")) {
            float v = ctrl.sectorWages[s] - 0.20f;
            if (v < ctrl.minWage) v = ctrl.minWage;
            if (outAction && outAction->kind == PanelAction::Kind::None) {
                outAction->kind  = PanelAction::Kind::SetSectorWage;
                outAction->index = s;
                outAction->value = v;
            }
        }
        ImGui::SameLine();
        if (ImGui::SmallButton("+0.20")) {
            float v = ctrl.sectorWages[s] + 0.20f;
            if (outAction && outAction->kind == PanelAction::Kind::None) {
                outAction->kind  = PanelAction::Kind::SetSectorWage;
                outAction->index = s;
                outAction->value = v;
            }
        }
        ImGui::PopID();
    }
}

void DrawEnterprisesTable(const EnterpriseList& list, const char* sym)
{
    if (list.totalCount <= 0) {
        ImGui::TextColored(kQuiet,
            "No private enterprises registered yet.");
        ImGui::TextColored(kQuiet,
            "Tag a building [P] or click its Privatise button to register one.");
        return;
    }

    ImGui::TextColored(kQuiet, "Showing %d of %d private enterprise(s) (%s)",
                       list.count, list.totalCount, (sym && sym[0]) ? sym : "RUB");
    ImGui::Spacing();

    if (!ImGui::BeginTable("##almanac_enterprises", 5,
            ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerH |
            ImGuiTableFlags_SizingStretchProp)) {
        return;
    }
    ImGui::TableSetupColumn("Enterprise", ImGuiTableColumnFlags_WidthStretch, 2.0f);
    ImGui::TableSetupColumn("Capital",    ImGuiTableColumnFlags_WidthStretch, 1.0f);
    ImGui::TableSetupColumn("Month Net",  ImGuiTableColumnFlags_WidthStretch, 1.0f);
    ImGui::TableSetupColumn("Markup",     ImGuiTableColumnFlags_WidthStretch, 0.7f);
    ImGui::TableSetupColumn("Debt",       ImGuiTableColumnFlags_WidthStretch, 0.9f);
    ImGui::TableHeadersRow();

    for (int i = 0; i < list.count; ++i) {
        const EnterpriseRow& r = list.rows[i];
        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);
        ImGui::TextColored(r.solvent ? kInk : kWarn, "%s%s",
                           r.name, r.solvent ? "" : " [!]");

        ImGui::TableSetColumnIndex(1);
        ImGui::TextColored(r.capital >= 0.0 ? kInk : kWarn, "%.0f", r.capital);

        ImGui::TableSetColumnIndex(2);
        ImGui::TextColored(r.monthNet >= 0.0 ? kLive : kWarn, "%+.0f", r.monthNet);

        ImGui::TableSetColumnIndex(3);
        ImGui::TextColored(kQuiet, "x%.2f", r.markup);

        ImGui::TableSetColumnIndex(4);
        if (r.debt > 0.5) {
            ImGui::TextColored(kWarn, "%.0f", r.debt);
        } else {
            ImGui::TextColored(kQuiet, "-");
        }
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

bool DrawAlmanacPanel(const PanelModel& model, LauncherRect* outRect,
                      int* activeTab, PanelAction* outAction,
                      const PanelLayout* initialLayout)
{
    if (outRect) *outRect = LauncherRect{};
    if (outAction) *outAction = PanelAction{};
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
    const int   rows    = kEmptyDataRows;
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

    // Initial placement from saved layout (if any) or top-left below the
    // launcher button; user may drag and resize freely afterwards.
    const float initX = (initialLayout && initialLayout->hasSavedPos)
                            ? initialLayout->x : (origin.x + 24.0f);
    const float initY = (initialLayout && initialLayout->hasSavedPos)
                            ? initialLayout->y : (origin.y + 104.0f);
    const float initW = (initialLayout && initialLayout->hasSavedSize)
                            ? initialLayout->w : width;
    const float initH = (initialLayout && initialLayout->hasSavedSize)
                            ? initialLayout->h : (height + 120.0f);
    ImGui::SetNextWindowPos(ImVec2(initX, initY), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(initW, initH), ImGuiCond_FirstUseEver);
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
            if (tab == 3) {
                DrawEnterprisesTable(model.enterprises, model.controls.currencySymbol);
            } else if (model.hasLiveData) {
                DrawLiveTable(model.tabs[tab], kTabServiceNames[tab]);
                if (tab == 0) {
                    DrawStateBudgetSection(model.budget, model.controls.currencySymbol);
                    DrawEconomyControls(model.controls, outAction);
                } else if (tab == 1) {
                    DrawLabourControls(model.controls, outAction);
                }
            } else {
                DrawNoData(kTabServiceNames[tab]);
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
