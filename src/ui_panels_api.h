// ui_panels_api.h - the in-window row contract.
//
// ui_panels is a Level 2 service: it depends on nothing to draw, and it
// exists so no plugin above it has to know a panel builder's RVA, prologue
// bytes, or the window struct layout.
//
// It owns:
//
//   - the inline post-hooks on the building panel builders (a table of
//     verified RVAs + prologue bytes; the loader memcmps the prologue and
//     refuses the hook on mismatch, so a game update degrades to "no rows",
//     never to a crash);
//   - the row geometry contract: rows append at the bottom edge the game's
//     own builder left in `window+0x250`, one row of `DPI * 35` apart, drawn
//     with the game's own font so they are indistinguishable from stock rows
//     (the pattern `depletion` has used in game since v1.1.1.9 - see
//     docs/17-ui-and-integration.md sections 2 and 10);
//   - the provider table and the per-window append.
//   - (v2) the native widget pass: the exe's own 0..100 slider with -/+
//     buttons (RVA 0x2727F0) and the labelled checkbox (RVA 0x3956F0),
//     called on the render thread inside the same post-hook that prints
//     text rows, with the same `window+0x250` bottom-edge advance
//     (UI investigation.md section 11, live reference cities.cpp).
//
// A provider fills row slots for ONE building. Contract:
//
//   - it runs on the render thread, only while that building's window is
//     open, and must not block, hook, or call back into the loader;
//   - it appends with `out->rows[out->count++]` after checking
//     `out->count < out->capacity` (capacity <= TSM_PANEL_MAX_ROWS);
//   - it returns count unchanged (0) for a building it has nothing to say
//     about - ui_panels then draws nothing and writes nothing back, and the
//     window is byte-identical to vanilla;
//   - `colour` 0 means "use the hook's configured label colour".
//
// A widget provider draws native clickable controls for ONE building.
// Contract:
//
//   - it runs on the render thread, inside the same post-hook pass, AFTER
//     the text rows have been printed and only while that building's
//     window is open;
//   - it must NOT call the exe's widget addresses directly and must NOT
//     read window/game layout itself: it calls only `widgetSlider` /
//     `widgetCheckbox`, which own the DPI geometry, the live gate, the
//     busy gate, the SEH bracket and the `window+0x250` advance;
//   - each helper consumes exactly one standard row (`DPI * 35`) and
//     returns 1 on the exact frame its value changed, 0 otherwise;
//   - commit-on-change only: copy state under the provider's own lock,
//     call the helper with NO lock held, and commit through the
//     provider's own setters only when the helper reports a change;
//   - it does nothing (no helper calls) for a building it has nothing
//     to say about - the window is then byte-identical to the text pass.

#ifndef TSM_UI_PANELS_API_H
#define TSM_UI_PANELS_API_H

#include <stddef.h>   // offsetof for the v2 structSize guard

