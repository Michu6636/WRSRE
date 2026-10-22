// ui_overlay_api.h - the HUD contract.
//
// ui_overlay is a Level 0 service: it depends on nothing, and it exists
// so that no plugin above it has to know how the HUD is drawn.
//
// It owns:
//
//   - the C3D_TERRAIN::Render hook, which is the de facto per-frame
//     draw callback every plugin in this project uses;
//   - the one lookup of C3D_FONTMANAGER::PrintLeftUnicodeNoArg, and
//     the outlined-text helper built on it;
//   - the DPI scalar and the panel font handle, so a plugin that has
//     to do its own layout maths can;
//   - the visibility toggle and its hotkey.
//
// A plugin does not draw by hooking TerrainRender itself. It registers
// a drawer with registerDrawer, and ui_overlay calls it once per frame
// with the column position already resolved. Drawers chain: each is
// handed a y and returns the y it advanced to, so a drawer that draws
// three lines naturally leaves the next drawer starting below them.
//
//
// WHAT THE DRAWER IS HANDED, AND WHY
//
//   userData   the pointer passed to registerDrawer.
//   x          the column's left edge, in screen pixels.
//   y          where this drawer's first line should go.
//   step       one line of vertical advance, in screen pixels. A drawer
//              that draws N lines with fixed spacing uses `step` for
//              the spacing; a drawer that draws text at a fixed size
//              uses `step` for the font scale too.
//
// The drawer returns the y it wants the next drawer to start at. A
// drawer that draws nothing returns y unchanged.
//
// There is no "dpi" argument. A drawer that needs the raw scalar calls
// getDPI(). Almost none do - the two that need anything beyond x/y/step
// are drawing a fixed-size icon or aligning to a fixed-size widget, and
// those can call getDPI() from inside.

#ifndef TSM_UI_OVERLAY_API_H
#define TSM_UI_OVERLAY_API_H

#ifdef __cplusplus
extern "C" {
#endif

#define TSM_SERVICE_UI_OVERLAY "ui_overlay"
#define TSM_UI_OVERLAY_VERSION 1u

// A drawer. Called once per frame, in registration order, on the render
// thread. Must not block. A faulting drawer is disabled for the session
// and every other drawer carries on.
//
// The `text` passed to drawText and drawTextOutlined is a wide string
// and is copied during the call - the caller's buffer can be reused
// immediately after. It is not NUL-checked, so do not pass null.
typedef float (*TsmHudDrawer)(void* userData,
                              float x, float y, float step);

typedef struct TsmUiOverlayApi {
    unsigned structSize;

    // --- text --------------------------------------------------------------
    //
    // These are the two primitives every current plugin uses. drawText
    // issues a single print call in the requested colour.
    // drawTextOutlined adds four black offsets around the requested
    // position first, so the text is legible against any background.
    // Both are dpi-aware: the outline offset scales with the display,
    // so text looks the same thickness everywhere.

    void (*drawText)(float x, float y, unsigned long color,
                     const wchar_t* text);
    void (*drawTextOutlined)(float x, float y, unsigned long color,
                             const wchar_t* text);

    // --- layout context ----------------------------------------------------
    //
    // getDPI returns the scalar every size in the HUD is scaled by.
    // 1.0 on a standard 96 dpi display, 1.25 on 120, 2.0 on 192.
    // Re-read each frame so a window dragged to a differently-scaled
    // monitor updates immediately.
    //
    // getColumnX returns the column's left edge. Almost every drawer
    // ignores this and uses the `x` it is handed; the accessor is for
    // a plugin that wants to draw something *outside* the column and
    // align it relative to the column's edge.

    float (*getDPI)(void);
    float (*getColumnX)(void);

    // --- visibility --------------------------------------------------------
    //
    // Whether the HUD is currently shown. Toggled by the hotkey in
    // ui_overlay.ini, or programmatically. A drawer registered via
    // registerDrawer is not called while the HUD is hidden, so a plugin
    // does not have to check isVisible itself unless it wants to know
    // for some other reason.

    int  (*isVisible)(void);
    void (*setVisible)(int visible);

    // --- drawers -----------------------------------------------------------

    int  (*registerDrawer)(TsmHudDrawer cb, void* userData);
    void (*unregisterDrawer)(int handle);
} TsmUiOverlayApi;

#ifdef __cplusplus
}
#endif

#endif // TSM_UI_OVERLAY_API_H