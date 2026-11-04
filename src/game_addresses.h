// game_addresses.h - centralized SOVIET64.exe RVA table, PE build gate,
// and runtime `addresses.ini` override loader.
//
// WHY THIS HEADER EXISTS
// ----------------------
// Every plugin in WRSRE targets `SOVIET64.exe` (64-bit, ASLR enabled).
// Struct member offsets (`src/game_world.h`) rarely move across minor game
// updates or DLCs unless 3Division inserts fields inside CBuilding/CPerson,
// whereas `.text`, `.rdata`, and `.data` RVAs shift on EVERY recompile.
//
// Previously:
//   1. RVAs were scattered across 10+ `.cpp` files (`0x9D4F10` in four
//      places, UI globals in two, direct function pointers in four).
//   2. Inline hooks (`InstallInlineHook`) checked prologue bytes and refused
//      safely on a new game build, EXCEPT unguarded global reads
//      (`TSM_GO_RVA`, `RVA_FONT`, `RVA_DPI`) and direct native function calls
//      (`0x83CA50` relocate, `0x13FE70` abandon, `0x822FD0` eject worker,
//      `0x2727F0` slider, `0x3956F0` checkbox) had no build gate and could
//      fault if launched with `--ignore-version` on a new executable.
//
// HOW IT WORKS
// ------------
//   - Compiled-in defaults target `SOVIET64.exe` v1.1.1.9
//     (PE TimeDateStamp `0x6A3EB6AD`, 2026-06-26 17:28:13 UTC).
//   - `TsmBind(host)` in `tesmio_plugin.h` calls `TsmAddrInit(host)` once per
//     plugin DLL during `TsmPluginInit` (zero TSM_API_VERSION bump; works
//     with any existing v3/v4 `tesmioloader.dll`).
//   - `TsmAddrInit` checks the loaded executable's PE `TimeDateStamp`:
//       * Matches `0x6A3EB6AD` (and no conflicting `addresses.ini`):
//         `exeVerified = 1`, compiled-in v1.1.1.9 RVAs active.
//       * Matches `[build] target_stamp = 0x...` in `addresses.ini`
//         (generated in ~2 seconds by `python tools/scan_exe_update.py`):
//         `exeVerified = 1`, relocated RVAs loaded dynamically from
//         `addresses.ini` WITHOUT requiring a C++ recompile of every plugin!
//         Setting any single RVA to `0` in `addresses.ini` cleanly disables
//         only that specific feature/hook while the rest of the mod runs.
//       * Unknown `SOVIET64.exe` build with no matching `addresses.ini`
//         (unless `[tesmioloader] allow_unverified_exe = 1`):
//         `exeVerified = 0`. `TsmGameObject()` and `TsmCallableCodePtr()`
//         return NULL, pausing raw RVA reads/calls cleanly instead of
//         crashing or corrupting a save.

#ifndef TSM_GAME_ADDRESSES_H
#define TSM_GAME_ADDRESSES_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "tesmio_api.h"

// -----------------------------------------------------------------------------
// Baseline build identity: SOVIET64.exe v1.1.1.9
// -----------------------------------------------------------------------------

#define TSM_BASELINE_EXE_STAMP          0x6A3EB6ADu
#define TSM_ADDRESSES_INI               "addresses.ini"

// -----------------------------------------------------------------------------
// Compiled-in v1.1.1.9 RVA defaults (single source of truth)
// -----------------------------------------------------------------------------

// Core globals
#define TSM_RVA_DEFAULT_GAME_OBJECT     0x9D4F10u
#define TSM_RVA_DEFAULT_SIM_TIMER       0x9D4EE0u
#define TSM_RVA_DEFAULT_DAY_LENGTH      0x90AA78u
#define TSM_RVA_DEFAULT_DAYTICK_POWER   0x334AA0u
#define TSM_RVA_DEFAULT_MENU_VER_SITE   0x28B5CCu

// UI / Font / Layout globals
#define TSM_RVA_DEFAULT_DPI             0x992088u
#define TSM_RVA_DEFAULT_FONTMGR         0x996FB0u
#define TSM_RVA_DEFAULT_FONT_DEFAULT    0x994200u
#define TSM_RVA_DEFAULT_FONT_ALT_A      0x995220u
#define TSM_RVA_DEFAULT_FONT_ALT_B      0x995228u
#define TSM_RVA_DEFAULT_FONT_SIZE       0x99528Cu

#define TSM_RVA_DEFAULT_ROW_STEP        0x90A9CCu
#define TSM_RVA_DEFAULT_X_INDENT        0x90AB98u
#define TSM_RVA_DEFAULT_X_BACK          0x90A8CCu
#define TSM_RVA_DEFAULT_CHECK_SIZE      0x90A928u
#define TSM_RVA_DEFAULT_TAB_BTN_SIZE    0x90AAB8u
#define TSM_RVA_DEFAULT_TAB_ROW_DY      0x90AB84u
#define TSM_RVA_DEFAULT_ICON_BTN_SIZE   0x90AAA8u

