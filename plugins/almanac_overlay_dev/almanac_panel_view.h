// almanac_panel_view.h - the one panel this plugin draws. Declarations only.
//
// The body is original WRSRE code. It reads a plain value struct and emits ImGui
// calls; it cannot open itself, capture input, reach the game or allocate GPU
// resources. Whoever calls it owns the context, the frame and the surface.
#pragma once

namespace almanac {

struct TabRow {
    char label[64]{};
    char value[64]{};
};

struct TabData {
    static constexpr int kMaxRows = 16;
    TabRow rows[kMaxRows]{};
    int    count = 0;
};

struct ControlState {
    bool   hasTaxControls  = false;
    bool   vatEnabled      = true;
    float  vatRate         = 0.20f;
    bool   profitEnabled   = true;
    float  profitRate      = 0.15f;
    bool   propertyEnabled = true;
    float  propertyBase    = 250.0f;
    bool   hasBankControl  = false;
    double bankRate        = 0.06;
    bool   hasSectorWages  = false;
    float  minWage         = 2.50f;
    float  sectorWages[6]{};
    char   currencySymbol[16] = "RUB";
};

struct PanelAction {
    enum class Kind {
        None = 0,
        SetTaxEnabled,
        SetTaxRate,
        SettleTaxesNow,
        SetBankRate,
        SetSectorWage,
    };
    Kind  kind  = Kind::None;
    int   index = 0;      // taxKind (0..2) or sectorId (0..5)
    bool  flag  = false;  // enabled/disabled
    float value = 0.0f;   // new rate or wage
};

struct PanelLayout {
    bool  hasSavedPos  = false;
    float x            = 24.0f;
    float y            = 104.0f;
    bool  hasSavedSize = false;
    float w            = 560.0f;
    float h            = 420.0f;
};

// Everything the panel displays. Deliberately a value type: no service
// pointers, no game handles, no live repository state. When hasLiveData is
// true, tabs[0..2] hold live read-only values sampled from WRSRE services;
// otherwise the view falls back to its built-in synthetic placeholder tables.
struct PanelModel {
    const char*  title          = "Almanac";
    const char*  subtitle       = "Market economy suite";
    const char*  dataNotice     = "Synthetic placeholder values - not read from your republic.";
    const char*  toggleKeyName  = "F9";
    const char*  buildNotice    = "";
    int          drawnFrames    = 0;
    int          processId      = 0;
    bool         autoShow       = false;
    bool         hasLiveData    = false;
    TabData      tabs[3]{};
    ControlState controls{};
};

// Screen-space bounding box of the floating Almanac launcher button, published
// each frame so WndProc can hit-test and swallow clicks on the button without
// touching ImGui from the window-message path.
struct LauncherRect {
    float left   = 0.0f;
    float top    = 0.0f;
    float right  = 0.0f;
    float bottom = 0.0f;
    bool  valid  = false;
};

// Draw the compact floating launcher button anchored top-left below the HUD.
// Must be called between ImGui::NewFrame() and ImGui::Render(). Returns the
// button's screen-space rect (valid = false if the viewport is too small).
LauncherRect DrawAlmanacLauncherButton(bool panelOpen, bool hovered,
                                       const char* toggleKeyName);

// Draw the panel. Must be called between ImGui::NewFrame() and
// ImGui::Render(), on the owning thread, with a current context.
//
// Supports interactive tab switching (Economy / Labour / Logistics), scrolling,
// dragging, and resizing. Populates `outRect` with the panel's current
// screen-space bounds so WndProc can route and swallow mouse/wheel input over
// the panel without affecting the game world behind it. Returns true while the
// panel is drawn; false only if the viewport is too small to lay out.
bool DrawAlmanacPanel(const PanelModel& model,
                      LauncherRect* outRect = nullptr,
                      int* activeTab = nullptr,
                      PanelAction* outAction = nullptr,
                      const PanelLayout* initialLayout = nullptr);

} // namespace almanac