#ifdef __cplusplus
extern "C" {
#endif

#define TSM_SERVICE_UI_PANELS "ui_panels"

// v2 appends the native-widget tail (registerWidgetProvider plus the two
// immediate-mode helpers). The registry matches versions exactly, so the
// provider dual-publishes the v1 prefix view and the full v2 iface. Same
// precedent as currency (v3/v4), citizen_labor (v2/v3/v4), taxes (v1/v2).
// v3 (City Hall square button): appends ONE tail helper, widgetButton -
// the engine's own 9-patch chrome tile (DrawFrom9Patch + the game's cached
// 9-patch texture), GetMouseSolid+Collision hit-test, hover highlight,
// gui_click audio, and an active/depressed indicator. A v2 provider reads
// exactly the prefix it was compiled against (registerWidgetProvider /
// widgetSlider / widgetCheckbox still sit before this tail); guard new
// calls with TsmUiPanelsHasButton (below). The provider publishes THREE
// views: v1 prefix, v2 prefix, and the full v3 iface - same dual/triple-
// provide precedent as currency (v3/v4), citizen_labor (v2/v3/v4) and
// taxes (v1/v2).
#define TSM_UI_PANELS_VERSION    6u
#define TSM_UI_PANELS_VERSION_V5 5u
#define TSM_UI_PANELS_VERSION_V4 4u
#define TSM_UI_PANELS_VERSION_V3 3u
#define TSM_UI_PANELS_VERSION_V2 2u
#define TSM_UI_PANELS_VERSION_V1 1u

// v-Next (private-economy overhaul): raised 8 -> 12 so the enterprise
// provider can append its M2 rows (Owner, Capital, Sales, Wages/taxes,
// Wage, Labor, Net profit, Investment pool) plus the new solvency Status
// row and the Republic Market Condition row without silently truncating.
// The struct array grows; every provider and ui_panels itself are rebuilt
// together by build.bat, and providers still clamp to out->capacity.
// v2 note: the 12-row budget covers text rows AND widget rows together.
// A widget helper refuses (returns 0, advances nothing) once the budget
// is spent, so a full enterprise card can never be pushed off by widgets.
// v8.5 task: 14, not 12. The enterprise privatisation tile (widgetButton)
// costs 2 content rows in the fallback flow; a fully-loaded private shop
// (11 text rows) + the tile = 13 would no-op at a 12 cap - the button
// would vanish exactly where it is needed. Capacity guards keep mixed
// old/new DLL installs memory-safe in both directions (a provider always
// bounds by ui_panels' capacity first).
#define TSM_PANEL_MAX_ROWS 14
#define TSM_PANEL_LABEL    48
#define TSM_PANEL_VALUE    64

typedef struct TsmPanelRow {
    wchar_t      label[TSM_PANEL_LABEL];
    wchar_t      value[TSM_PANEL_VALUE];
    unsigned long colour;   // 0 = the hook's default label colour
} TsmPanelRow;

typedef struct TsmPanelRows {
    int         capacity;   // slots available; <= TSM_PANEL_MAX_ROWS
    int         count;      // rows filled so far
    TsmPanelRow rows[TSM_PANEL_MAX_ROWS];
} TsmPanelRows;

// Called for one building while its window is open, on the render thread.
typedef void (*TsmPanelRowProvider)(void* building, TsmPanelRows* out,
                                    void* userData);

// v2: called for one building while its window is open, on the render
// thread, after the text rows have been printed. May call widgetSlider /
// widgetCheckbox (valid only inside this callback); must not block, hook,
// or call back into the loader.
typedef void (*TsmPanelWidgetProvider)(void* building, void* userData);

typedef struct TsmUiPanelsApi {
    unsigned structSize;

    // Registration happens during Start (single-threaded; the loader runs
    // every plugin's Init before any Start). Returns a handle >= 1, 0 on
    // a full table or a null callback.
    int  (*registerRowProvider)(TsmPanelRowProvider cb, void* userData);
    void (*unregisterRowProvider)(int handle);

    // Optional. Reserved for a provider whose values change every frame;
    // ui_panels currently rebuilds the row buffer on every append (the
    // providers are copy-under-lock, format-on-read). May be null-safe.
    void (*markDirty)(void* building);

    // --- v2: native widgets (append-only; guard with TsmUiPanelsHasWidgets)
    //
    // registerWidgetProvider / unregisterWidgetProvider behave exactly
    // like their row twins (handle >= 1, 0 on full/null).
    //
    // widgetSlider draws one row: caption printed in the hook's font at
    // the row origin, plus the exe's own 0..100 slider (RVA 0x2727F0)
    // with native -/+ buttons and gui_click audio. value is 0..100
    // (clamped); step is the -/+ increment per click (1..50, cities
    // default 2); enabled gates interaction (ANDed with live).
    // Returns 1 when *value changed on this exact frame.
    //
    // widgetCheckbox draws one row: the exe's own labelled checkbox
    // (RVA 0x3956F0) with a wide-character label. value is 0/1
    // (clamped); colour 0 means the hook's default label colour.
    // Returns 1 when *value toggled on this exact frame.
    //
    // Both consume exactly DPI*35 of window+0x250, count one row against
    // TSM_PANEL_MAX_ROWS, and are no-ops (return 0) outside a widget
    // pass or once the row budget is spent. Both are SEH-bracketed.
    int  (*registerWidgetProvider)(TsmPanelWidgetProvider cb, void* userData);
    void (*unregisterWidgetProvider)(int handle);
    int  (*widgetSlider)(const wchar_t* caption, unsigned* value,
                         int step, int enabled);
    int  (*widgetCheckbox)(unsigned* value, const wchar_t* label,
                           unsigned long colour);

    // --- v3: native square button tile (append-only; guard with TsmUiPanelsHasButton)
    //
    // widgetButton draws ONE square button tile at the row origin, drawn
    // with the engine's own 9-patch chrome (C3D_PANEL2D::DrawFrom9Patch
    // over the game's cached button texture), sized like the City Hall
    // icon buttons (DPI * g_buttonSize, default 36 unscaled px). The wide
    // `label` is printed centered inside the tile (engine default font,
    // colour 0 = the hook's default). Interaction is the engine idiom:
    // C3D_INPUT::GetMouseSolid + C3D_PANEL2D::Collision over the tile rect
    // (hover -> highlight tint), click latch 0xA54E91 consumed on press
    // (gui_click audio via the game's configurator, exactly like the
    // native checkbox), gated on live (window+0x20; the busy gate
    // applies to value widgets only, not buttons). `active`
    // draws the tile in the depressed/highlighted state (panel open).
    // Returns 1 on the exact frame the button was clicked (edge), 0
    // otherwise. v3.4: the hit-test is the engine's own GetMouseSolid +
    // Collision pair (the natives' recipe), so it always agrees with the
    // drawn tile; consumes TWO rows (2 * DPI * 35) of window+0x250 so
    // the native-sized tile (70 units = 2 pitches) gets chrome around it
    // and two rows of the TSM_PANEL_MAX_ROWS budget; no-ops (returns 0)
    // outside a widget pass, with fewer than 2 rows of budget left, or if
    // the engine pieces are unreadable. SEH-bracketed.
    int  (*widgetButton)(const wchar_t* label, int active);

    // --- v4: toolbar-docked small tile + foreign-window widget pass
    // (append-only; guard with TsmUiPanelsHasButtonSmall /
    // TsmUiPanelsHasAppend)
    //
    // widgetButtonSmall is widgetButton docked in the panel's TOP TOOLBAR
    // row - the [demolish][+][-][stats][workers][R][T] strip every building
    // window carries - instead of the tab/icon row. Smaller square
    // (DPI * toolbar_tile_size, INI default 42 units), row top
    // W_Y + W_OFFY + DPI * toolbar_tile_y (INI default 110: the toolbar
    // row the City Hall builder computes as its first row + button
    // square), same X slot so it clears the native toolbar buttons.
    // Every interaction semantic of widgetButton applies unchanged
    // (engine GetMouseSolid + Collision hit-test agreeing with the draw,
    // click latch, two-row budget, SEH). Used by the TAX tile and by
    // enterprise_base's construction-site privatisation tile.
    int  (*widgetButtonSmall)(const wchar_t* label, int active);

    // appendWidgetsToWindow runs the registered widget providers for a
    // window ui_panels does NOT itself hook (e.g. the construction-site
    // panel builder 0x7838E0, which private_construction owns at the
    // prologue). The caller must invoke it AFTER that window's builder
    // has run for the frame, on the render thread, with the game and
    // window pointers the builder received. Geometry (x/y/dpi/step) is
    // read from the window struct exactly as the hooked builders do it;
    // NO text rows are appended (widgets only). Returns 1 if the pass
    // ran, 0 if skipped (disabled, hidden window, no sane bottom edge).
    int  (*appendWidgetsToWindow)(void* game, void* window);

    // --- v5: person-surface row providers (append-only; guard with
    //     TsmUiPanelsHasPersonRows)
    //
    // THE PROBLEM THIS SOLVES. window+0x240 is a UNION: a Building* on
    // every building panel, a Person* on the citizen panel (kind 2,
    // builder 0x7CEEB0). Before v5 there was one row table and one widget
    // table, and BOTH loops handed whatever sat in that slot to every
    // provider. The moment a citizen panel opened, every building-only
    // provider was asked to describe a Person:
    //
    //   citizen_labor, enterprise_base and taxes all called
    //   TsmBuildingType()
    //     -> *(void**)(person + 0x318), which is not a type descriptor but
    //        the FIFTH demand record (0x118 + 4*0x80) = the float pair
    //        {remaining, rate};
    //     -> *(int*)(that + 0x360) took an access violation;
    //     -> ui_panels dropped that provider FOR THE SESSION.
    //
    // A live log of this is six faults in 7 ms and then a dead UI:
    //   provider 1, 2, 3 faulted          (citizen_labor, enterprise_base,
    //                                      taxes - the ROW table)
    //   widget provider 1, 2, 3 faulted   (the same three - the WIDGET
    //                                      table, which carried the PRIV
    //                                      tile and the TAX tile)
    //
    // So the two surfaces get two tables. registerRowProvider is now
    // BUILDING-ONLY and registerPersonRowProvider is PERSON-ONLY; a
    // provider registered in one is never invoked for the other. Existing
    // building-only providers need NO source change - they simply stop
    // being called on a surface they were never written for. It is safe by
    // construction instead of by every provider remembering to
    // self-validate.
    //
    // The callback signature is identical to TsmPanelRowProvider: the
    // `building` argument is simply a Person* on this table.
    //
    // Registration semantics match the v1 twins (handle >= 1, 0 on a full
    // table or a null callback).
    int  (*registerPersonRowProvider)(TsmPanelRowProvider cb, void* userData);
    void (*unregisterPersonRowProvider)(int handle);

    // --- v6: the wide (full-width) button (append-only; guard with
    //     TsmUiPanelsHasWideButton)
    //
    // widgetWideButton draws the SAME button as widgetButton - same native
    // 9-patch/textured fill, same engine hit-test, same click latch, same
    // two-row budget, same SEH - but as a horizontal BAR instead of a
    // square tile:
    //
    //     width  = DPI * wide_button_width  (INI, default 280 units)
    //     height = DPI * 2 * row step       (ONE row of ~70, the same
    //                                        footprint widgetButton takes)
    //
    // It exists because a 40-unit toolbar tile is unreadable on a panel
    // that is already small, and because a square cannot hold a sentence:
    // "MAKE PRIVATE" needs a bar. The label is printed centred in the bar.
    //
    // Placement: the bar starts at the row left edge (the x every row
    // prints at) and flows to the next bottom edge, exactly like
    // widgetButton. Nothing else about the contract changes - a provider
    // that can draw widgetButton can draw this by changing one call.
    int  (*widgetWideButton)(const wchar_t* label, int active);
} TsmUiPanelsApi;

// Guard: does this iface carry the v2 widget tail?
static inline int TsmUiPanelsHasWidgets(const TsmUiPanelsApi* api)
{
    return api &&
        api->structSize >= offsetof(TsmUiPanelsApi, widgetCheckbox) +
                              sizeof(api->widgetCheckbox) &&
        api->registerWidgetProvider && api->unregisterWidgetProvider &&
        api->widgetSlider && api->widgetCheckbox;
}

// Guard: does this iface carry the v3 button tail?
static inline int TsmUiPanelsHasButton(const TsmUiPanelsApi* api)
{
    return TsmUiPanelsHasWidgets(api) &&
        api->structSize >= offsetof(TsmUiPanelsApi, widgetButton) +
                              sizeof(api->widgetButton) &&
        api->widgetButton;
}

// Guard: does this iface carry the v4 small (toolbar-docked) button?
static inline int TsmUiPanelsHasButtonSmall(const TsmUiPanelsApi* api)
{
    return TsmUiPanelsHasButton(api) &&
        api->structSize >= offsetof(TsmUiPanelsApi, widgetButtonSmall) +
                              sizeof(api->widgetButtonSmall) &&
        api->widgetButtonSmall;
}

// Guard: does this iface carry the v4 foreign-window widget pass?
static inline int TsmUiPanelsHasAppend(const TsmUiPanelsApi* api)
{
    return TsmUiPanelsHasButton(api) &&
        api->structSize >= offsetof(TsmUiPanelsApi, appendWidgetsToWindow) +
                              sizeof(api->appendWidgetsToWindow) &&
        api->appendWidgetsToWindow;
}

// Guard: does this iface carry the v5 person-surface row table? A provider
// that wants the citizen panel MUST check this - against a pre-v5 build it
// has nowhere to register, and falling back to registerRowProvider would
// re-create the union bug this tail exists to remove.
static inline int TsmUiPanelsHasPersonRows(const TsmUiPanelsApi* api)
{
    return api &&
        api->structSize >= offsetof(TsmUiPanelsApi, registerPersonRowProvider) +
                              sizeof(api->registerPersonRowProvider) &&
        api->registerPersonRowProvider;
}

// Guard: does this iface carry the v6 wide button?
static inline int TsmUiPanelsHasWideButton(const TsmUiPanelsApi* api)
{
    return api &&
        api->structSize >= offsetof(TsmUiPanelsApi, widgetWideButton) +
                          sizeof(api->widgetWideButton) &&
        api->widgetWideButton != 0;
}

#ifdef __cplusplus
}
#endif

#endif // TSM_UI_PANELS_API_H