// C3D_PANEL2D singleton + draw-state cluster (+0x288 .. +0x2AC from panel2d)
#define TSM_RVA_DEFAULT_PANEL2D         0x9BE060u
#define TSM_RVA_DEFAULT_P2D_RECT_W      0x9BE2E8u
#define TSM_RVA_DEFAULT_P2D_RECT_H      0x9BE2ECu
#define TSM_RVA_DEFAULT_P2D_RECT_X      0x9BE2F0u
#define TSM_RVA_DEFAULT_P2D_RECT_Y      0x9BE2F4u
#define TSM_RVA_DEFAULT_P2D_ROT         0x9BE2F8u
#define TSM_RVA_DEFAULT_P2D_TINT        0x9BE30Cu

#define TSM_RVA_DEFAULT_TINT_HOVER      0x90C4C0u
#define TSM_RVA_DEFAULT_TINT_NORMAL     0x90C100u
#define TSM_RVA_DEFAULT_9P_BORDER       0x90A9A0u
#define TSM_RVA_DEFAULT_9P_UV_A         0x909D04u
#define TSM_RVA_DEFAULT_9P_UV_B         0x909E78u
#define TSM_RVA_DEFAULT_UV_FULL         0x909F58u
#define TSM_RVA_DEFAULT_PANEL_STATE     0x9EAD08u

// C3D_INPUT singleton + key/mouse edge cluster (+0x01, +0x101, +0x300, +0x301)
#define TSM_RVA_DEFAULT_INPUT           0xA54B90u
#define TSM_RVA_DEFAULT_ESC_DOWN        0xA54B91u
#define TSM_RVA_DEFAULT_ESC_EDGE        0xA54C91u
#define TSM_RVA_DEFAULT_MOUSE_DOWN      0xA54E90u
#define TSM_RVA_DEFAULT_CLICK_LATCH     0xA54E91u

// World-size float cluster (`map_expander`)
#define TSM_RVA_DEFAULT_WORLD_SIZE      0x90B1A4u
#define TSM_RVA_DEFAULT_HALF_WORLD      0x90B180u
#define TSM_RVA_DEFAULT_GATE_HI         0x90B178u
#define TSM_RVA_DEFAULT_GATE_LO         0x90BD2Cu
#define TSM_RVA_DEFAULT_MARGIN_HI       0x90B164u
#define TSM_RVA_DEFAULT_MARGIN_LO       0x90BD20u

// Direct-callable native engine helpers
#define TSM_RVA_DEFAULT_RELOCATE_PERSON 0x83CA50u  // FUN_14083CA50 (enterprise_base)
#define TSM_RVA_DEFAULT_ABANDON_BLD     0x13FE70u  // FUN_14013FE70 (enterprise_base)
#define TSM_RVA_DEFAULT_EJECT_WORKER    0x822FD0u  // FUN_140822FD0 (citizen_labor)
#define TSM_RVA_DEFAULT_UI_SLIDER       0x2727F0u  // FUN_1402727F0 (ui_panels)
#define TSM_RVA_DEFAULT_UI_CHECKBOX     0x3956F0u  // FUN_1403956F0 (ui_panels)
#define TSM_RVA_DEFAULT_PLAY_GUI_SOUND  0x3FFAD0u  // FUN_1403FFAD0 (ui_panels)
#define TSM_RVA_DEFAULT_CLIP_PUSH       0x4461F0u  // FUN_1404461F0 (ui_panels)
#define TSM_RVA_DEFAULT_VEC_PUSH_BACK   0x08E050u  // FUN_14008E050 (cataloged: private_logistics)
#define TSM_RVA_DEFAULT_TASKCFG_CTOR    0x1512A0u  // FUN_1401512A0 (cataloged: private_logistics)

// Inline & call-site hook targets
#define TSM_RVA_DEFAULT_LABOR_HOOK      0x1D3797u  // FUN_1401D2DD0+0x9C7 (citizen_labor)
#define TSM_RVA_DEFAULT_LABOR_JBE       0x1D37A9u  // +18 stolen bytes
#define TSM_RVA_DEFAULT_CHECKOUT_HOOK   0x1725DEu  // FUN_140171E10+0x7CE (citizen_retail)
#define TSM_RVA_DEFAULT_CHECKOUT_ALLOW  0x1725F0u  // +18 stolen bytes
#define TSM_RVA_DEFAULT_CHECKOUT_ABORT  0x17293Au  // skip-demand target

#define TSM_RVA_DEFAULT_WIN_DISPATCH    0x6E9B80u  // FUN_1406E9B80 window dispatcher
#define TSM_RVA_DEFAULT_PANEL_SHOP      0x7272F0u
#define TSM_RVA_DEFAULT_PANEL_RESIDENCE 0x761300u
#define TSM_RVA_DEFAULT_PANEL_PUB       0x723950u
#define TSM_RVA_DEFAULT_PANEL_FACTORY   0x799630u
#define TSM_RVA_DEFAULT_PANEL_CITYHALL  0x72DBF0u
#define TSM_RVA_DEFAULT_PANEL_CITIZEN   0x7CEEB0u
#define TSM_RVA_DEFAULT_PANEL_DO        0x741170u
#define TSM_RVA_DEFAULT_PANEL_STORAGE   0x4CABD0u
#define TSM_RVA_DEFAULT_STORAGE_CALL    0x6EA80Cu
#define TSM_RVA_DEFAULT_PANEL_WAREHOUSE 0x7FC880u
#define TSM_RVA_DEFAULT_WAREHOUSE_GUARD 0x6EA5E3u
#define TSM_RVA_DEFAULT_PANEL_STOR_TD5  0x71D290u
#define TSM_RVA_DEFAULT_STOR_TD5_GUARD  0x6EAE07u
#define TSM_RVA_DEFAULT_PANEL_CONSTRUCT 0x7838E0u  // private_construction
#define TSM_RVA_DEFAULT_LOGISTICS_XFER  0x420350u  // cataloged: private_logistics

// -----------------------------------------------------------------------------
// Per-DLL runtime address state
// -----------------------------------------------------------------------------

struct TsmAddressTable {
    int      initialized;
    int      exeVerified;       // 1 = stamp matches baseline OR addresses.ini target_stamp
    int      usingIniOverrides; // 1 = loaded matching overrides from addresses.ini
    int      loggedUnverified;  // 1 = emitted one-shot pause warning for this plugin
    uint32_t exeStamp;          // live PE TimeDateStamp of SOVIET64.exe (0 if synthetic/test)
    uint32_t targetStamp;       // expected stamp (0x6A3EB6AD or addresses.ini target_stamp)
    uint32_t textBegin;         // first executable section RVA (0x1000 fallback)
    uint32_t textEnd;           // end of executable section RVA (exeSize fallback)

    // Core globals
    uint32_t game_object;
    uint32_t sim_timer;
    uint32_t day_length;
    uint32_t daytick_power;
    uint32_t menu_version_site;

    // UI / Font / Layout globals
    uint32_t dpi;
    uint32_t fontmgr;
    uint32_t font_default;
    uint32_t font_alt_a;
    uint32_t font_alt_b;
    uint32_t font_size;

    uint32_t row_step;
    uint32_t x_indent;
    uint32_t x_back;
    uint32_t check_size;
    uint32_t tab_btn_size;
    uint32_t tab_row_dy;
    uint32_t icon_btn_size;

    uint32_t panel2d;
    uint32_t p2d_rect_w;
    uint32_t p2d_rect_h;
    uint32_t p2d_rect_x;
    uint32_t p2d_rect_y;
    uint32_t p2d_rot;
    uint32_t p2d_tint;

    uint32_t tint_hover;
    uint32_t tint_normal;
    uint32_t ninep_border;
    uint32_t ninep_uv_a;
    uint32_t ninep_uv_b;
    uint32_t uv_full;
    uint32_t panel_state;

    uint32_t input;
    uint32_t esc_down;
    uint32_t esc_edge;
    uint32_t mouse_down;
    uint32_t click_latch;

    // Map bounds
    uint32_t world_size;
    uint32_t half_world;
    uint32_t gate_hi;
    uint32_t gate_lo;
    uint32_t margin_hi;
    uint32_t margin_lo;

    // Direct-callable native helpers
    uint32_t relocate_person;
    uint32_t abandon_building;
    uint32_t eject_worker;
    uint32_t ui_slider;
    uint32_t ui_checkbox;
    uint32_t play_gui_sound;
    uint32_t clip_push;
    uint32_t vec_push_back;
    uint32_t taskcfg_ctor;

    // Hook sites & panel builders
    uint32_t labor_mobility_hook;
    uint32_t labor_mobility_jbe;
    uint32_t retail_checkout_hook;
    uint32_t retail_checkout_allow;
    uint32_t retail_checkout_abort;

    uint32_t window_dispatch;
    uint32_t panel_shop;
    uint32_t panel_residence;
    uint32_t panel_pub;
    uint32_t panel_factory;
    uint32_t panel_cityhall;
    uint32_t panel_citizen;
    uint32_t panel_do;
    uint32_t panel_storage;
    uint32_t storage_callsite;
    uint32_t panel_warehouse;
    uint32_t warehouse_guard;
    uint32_t panel_storage_td5;
    uint32_t storage_td5_guard;
    uint32_t panel_construction;
    uint32_t logistics_transfer;
};

static TsmAddressTable g_tsmAddr = {
    0, 1, 0, 0,
    0u, TSM_BASELINE_EXE_STAMP, 0x1000u, 0x900000u,

    TSM_RVA_DEFAULT_GAME_OBJECT,
    TSM_RVA_DEFAULT_SIM_TIMER,
    TSM_RVA_DEFAULT_DAY_LENGTH,
    TSM_RVA_DEFAULT_DAYTICK_POWER,
    TSM_RVA_DEFAULT_MENU_VER_SITE,

    TSM_RVA_DEFAULT_DPI,
    TSM_RVA_DEFAULT_FONTMGR,
    TSM_RVA_DEFAULT_FONT_DEFAULT,
    TSM_RVA_DEFAULT_FONT_ALT_A,
    TSM_RVA_DEFAULT_FONT_ALT_B,
    TSM_RVA_DEFAULT_FONT_SIZE,

    TSM_RVA_DEFAULT_ROW_STEP,
    TSM_RVA_DEFAULT_X_INDENT,
    TSM_RVA_DEFAULT_X_BACK,
    TSM_RVA_DEFAULT_CHECK_SIZE,
    TSM_RVA_DEFAULT_TAB_BTN_SIZE,
    TSM_RVA_DEFAULT_TAB_ROW_DY,
    TSM_RVA_DEFAULT_ICON_BTN_SIZE,

    TSM_RVA_DEFAULT_PANEL2D,
    TSM_RVA_DEFAULT_P2D_RECT_W,
    TSM_RVA_DEFAULT_P2D_RECT_H,
    TSM_RVA_DEFAULT_P2D_RECT_X,
    TSM_RVA_DEFAULT_P2D_RECT_Y,
    TSM_RVA_DEFAULT_P2D_ROT,
    TSM_RVA_DEFAULT_P2D_TINT,

    TSM_RVA_DEFAULT_TINT_HOVER,
    TSM_RVA_DEFAULT_TINT_NORMAL,
    TSM_RVA_DEFAULT_9P_BORDER,
    TSM_RVA_DEFAULT_9P_UV_A,
    TSM_RVA_DEFAULT_9P_UV_B,
    TSM_RVA_DEFAULT_UV_FULL,
    TSM_RVA_DEFAULT_PANEL_STATE,

    TSM_RVA_DEFAULT_INPUT,
    TSM_RVA_DEFAULT_ESC_DOWN,
    TSM_RVA_DEFAULT_ESC_EDGE,
    TSM_RVA_DEFAULT_MOUSE_DOWN,
    TSM_RVA_DEFAULT_CLICK_LATCH,

    TSM_RVA_DEFAULT_WORLD_SIZE,
    TSM_RVA_DEFAULT_HALF_WORLD,
    TSM_RVA_DEFAULT_GATE_HI,
    TSM_RVA_DEFAULT_GATE_LO,
    TSM_RVA_DEFAULT_MARGIN_HI,
    TSM_RVA_DEFAULT_MARGIN_LO,

    TSM_RVA_DEFAULT_RELOCATE_PERSON,
    TSM_RVA_DEFAULT_ABANDON_BLD,
    TSM_RVA_DEFAULT_EJECT_WORKER,
    TSM_RVA_DEFAULT_UI_SLIDER,
    TSM_RVA_DEFAULT_UI_CHECKBOX,
    TSM_RVA_DEFAULT_PLAY_GUI_SOUND,
    TSM_RVA_DEFAULT_CLIP_PUSH,
    TSM_RVA_DEFAULT_VEC_PUSH_BACK,
    TSM_RVA_DEFAULT_TASKCFG_CTOR,

    TSM_RVA_DEFAULT_LABOR_HOOK,
    TSM_RVA_DEFAULT_LABOR_JBE,
    TSM_RVA_DEFAULT_CHECKOUT_HOOK,
    TSM_RVA_DEFAULT_CHECKOUT_ALLOW,
    TSM_RVA_DEFAULT_CHECKOUT_ABORT,

    TSM_RVA_DEFAULT_WIN_DISPATCH,
    TSM_RVA_DEFAULT_PANEL_SHOP,
    TSM_RVA_DEFAULT_PANEL_RESIDENCE,
    TSM_RVA_DEFAULT_PANEL_PUB,
    TSM_RVA_DEFAULT_PANEL_FACTORY,
    TSM_RVA_DEFAULT_PANEL_CITYHALL,
    TSM_RVA_DEFAULT_PANEL_CITIZEN,
    TSM_RVA_DEFAULT_PANEL_DO,
    TSM_RVA_DEFAULT_PANEL_STORAGE,
    TSM_RVA_DEFAULT_STORAGE_CALL,
    TSM_RVA_DEFAULT_PANEL_WAREHOUSE,
    TSM_RVA_DEFAULT_WAREHOUSE_GUARD,
    TSM_RVA_DEFAULT_PANEL_STOR_TD5,
    TSM_RVA_DEFAULT_STOR_TD5_GUARD,
    TSM_RVA_DEFAULT_PANEL_CONSTRUCT,
    TSM_RVA_DEFAULT_LOGISTICS_XFER
};

// -----------------------------------------------------------------------------
// Portable PE64 header parser & INI helpers (no <windows.h> struct dependency)
// -----------------------------------------------------------------------------

static inline bool TsmParseHexOrUint(const char* s, uint32_t* outVal)
{
    if (!s || !outVal) return false;
    while (*s == ' ' || *s == '\t') s++;
    if (!*s) return false;
    char* end = nullptr;
    unsigned long v = strtoul(s, &end, 0);
    if (end == s) return false;
    while (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n') end++;
    if (*end != '\0') return false;
    *outVal = (uint32_t)v;
    return true;
}

static inline bool TsmReadRvaOverride(const TsmHost* host, const char* key,
                                      uint32_t* field, bool* wasSet = nullptr)
{
    if (wasSet) *wasSet = false;
    if (!host || !host->configString || !key || !field) return false;
    char buf[64] = {0};
    host->configString(TSM_ADDRESSES_INI, "rva", key, buf, (int)sizeof(buf), "");
    if (!buf[0]) return false;
    uint32_t v = 0;
    if (!TsmParseHexOrUint(buf, &v)) return false;
    *field = v;
    if (wasSet) *wasSet = true;
    return true;
}

// Inspects the in-memory PE64 header of `SOVIET64.exe` safely via `readablePtr`.
// Populates `exeStamp` and executable section bounds `[textBegin, textEnd)`.
static inline bool TsmInspectExePe(const unsigned char* exeBase, size_t exeSize,
                                   const TsmHost* host,
                                   uint32_t* outStamp,
                                   uint32_t* outTextBegin,
                                   uint32_t* outTextEnd)
{
    if (!exeBase || exeSize < 0x1000) return false;
    if (host && host->readablePtr && !host->readablePtr(exeBase, 0x400))
        return false;
    if (exeBase[0] != 'M' || exeBase[1] != 'Z') return false;

    uint32_t lfanew = 0;
    memcpy(&lfanew, exeBase + 0x3C, 4);
    if (lfanew < 0x40 || lfanew > 0x100000 || (size_t)lfanew + 0x108 > exeSize)
        return false;
    if (host && host->readablePtr && !host->readablePtr(exeBase + lfanew, 0x108))
        return false;
    if (memcmp(exeBase + lfanew, "PE\0\0", 4) != 0) return false;

    uint16_t machine = 0, numSec = 0, optSize = 0, magic = 0;
    uint32_t stamp = 0;
    memcpy(&machine, exeBase + lfanew + 4, 2);
    memcpy(&numSec,  exeBase + lfanew + 6, 2);
    memcpy(&stamp,   exeBase + lfanew + 8, 4);
    memcpy(&optSize, exeBase + lfanew + 20, 2);
    memcpy(&magic,   exeBase + lfanew + 24, 2);
    if (machine != 0x8664u || magic != 0x020Bu) return false;

    if (outStamp) *outStamp = stamp;

    uint32_t secOff = lfanew + 24u + (uint32_t)optSize;
    if (numSec > 0 && numSec <= 96 &&
        (size_t)secOff + (size_t)numSec * 40u <= exeSize &&
        (!host || !host->readablePtr ||
         host->readablePtr(exeBase + secOff, (size_t)numSec * 40u)))
    {
        uint32_t minExec = 0xFFFFFFFFu;
        uint32_t maxExec = 0u;
        for (uint16_t i = 0; i < numSec; i++) {
            const unsigned char* sh = exeBase + secOff + (size_t)i * 40u;
            uint32_t vsize = 0, vaddr = 0, flags = 0;
            memcpy(&vsize, sh + 8, 4);
            memcpy(&vaddr, sh + 12, 4);
            memcpy(&flags, sh + 36, 4);
            // IMAGE_SCN_MEM_EXECUTE (0x20000000)
            if ((flags & 0x20000000u) != 0 && vsize > 0) {
                if (vaddr < minExec) minExec = vaddr;
                if (vaddr + vsize > maxExec) maxExec = vaddr + vsize;
            }
        }
        if (minExec < maxExec) {
            if (outTextBegin) *outTextBegin = minExec;
            if (outTextEnd)   *outTextEnd   = maxExec;
        }
    }
    return true;
}

static inline void TsmAddrApplyOverrides(const TsmHost* host)
{
    TsmReadRvaOverride(host, "game_object",      &g_tsmAddr.game_object);
    TsmReadRvaOverride(host, "sim_timer",        &g_tsmAddr.sim_timer);
    TsmReadRvaOverride(host, "day_length",       &g_tsmAddr.day_length);
    TsmReadRvaOverride(host, "daytick_power",    &g_tsmAddr.daytick_power);
    TsmReadRvaOverride(host, "menu_version_site",&g_tsmAddr.menu_version_site);

    TsmReadRvaOverride(host, "dpi",              &g_tsmAddr.dpi);
    TsmReadRvaOverride(host, "fontmgr",          &g_tsmAddr.fontmgr);
    TsmReadRvaOverride(host, "font_default",     &g_tsmAddr.font_default);
    TsmReadRvaOverride(host, "font_alt_a",       &g_tsmAddr.font_alt_a);
    TsmReadRvaOverride(host, "font_alt_b",       &g_tsmAddr.font_alt_b);
    TsmReadRvaOverride(host, "font_size",        &g_tsmAddr.font_size);

    TsmReadRvaOverride(host, "row_step",         &g_tsmAddr.row_step);
    TsmReadRvaOverride(host, "x_indent",         &g_tsmAddr.x_indent);
    TsmReadRvaOverride(host, "x_back",           &g_tsmAddr.x_back);
    TsmReadRvaOverride(host, "check_size",       &g_tsmAddr.check_size);
    TsmReadRvaOverride(host, "tab_btn_size",     &g_tsmAddr.tab_btn_size);
    TsmReadRvaOverride(host, "tab_row_dy",       &g_tsmAddr.tab_row_dy);
    TsmReadRvaOverride(host, "icon_btn_size",    &g_tsmAddr.icon_btn_size);

    bool p2dSet = false;
    TsmReadRvaOverride(host, "panel2d", &g_tsmAddr.panel2d, &p2dSet);
    if (p2dSet && g_tsmAddr.panel2d != 0) {
        // Propagate fixed C3D_PANEL2D internal offsets unless individually overridden.
        g_tsmAddr.p2d_rect_w = g_tsmAddr.panel2d + 0x288u;
        g_tsmAddr.p2d_rect_h = g_tsmAddr.panel2d + 0x28Cu;
        g_tsmAddr.p2d_rect_x = g_tsmAddr.panel2d + 0x290u;
        g_tsmAddr.p2d_rect_y = g_tsmAddr.panel2d + 0x294u;
        g_tsmAddr.p2d_rot    = g_tsmAddr.panel2d + 0x298u;
        g_tsmAddr.p2d_tint   = g_tsmAddr.panel2d + 0x2ACu;
    }
    TsmReadRvaOverride(host, "p2d_rect_w",       &g_tsmAddr.p2d_rect_w);
    TsmReadRvaOverride(host, "p2d_rect_h",       &g_tsmAddr.p2d_rect_h);
    TsmReadRvaOverride(host, "p2d_rect_x",       &g_tsmAddr.p2d_rect_x);
    TsmReadRvaOverride(host, "p2d_rect_y",       &g_tsmAddr.p2d_rect_y);
    TsmReadRvaOverride(host, "p2d_rot",          &g_tsmAddr.p2d_rot);
    TsmReadRvaOverride(host, "p2d_tint",         &g_tsmAddr.p2d_tint);

    TsmReadRvaOverride(host, "tint_hover",       &g_tsmAddr.tint_hover);
    TsmReadRvaOverride(host, "tint_normal",      &g_tsmAddr.tint_normal);
    TsmReadRvaOverride(host, "ninep_border",     &g_tsmAddr.ninep_border);
    TsmReadRvaOverride(host, "ninep_uv_a",       &g_tsmAddr.ninep_uv_a);
    TsmReadRvaOverride(host, "ninep_uv_b",       &g_tsmAddr.ninep_uv_b);
    TsmReadRvaOverride(host, "uv_full",          &g_tsmAddr.uv_full);
    TsmReadRvaOverride(host, "panel_state",      &g_tsmAddr.panel_state);

    bool inpSet = false;
    TsmReadRvaOverride(host, "input", &g_tsmAddr.input, &inpSet);
    if (inpSet && g_tsmAddr.input != 0) {
        // Propagate fixed C3D_INPUT byte offsets unless individually overridden.
        g_tsmAddr.esc_down    = g_tsmAddr.input + 0x001u;
        g_tsmAddr.esc_edge    = g_tsmAddr.input + 0x101u;
        g_tsmAddr.mouse_down  = g_tsmAddr.input + 0x300u;
        g_tsmAddr.click_latch = g_tsmAddr.input + 0x301u;
    }
    TsmReadRvaOverride(host, "esc_down",         &g_tsmAddr.esc_down);
    TsmReadRvaOverride(host, "esc_edge",         &g_tsmAddr.esc_edge);
    TsmReadRvaOverride(host, "mouse_down",       &g_tsmAddr.mouse_down);
    TsmReadRvaOverride(host, "click_latch",      &g_tsmAddr.click_latch);

    TsmReadRvaOverride(host, "world_size",       &g_tsmAddr.world_size);
    TsmReadRvaOverride(host, "half_world",       &g_tsmAddr.half_world);
    TsmReadRvaOverride(host, "gate_hi",          &g_tsmAddr.gate_hi);
    TsmReadRvaOverride(host, "gate_lo",          &g_tsmAddr.gate_lo);
    TsmReadRvaOverride(host, "margin_hi",        &g_tsmAddr.margin_hi);
    TsmReadRvaOverride(host, "margin_lo",        &g_tsmAddr.margin_lo);

    TsmReadRvaOverride(host, "relocate_person",  &g_tsmAddr.relocate_person);
    TsmReadRvaOverride(host, "abandon_building", &g_tsmAddr.abandon_building);
    TsmReadRvaOverride(host, "eject_worker",     &g_tsmAddr.eject_worker);
    TsmReadRvaOverride(host, "ui_slider",        &g_tsmAddr.ui_slider);
    TsmReadRvaOverride(host, "ui_checkbox",      &g_tsmAddr.ui_checkbox);
    TsmReadRvaOverride(host, "play_gui_sound",   &g_tsmAddr.play_gui_sound);
    TsmReadRvaOverride(host, "clip_push",        &g_tsmAddr.clip_push);
    TsmReadRvaOverride(host, "vec_push_back",    &g_tsmAddr.vec_push_back);
    TsmReadRvaOverride(host, "taskcfg_ctor",     &g_tsmAddr.taskcfg_ctor);

    bool labSet = false;
    TsmReadRvaOverride(host, "labor_mobility_hook", &g_tsmAddr.labor_mobility_hook, &labSet);
    if (labSet) {
        g_tsmAddr.labor_mobility_jbe = g_tsmAddr.labor_mobility_hook
            ? (g_tsmAddr.labor_mobility_hook + 18u) : 0u;
    }
    TsmReadRvaOverride(host, "labor_mobility_jbe", &g_tsmAddr.labor_mobility_jbe);

    bool chkSet = false;
    TsmReadRvaOverride(host, "retail_checkout_hook", &g_tsmAddr.retail_checkout_hook, &chkSet);
    if (chkSet) {
        g_tsmAddr.retail_checkout_allow = g_tsmAddr.retail_checkout_hook
            ? (g_tsmAddr.retail_checkout_hook + 18u) : 0u;
    }
    TsmReadRvaOverride(host, "retail_checkout_allow", &g_tsmAddr.retail_checkout_allow);
    TsmReadRvaOverride(host, "retail_checkout_abort", &g_tsmAddr.retail_checkout_abort);

    TsmReadRvaOverride(host, "window_dispatch",   &g_tsmAddr.window_dispatch);
    TsmReadRvaOverride(host, "panel_shop",        &g_tsmAddr.panel_shop);
    TsmReadRvaOverride(host, "panel_residence",   &g_tsmAddr.panel_residence);
    TsmReadRvaOverride(host, "panel_pub",         &g_tsmAddr.panel_pub);
    TsmReadRvaOverride(host, "panel_factory",     &g_tsmAddr.panel_factory);
    TsmReadRvaOverride(host, "panel_cityhall",    &g_tsmAddr.panel_cityhall);
    TsmReadRvaOverride(host, "panel_citizen",     &g_tsmAddr.panel_citizen);
    TsmReadRvaOverride(host, "panel_do",          &g_tsmAddr.panel_do);
    TsmReadRvaOverride(host, "panel_storage",     &g_tsmAddr.panel_storage);
    TsmReadRvaOverride(host, "storage_callsite",  &g_tsmAddr.storage_callsite);
    TsmReadRvaOverride(host, "panel_warehouse",   &g_tsmAddr.panel_warehouse);
    TsmReadRvaOverride(host, "warehouse_guard",   &g_tsmAddr.warehouse_guard);
    TsmReadRvaOverride(host, "panel_storage_td5", &g_tsmAddr.panel_storage_td5);
    TsmReadRvaOverride(host, "storage_td5_guard", &g_tsmAddr.storage_td5_guard);
    TsmReadRvaOverride(host, "panel_construction",&g_tsmAddr.panel_construction);
    TsmReadRvaOverride(host, "logistics_transfer",&g_tsmAddr.logistics_transfer);
}

static inline void TsmAddrInit(const TsmHost* host)
{
    g_tsmAddr.initialized = 1;
    if (!host || !host->exeBase || host->exeSize == 0) {
        // Offline / unit-test harness without a real PE image mapped: keep defaults active.
        g_tsmAddr.exeVerified = 1;
        return;
    }

    g_tsmAddr.textEnd = (host->exeSize < 0xFFFFFFFFu)
        ? (uint32_t)host->exeSize : 0xFFFFFFFFu;

    uint32_t stamp = 0;
    const bool peOk = TsmInspectExePe(host->exeBase, host->exeSize, host,
                                      &stamp,
                                      &g_tsmAddr.textBegin,
                                      &g_tsmAddr.textEnd);
    g_tsmAddr.exeStamp = stamp;

    // Check optional `addresses.ini` [build] target_stamp = 0x...
    uint32_t iniStamp = 0;
    bool hasIniStamp = false;
    if (host->configString) {
        char buf[64] = {0};
        host->configString(TSM_ADDRESSES_INI, "build", "target_stamp",
                           buf, (int)sizeof(buf), "");
        if (buf[0] && TsmParseHexOrUint(buf, &iniStamp) && iniStamp != 0)
            hasIniStamp = true;
    }

    if (!peOk) {
        // Synthetic memory buffer in tests (not a PE image): allow defaults.
        g_tsmAddr.exeVerified = 1;
        return;
    }

    if (hasIniStamp && iniStamp == stamp) {
        g_tsmAddr.targetStamp = iniStamp;
        g_tsmAddr.usingIniOverrides = 1;
        g_tsmAddr.exeVerified = 1;
        TsmAddrApplyOverrides(host);
        if (host->log) {
            host->log("addresses  loaded overrides from %s for SOVIET64.exe stamp 0x%08X",
                      TSM_ADDRESSES_INI, (unsigned)stamp);
        }
        return;
    }

    if (stamp == TSM_BASELINE_EXE_STAMP) {
        g_tsmAddr.targetStamp = TSM_BASELINE_EXE_STAMP;
        g_tsmAddr.exeVerified = 1;
        // Also permit optional fine-tuning overrides when target_stamp is omitted
        if (!hasIniStamp)
            TsmAddrApplyOverrides(host);
        return;
    }

    // Mismatched executable build! Pause raw RVA access unless operator explicitly
    // forced `allow_unverified_exe = 1` in `tesmioloader.ini`.
    int forceAllow = 0;
    if (host->configInt) {
        forceAllow = host->configInt("tesmioloader.ini", "tesmioloader",
                                     "allow_unverified_exe", 0);
    }
    g_tsmAddr.exeVerified = (forceAllow != 0) ? 1 : 0;
}

// Logs once per plugin when paused due to an unrecognized SOVIET64.exe build,
// and returns true iff RVAs are verified safe to touch.
static inline bool TsmCheckExeBuild(const TsmHost* host, const char* pluginName)
{
    if (g_tsmAddr.exeVerified) return true;
    if (!g_tsmAddr.loggedUnverified && host && host->log) {
        g_tsmAddr.loggedUnverified = 1;
        host->log("%s  PAUSED  SOVIET64.exe stamp 0x%08X != baseline 0x%08X "
                  "(run tools/scan_exe_update.py --write-ini addresses.ini)",
                  pluginName ? pluginName : "plugin",
                  (unsigned)g_tsmAddr.exeStamp,
                  (unsigned)TSM_BASELINE_EXE_STAMP);
    }
    return false;
}

// Safe helper for direct-callable native functions (relocate, abandon, eject,
// slider, checkbox, gui sound, clip push). Returns NULL if the executable build
// is unverified, the RVA was zeroed in addresses.ini, the RVA lies outside the
// executable section, or the entry bytes are padding/stub (INT3/zero/bare RET).
static inline const unsigned char* TsmCallableCodePtr(const TsmHost* host,
                                                      uint32_t rva,
                                                      const char* label = nullptr)
{
    if (!g_tsmAddr.exeVerified || !host || !host->exeBase || rva == 0)
        return nullptr;
    if (rva < g_tsmAddr.textBegin || rva + 16u > g_tsmAddr.textEnd)
        return nullptr;
    if ((size_t)rva + 16u > host->exeSize)
        return nullptr;
    const unsigned char* p = host->exeBase + rva;
    if (host->readablePtr && !host->readablePtr(p, 16))
        return nullptr;
    // Reject obvious non-function bytes (INT3 padding, zero fill, bare RET).
    if (p[0] == 0xCC || p[0] == 0xC3 || (p[0] == 0x00 && p[1] == 0x00)) {
        if (host->log && label)
            host->log("addresses  REFUSED direct call %s at RVA 0x%X (non-code byte 0x%02X)",
                      label, (unsigned)rva, (unsigned)p[0]);
        return nullptr;
    }
    return p;
}

#endif // TSM_GAME_ADDRESSES_H
