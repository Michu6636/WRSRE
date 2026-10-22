// v6.6 updates (PRIV TILE REGRESSION - root cause: no building/person
//     discriminator in the widget pass):
//   - SYMPTOM: after the Citizen wallet hook moved from the dead candidate
//     0x7D5460 to the live builder 0x7CEEB0, enterprise_base's [PRIV TILE]
//     stopped appearing on EVERY building panel (shop, residence, pub,
//     factory), for the rest of the session, with no error in the log.
//   - ROOT CAUSE (this file, not the citizen hook site): window+0x240 is a
//     union - Building* on building panels, Person* on the citizen panel.
//     The citizen hook is the first hook that makes AppendRowsImpl run on a
//     non-building window, and it ran the WIDGET pass with that Person* as
//     the `building` argument. Every registered widget provider then did
//     building work on it; enterprise_base's does TsmBuildingType() ->
//     *(void**)(person + 0x318), and 0x318 is the FIFTH demand record
//     (0x118 + 4*0x80) = the float pair {remaining, rate}. The range-only
//     TsmIsValidGamePtr let a float pair through as a pointer, the next
//     read took an AV, and the provider loop's __except did exactly what it
//     says on the tin: g_widgets[i].cb = NULL, provider gone for the
//     session. Hence "the buttons disappeared everywhere, permanently".
//   - FIX 1: OBJ_NONE/OBJ_BUILDING/OBJ_PERSON, derived from the hook index
//     (the authoritative source - it names the builder that fired) and
//     published/restored by RunHook around the append.
//   - FIX 2: AppendWidgetsImpl refuses to run unless the kind is
//     OBJ_BUILDING *and* TsmLooksLikeBuilding(window+0x240) - two
//     independent checks, both allocation-free.
//   - FIX 3: AppendWidgetsToWindowImpl (the foreign path private_construction
//     uses) publishes OBJ_BUILDING explicitly instead of inheriting whatever
//     the last hook left behind, and is now SEH-guarded.
//   - FIX 4: a fault in ONE append no longer sets g_enabled = 0 (which
//     silently switched rows AND widgets off on all six hooks). It now
//     disables that hook's append only; the detour keeps calling the game's
//     builder so the window still draws.
//   - FIX 5: a faulting widget provider logs its callback address, the
//     building it died on, and a running session total.
//   - FIX 6: the wallet row's DEBUG line is once per person, not once per
//     rendered frame.
//   - FIX 7 (API v5 - TWO ROW TABLES). A live log of the outage (2026-10-18,
//     17:56:22) showed the damage is bigger than the missing tile:
//         provider 1/2/3 faulted           <- citizen_labor, enterprise_base,
//                                             taxes, in the ROW loop
//         widget provider 1/2/3 faulted    <- the same three, WIDGET loop
//     Six faults in 7 ms, and from then on the panel subsystem had one
//     working provider left (the wallet row, the only one that self-gated).
//     Cause in every case: TsmBuildingType() on a Person*, faulting at
//     `td + 0x360` with td = *(void**)(person + 0x318) = 0x124325CF0B0 and
//     the faulting read at 0x124325CF410 - the same address in
//     citizen_labor.dll+0x878D, enterprise_base.dll+0xC44D and
//     taxes.dll+0x2A36, i.e. the same inlined helper in three modules.
//     So the row pass could not be left to "providers self-validate":
//     registerRowProvider is now BUILDING-ONLY and the new
//     registerPersonRowProvider (API v5) is PERSON-ONLY, with one table
//     each. A provider registered in one is never invoked for the other.
//     citizen_labor / enterprise_base / taxes need NO source change - they
//     simply stop being asked to describe a person. ui_panels' own wallet
//     provider moves to the person table.
//   - Requires game_world.h v8.10 (TsmBuildingType readability-gated,
//     TsmLooksLikeBuilding added).
//   - Version -> 6.6. API is APPEND-ONLY: v1/v2/v3/v4 prefix views are
//     published unchanged alongside the new v5 full iface, so mixed
//     builds stay safe and a v4 consumer's HasAppend still reads true.
//
// v6.5 updates (Task: texture crash fix):
//   - LABEL: the construction-site tile now reads PC (private construction),
//     matching the PRIV convention on finished panels.
//   - BACKGROUND: BindButtonTexture reverted to the crash-safe game-object
//     texture chain (G_TEX_ALT2/3/4/1, G_TEX_9PATCH). The v6.4 window-slot
//     search loop (kWinOff 0xB210/0xB218/0xB220) read 45 KB out of bounds on
//     g_frame.win/bld and produced thousands of C0000005 exceptions per
//     second; it is removed. All button tiles now use the proven ALT-frame
//     chain regardless of size.
//     Version -> 6.5.
// v6.3 updates (tester: INI edits seemed ignored):
//   - Root cause of the confusion: v6.2 added toolbar_tile_x as THE X knob
//     but the tester kept turning toolbar_tile_x_base (the legacy triple,
//     ignored unless toolbar_tile_x = -1). The log proved the new knob
//     works (x 445 -> 438 loaded live). This version removes the trap:
//     the legacy triple is GONE from the INI (kept in code only as an
//     x < 0 fallback) and the comments now state both knobs identically.
//   - DIRECTION CHEAT SHEET (screen coords, origin top-left):
//       toolbar_tile_x: BIGGER = right, SMALLER = left  (~0.8px per unit)
//       toolbar_tile_y: BIGGER = DOWN,  SMALLER = up    (~0.8px per unit)
//     So "a little up and to the left" = DECREASE both numbers.
//   - Applied the tester's request directly: x 445 -> 438 (left ~6px),
//     y 10 -> 4 (up ~5px). Version -> 6.3.
// v6.2 updates (toolbar X simplification - tester found base/pitch/slot
//     interaction confusing: pitch is amplified 6x by slot, so it jumps
//     while base glides):
//   - Added toolbar_tile_x: a SINGLE absolute X offset (same units as Y -
//     engine units before dpi, 1 unit ~= 0.8px). When set (>= 0) the dock
//     is rowLeft = W_X + W_OFFX + dpi*x, ignoring base/pitch/slot.
//     Geometric equivalent of the old triple: x = base + pitch*slot, so
//     the current position (25 + 70*6 = 445) is x = 445 - identical
//     placement, one knob. base/pitch/slot stay live underneath for
//     compatibility. Log line prints the effective X every session.
//     Version -> 6.2.
// v6.1 updates (CON toolbar match - tester screenshot + native disasm):
//   - SIZE: toolbar_tile_size 70 -> 40. The construction toolbar loop
//     (FUN_1407838e0, constr_tbloop.asm:226-230) draws its own buttons at
//     dpi*DAT_14090a9f0 = dpi*40. CON at 70 was 175% of every neighbour
//     (screenshot: CON towers over demolish/R_). At 40 it is the same
//     square as the natives, automatically full-height once on the row.
//   - Y: rowDy -10 -> 10. Native center Y = XMM8 - XMM0 where XMM0 includes
//     dpi*35 (constr_tbloop.asm:275-278), i.e. the button CENTERS sit
//     dpi*35 below the toolbar content origin; a 40-unit tile has half-size
//     dpi*20, so its top sits dpi*(35-20) = dpi*15 below the content origin
//     (W_Y+W_OFFY is that origin in our dock, line 1362). rowDy 10 lands
//     within half a unit of that (the v5.9 -10 value was calibrated for a
//     70-unit tile and left a 40-unit tile floating ~16 units high).
//     Version -> 6.1.
// v6.0 updates (WASD/mouse-camera/ESC input corruption - ROOT CAUSE + fix):
//   - ROOT CAUSE: `typedef C3DVec3 (*t_GetMouseSolid)(void*)` returned the
//     12-byte mouse vector BY VALUE. Native disassembly (0x14034160a-635)
//     proves the MSVC x64 sret convention: caller passes a HIDDEN result
//     pointer in RDX, object in RCX. Our call passed nothing in RDX, so the
//     callee wrote 12 bytes through garbage RDX every frame a button was
//     live - smashing input state. Log proof: btn probe's `mouse` field
//     showed -9.7e28 (garbage RDX contents) while the OS cursor fallback
//     was healthy. This single bug explains ALL THREE symptoms: WASD dead,
//     mouse driving the camera, ESC dead on button panels. The old
//     Collision typedef (by-value struct param) had the same bug class in
//     reverse - the native takes the vector POINTER in RDX (MOV RDX,RAX).
//   - FIX: GetMouseSolid is now (this, out*)->void, Collision takes
//     (this, const-vector*); the call passes the input singleton in RCX
//     and &m in RDX with an SEH guard + sanity clamp.
//     Version -> 6.0.
// v5.9 updates (ESC probe fix + CON position nudge):
//   - ESC PROBE FIX: the v5.8 probe sampled 0xA54C91 (ESC edge) at button-draw
//     time, but the dispatcher's close logic (FUN_1406e9b80, runs BEFORE the
//     draw loop) consumes that edge first - the probe never saw escEdge=1
//     (only 4 lines, all edge=0). v5.9 detects the PHYSICAL ESC key
//     (GetAsyncKeyState, bypassing engine edge order) and logs the STABLE
//     close-path fields: game+0xCB78 (close debounce latch - 1 = every close
//     skipped), game+0xD428 (modal tool), 0xA54B91 (ESC held), 0xA54C91,
//     window+0x01/0x00/0x34. Runs in AppendWidgetsImpl (every frame a button
//     panel is open, not budget-gated), gated on g_probe, capped at 120.
//   - CON NUDGE: tester "slightly too right, a little more up" ->
//     toolbar_tile_y 0 -> -10 (up ~8px), x_base 33 -> 25 (left ~6px).
//     Size/pitch/slot unchanged so the nudge is isolated. The construction
//     toolbar's REAL native values (disasm FUN_1407838e0 toolbar loop,
//     constr_tbloop.asm:220-324) are size 40 / pitch 85 - NOT 70 (the City
//     Hall tab values) and NOT 47 (the chrome helper). Set toolbar_tile_size
//     = 40 and toolbar_tile_x_pitch = 85 to dock CON onto the native grid.
//     Version -> 5.9.
// v5.8 updates (ESC-close diagnostic probe):
//   - The v5.6/v5.7 "ClipPush traps ESC" theory was DISPROVEN against the
//     binary this session. FUN_1404465a0 is camera/menu-zoom math, not a clip
//     pop; FUN_1404461f0 is a pure rectangle accumulator with no input code;
//     native sliders/checkboxes call it unpaired every frame and ESC works.
//     The REAL building-window ESC close lives in the window dispatcher
//     FUN_1406e9b80 (scratch/do_dispatcher.c:186-220): it needs game+0xD428==0
//     (no placement tool) AND the ESC press-edge 0xA54C91 (set by the input
//     decoder FUN_140443b90 -> param+0x69) AND a visible window (window+0x01),
//     then sets window+0x00=1 and zeroes 0xA54C91. Our button only ever eats
//     0xA54E91 (confirm/click) - a DIFFERENT byte - so it cannot block ESC by
//     construction, and the CON path already skips the clip yet still fails.
//   - Added EscCloseProbe(): a read-only, g_probe-gated, hard-capped probe that
//     logs the real close-path state (escEdge 0xA54C91, escDown 0xA54B91,
//     modalTool game+0xD428, window+0x01/0x00/0x34/0x5C8) each frame a button
//     is live, so a live session shows exactly which gate ESC fails on.
//     Version -> 5.8.
// v5.7 updates (CON invisible + ESC fix):
//   - POSITION (invisible tile): rowDy -70 -> 0. The probe showed the CON
//     tile at rect (1818,208)+56 - y=208 is ABOVE the panel's visible
//     content fold (W_Y+W_OFFY = 264), so the engine clipped it out of
//     view entirely (the "no longer visible at all" report). rowDy 0 puts
//     the top at content_Y = 264, inside the panel. X slot 7 -> 6 so the
//     tile lands immediately right of the R_ rename button instead of one
//     cell further off the visible panel (the "scroll bar" report).
//   - ESC: the CON tile runs on the construction panel via the FOREIGN
//     append (AppendWidgetsToWindow, after o_PanelBuilder returns). Its
//     per-frame ClipPush landed outside the builder's clip scope, was never
//     popped, and blocked ESC (no cancel-placement, no panel-close). Skip
//     the clip only on that path (s_foreignClip); PRIV on finished panels
//     keeps the balanced in-builder clip. Version -> 5.7.
// v5.6 updates (CON row/size - builder ground truth; ESC fix):
//   - POSITION: decompiled the construction builder itself (0x7838E0,
//     scratch/constr_full.c:443/447). It draws its OWN toolbar buttons
//     (demolish/plaster/+/-/stats/workers/R_, centered, dpi*70 = 56px) with
//     the strip center at Y = W_Y+W_OFFY-dpi*35. v5.5 wrongly keyed on the
//     generic chrome helper 0x1406F8CC0 (dpi*47 close/pin buttons, a
//     DIFFERENT set) -> one row low + shrunken. So toolbar_tile_size back
//     to 70 and rowDy to -70 (top = content_Y-dpi*70 = 208, center = 236,
//     matching natives); x_base 15 -> 33 so the slot-7 tile docks one cell
//     past the R_ rename button instead of overlapping it. All INI-tunable.
//   - ESC: the CON tile runs on the construction panel via the FOREIGN
//     append (AppendWidgetsToWindow, after o_PanelBuilder returns). Its
//     per-frame ClipPush landed outside the builder's clip scope, was never
//     popped, and blocked ESC (no cancel-placement, no panel-close). Skip
//     the clip only on that path (s_foreignClip); PRIV on finished panels
//     keeps the balanced in-builder clip. Version -> 5.6.
// v5.5 updates (CON size + row - chrome-helper ground truth):
//   - The construction toolbar (demolish/plaster/-/+/stats/workers/R_) that
//     CON must align with is window CHROME drawn by the shared helper
//     0x1406F8CC0 (called by every building window), NOT the construction
//     builder's content rows I traced in v5.2-v5.4. Decoded from that helper:
//     each native button is a dpi*47 square (DAT_14090AA18=47, written to the
//     same rect W/H globals 0x9BE2E8/0x9BE2EC we set) and the strip Y is
//     W_Y+W_OFFY+dpi*15 (DAT_14090A8CC=15). So toolbar_tile_size 70 -> 47 and
//     toolbar_tile_y 45 -> 15: the tile now matches the native demolish/R_
//     buttons in BOTH size and row. The v5.2-v5.4 dpi*45 term was the
//     construction CONTENT row (dpi*35 banner + the banner helper's internal
//     dpi*10), one step below the chrome buttons - the persistent
//     one-row-low, oversized misalignment the tester reported. X slot
//     unchanged (slot 7 still lands just past R_ rename). Version -> 5.5.
// v5.4 updates (CON row + size, disasm-exact):
//   - The construction toolbar buttons paint at Y = W_Y+W_OFFY+dpi*35 +
//     dpi*10 (constr_toolbar.asm XMM10 + the draw helper 0x1406f36b0's
//     internal +dpi*10) = dpi*45 total. btn geom (W_Y 484.8, W_OFFY -220.8,
//     dpi 0.8) pins that at y=300; rowDy 0 rendered 264 (too high), rowDy 35
//     rendered 292 (the row before the helper's offset). rowDy 45 lands the
//     tile on the native button row; size 42 -> 70 matches the native
//     toolbar cell (34px -> 56px). X (base 15 + pitch 70 * slot 7 = 1804)
//     unchanged, still just past R_ rename. Version -> 5.4.
// v5.3 updates (CON row alignment):
//   - The demolish/plaster/-/+/stats/workers/R_ toolbar strip is the TOP
//     CONTENT ROW at W_Y+W_OFFY+dpi*0. The btn geom dump (dpi 0.8, W_Y
//     484.8, W_OFFY -220.8) proved the tile at rowDy 35 landed at y=292 -
//     exactly the dpi*35 "Construction has stopped" TEXT row one step
//     below the buttons. rowDy 35 -> 0 lifts it one row-step (~28px) onto
//     the button strip; X (base 15 + pitch 70 * slot 7 = 1804) already sits
//     immediately right of R_ rename. INI floor lowered to -20 so 0/negative
//     nudges survive. Version -> 5.3.
// v5.2 updates (CON toolbar dock - disasm-grounded):
//   - The construction toolbar is the builder's FIRST CONTENT ROW at
//     W_Y+W_OFFY+dpi*35 (constr_toolbar.asm XMM10) with the first button
//     at W_X+W_OFFX+dpi*15 (XMM6), and it SCROLLS with W_OFFY - NOT the
//     City Hall tab row (dpi*110) and NOT chrome. v5.0/v5.1 both read the
//     wrong row and/or zeroed the scroll offset, so CON sat ~150px low and
//     out on the scrollbar (probe: rect (1873,573)+34). Fixed: rowDy 35,
//     base 15, pitch 70, slot 7 (just past R_ rename); offsets now read
//     for every dock. INI-tunable; the btn probe gained a raw-geometry dump
//     (W_Y/W_OFFY/W_X/W_OFFX/dpi) so the slot is one log line, not a guess.
//   - Version -> 5.2.
// v5.1 updates (CON toolbar dock + TAX nudge):
//   - CON (widgetButtonSmall, construction panel) now docks in the TOP
//     TOOLBAR row with its OWN geometry (base 20 + pitch 38 * slot 7),
//     landing beside the R_ (rename) button. The old code reused the tab
//     slot formula and overshot onto the scrollbar (probe: rect (1886,573)
//     +34, off-panel). INI-tunable: toolbar_tile_x_base/_pitch/_slot.
//   - TAX (big tab tile) nudged up a few px: tab_tile_row_dy 185 -> 180.
//   - Version -> 5.1.
// v5.0 updates (tile-role corrections):
//   - TAX is the BIG tile again: taxes draws it with widgetButton in the
//     tab/icon row next to the native "Domestic production &
//     consumption" icon (same size as the natives), not the small
//     toolbar tile. The tab-row dock is now INI-tunable
//     (tab_tile_x_pitch 63 / tab_tile_row_dy 185) - the old fixed
//     70-pitch/175 term sat ~22px right and ~8px above the natives.
//   - The toolbar dock (widgetButtonSmall) now reads the window origin
//     WITHOUT the scroll offsets: the toolbar is window chrome and does
//     not scroll, so a scrolled construction panel no longer pushes the
//     CON tile a scroll-height below the toolbar row.
//   - Version -> 5.0.
// v4.9 updates (toolbar-row tile + foreign-window widget pass):
//   - widgetButtonSmall: widgetButton docked in the panel's TOP TOOLBAR
//     row (the [demolish][+][-][stats][workers][R][T] strip) instead of
//     the tab/icon row - smaller square (toolbar_tile_size, default 42)
//     at the toolbar row (toolbar_tile_y, default 110: the City Hall
//     builder's first row + its 70-unit button square). Same X slot, so
//     it clears the native toolbar buttons. INI-tunable.
//   - appendWidgetsToWindow(game, window): runs the registered widget
//     providers on a window ui_panels does not itself hook. Lets
//     private_construction run the widget pass on the construction-site
//     panel (0x7838E0), which it owns at the prologue.
//   - API v4 (quad-publish v1/v2/v3/v4 with truncated structSizes);
//     new entries guarded by TsmUiPanelsHasButtonSmall /
//     TsmUiPanelsHasAppend - older consumers are untouched.
//   - Version -> 4.9.
// v4.8 updates (privatisation-tile alignment, final):
//   - Size 63 -> 65 units (DAT_14090AAA8): the v4.7 btn-probe rect
//     (1696,414)+50 pins the dpi at ~0.79, so the tester's "barely
//     smaller than native buttons" means the natives are ~52px = ~65
//     units. 65 is the minimal exact match; 70 (the flag-gated button's
//     rect size) would overshoot to visibly larger.
//   - Position (pitch 80 / rowDy 188) confirmed perfect by the tester -
//     untouched.
//   - Version -> 4.8.
// v4.7 updates (privatisation-tile alignment, round 5):
//   - Size settled at the natives' 63-unit square (no size complaint this
//     round); only position remained.
//   - Decisive position jump per "more to the right and down": X pitch
//     73 -> 80 (~22px right), row-top term 168 -> 188 (~16px down).
//   - BOTH VALUES NOW INI-TUNABLE (icon_tile_x_pitch / icon_tile_row_dy):
//     five screenshot rounds of pixel calibration is a config problem,
//     not a rebuild problem. The live log prints the active values.
//   - Version -> 4.7.
// v4.6 updates (privatisation-tile alignment, round 4):
//   - SIZE: 60 -> 63 units. PROVEN from the builder disasm: the button-
//     draw block at 140762dc6-140762ddf computes dpi*63 (DAT_14090AA90)
//     into the rect-globals setup - the natives' icon buttons are 63-unit
//     squares, so the 60-unit tile was slightly smaller (user: "perhaps
//     slightly smaller than the native buttons").
//   - ROW TOP: back to the shared 105+ownSize formula -> 105+63 = 168,
//     ~6px lower than v4.5's 160 ("a bit lower" wanted). No special-case
//     row term.
//   - X PITCH: 70 -> 73 (+~10px right of the natives' row end, "a little
//     more to the right" wanted; v4.4's 60 overlapped them).
//   - Version -> 4.6.
// v4.5 updates (privatisation-tile alignment, round 3):
//   - Reverted v4.4's changes per user feedback: 50-unit size was
//     "shrunk" (60 was correct) and the 60-unit X pitch pulled the tile
//     32px LEFT, overlapping the native buttons.
//   - Residences now dock with: 60-unit square (DAT_14090AA78), row-top
//     term 160 (v4.3's 165 was ~5px low, v4.4's 155 slightly high - 160
//     is the natives' measured top), and the 70-unit tab-square X pitch
//     (the v4.2/v4.3 position, clear of all natives).
//   - Version -> 4.5.
// v4.4 updates (privatisation-tile alignment, round 2):
//   - v4.3's 60-unit icon square was still wrong; the v4.3 btn-probe rect
//     (1664,396)+48 back-computes W_Y+W_OFFY = 264, and the natives' row
//     top in the user screenshot sits at screen 388 = W_Y+W_OFFY+dpi*155
//     - the tab row's 105 base plus the 50-unit square (DAT_14090AA28,
//     the same constant the residence builder loads at 140764355).
//   - Residences now dock at the 50-unit square (size + rowTop formula
//     both switch) and a 60-unit X pitch so the tile clears the 4th
//     native icon by one button width. The same arithmetic predicts both
//     user screenshots' offsets (16px at v4.2's 70, 8px at v4.3's 60).
//   - No other window type touched; version -> 4.4.
// v4.3 updates (privatisation-tile alignment):
//   - Residence windows (type 2) now dock the tile in the native 4-icon
//     stats row at the row's own 60-unit square (DAT_14090AA78, decoded
//     from the exe layout table) instead of the 70-unit City Hall tab
//     square - the tile previously rendered ~20% oversized and ~8px low
//     against the native buttons (user screenshot). Row top uses the same
//     formula family with the row's own size (105+60 vs 105+70); the X
//     slot keeps the tab-square pitch so the tile still starts clear of
//     the 4th native icon. All other window types are untouched.
// v4.2 updates (privatisation-tile visibility diagnosis):
//   - Added a one-shot y-abort trace (probe = 1): when a hooked window's
//     +0x250 bottom edge is not sane at append time the WHOLE pass
//     (rows + widgets, and with them the PRIV tile) is skipped for that
//     frame; the trace logs it once per hook so a persistent abort is
//     visible in tesmioloader.log instead of silent.
//   - No behaviour change otherwise; version -> 4.2.
// v4.1 updates ([P] tag replacement task):
//   - TSM_PANEL_MAX_ROWS 12 -> 14 (ui_panels_api.h) and max_rows 14 in
//     the shipped ini: enterprise_base's privatisation tile (widgetButton)
//     consumes 2 content rows in the fallback flow, and a fully-loaded
//     private shop pushes 11 text rows - 13 total, which no-opped the
//     button at a 12 cap. Mixed old/new DLLs stay memory-safe because a
//     provider bounds writes by ui_panels' capacity first.
//   - Service ABI unchanged (append-only v1/v2/v3 tail intact);
//     version 4.1.
// v4.0 updates ([P] tag + rent + legibility task):
//   - ROW COLOUR: g_colour 0xFF990000 (dark red) -> 0xFF1A1A1A (opaque
//     native dark charcoal). Dark red strained against the parchment
//     beige; charcoal is the native text family (BTN_LABEL_COLOUR
//     0xFF202020) and keeps full contrast. INI `colour` default + shipped
//     ui_panels.ini updated to match; existing INIs override.
//   - Service ABI unchanged (v1/v2/v3 prefixes intact); only the plugin
//     version string moves to 4.0.
// v3.9 updates (polish pass after v3.8's geometry fixes):
//   - LABEL COLOUR: 0xFFFFFFFF (white) -> 0xFF202020 (opaque black/charcoal)
//     via the BTN_LABEL_COLOUR macro, on all three print paths (o_Print,
//     o_PrintNoArg, o_PrintCenter). White washed out against the beige
//     parchment bevel; black contrasts sharply.
//   - TAB-ROW DOCKING restored, but DERIVED instead of hardcoded. v3.8
//     removed docking entirely; the mission wants the tile beside [Chart].
//     Solving FUN_14072DBF0's tab loop (ch_builder_full.c:549-568 with the
//     header accumulator at 57-75 / 476-540):
//       Y  = W_Y + W_OFFY + dpi*105 + dpi*70      (RVA_TAB_ROW_DY = 0x90AB84
//                                                     = 105, plus the tab edge)
//       X  = W_X + W_OFFX + dpi*(35 + 15 + 70*slot)
//     Both read from the window struct at runtime, so the tile tracks the
//     natives' row whatever the header height - no magic 175 constant.
//     Slot is now 4 (the natives fill 0..3: Family, Warehouse, Exchange,
//     Chart), so TAX sits immediately right of Chart.
//   - Docked in the tab row the tile is CHROME and does not advance
//     window+0x250, so the collapsed window keeps its default height. The
//     content-flow fallback (sane tab geometry unavailable) still gives its
//     two rows back, so the button is never lost.
//   - v3.8's centered-draw compensation (bx + size/2, y + sink + size/2) is
//     unchanged - it is what makes the visual tile and the hitbox coincide.


// v3.4 updates (live re-test 3: the click landed, but only the tile's lower
// sliver was live and the label sat off the tile):
//   - HIT-TEST (root cause): the natives call
//     `GetMouseSolid(&DAT_140a54b90)` as a BARE STATEMENT and then
//     `Collision((C3D_PANEL2D*)&DAT_1409be060)` with ONE argument - Ghidra
//     prints all seven args for their Draw call but only the panel pointer
//     for Collision, so the mouse is read from the C3D_INPUT singleton that
//     GetMouseSolid refreshed IN PLACE. v3.3 assigned that (garbage) return
//     value and passed it to Collision as a second argument, so the engine
//     path was dead on every frame and hover fell through to a raw
//     client-pixel test against panel-space coords - hence a live sliver
//     instead of the whole tile. Collision now takes one arg like the
//     natives and tests the SAME rect globals Draw just used, so hit and
//     draw agree by construction whatever the panel's screen offset is.
//     The client-cursor test survives only as a fallback.
//   - LABEL: dropped the -1.36*fs fudge. The row text (slider captions)
//     passes the row Y straight to o_Print and lands correctly, so the
//     engine's Print Y IS the row anchor. The 70-unit tile is exactly two
//     35-unit row pitches, so one row band = size/2 and centring that band
//     inside the tile puts the label at ty + size/4.
//   - Probe: logs the engine hover separately from the cursor fallback,
//     plus the label geometry (font size, print X/Y, font pointer).


// v3.3 updates (live re-test 2: probe proved the engine mouse path dead):
//   - HIT-TEST: every probe frame showed `mouse (0.0,0.0)` from
//     GetMouseSolid while live/busy/latch/edge/tex/font were ALL healthy -
//     the engine vector is unusable in this context. Hover now ORs a
//     direct GetCursorPos + ScreenToClient test of the tile rect (same
//     client-pixel space the P2D rect globals live in); the engine pair
//     stays as a secondary. Probe now logs the cursor client point.
//   - SIZE: the native City Hall tab square is DAT_14090AAB8 = 70 units
//     (= exactly 2 row pitches of 35) - read at runtime with a guard.
//     The 36-unit tile was half-size next to the native buttons.
//   - ROW: a 70-unit tile needs 2 rows of window+0x250 so the chrome
//     grows around it; widgetButton now consumes TWO rows (budget < 2 =
//     no-op). taxes drops its cosmetic title row to keep the 12-row fit.
//   - LABEL: engine Print Y is not the glyph top (measured ~1.36*fs
//     lower on the live screenshot); Y corrected to center the glyphs,
//     and the width approximation widened to 0.85*fs/char (measured
//     12.7px/char for TAX at fs=14).
//
// v3.2 updates (live re-test: tile still blank + unclickable - the native
// City Hall builder FUN_14072DBF0 decompiled end-to-end this session):
//   - DRAW: native buttons use C3D_PANEL2D::Draw(0,0,uv,uv,0,true) with
//     the rect from the P2D globals - DrawFrom9Patch with the labour-chrome
//     constants (a PROGRESS-BAR fn) rendered the flat white slab. Draw is
//     now primary; texture chain reordered checkbox-box bevel first
//     (0xB460/0xB458/0xB768 -> 0xB498 -> 0xB028 bar last).
//   - HIT-TEST: natives gate ONLY on window+0x20 (hover && window+0x20),
//     they do NOT check game+0xAD74. The busy gate inside
//     WidgetLiveAndFree was ANDed into hover/click - the prime dead-click
//     suspect. Button now uses window+0x20 alone (native recipe); busy is
//     read for diagnostics only.
//   - CLIP: natives push the clip only when game+0xADA8 == -1 or ==
//     game+0xADF0; we pushed unconditionally.
//   - Bounded probe lines (first frames + state changes, cap 60) log
//     rect/mouse/hover/live/busy/latch/edge so any residual failure is
//     diagnosable from tesmioloader.log alone.
//
// v3.1 updates (City Hall TAX button hardening - empty tile + dead clicks):
//   - Font fallback chain: g_frame.font -> *(exe+0x994200) -> *(exe+0x995220)
//     -> *(exe+0x995228); opaque white 0xFFFFFFFF; PrintCenter primary with
//     SEH, PrintLeft / PrintLeftNoArg manual-center fallback
//     (x + (size - approxW)/2) so the tile never renders blank.
//   - Texture fallback chain: game+0xB028 -> 0xB498 -> 0xB460 -> 0xB458 ->
//     0xB768; Draw (MMMMM) solid-frame fallback; row ALWAYS advances once
//     the tile is attempted so window+0x250 never leaks.
//   - Hit-test hardened: engine GetMouseSolid+Collision primary, manual
//     rect test fallback in the same coordinate space; C3DRECT stays
//     (left,top,right,bottom) ints, mouse is panel-space.
//   - Click latch OR physical edge: engine 0xA54E91 consume + GetAsyncKeyState
//     (VK_LBUTTON) edge fallback (City Hall natives starve the latch),
//     gui_click audio, 2px sink, return 1.
//   - g_buttonsReady relaxed to Draw9Patch+Collision+MouseSolid core;
//     printers/sound/clip optional via fallbacks.
//   - AppendRowsImpl no longer aborts the widget pass on a null text font.
//
// v3.0 updates:
//   - New native square BUTTON TILE helper (widgetButton): the engine's own
//     9-patch chrome (C3D_PANEL2D::DrawFrom9Patch over the game's cached
//     9-patch texture at game+0xB028), GetMouseSolid+Collision hover,
//     click-latch 0xA54E91 edge + gui_click audio, hover/active tint pair
//     and a 1-2px sink for the pressed/active state. Label printed centered
//     (PrintCenterUnicode). All pieces verified in scratch/ui_probe_widgets.c,
//     scratch/labor_chrome_decomp.c and scratch/ui_probe_menucand.c.
//   - The v3 iface is only PUBLISHED when every engine piece resolved;
//     otherwise consumers see the v2 view and degrade to the v2 widgets.
//   - Triple-provides v1 (text rows) + v2 (slider/checkbox) + v3 (button).
//
// v2.2 updates:
//   - The append pass no longer early-returns on zero TEXT rows when a
//     widget provider is registered: a collapsed widget-only panel
//     (City Hall fiscal header) still needs its widget pass. Byte-identical
//     vanilla guarantee kept: the helpers only write window+0x250 when a
//     widget is actually drawn.

// ui_panels - the in-window information layer.
//
// One inline post-hook per building panel builder (table below; every
// prologue byte verified by Ghidra - docs/17 section 10 - and memcmp-checked
// by the loader before patching), and one shared row printer:
//
//   after the game has laid its own panel out and left the bottom edge in
//   window[0x250], gather rows from the registered providers for that
//   building, print them at the bottom edge in the game's own font, colour
//   and row pitch, then hand the new bottom edge back so the window grows by
//   exactly the rows we added. v2 adds the native widget pass: after the
//   text rows, widget providers draw the exe's own 0..100 slider with
//   -/+ buttons (RVA 0x2727F0) and the labelled checkbox (RVA 0x3956F0)
//   through widgetSlider / widgetCheckbox, which own the live gate, the
//   busy gate, the SEH bracket and the same window+0x250 advance
//   (UI investigation.md section 11, live reference cities.cpp).
//
// A provider that returns zero rows costs nothing: no print, no write - the
// window is byte-identical to vanilla. A faulting append disables the whole
// plugin for the session; a refused hook only loses that window's rows.
//
// The geometry mirrors the game's own builders (both verified):
//
//   x = window[4] + window[0x28] + DPI*(rowStep + indent - back)
//       rowStep 0x90A9CC = 35, indent 0x90AB98 = 110, back 0x90A8CC = 15
//   y = window[0x250]  (the running bottom the builder just wrote)
//   y += DPI*35 per row; final y written back to window[0x250]

#include "../../src/tesmio_plugin.h"
#include "../../src/ui_panels_api.h"
#include "../../src/game_world.h"     // TsmBuildingType - the fallback-hook type filter
#include "../../src/currency_api.h"   // getWalletBalance / TsmCurrencySymbol for the wallet row

#include <wchar.h>   // wcscpy_s, _snwprintf_s

// v3.1: physical mouse edge detection (GetAsyncKeyState) in widgetButton.
#pragma comment(lib, "user32.lib")

// ---------------------------------------------------------------- sites (v1.1.1.9)

#define RVA_DPI          0x992088  // the scale every layout constant is multiplied by
#define RVA_FONTMGR      0x996FB0  // C3D_FONTMANAGER, by address
#define RVA_FONT_DEFAULT 0x994200  // the C3D_FONT* shop/residence rows print with
#define RVA_FONT_ALT_A   0x995220  // checkbox decomp default when font==NULL
#define RVA_FONT_ALT_B   0x995228  // menucand center-print font
#define RVA_FONT_SIZE    0x99528C  // menucand PrintCenter sizing constant
#define RVA_ROW_STEP     0x90A9CC  // 35 - one row, and part of the x indent
#define RVA_X_INDENT     0x90AB98  // 110
#define RVA_X_BACK       0x90A8CC  // 15
#define RVA_CHECK_SIZE   0x90A928  // 20 - checkbox box size (cities LAYOUT_20)

// v2 native widgets (UI investigation.md section 11, live in cities.cpp).
// The exe draws, hit-tests AND mutates the value itself; the helper only
// owns the gating (live, busy), the SEH bracket and the row advance.
#define RVA_SLIDER       0x2727F0  // 0..100 slider with -/+ buttons
#define RVA_CHECKBOX     0x3956F0  // labelled checkbox, wide-string caption

// window fields - docs/17 section 2, re-verified in section 10 for both hooks
#define W_HIDDEN   0x01   // non-zero while the panel is not drawn
#define W_X        0x04   // panel position, float
#define W_ACTIVE   0x20   // non-zero while the window accepts input (live)
#define W_OFFX     0x28   // panel scroll/offset, float
#define W_Y        0x08   // panel position Y, float (tab-row origin)
#define W_OFFY     0x2C   // panel scroll/offset Y, float (tab-row origin)
#define W_BUILDING 0x240  // the building the window is about
#define W_BOTTOM   0x250  // the running Y - the panel's bottom edge

// v5.8 ESC-close diagnostics. Ground truth from Ghidra (this session):
// the building-window ESC close lives in the window dispatcher
// FUN_1406e9b80, NOT in any clip/modal path. Its close block
// (scratch/do_dispatcher.c:186-220) requires:
//     game+0xD428 == 0        (no active placement/modal tool), AND
//     0xA54C91  != 0          (the ESC press-edge, set by the input decoder
//                              FUN_140443b90 -> param+0x69), AND
//     a window with window+0x01 != 0 (visible) to close, then it sets
//     window+0x00 = 1 (close flag) and zeroes 0xA54C91.
// The input decoder (scratch/input_poll.c:104-109) maps:
//     0xA54E91 <- confirm/click press-edge  (the ONLY byte our button eats)
//     0xA54B91 <- ESC key held
//     0xA54C91 <- ESC press-edge            (the real window-close trigger)
// None of these are the clip context (game+0xADB0); the v5.6/v5.7 ClipPush
// "modal capture" theory was disproven - FUN_1404465a0 is camera/menu-zoom
// math and FUN_1404461f0 is a pure rectangle accumulator with no input code.
// The probe below logs the real close-path state each frame a button panel
// is open, so a live session shows exactly which gate ESC is failing on.
#define W_CLOSE     0x00   // window close flag - dispatcher sets this to 1
#define W_KIND      0x34   // window kind (int): 0x21 construction, 0x27/0x28
                           // city-hall family, etc.
#define G_MODAL_TOOL 0xD428 // active placement/modal tool pointer, or 0. When
                            // non-zero the dispatcher DISABLES ESC window-close
                            // (scratch/do_dispatcher.c:186, :1037).
#define G_CLOSE_LATCH 0xCB78 // dispatcher "close pending" debounce byte. Set to 1
                            // when ESC closes a window; cleared only when the ESC
                            // KEY is released (0xA54B91 == 0). If it sticks at 1
                            // the whole ESC-close block is skipped every frame
                            // (scratch/do_dispatcher.c:180-185) -> "ESC does
                            // nothing". Prime suspect for the live bug.
#define RVA_ESC_EDGE 0xA54C91 // byte: ESC press-edge (input decoder param+0x69)
#define RVA_ESC_DOWN 0xA54B91 // byte: ESC key held    (input decoder param+0x68)

// game field - the widget currently being edited/dragged, or -1.
// cities.cpp gates every widget call on this (P_WIDGET_BUSY).
#define G_WIDGET_BUSY 0xAD74

// v3.1 texture fallback chain (checkbox/slider decomp texture slots).
// Primary 0xB028 is blank in some City Hall frames; the exe's own widgets
// bind 0xB498/0xB4A0 (slider), 0xB458/0xB460 (checkbox box), 0xB768
// (checkbox alt). All are C3DAPI_TEXTURE* with the same vtable+0x70 bind.
#define G_TEX_ALT1    0xB498
#define G_TEX_ALT2    0xB460
#define G_TEX_ALT3    0xB458
#define G_TEX_ALT4    0xB768
// v3.8: the ONLY chrome slots proven to hold a BUTTON FRAME in the shared
// panels - the bevelled checkbox box (0xB460/0xB458), the checkbox alt
// (0xB768), the slider (0xB498) and the 9-patch chrome (0xB028).
// G_TEX_TAB0..4 (0xB648..0xB670) were REMOVED: FUN_14072DBF0 binds those in
// its tab loop as the per-tab ICONS (tab 0 = the family/citizen portrait).
// Binding them textured our TAX tile with the game's citizen photo - exactly
// the family picture seen in the live test. They are icons, never frames.
#define G_TEX_TAB0    0xB648  // tab 0 ICON (family portrait) - NOT a frame
#define G_TEX_TAB1    0xB650  // tab 1 icon
#define G_TEX_TAB2    0xB658  // tab 2 icon
#define G_TEX_TAB3    0xB660  // tab 3 icon
#define G_TEX_TAB4    0xB670  // tab 4 icon

// Widget layout, in unscaled units from the row's own left edge
// (left = the same x the text rows print at).
// v2.1: slider moved from +200 to +180 so its [-] button clears the
// caption; caption stays at +10.
#define WIDGET_LABEL_X  10.0f
#define WIDGET_SLIDER_X 180.0f
#define WIDGET_CHECK_DY 25.0f

// v3 native BUTTON TILE (verified in scratch/ui_probe_widgets.c,
// scratch/labor_chrome_decomp.c, scratch/ui_probe_menucand.c - the same
// 9-patch + collision + latch idiom every native panel button uses).
#define WIDGET_BUTTON_SIZE 36.0f   // fallback; native tab square is 70 units

// v6.8: width of a wide button in native units (DPI-scaled at draw time).
// 280 is about eight row pitches: wider than the values column, narrower
// than any building panel. INI key wide_button_width.
static float g_wideButtonW = 280.0f;
#define RVA_TAB_BTN_SIZE 0x90AAB8  // DAT_14090AAB8 = 70 - native button edge
                                   // (= 2 x LAYOUT_35 row pitches exactly)

// v3.9 LABEL COLOUR: opaque black, not white. The tile's chrome is the beige
// parchment bevel, and white on beige washed the label out in the live tests.
// Dark charcoal (0xFF202020) reads as black on the parchment while staying a
// hair softer than pure 0xFF000000 on the anti-aliased glyph edges.
#define BTN_LABEL_COLOUR 0xFF202020ul

// v3.9 NATIVE TAB-ROW DOCKING. Solved from FUN_14072DBF0's own tab loop
// (ch_builder_full.c:549-568 + the header accumulator at 57-75/476-540):
//   the tab square's rect Y comes from local_b08, which at the tab loop holds
//     fVar25 = dpi*105 + (W_Y + W_OFFY) + dpi*70     (line 62, after the
//     conditional -dpi*70 / +dpi*10 adjustments at 66/67/74/75)
//   and its X is
//     X(i) = local_b04 + (i*70 + 15)*dpi
//          = W_X + W_OFFX + dpi*35 + (i*70 + 15)*dpi      (lines 58, 559-561)
// Both are read straight from the window struct at runtime, so the tile docks
// to the same row the natives draw regardless of header height.
#define RVA_TAB_LEFT_PAD  0x90A9CC  // DAT_14090A9CC = 35
#define RVA_TAB_LEFT_BACK 0x90A8CC  // DAT_14090A8CC = 15
#define RVA_TAB_ROW_DY    0x90AB84  // DAT_14090AB84 = 105 - tab row Y term
// v4.8: residence icon-row square. The button-draw block at
// 140762dc6-140762e52 computes dpi*70 for the flag-gated button's rect
// globals, but the stats row's X advance is the 63-unit pitch, and the
// v4.7 btn-probe shows 63 renders 50px while the tester reports the
// natives "barely" bigger - so 65 (DAT_14090AAA8, the same constant
// cluster, used by this builder at 140763286) is the minimal exact
// match. 70 would overshoot to visibly-larger.
#define RVA_ICON_BTN_SIZE  0x90AAA8  // DAT_14090AAA8 = 65 - icon-row square
// Tab slot for the TAX tile. The natives fill slots 0..3 (Family, Warehouse,
// Exchange, Chart) in this build, so slot 4 sits immediately right of Chart.
#define WIDGET_BUTTON_TAB 4

// C3D_PANEL2D singleton draw-state globals (the exe's own widgets write
// these every frame before Draw/DrawFrom9Patch/Collision).
#define RVA_PANEL2D     0x9BE060  // the C3D_PANEL2D singleton
#define RVA_P2D_RECT_W  0x9BE2E8  // current-rect width  (float)
#define RVA_P2D_RECT_H  0x9BE2EC  // current-rect height (float)
#define RVA_P2D_RECT_X  0x9BE2F0  // current-rect x (float)
#define RVA_P2D_RECT_Y  0x9BE2F4  // current-rect y (float)
#define RVA_P2D_ROT     0x9BE2F8  // rotation (float, 0)
#define RVA_P2D_TINT    0x9BE30C  // 16-byte modulate colour (C3DFOLOR)
#define RVA_TINT_HOVER  0x90C4C0  // 16-byte hover/active tint source
#define RVA_TINT_NORMAL 0x90C100  // 16-byte normal tint source
#define RVA_9P_BORDER   0x90A9A0  // 9-patch border inset, unscaled
#define RVA_9P_UV_A     0x909D04  // 9-patch float A (labour chrome call)
#define RVA_9P_UV_B     0x909E78  // 9-patch float B
#define RVA_UV_FULL     0x909F58  // DAT_140909f58 - UV extent used by Draw calls
#define RVA_PANEL_STATE 0x9EAD08  // DAT_1409ead08 - texture-set 3rd arg
#define RVA_INPUT       0xA54B90  // C3D_INPUT singleton (GetMouseSolid)
#define RVA_CLICK_LATCH 0xA54E91  // byte: click edge, consume by zeroing
#define RVA_MOUSE_DOWN  0xA54E90  // byte: left button held
#define G_TEX_9PATCH    0xB028    // game -> chrome 9-patch texture object
#define G_CLIP_CTX      0xADB0    // game -> clip context (FUN_1404461F0)
#define G_CLIP_ID       0xADA8    // game -> clip id int
#define G_CLIP_PAD      0xADF0    // game -> clip id pair (native push condition)

// exe-internal helpers the native widgets call themselves.
#define FN_PLAY_GUI_SOUND 0x3FFAD0  // (game, "gui_click", 1)
#define FN_CLIP_PUSH      0x4461F0  // (game+0xADB0, x, y, clipId)

// The variadic printer (private_construction v3.3.4 learned the cost of
// misspelling this: FindIatSlot compares the mangled name exactly).
#define SYM_PRINT_VARIADIC \
    "?PrintLeftUnicode@C3D_FONTMANAGER@@QEAAXPEAVC3D_FONT@@MMKPEB_WZZ"

// v3 engine imports - all four verified present in SOVIET64.exe's IAT.
// v3.1: the IAT spells are the FONTMANAGER forms; the FONT forms are the
// proven fallback (both verified in C3DDLL64_dll.exports.txt).
#define SYM_PRINT_LEFT_NOARG \
    "?PrintLeftUnicodeNoArg@C3D_FONTMANAGER@@QEAAXPEAVC3D_FONT@@MMKPEB_W@Z"
#define SYM_DRAW_PLAIN \
    "?Draw@C3D_PANEL2D@@QEAAXMMMMM_N@Z"
#define SYM_DRAW_9PATCH \
    "?DrawFrom9Patch@C3D_PANEL2D@@QEAAXUC3DRECT@@MMMMMM@Z"
#define SYM_COLLISION \
    "?Collision@C3D_PANEL2D@@QEAA_NVC3DVECTOR3@@MM@Z"
#define SYM_MOUSE_SOLID \
    "?GetMouseSolid@C3D_INPUT@@QEAA?AVC3DVECTOR3@@XZ"
#define SYM_PRINT_CENTER \
    "?PrintCenterUnicode@C3D_FONTMANAGER@@QEAAXPEAVC3D_FONT@@MMKPEB_WZZ"

// ---------------------------------------------------------------- types

typedef void (*t_PanelFn)(void* game, void* window, void* p3);
typedef void (*t_PrintVar)(void* fontMgr, void* font,
                           float x, float y, unsigned long colour,
                           const wchar_t* fmt, ...);

// v2 native widgets. Exact cities.cpp typedefs (live reference): the
// trailing narrow args are 1-byte chars on the stack - declared as char
// so the callee reads exactly what the exe's own callers write.
typedef int (*t_NativeSlider)(void* game, float x, float y, int textId,
                              unsigned* value, float scale, char enabled,
                              int step, char drag, void* hover);
typedef int (*t_NativeCheckbox)(void* game, float x, float y,
                                unsigned* value, const wchar_t* label,
                                unsigned long colour, void* font,
                                float size, float labelDy,
                                char live, char p11, char p12);

// v3 engine imports for the square button tile. C3DRECT/C3DVECTOR3 are
// passed by value per the mangled names; x64 passes both by reference
// under the hood, which MSVC reproduces from these declarations.
struct C3DRect { int left, top, right, bottom; };
struct C3DVec3 { float x, y, z; };
typedef void  (*t_Draw9Patch)(void* panel2d, C3DRect rect,
                              float borderX, float borderY,
                              float a, float b, float c, float d);
// v6.0 ABI FIX (WASD/mouse-camera/ESC corruption - root cause found):
//   The native GetMouseSolid disassembly (0x14034160a-0x140341635) proves the
//   MSVC x64 sret convention: the caller passes a HIDDEN result pointer in
//   RDX (LEA RDX,[RBP-0x30]) and the object in RCX. The old
//   `typedef C3DVec3 (*t_GetMouseSolid)(void*)` returned the 12-byte struct
//   BY VALUE, so MSVC passed nothing in RDX - the callee wrote 12 bytes of
//   mouse vector through whatever garbage was in RDX, smashing adjacent
//   stack/registers. That corruption is what killed WASD, drove the camera
//   with the mouse, and kept ESC dead. The old Collision typedef had the same
//   class of bug in reverse: the native takes the vector pointer in RDX
//   (MOV RDX,RAX at 0x140341646, the sret address GetMouseSolid returned),
//   but a 12-byte BY-VALUE struct parameter is NOT passed as a pointer in
//   RDX under MSVC x64 - so Collision read a garbage address. Both are now
//   declared exactly as the binary calls them: GetMouseSolid takes
//   (this, out*) - this STAYS in RCX for MSVC x64 member sret, hidden
//   result goes in RDX (native: LEA RCX,input; LEA RDX,[RBP-0x30]);
//   Collision takes (this, const-vector*).
typedef void  (*t_GetMouseSolid)(void* input, C3DVec3* out);
typedef bool  (*t_Collision)(void* panel2d, const C3DVec3* mouse,
                             float padX, float padY);
typedef void  (*t_PrintCenter)(void* fontMgr, void* font,
                               float x, float y, unsigned long colour,
                               const wchar_t* fmt, ...);
// v3.1 fallbacks: solid-frame Draw + NoArg left printer for manual centering.
typedef void  (*t_DrawPlain)(void* panel2d, float a, float b, float c,
                             float d, float e, bool f);
typedef void  (*t_PrintNoArg)(void* fontMgr, void* font,
                              float x, float y, unsigned long colour,
                              const wchar_t* text);

// exe-internal helpers (direct RVA, readability-checked like the widgets).
typedef void (*t_PlayGuiSound)(void* game, const char* name, int flag);
typedef void (*t_ClipPush)(void* clipCtx, float x, float y, int clipId);

static t_PrintVar      o_Print;
static t_NativeSlider  o_Slider;
static t_NativeCheckbox o_Checkbox;
static t_Draw9Patch    o_Draw9Patch;
static t_Collision     o_Collision;
static t_GetMouseSolid o_MouseSolid;
static t_PrintCenter   o_PrintCenter;
static t_DrawPlain     o_DrawPlain;    // v3.1 solid-frame fallback
static t_PrintNoArg    o_PrintNoArg;   // v3.1 manual-center fallback
static t_PlayGuiSound  o_PlaySound;
static t_ClipPush      o_ClipPush;
static int             g_buttonsReady;  // v3.1: core Draw9Patch+Collision+MouseSolid
static int             s_wasLButtonDown; // v3.1 physical mouse edge latch
static int             s_clickConsumed;  // v3.7 one click per physical press
// v5.6 ESC fix: the CON tile runs on the construction panel via the FOREIGN
// append (appendWidgetsToWindow), which fires AFTER o_PanelBuilder returns.
// There the widget's ClipPush (FUN_1404461f0) lands outside the builder's
// clip scope and is never popped -> a stray clip stays on game+0xADB0 and the
// engine's input routing treats it as an active modal capture, so ESC stops
// registering (can't cancel placement / can't close the site panel while it
// is open). The finished-panel path (AppendRowsImpl) runs inside the builder
// scope where the push is balanced, so PRIV is unaffected. Skip the clip only
// on the foreign path; the tile still draws (nothing clips it when no clip is
// live). Set/cleared around AppendWidgetsImpl in AppendWidgetsToWindowImpl.
static int             s_foreignClip;
// v3.2 bounded button diagnostics: first frames + state changes, hard cap
// so a session can never spam the log (60 lines max).
static int             s_btnProbe;
static int             s_btnPrevHover = -1, s_btnPrevLive = -1;
static int             s_btnPrevLatch = -1, s_btnPrevBusy = -999;
#define BTN_PROBE_MAX 60

// v5.9 ESC-close diagnostics. The v5.8 probe sampled 0xA54C91 at button-draw
// time, but the dispatcher's close logic (FUN_1406e9b80, runs BEFORE the draw
// loop) consumes that edge first - so the probe never saw escEdge=1. v5.9
// instead detects the PHYSICAL ESC key (GetAsyncKeyState, bypassing engine
// edge order) and logs the STABLE close-path fields that persist across the
// frame, so the log shows exactly which gate ESC fails on:
//   closeLatch = game+0xCB78  debounce latch (1 = every ESC-close skipped)
//   modalTool  = game+0xD428  placement tool (1 = ESC-close disabled)
//   escHeld    = 0xA54B91     engine ESC-held byte
//   escEdge    = 0xA54C91     engine ESC press-edge (usually 0 by draw time)
// Runs in AppendWidgetsImpl (every frame a button panel is open, not
// budget-gated). Hard-capped; one line per physical ESC press / modal change.
static int   s_escProbe;
static int   s_escWasDown;             // physical ESC held (GetAsyncKeyState)
static BYTE* s_escPrevWin  = nullptr;
static long long s_escPrevModal = -1;  // game+0xD428
#define ESC_PROBE_MAX 120

static float LayoutF(DWORD rva);   // defined ahead of the append pass

// ---------------------------------------------------------------- config

static int   g_enabled  = 1;
static int   g_probe    = 1;   // one log line per hook: y before/after the builder
static int   g_testRow  = 1;   // built-in smoke row - off once a provider fills
static int   g_maxRows  = 6;   // hard cap TSM_PANEL_MAX_ROWS

// Consumed from the currency service (v4 preferred, v3 fallback). Null if
// currency is not installed - the wallet row provider then no-ops safely.
static const TsmCurrencyApi* g_walletCurrency = nullptr;

// Window kind (window+0x248) of the panel currently being appended. Set by
// AppendRowsImpl before providers run. DIAGNOSTIC ONLY as of v6.6 - it is
// NOT the wallet row's gate. This comment claimed the provider gated on
// kind == 8 for months while the code gated on the hook index, and the
// kind-8 story was never true in the first place (the kind-8 builder at
// 0x7CFF50 never fires for citizens). The gate is the object kind published
// by RunHook: OBJ_PERSON means the builder is the citizen window 0x7CEEB0.
static int g_currentWindowKind = -1;
static unsigned long g_colour = 0xFF1A1A1Au;  // v4.0: native dark charcoal for
    // parchment legibility (was 0xFF990000 dark red - strained on beige).
    // Matches the TAX-tile BTN_LABEL_COLOUR family (0xFF202020); opaque.
// v4.7: residence PRIV-tile dock calibration, INI-tunable. Five screenshot
// rounds (v4.2-v4.6) walked the tile right and down toward the user's
// target; pitch 80 / rowDy 188 is the next decisive step (~22px right,
// ~16px down of v4.6). Tune with icon_tile_x_pitch / icon_tile_row_dy.
static float g_iconXPitch = 80.0f;
static float g_iconRowDy  = 188.0f;
// v5.0: tab-row dock geometry (the big tile next to native tab icons -
// TAX on City Hall, PRIV on shop/pub/factory panels). Calibrated for the
// City Hall: the old fixed 70-unit pitch put the tile ~22px right of the
// 4th tab icon (the natives advance at 63), and the 105+70 row term sat
// ~8px above the tab row - the tester's "too far right and a bit too
// up". INI-tunable.
static float g_tabXPitch = 63.0f;
static float g_tabRowDy  = 180.0f;   // v5.1: nudged up a few px from 185
// v5.9 CON position nudge (tester: "slightly too right, a little more up").
// Only the two INI knobs moved: x_base 33 -> 25 (left ~6px), y 0 -> -10 (up
// ~8px). Size/pitch/slot unchanged this round so the nudge is isolated. The
// live btn geom line prints the resulting rect, so base/y can be fine-tuned
// in the INI without a rebuild.
//
// v6.1: native construction toolbar geometry (constr_tbloop.asm:226-230,
// 319-320): 40-unit squares at 85 pitch. CON at anything else can never
// sit IN the row - the screenshot proved 70 towers over the natives.
static float g_tbY    = 4.0f;     // v6.3: up ~5px per tester (was 10)
static float g_tbSize = 40.0f;    // v6.1: native button square (was 70)
// v6.2: single-knob X. x is the whole story (legacy triple kept in code
// only as an x < 0 fallback, no longer in the INI).
static float g_tbX     = 438.0f;  // v6.3: left ~6px per tester (was 445)
static float g_tbBaseX  = 25.0f;  // v5.9: nudged left (was 33)
static float g_tbXPitch = 70.0f;  // native construction pitch is 85 - see note
static float g_tbSlot   = 6.0f;
static DWORD  g_fontRva = RVA_FONT_DEFAULT;

// ---------------------------------------------------------------- hooks

// Prologue bytes straight from Ghidra (docs/17 section 10). The loader
// memcmps these before patching and refuses on any mismatch - that refusal
// is the whole point: a game update costs rows, not a crash.
//
// shop 0x7272F0: register-only, 21 bytes, cookie load far past the steal.
// residence 0x761300: 19 bytes, ends immediately BEFORE a `call rel32`
// (chkstk) - never let a relative instruction into a stolen region.
// pub 0x723950 / factory 0x799630: 21-byte register-only prefix + LEA,
// verified in docs/18 (G2/G3); cookie load far past the steal.
static const BYTE kShopExpect[] = {
    0x48, 0x8B, 0xC4, 0x55, 0x56, 0x57,
    0x41, 0x54, 0x41, 0x55, 0x41, 0x56, 0x41, 0x57,
    0x48, 0x8D, 0xA8, 0x58, 0xF5, 0xFF, 0xFF
};
static const BYTE kResExpect[] = {
    0x40, 0x55, 0x56, 0x57, 0x41, 0x56,
    0x48, 0x8D, 0xAC, 0x24, 0xA8, 0xDB, 0xFF, 0xFF,
    0xB8, 0x68, 0x25, 0x00, 0x00
};
static const BYTE kPubExpect[] = {
    0x48, 0x8B, 0xC4, 0x55, 0x56, 0x57,
    0x41, 0x54, 0x41, 0x55, 0x41, 0x56, 0x41, 0x57,
    0x48, 0x8D, 0xA8, 0x68, 0xF5, 0xFF, 0xFF
};
static const BYTE kFactoryExpect[] = {
    0x48, 0x8B, 0xC4, 0x55, 0x56, 0x57,
    0x41, 0x54, 0x41, 0x55, 0x41, 0x56, 0x41, 0x57,
    0x48, 0x8D, 0xA8, 0xF8, 0xF3, 0xFF, 0xFF
};
// cityhall: City Hall / Party HQ panel builder 0x72DBF0 (FUN_14072DBF0,
// v1.1.1.9, UI investigation.md section 11.16). 17-byte register-only
// prefix ending right after the LEA at +0x0A - this is what the cities.cpp
// reference hook steals:
//   4C 8B DC                MOV R11,RSP           (3)
//   55 56 57                PUSH RBP RSI RDI      (3)  -> 6
//   41 54 41 57             PUSH R12 R15          (4)  -> 10
//   49 8D AB 98 F5 FF FF    LEA RBP,[R11-0xA68]   (7)  -> 17
// The __security_cookie load sits AFTER the steal (at +0x18), outside the
// window - hooking rule 2 satisfied. Type 39 routed by caller CMP ECX,0x27
// The provider path is TYPE-FILTERED to City Hall (39) so no other
// window routed here is touched.
static const BYTE kCityHallExpect[] = {
    0x4C, 0x8B, 0xDC,
    0x55, 0x56, 0x57,
    0x41, 0x54, 0x41, 0x57,
    0x49, 0x8D, 0xAB, 0x98, 0xF5, 0xFF, 0xFF
};

struct PanelHook {
    const char* name;
    DWORD       rva;
    size_t      stolen;
    const BYTE* expect;
    void*       trampoline;
    int         wanted;     // from ini
    int         installed;
    int         probed;     // y-before/after logged once
    int         appended;   // first append logged once
    int         disabled;   // v6.6: runtime kill for THIS hook only, after a
                            // fault inside its append. The detour keeps
                            // calling the real builder (so the window still
                            // draws), it just stops appending to it.
};

#define MAX_HOOKS 8
#define HOOK_STORAGE 7    // v6.11: type-5 storage panel, verified call-site hook
#define HOOK_CITYHALL 4     // index of the City Hall builder (0x72DBF0)
#define HOOK_CITIZEN  5     // index of the citizen panel builder (candidate RVA below)
#define HOOK_DO       6     // v6.7: distribution office (type 43), CALL-SITE hook

// v6.6 OBJECT-KIND DISCRIMINATOR (fixes the disappearing PRIV tile).
//
// window+0x240 is a UNION slot: Building* on building panels, Person* on
// the citizen panel (kind 2, builder 0x7CEEB0). Nothing in the append path
// used to care, so once the citizen hook went live it ran the WIDGET pass
// with a Person* as the `building` argument and every registered widget
// provider treated it as a Building:
//
//   enterprise_base::EnterpriseWidgetProvider -> TsmBuildingType(person)
//     -> *(void**)(person + 0x318)
//
// and person+0x318 is NOT a type descriptor: 0x118 + 4 * 0x80 = 0x318 is
// exactly the FIFTH demand record, i.e. the float pair {remaining, rate}.
// TsmIsValidGamePtr is a range check only, so a pair whose high dword is
// zero reads as a plausible pointer, the follow-up `*(int*)(td + 0x360)`
// takes an access violation, and the provider loop's __except does what it
// was written to do: `g_widgets[i].cb = NULL` - the provider is dropped for
// the session. Every building panel after that first citizen-panel render
// has no PRIV tile and logs no [PRIV TILE] line, for every building type,
// until the game restarts. Open a shop first and it still works; open a
// citizen, then a shop, and the tile is gone - which is exactly the
// reported "the Citizen UI hook broke the Privatisation buttons".
//
// The hook index is the authoritative discriminator (it names which
// builder fired), so the kind is derived from it, not guessed from the
// object. Widget providers are BUILDING-ONLY: there is no person variant
// of the TsmPanelWidgetProvider contract, so a person panel must never
// enter that pass.
enum { OBJ_NONE = 0, OBJ_BUILDING = 1, OBJ_PERSON = 2 };
static int g_currentObjectKind = OBJ_NONE;

static int HookServesBuilding(int idx)
{
    if (idx < 0 || idx >= MAX_HOOKS) return 0;
    return (idx == HOOK_CITIZEN) ? 0 : 1;
}

// The citizen panel builder: FUN_1407CEEB0 (RVA 0x7CEEB0), the dispatcher's
// kind-2 builder. FOUND BY DATA TRACE (2026-10-18), not by guessing kinds:
//   FindDisp person+0xD4/+0x110 -> FUN_1407BADA0 (the renderer):
//     CVTTSS2SI EAX,[R15+0xd4]      age float -> whole years (display)
//     CMP [R15+0x110],0x0/R14D      demand-count loop ("Current demands")
//     114 GetString + 84 PrintLeft + 8 Collision/GetMouseSolid = full panel
//   Xrefs: FUN_1407BADA0 has exactly one window wrapper - FUN_1407CEEB0 -
//   and FUN_1407CEEB0 has exactly one caller: the dispatcher kind-2 branch
//   (FUN_1406E9B80 at 0x1406EA931, CMP ECX,0x2 / MOV RDX,RSI / MOV RCX,RDI).
//   The wrapper honors the full window contract: reads window+0x240 (Person*)
//   into R9, geometry from +0x04/+0x08/+0x28/+0x2C * DPI, writes back
//   window+0x250 (MOVSS [RBX+0x250],XMM0 after the renderer returns).
//   NOTE: FUN_140317DB0 also calls FUN_1407BADA0 directly (sidebar/cityhall
//   list contexts) - those are NOT (game,window) builders, not hooked here.
//   Prior dead candidates: kind 8 (0x7CFF50) and kind 1 (0x7D5460) never
//   fired in-game; residence logs proved building windows are kind 0.
// 15-byte prologue (>=14, ends on an instruction boundary BEFORE the
// RIP-relative JNZ - never steal a rel32):
//   48 8B C4          MOV RAX,RSP
//   53                PUSH RBX
//   48 83 EC 40       SUB RSP,0x40
//   80 7A 01 00       CMP byte ptr [RDX+0x1],0x0   (hidden gate; RDX=window
//                                                   survives into trampoline)
//   48 8B DA          MOV RBX,RDX
//   0F 85 86 00 00 00 JNZ <epilogue>               <- NOT stolen (rel32)
// Trampoline-safe: the wrapper's shadow-space scratch (RAX+8/RAX+0x10 vs
// [RSP+0x50] read-back) stays self-consistent within the trampoline chain,
// and the epilogue (ADD RSP,0x40/POP RBX/RET) unwinds it - including the
// hidden-window early-out path.
// v6.14: storage inline-hook prologue - 22 bytes from live dump 0x4CABD0 = 40 55 57 41 54 41 56 41 57 48 8D AC 24 B0 C9 FF FF B8 50 37 00 00 (bumped from 21, fixes B0 C9 mismatch) (full 21 logged in diag, verified to match shop/factory layout)
static const BYTE kStorageExpect[] = {
    0x40, 0x55, 0x57, 0x41, 0x54, 0x41, 0x56, 0x41,
    0x57, 0x48, 0x8D, 0xAC, 0x24, 0xB0, 0xC9, 0xFF,
    0xFF, 0xB8, 0x50, 0x37, 0x00, 0x00
};
static const BYTE kCitizenExpect[] = {
    0x48, 0x8B, 0xC4, 0x53,
    0x48, 0x83, 0xEC, 0x40,
    0x80, 0x7A, 0x01, 0x00,
    0x48, 0x8B, 0xDA
};
// ---------------------------------------------------------------- v6.8 wide button
//
// widgetWideButton draws the same native button as widgetButton - same
// chrome texture, same engine Collision hit-test, same click latch, same
// two-row budget, same SEH - as a BAR instead of a square: width
// DPI*wide_button_width (280 units), height DPI*2*step. One call in
// enterprise_base changes the PRIV control from a 40-unit tile to a bar
// that can hold a sentence.
//
// The only geometry that had to be separated was width-vs-height: the tile
// used one `size` for both. Everything that was "size" as a WIDTH now reads
// `width`, everything that was height still reads `size`, and for a square
// the two are equal - so widgetButton, widgetButtonSmall, the TAX tile and
// the construction PC tile are arithmetically unchanged.

// ---------------------------------------------------------------- v6.7 DO
//
// THE BUG THIS FIXES. A distribution office has no PRIV tile. The window
// dispatcher FUN_1406E9B80 routes type 0x2B (43) to FUN_140741170, and that
// builder is in none of the six hook rows above - so no post-hook runs, no
// row or widget provider is ever asked about a DO, and enterprise_base's
// PrivateEnterpriseTile never draws. Naming the office "Depot [P]" never
// worked either: v8.5 removed the custom-name scan from the hot path.
//
// WHY A CALL-SITE HOOK AND NOT A SEVENTH INLINE HOOK. An inline hook needs
// >= 14 stolen bytes whose prologue is known and verifiable, and the DO
// builder's prologue has never been read. Patching the CALLER does not need
// the callee's prologue at all: we rewrite the rel32 of the `call
// FUN_140741170` inside the dispatcher and leave E8 in place, exactly the
// mechanism private_logistics uses on the goods-transfer sites (proven in
// game across 735 calls). A cave does:
//
//     <bracketed append pass>   ; same body as every other hook
//     jmp  FUN_140741170        ; and then the real builder, untouched
//
// SELF-VERIFYING. The call site is not hardcoded - it is found by scanning
// the dispatcher for an E8 whose rel32 resolves to exactly exeBase+0x741170.
// A false positive is impossible (the target check is exact), and if no such
// call exists the hook reports it and installs nothing. That means this
// feature degrades to "no tile on a DO", never to a crash or a corrupt
// window.
//
// ORDER IS THE SAME AS EVERY OTHER HOOK: the real builder runs first (it
// rewrites window+0x250 itself), then the append pass adds rows below the
// edge the builder just set. A call-site shim gets control both before and
// after the callee returns, so no trampoline and no reordering is needed -
// the shim simply calls FUN_140741170 by address and then appends.
#define RVA_DO_BUILDER      0x741170
#define RVA_STORAGE_BUILDER 0x4CABD0
#define RVA_STORAGE_CALLSITE 0x6EA80C
#define RVA_WINDOW_DISPATCH 0x6E9B80
#define DISPATCH_SCAN_LEN   0x8000   // 32 KiB - the switch plus its branches

static void* s_doCave      = nullptr;
static DWORD s_doCallAt    = 0;         // RVA of the patched call, 0 = none
static void* s_storageCave = nullptr;
static DWORD s_storageCallAt = 0;

static PanelHook g_hooks[MAX_HOOKS] = {
    { "shop",     0x7272F0, sizeof kShopExpect,    kShopExpect,    NULL, 0, 0, 0, 0, 0 },
    { "residence", 0x761300, sizeof kResExpect,    kResExpect,     NULL, 0, 0, 0, 0, 0 },
    { "pub",      0x723950, sizeof kPubExpect,    kPubExpect,     NULL, 0, 0, 0, 0, 0 },
    { "factory",  0x799630, sizeof kFactoryExpect, kFactoryExpect, NULL, 0, 0, 0, 0, 0 },
    { "cityhall", 0x72DBF0, sizeof kCityHallExpect, kCityHallExpect, NULL, 0, 0, 0, 0, 0 },
    { "citizen",  0x7CEEB0, sizeof kCitizenExpect,  kCitizenExpect,   NULL, 0, 0, 0, 0, 0 },
    // v6.7: stolen == 0 marks a CALL-SITE hook - there is no prologue to
    // verify, so the row carries the callee RVA and the installer scans for
    // the call. Everything downstream (providers, budget, probe) is shared.
    { "do",       0x741170, 0,                      NULL,             NULL, 0, 0, 0, 0, 0 },
    // v6.13: inline hook for 0x4CABD0 (21-byte prologue 40 55 57 41 54...), fallback to call-site if inline refuses
    { "storage",  0x4CABD0, sizeof kStorageExpect, kStorageExpect, NULL, 0, 0, 0, 0, 0 },
};

// ---------------------------------------------------------------- providers

#define MAX_PROVIDERS 8
struct PanelProvider {
    TsmPanelRowProvider cb;
    void*               ud;
};
// v6.6 / API v5: TWO tables, because window+0x240 is a union.
//   g_providers       -> building surfaces only (registerRowProvider)
//   g_personProviders -> person surfaces only   (registerPersonRowProvider)
// Before this split a single table fed every provider every surface, which
// is how a citizen panel deleted three building providers in one frame.
static PanelProvider g_providers[MAX_PROVIDERS];        // zeroed = free slot
static int           g_providerCount;                   // high-water mark
static PanelProvider g_personProviders[MAX_PROVIDERS];  // zeroed = free slot
static int           g_personProviderCount;             // high-water mark

static int RegisterRowProviderImpl(TsmPanelRowProvider cb, void* ud)
{
    if (!cb) return 0;
    for (int i = 0; i < MAX_PROVIDERS; i++)
    {
        if (g_providers[i].cb) continue;
        g_providers[i].cb = cb;
        g_providers[i].ud = ud;
        if (i + 1 > g_providerCount) g_providerCount = i + 1;
        Logf("ui_panels  row provider %d registered", i + 1);
        return i + 1;
    }
    Logf("ui_panels  provider table full - refusing");
    return 0;
}

static void UnregisterRowProviderImpl(int handle)
{
    if (handle < 1 || handle > MAX_PROVIDERS) return;
    g_providers[handle - 1].cb = NULL;
    g_providers[handle - 1].ud = NULL;
}

// v6.6 / API v5: the person-surface twin. Handles are per-table, so a
// consumer must remember which register call its handle came from - the
// two tables are never consulted together.
static int RegisterPersonRowProviderImpl(TsmPanelRowProvider cb, void* ud)
{
    if (!cb) return 0;
    for (int i = 0; i < MAX_PROVIDERS; i++)
    {
        if (g_personProviders[i].cb) continue;
        g_personProviders[i].cb = cb;
        g_personProviders[i].ud = ud;
        if (i + 1 > g_personProviderCount) g_personProviderCount = i + 1;
        Logf("ui_panels  person row provider %d registered", i + 1);
        return i + 1;
    }
    Logf("ui_panels  person provider table full - refusing");
    return 0;
}

static void UnregisterPersonRowProviderImpl(int handle)
{
    if (handle < 1 || handle > MAX_PROVIDERS) return;
    g_personProviders[handle - 1].cb = NULL;
    g_personProviders[handle - 1].ud = NULL;
}

static void MarkDirtyImpl(void* /*building*/) { /* reserved - see the header */ }

// ---------------------------------------------------------------- citizen wallet row
//
// Person-window heuristic: the citizen panel stores the Person* at
// window+0x240 (the same field building windows use for Building*). The
// provider gates on window kind (window+0x248 == 8) so it never fires on
// building windows (shop, residence, pub, factory, cityhall). As a
// secondary guard it also checks person fields (+0x110 demand count,
// +0xD4 age) which building objects typically won't satisfy both of.
static int g_currentHookIdx = -1;  // which hook fired (for provider gating)

static void CitizenWalletRowProvider(void* object, TsmPanelRows* out, void* /*ud*/)
{
    if (!g_walletCurrency || !out || !object) return;

    // Definitive gate: only fire from the citizen builder hook. The
    // prior kind==8 gate was wrong - the citizen panel is not kind 8
    // (the kind-8 builder at 0x7CFF50 never fires for citizens).
    // v6.6: the object kind published by RunHook is the primary gate
    // (it also protects the foreign append path, which never sets the
    // hook index); the index check stays as a second lock.
    if (g_currentObjectKind != OBJ_PERSON) return;
    if (g_currentHookIdx != HOOK_CITIZEN) return;

    // v6.6: once per window, not once per rendered frame. The old line ran
    // at panel render rate (~20 Hz) for as long as the citizen panel was
    // open, which is a log flood that hides the lines that matter.
    if (g_probe)
    {
        static void* s_lastWalletObj = NULL;
        if (object != s_lastWalletObj) {
            s_lastWalletObj = object;
            Logf("ui_panels  wallet row: person %p (window kind %d)",
                 object, g_currentWindowKind);
        }
    }

    // Person heuristic guards: demand count 0..7 at +0x110, sane age at +0xD4.
    if (!ReadablePtr(object, 0x118)) return;
    int demandN = *(int*)((const BYTE*)object + 0x110);
    if (demandN < 0 || demandN > TSM_PERSON_DEMAND_CAP) return;
    if (!ReadablePtr((const BYTE*)object + TSM_PERSON_AGE, sizeof(float))) return;
    float age = *(const float*)((const BYTE*)object + TSM_PERSON_AGE);
    if (!(age >= 0.0f && age <= 120.0f)) return;

    float bal = g_walletCurrency->getWalletBalance(object);

    wchar_t value[TSM_PANEL_VALUE];
    const char* sym = TsmCurrencySymbol(g_walletCurrency);
    _snwprintf_s(value, _countof(value), _TRUNCATE, L"%.2f %hs", (double)bal, sym);

    if (out->count >= out->capacity || out->count >= TSM_PANEL_MAX_ROWS) return;
    TsmPanelRow* r = &out->rows[out->count++];
    wcsncpy_s(r->label, TSM_PANEL_LABEL, L"Wallet:", _TRUNCATE);
    wcsncpy_s(r->value, TSM_PANEL_VALUE, value, _TRUNCATE);
    r->colour = 0;   // the hook's default label colour
}

// ---------------------------------------------------------------- v2 widgets

#define MAX_WIDGET_PROVIDERS 8
struct PanelWidgetProvider {
    TsmPanelWidgetProvider cb;
    void*                  ud;
};
static PanelWidgetProvider g_widgets[MAX_WIDGET_PROVIDERS]; // zeroed = free
static int                 g_widgetCount;                   // high-water mark
static int                 s_rowProviderFaults;               // v6.6: session
static int                 s_widgetProviderFaults;           // v6.6: session
                                                    // total, so a slow bleed of
                                                    // providers is one grep.

// The widget frame: valid only inside a widget provider callback, while
// its window's post-hook is running on the render thread.
struct WidgetFrame {
    int   active;    // 1 while a widget pass is running
    BYTE* game;      // root game object (TsmGameObject)
    BYTE* win;       // the panel window
    float left;      // row x (same x the text rows print at)
    float y;         // running bottom edge (== window+0x250)
    float dpi;
    float step;      // layout row step (35)
    int   live;      // window+0x20 != 0
    void* fontMgr;
    void* font;
    int   rowsUsed;  // text rows + widget rows consumed so far
    int   rowBudget; // g_maxRows clamped to TSM_PANEL_MAX_ROWS
};
static WidgetFrame g_frame;   // render-thread only, like the row buffer

static int RegisterWidgetProviderImpl(TsmPanelWidgetProvider cb, void* ud)
{
    if (!cb) return 0;
    for (int i = 0; i < MAX_WIDGET_PROVIDERS; i++)
    {
        if (g_widgets[i].cb) continue;
        g_widgets[i].cb = cb;
        g_widgets[i].ud = ud;
        if (i + 1 > g_widgetCount) g_widgetCount = i + 1;
        Logf("ui_panels  widget provider %d registered", i + 1);
        return i + 1;
    }
    Logf("ui_panels  widget table full - refusing");
    return 0;
}

static void UnregisterWidgetProviderImpl(int handle)
{
    if (handle < 1 || handle > MAX_WIDGET_PROVIDERS) return;
    g_widgets[handle - 1].cb = NULL;
    g_widgets[handle - 1].ud = NULL;
}

// One shared budget: text rows + widget rows <= g_maxRows. A widget
// helper with no budget left is a silent no-op (returns 0).
static int WidgetBudgetLeft(void)
{
    int left = g_frame.rowBudget - g_frame.rowsUsed;
    return (left > 0) ? left : 0;
}

// The cities.cpp gating recipe, owned here so no provider reimplements
// it: live = window+0x20, and skip while another widget drags
// (game+0xAD74 != -1). The caller already validated window and game
// readability for the text pass.
static int WidgetLiveAndFree(void)
{
    if (!g_frame.live) return 0;
    if (!g_frame.game || !ReadablePtr(g_frame.game + G_WIDGET_BUSY,
                                      sizeof(int))) return 0;
    return *(int*)(g_frame.game + G_WIDGET_BUSY) == -1;
}

static int WidgetSliderImpl(const wchar_t* caption, unsigned* value,
                            int step, int enabled)
{
    if (!g_frame.active || !caption || !value) return 0;
    if (!o_Slider || !o_Print) return 0;
    if (WidgetBudgetLeft() <= 0) return 0;
    if (*value > 100) *value = 100;
    if (step < 1)  step = 1;
    if (step > 50) step = 50;

    float dpi  = g_frame.dpi;
    float y    = g_frame.y;
    float capX = g_frame.left + dpi * WIDGET_LABEL_X;
    float sldX = g_frame.left + dpi * WIDGET_SLIDER_X;
    unsigned was = *value;

    if (g_frame.fontMgr && g_frame.font &&
        ReadablePtr(caption, sizeof(wchar_t)))
    {
        __try {
            o_Print(g_frame.fontMgr, g_frame.font, capX, y, g_colour,
                    L"%ls", caption);
        } __except (FaultFilter("ui_panels widget caption",
                                GetExceptionInformation())) {
            return 0;
        }
    }

    int live = (enabled && WidgetLiveAndFree()) ? 1 : 0;
    int changed = 0;
    __try {
        o_Slider(g_frame.game, sldX, y, -1, value, dpi,
                 (char)live, step, (char)live, NULL);
        changed = (*value != was) ? 1 : 0;
    } __except (FaultFilter("ui_panels widget slider",
                            GetExceptionInformation())) {
        *value = was;
        return 0;
    }

    if (*value > 100) *value = 100;
    g_frame.y = y + dpi * g_frame.step;
    g_frame.rowsUsed++;
    if (ReadablePtr(g_frame.win + W_BOTTOM, sizeof(float)))
        *(float*)(g_frame.win + W_BOTTOM) = g_frame.y;
    return changed;
}

static int WidgetCheckboxImpl(unsigned* value, const wchar_t* label,
                              unsigned long colour)
{
    if (!g_frame.active || !value || !label) return 0;
    if (!o_Checkbox) return 0;
    if (WidgetBudgetLeft() <= 0) return 0;
    if (*value > 1) *value = (*value != 0) ? 1 : 0;

    float dpi = g_frame.dpi;
    float y   = g_frame.y;
    float boxX = g_frame.left + dpi * WIDGET_LABEL_X;
    float boxY = y + dpi * WIDGET_CHECK_DY;
    unsigned was = (*value != 0) ? 1u : 0u;
    *value = was;

    float size = LayoutF(RVA_CHECK_SIZE);
    if (!(size > 4.0f && size < 200.0f)) size = 20.0f;
    unsigned long col = colour ? colour : g_colour;

    int live = WidgetLiveAndFree() ? 1 : 0;
    int changed = 0;
    __try {
        o_Checkbox(g_frame.game, boxX, boxY, value, label, col,
                   NULL, size, 0.0f, (char)live, (char)1, (char)0);
        if (*value != 0 && *value != 1) *value = was;
        changed = (*value != was) ? 1 : 0;
    } __except (FaultFilter("ui_panels widget checkbox",
                            GetExceptionInformation())) {
        *value = was;
        return 0;
    }

    g_frame.y = y + dpi * g_frame.step;
    g_frame.rowsUsed++;
    if (ReadablePtr(g_frame.win + W_BOTTOM, sizeof(float)))
        *(float*)(g_frame.win + W_BOTTOM) = g_frame.y;
    return changed;
}

// v3.1 helpers: resolve a live font + bind a live chrome texture.
// The empty-tile bug was both halves failing silently in City Hall.
static void* ResolveButtonFont(void)
{
    // v3.1 font fallback chain: frame font first (the game's own row font
    // the append pass resolved), then the three verified exe C3D_FONT*
    // slots: 0x994200 (shop/residence rows), 0x995220 (checkbox decomp
    // default when font==NULL), 0x995228 (menucand center-print font).
    if (g_frame.font && ReadablePtr(g_frame.font, sizeof(void*)))
        return g_frame.font;
    static const DWORD kFonts[] = {
        RVA_FONT_DEFAULT, RVA_FONT_ALT_A, RVA_FONT_ALT_B
    };
    for (unsigned i = 0; i < sizeof(kFonts) / sizeof(kFonts[0]); i++)
    {
        if (!ReadablePtr(g_exeBase + kFonts[i], sizeof(void*))) continue;
        void* f = *(void**)(g_exeBase + kFonts[i]);
        if (f && ReadablePtr(f, sizeof(void*))) return f;
    }
    return NULL;
}

static void* ResolveButtonFontMgr(void)
{
    if (g_frame.fontMgr) return g_frame.fontMgr;
    // C3D_FONTMANAGER singleton, by address (same object the slider,
    // checkbox and menucand prints all use).
    if (ReadablePtr(g_exeBase + RVA_FONTMGR, 16))
        return (void*)(g_exeBase + RVA_FONTMGR);
    return NULL;
}

static int BindButtonTexture(void)
{
    // v6.4: the small (toolbar) tile wears the WINDOW's own button texture -
    // the construction loop binds from R13+0xB210... (constr_tbloop.asm:181),
    // not the game object's ALT slots. Try those first on the small path so
    // PC matches demolish/R_ exactly; big tiles keep the generic chain.
    if (!g_frame.game || !ReadablePtr(g_exeBase + RVA_PANEL_STATE,
                                      sizeof(void*)))
        return 0;
    typedef void (*t_TexBind)(void* self, int slot, void* state);
    void* panelState = *(void**)(g_exeBase + RVA_PANEL_STATE);
    // v6.5: the window-slot search (kWinOff 0xB210/0xB218/0xB220) is GONE -
    // it read 45 KB out of bounds on g_frame.win/bld and flooded the log
    // with C0000005 ACCESS_VIOLATION exceptions. All button tiles now bind
    // exclusively from the proven game-object ALT-frame chain below.
    // v3.8 order: bevel/checkbox/slider/9-patch FRAME textures ONLY.
    // The tab-icon slots (0xB648..0xB670) are deliberately absent - they hold
    // the per-tab ICONS (tab 0 is the family portrait), which is what put a
    // citizen photo on the TAX tile in the live test.
    static const DWORD kTex[] = {
        G_TEX_ALT2, G_TEX_ALT3, G_TEX_ALT4, G_TEX_ALT1, G_TEX_9PATCH
    };
    for (unsigned i = 0; i < sizeof(kTex) / sizeof(kTex[0]); i++)
    {
        if (!ReadablePtr(g_frame.game + kTex[i], sizeof(void*))) continue;
        void* tex = *(void**)(g_frame.game + kTex[i]);
        if (!tex || !ReadablePtr(tex, sizeof(void*))) continue;
        __try {
            void** vtable = *(void***)tex;
            if (!vtable || !ReadablePtr(vtable, 15 * sizeof(void*))) continue;
            ((t_TexBind)vtable[0x70 / 8])(tex, 0, panelState);
            return 1;
        } __except (FaultFilter("ui_panels button texture",
                                GetExceptionInformation())) {
            continue;
        }
    }
    return 0;
}

// v3.1: print the tile label. PRIMARY = the proven PrintLeftUnicode
// import the text rows already use live in this hook, with the center X
// computed mathematically: cx = x + (buttonWidth - textWidth) / 2
// (textWidth approximated from the exe's font-size constant - fine for
// short labels like TAX). PrintLeftUnicodeNoArg and PrintCenterUnicode
// sit behind it as secondary paths. Opaque white 0xFFFFFFFF. SEH-bracketed
// at every step so one bad path degrades to the next, never to silence.
// v6.10: keep a caption inside its button.
//
// There is no text-measure entry point in the plugin API, so the glyph width
// is estimated. v3.8 used 0.70 x fs, which under-counts an uppercase run: the
// live session showed "PRIVATISE THIS BUILDING" (22 chars) estimating to 215
// px and still running past a 224 px bar, because the clamp below had already
// pushed its left edge to bx+2. This uses 0.78 x fs - deliberately generous -
// for BOTH the fit test and the centring, so the two agree.
//
// Labels that still do not fit are truncated and elided with three ASCII dots
// (not U+2026, which is not in every UI font). That is the guarantee: whatever
// a caller passes and whatever wide_button_width is set to, the text cannot
// leave the button.
static const wchar_t* FitLabelToWidth(const wchar_t* label, float width,
                                      float fs, wchar_t* buf, size_t cap)
{
    if (!label || !buf || cap < 8) return label;
    size_t len = wcslen(label);
    if (len == 0) return label;

    const float cell = fs * 0.78f;
    if (!(cell > 0.5f)) return label;

    const float usable = width - 8.0f;          // 4 px inset on each side
    if (!(usable > cell * 4.0f)) return label;  // absurdly narrow: leave alone

    int maxChars = (int)(usable / cell);
    if (maxChars >= (int)len) return label;     // fits as-is

    if (maxChars > (int)cap - 4) maxChars = (int)cap - 4;
    if (maxChars < 4) return label;

    size_t keep = (size_t)(maxChars - 3);
    for (size_t i = 0; i < keep; i++) buf[i] = label[i];
    buf[keep]     = L'.';
    buf[keep + 1] = L'.';
    buf[keep + 2] = L'.';
    buf[keep + 3] = 0;
    return buf;
}

static void PrintButtonLabel(void* mgr, void* font, float bx, float ty,
                             float width, float height, const wchar_t* label,
                             float* olx, float* ocy)
{
    if (!mgr || !font || !label) return;
    size_t len = wcslen(label);
    if (len == 0 || len > 64) return;
    // v6.9: WIDTH centres the glyphs, HEIGHT centres them vertically. They
    // were one parameter until v6.8's wide bar: a 224-unit caption passed as
    // `size` moved the baseline to ty + (224 - fs)/2 = ty + 106, i.e. 50
    // units BELOW a 56-unit bar, where the panel clip ate it. The live probe
    // said exactly that: rect y 1682, ly 1788. For a square the two are
    // equal, so every pre-v6.9 button is arithmetically unchanged.
    float cx = bx + width * 0.5f;

    // Approximate the centered X mathematically (directive A.1).
    // v6.10: 0.78*fs per glyph, the same factor FitLabelToWidth uses to
    // decide the fit, so centring and truncation cannot disagree.
    // (v3.8 used 0.70: it under-measured uppercase runs, which is the bleed.)
    float fs = LayoutF(RVA_FONT_SIZE);
    if (!(fs > 1.0f && fs < 200.0f)) fs = 14.0f;

    // v6.10: fit first, then centre the fitted string. len and the estimate
    // must describe the text that is actually printed, not the caller's.
    wchar_t fitbuf[96];
    label = FitLabelToWidth(label, width, fs, fitbuf, 96);
    len   = wcslen(label);
    if (len == 0) return;

    float approxW = (float)len * fs * 0.78f;
    float lx = cx - approxW * 0.5f;
    if (lx < bx + 2.0f) lx = bx + 2.0f;
    // Belt and braces: the fit above should make this unreachable, but a
    // wrong fs (or a caller passing width != the drawn width) must still not
    // push glyphs out of the button.
    if (lx + approxW > bx + width - 2.0f) {
        float slid = bx + width - 2.0f - approxW;
        lx = (slid > bx + 2.0f) ? slid : (bx + 2.0f);
    }

    // v3.8: centre the glyph box inside the tile. The row-text convention
    // (proven by the slider captions, which pass the row Y straight to
    // o_Print and land correctly) means the engine's Print Y is the TOP of
    // the glyph box, so the true vertical centre is ty + (size - fs)/2.
    // v3.7 used ty + size*0.25, which for a 70-unit tile and fs=14 drops the
    // baseline ~9 units low - the "TAX sits below the tile" report.
    float cy = ty + (height - fs) * 0.5f;
    if (ocy) *ocy = cy;
    if (olx) *olx = lx;      // v6.9: the probe reported 0 for every frame

    if (o_Print) {
        __try {
            o_Print(mgr, font, lx, cy, BTN_LABEL_COLOUR, L"%ls", label);
            return;
        } __except (FaultFilter("ui_panels button print",
                                GetExceptionInformation())) {
            // fall through to the NoArg path
        }
    }
    if (o_PrintNoArg) {
        __try {
            o_PrintNoArg(mgr, font, lx, cy, BTN_LABEL_COLOUR, label);
            return;
        } __except (FaultFilter("ui_panels button print noarg",
                                GetExceptionInformation())) {
            // fall through to PrintCenter
        }
    }
    if (o_PrintCenter) {
        __try {
            o_PrintCenter(mgr, font, cx, cy, BTN_LABEL_COLOUR, L"%ls", label);
        } __except (FaultFilter("ui_panels button center print",
                                GetExceptionInformation())) {
        }
    }
}

// v3.3: direct cursor hit-test. GetMouseSolid returned (0,0) on every
// probe frame of the live City Hall session, so hover is decided from
// the OS cursor instead: GetCursorPos -> ScreenToClient(game window)
// -> tile rect, all in the same client-pixel space the P2D rect globals
// (and the panel's own draw coords) live in. The game window handle is
// enumerated once per process and re-validated cheaply.
static BOOL CALLBACK s_EnumGameWnd(HWND h, LPARAM lp)
{
    DWORD pid = 0;
    GetWindowThreadProcessId(h, &pid);
    if (pid != GetCurrentProcessId() || !IsWindowVisible(h)) return TRUE;
    RECT rc;
    if (!GetClientRect(h, &rc)) return TRUE;
    if (rc.right - rc.left < 200 || rc.bottom - rc.top < 200) return TRUE;
    HWND* out = (HWND*)lp;
    if (!*out) { *out = h; return TRUE; }
    RECT best;
    if (GetClientRect(*out, &best) &&
        (rc.right - rc.left) * (rc.bottom - rc.top) >
        (best.right - best.left) * (best.bottom - best.top))
        *out = h;
    return TRUE;
}

static HWND GameWindow(void)
{
    static HWND s_hwnd;
    if (s_hwnd && IsWindow(s_hwnd)) return s_hwnd;
    s_hwnd = NULL;
    EnumWindows(s_EnumGameWnd, (LPARAM)&s_hwnd);
    return s_hwnd;
}

// v3.5: the natives' Collision compares GetMouseSolid's output (a raw
// cursor position) against the SAME rect globals Draw uses, so the UI
// rect globals are in SCREEN space. v3.3/v3.4 piped the cursor through
// ScreenToClient, which SUBTRACTS the window's non-client chrome (border
// + title bar). On a windowed game that shifted the entire hit rect
// down-right by exactly the chrome offset, so only the tile's lower-right
// sliver could ever register - the "only the lower part highlights" /
// "offset to the bottom right" symptom from both live tests. GetCursorPos
// yields screen coords with no conversion at all.
// The client point and the chrome offset are still captured for the probe
// so the next run proves the space instead of assuming it.
// v6.8: `w` is the box width; `size` stays the height. A square tile passes
// w == size, so the tile path is arithmetically identical to before.
static int CursorOverTile(float bx, float top, float size, float w,
                          LONG* osx, LONG* osy,
                          LONG* ocx, LONG* ocy,
                          LONG* ochromeX, LONG* ochromeY)
{
    if (osx) *osx = -1;
    if (osy) *osy = -1;
    if (ocx) *ocx = -1;
    if (ocy) *ocy = -1;
    if (ochromeX) *ochromeX = 0;
    if (ochromeY) *ochromeY = 0;

    POINT pt;
    if (!GetCursorPos(&pt)) return 0;
    if (osx) *osx = pt.x;
    if (osy) *osy = pt.y;

    HWND hwnd = GameWindow();
    if (hwnd) {
        POINT org = { 0, 0 };
        if (ClientToScreen(hwnd, &org)) {
            if (ochromeX) *ochromeX = org.x;
            if (ochromeY) *ochromeY = org.y;
            if (ocx) *ocx = pt.x - org.x;
            if (ocy) *ocy = pt.y - org.y;
        }
    }

    return (pt.x >= (LONG)bx && pt.x < (LONG)(bx + w) &&
            pt.y >= (LONG)top && pt.y < (LONG)(top + size)) ? 1 : 0;
}

// v3: the native square BUTTON TILE. Same draw-state machine every exe
// panel button runs (scratch/labor_chrome_decomp.c + ui_probe_menucand.c):
// set the C3D_PANEL2D current-rect globals, clip push, bind the game's
// chrome texture (vtable slot 0x70), GetMouseSolid + Collision for hover,
// hover/normal tint pair, DrawFrom9Patch, label, click-latch edge.
//
// v3.1 hardening (live City Hall defect: blank + unclickable tile):
//   - texture: BindButtonTexture tries game+0xB028 and the sibling chrome
//     slots the exe's own widgets use; a texture failure no longer aborts
//     the row advance (the old `return 0` leaked window+0x250);
//   - label: the row-text convention (v3.4) - the engine's Print Y IS the
//     row anchor (the slider captions prove it), and the 70-unit tile is
//     exactly two 35-unit pitches, so the label sits at ty + size/4. The
//     v3.3 -1.36*fs fudge is gone;
//   - hit-test: THE NATIVES' EXACT PAIR (v3.4) - GetMouseSolid(&DAT_
//     140a54b90) refreshes the C3D_INPUT singleton in place (the natives
//     discard the return), then Collision(&DAT_1409be060) takes ONLY the
//     panel pointer and reads the mouse from that singleton. v3.3 fed
//     Collision a caller-supplied (garbage) vector, killing the engine
//     path and leaving only the client-cursor fallback - which tested
//     panel-space coords and lit just a sliver. Collision tests the same
//     rect globals Draw used, so hit and draw always agree. The raw OS
//     cursor test remains only as a fallback for a dead engine path;
//   - clicks: engine latch 0xA54E91 OR a physical GetAsyncKeyState
//     (VK_LBUTTON) rising edge - City Hall's native tabs consume the
//     single-byte latch earlier in the frame, starving us before;
//   - `active` keeps the hover tint + a 2px sink while the panel is open;
//     a fresh click also sinks 2px on the click frame.
//
// v5.9 ESC-close diagnostic. Detects the PHYSICAL ESC key (GetAsyncKeyState,
// bypassing the engine's edge-consuming order) and logs the STABLE close-path
// fields the dispatcher (FUN_1406e9b80) gates on, so a live session shows
// exactly which gate ESC is failing on:
//   closeLatch = game+0xCB78  debounce latch (1 = every ESC-close skipped this
//                             frame; cleared only when ESC key is released)
//   modalTool  = game+0xD428  placement/modal tool (1 = ESC-close disabled)
//   escHeld    = 0xA54B91     engine ESC-held byte
//   escEdge    = 0xA54C91     engine ESC press-edge (usually 0 by draw time -
//                             the dispatcher consumes it BEFORE the draw loop)
//   hidden     = window+0x01, winClose = window+0x00, kind = window+0x34
// Fires one line per PHYSICAL ESC press, on a modal-tool change, and once per
// new window. Read-only, hard-capped, gated on g_probe.
static void EscCloseProbe(void)
{
    if (!g_probe || !g_frame.active) return;
    if (s_escProbe >= ESC_PROBE_MAX) return;
    if (!g_frame.game || !g_frame.win) return;

    BYTE* g = g_frame.game;
    BYTE* w = g_frame.win;

    // Physical ESC edge (Windows key state) - independent of engine edge order.
    int escDown   = (GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0;
    int escRising = (escDown && !s_escWasDown);
    s_escWasDown  = escDown;

    // Read every field defensively; a single unreadable field leaves it -1.
    long long closeLatch = -1, modal = -1;
    int engEdge = -1, engHeld = -1;
    int hidden = -1, winClose = -1, kind = -1;
    if (ReadablePtr(g + G_CLOSE_LATCH, 1))
        closeLatch = *(BYTE*)(g + G_CLOSE_LATCH) ? 1 : 0;
    if (ReadablePtr(g + G_MODAL_TOOL, sizeof(void*)))
        modal = (long long)*(void**)(g + G_MODAL_TOOL);
    if (ReadablePtr(g_exeBase + RVA_ESC_EDGE, 1))
        engEdge = *(BYTE*)(g_exeBase + RVA_ESC_EDGE) ? 1 : 0;
    if (ReadablePtr(g_exeBase + RVA_ESC_DOWN, 1))
        engHeld = *(BYTE*)(g_exeBase + RVA_ESC_DOWN) ? 1 : 0;
    if (ReadablePtr(w + W_HIDDEN, 1))  hidden   = *(BYTE*)(w + W_HIDDEN) ? 1 : 0;
    if (ReadablePtr(w + W_CLOSE, 1))   winClose = *(BYTE*)(w + W_CLOSE)  ? 1 : 0;
    if (ReadablePtr(w + W_KIND, sizeof(int))) kind = *(int*)(w + W_KIND);

    int newWin      = (s_escPrevWin != w);
    int modalChange = (modal != s_escPrevModal);

    if (escRising || newWin || modalChange)
    {
        s_escProbe++;
        Logf("ui_panels  ESC probe: %s | closeLatch %s modalTool %s | "
             "engEdge %d engHeld %d | win %p kind 0x%X hidden %d winClose %d",
             escRising ? "PHYSICAL ESC" : (newWin ? "new window" : "modal changed"),
             closeLatch ? "1(SKIPS CLOSE)" : "0",
             modal ? "SET(blocks ESC)" : "0",
             engEdge, engHeld, w, kind, hidden, winClose);
    }

    s_escPrevWin   = w;
    s_escPrevModal = modal;
}

// `wideUnits` > 0 draws a BAR of that many native units instead of a square
// tile of `bsz`. Height is unchanged - a bar occupies exactly the same two
// row pitches the square does - so the budget, the advance and every
// geometry guarantee below stay identical between the two shapes.
static int WidgetButtonImplEx(const wchar_t* label, int active, int small,
                              float wideUnits = 0.0f)
{
    if (!g_frame.active || !label) return 0;
    if (!g_buttonsReady) return 0;
    // v3.3: the native-sized tile occupies TWO row pitches (70 = 2x35).
    if (WidgetBudgetLeft() < 2) return 0;

    float dpi  = g_frame.dpi;
    // v6.x: bottom-edge content-flow placement. Y is the running bottom edge
    // (window+0x250) that the native builder just wrote; X is the native row
    // left edge (g_frame.left). Both read from the window struct at runtime,
    // so the button flows below native content regardless of DPI, resolution,
    // or monitor layout. Buttons advance window+0x250 by two pitches so they
    // sit cleanly below the rest of the panel content.
    float y    = g_frame.y;
    float bx   = g_frame.left + dpi * WIDGET_LABEL_X;

    // Native button size: residence icon row uses 65 units (RVA_ICON_BTN_SIZE),
    // everything else uses the native tab square (70 units, RVA_TAB_BTN_SIZE)
    // or the compile-time fallback (36 units). Small toolbar tiles use
    // g_tbSize (40 units) so they fit beside native toolbar buttons.
    float bsz;
    if (small) {
        bsz = g_tbSize;
        if (!(bsz >= 25.0f && bsz <= 90.0f)) bsz = WIDGET_BUTTON_SIZE;
    } else {
        bsz = LayoutF(RVA_TAB_BTN_SIZE);
        if (!(bsz >= 40.0f && bsz <= 90.0f)) bsz = WIDGET_BUTTON_SIZE;
        if (g_frame.win &&
            ReadablePtr(g_frame.win + W_BUILDING, sizeof(void*)))
        {
            void* bwin = *(void**)(g_frame.win + W_BUILDING);
            if (bwin && TsmIsValidGamePtr(bwin) && TsmBuildingType(bwin) == 2)
            {
                float ibsz = LayoutF(RVA_ICON_BTN_SIZE);
                if (ibsz >= 40.0f && ibsz <= 90.0f) bsz = ibsz;
            }
        }
    }
    // v6.8: a wide button keeps the square's HEIGHT (two row pitches) and
    // takes the width from the INI knob. `size` stays the height so every
    // downstream use of it - budget, rect bottom, label baseline, sink -
    // keeps the meaning it had for the tile.
    float size = dpi * bsz;
    float wide = 0.0f;
    if (wideUnits > 0.0f)
    {
        wide = dpi * wideUnits;
        const float minW = dpi * 120.0f;
        const float maxW = dpi * 900.0f;
        if (!(wide >= minW)) wide = minW;
        if (wide > maxW)     wide = maxW;
        // Height follows the ROW STEP, not the tile size: a bar should be two
        // rows tall like the tile it replaces, even where the tile is smaller.
        float rowH = g_frame.step > 1.0f ? g_frame.step : 35.0f;
        size = dpi * 2.0f * rowH;
    }
    const float width = (wide > 0.0f) ? wide : size;
    // v3.2: native City Hall tab buttons gate ONLY on window+0x20
    // (FUN_14072DBF0: `hover && *(char*)(window+0x20)`) - they do NOT
    // check game+0xAD74. The busy gate inside WidgetLiveAndFree was
    // starving this button of hover/clicks while native buttons worked.
    int   winLive = g_frame.live ? 1 : 0;
    int   clicked = 0;
    int   hover = 0, latch = 0, bound = 0, busy = -1;
    LONG  cxp = -1, cyp = -1;      // client point (probe only)
    LONG  sxp = -1, syp = -1;      // v3.5 screen point (hit-test space)
    LONG  chromeX = 0, chromeY = 0;// v3.5 client-origin in screen space
    C3DVec3 m = { 0.0f, 0.0f, 0.0f };
    float lblX = 0.0f, lblY = 0.0f;   // v3.4 probe: label print geometry
    int   engineHover = 0;             // v3.4 probe: engine Collision verdict

    // v3.1 physical edge: sample ONCE per frame, before any consumption,
    // so the rising edge is available even when the engine latch was
    // eaten by a native widget that ran earlier this frame.
    int isLDown = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
    int physEdge = (isLDown && !s_wasLButtonDown) ? 1 : 0;
    s_wasLButtonDown = isLDown;

    __try {
        *(float*)(g_exeBase + RVA_P2D_RECT_W) = width;
        *(float*)(g_exeBase + RVA_P2D_RECT_H) = size;
        // v6.x: set rect globals to CENTER here so Collision (below) and Draw
        // (later, with sink) both interpret them as center. With centered=true
        // the engine draws the quad CENTERED on the rect point; Collision uses
        // the same rect, so hit and draw agree by construction. Setting
        // top-left here (as v3.7 did) made Collision center on (bx, y) while
        // Draw centered on (bx+size/2, ...) � a -size/2 top-left offset
        // between hitbox and visuals.
        *(float*)(g_exeBase + RVA_P2D_RECT_X) = bx + width * 0.5f;
        *(float*)(g_exeBase + RVA_P2D_RECT_Y) = y + size * 0.5f;
        *(float*)(g_exeBase + RVA_P2D_ROT)    = 0.0f;

        // Balanced clip push: natives push the clip exactly when
        // game+0xADA8 is -1 or game+0xADF0, and pop it with the matching
        // call at the end of their frame. Replicating the same condition
        // keeps the clip stack balanced so the engine's Esc routing is not
        // trapped in a modal capture. The construction-panel CON tile runs
        // on the FOREIGN append (after the builder returns), so it must NOT
        // push a clip - the builder's clip scope is already closed by then
        // and a stray clip would trap ESC (can't cancel placement, can't
        // close the site panel). So skip the push on that path only.
        if (!s_foreignClip && o_ClipPush && ReadablePtr(g_frame.game + G_CLIP_ID, sizeof(int)) &&
            ReadablePtr(g_frame.game + G_CLIP_PAD, sizeof(int)))
        {
            int clipId = *(int*)(g_frame.game + G_CLIP_ID);
            if (clipId == -1 || clipId == *(int*)(g_frame.game + G_CLIP_PAD))
                o_ClipPush(g_frame.game + G_CLIP_CTX, bx, y, clipId);
        }

        // v3.2: texture chain (checkbox-box bevel first, bar texture last).
        bound = BindButtonTexture();

        // v3.6 hover: the natives' EXACT recipe (FUN_14072DBF0:0x14072ed00).
        // GetMouseSolid RETURNS the C3DVECTOR3 in RAX (sret pointer); the
        // natives pass THAT pointer straight to Collision as arg 2, with
        // padX/padY = 0.0. Collision tests the SAME rect globals Draw uses,
        // so hit and draw agree by construction - whatever offset the panel
        // draws at. v3.4/v3.5 declared Collision with one argument, so it
        // read three garbage registers and returned 0 every frame, which is
        // why the raw-cursor fallback was the only thing lighting up and
        // why the live tile was offset from its own clickable area.
        // v6.0 ABI: GetMouseSolid(this, out*) per the native sret convention
        // (0x140341610: LEA RCX,input; 0x14034160a: LEA RDX,[RBP-0x30]
        // hidden result). this STAYS in RCX, sret in RDX. The pre-v6.0
        // by-value call put the object in the wrong register and passed
        // nothing as the result pointer, corrupting input state (WASD dead,
        // mouse drove camera, ESC dead).
        if (winLive && o_MouseSolid && o_Collision) {
            m.x = m.y = m.z = 0.0f;
            __try {
                o_MouseSolid((void*)(g_exeBase + RVA_INPUT), &m);
            } __except (FaultFilter("ui_panels mouse solid",
                                    GetExceptionInformation())) {
                m.x = m.y = m.z = 0.0f;
            }
            if (m.x > -100000.0f && m.x < 100000.0f &&
                m.y > -100000.0f && m.y < 100000.0f)
                engineHover = o_Collision((void*)(g_exeBase + RVA_PANEL2D),
                                          &m, 0.0f, 0.0f) ? 1 : 0;
            if (engineHover) hover = 1;
        }
        // Fallback only when the engine path is unavailable or dead: the raw
        // OS cursor in screen space (v3.5 - see CursorOverTile).
        if (!hover && winLive) {
            if (CursorOverTile(bx, y, size, width, &sxp, &syp,
                               &cxp, &cyp, &chromeX, &chromeY)) hover = 1;
        }

        // v6.9: on the WIDE bar the caption carries the state
        // ("PRIVATISE THIS BUILDING" / "MAKE STATE-OWNED"), so the tint is
        // free to mean hover and nothing else. Before this, a private
        // building's bar was tinted 100 % of the time, so hovering it
        // changed nothing on screen - the "only the grocery store lights up"
        // report. Squares keep the old (hover || active) rule untouched.
        const int tintHot = (wide > 0.0f) ? hover : (hover || active);
        const void* tint = tintHot
            ? (const void*)(g_exeBase + RVA_TINT_HOVER)
            : (const void*)(g_exeBase + RVA_TINT_NORMAL);
        memcpy((void*)(g_exeBase + RVA_P2D_TINT), tint, 16);

        // v3.7 Click: engine latch OR physical rising edge, while hovered,
        // but AT MOST ONE click per physical press.
        // The live probe showed a single click producing TWO click frames -
        // the physical edge on frame N, then the engine latch still set on
        // frame N+1 (log: "latch 0 edge 1 click 1" immediately followed by
        // "latch 1 edge 0 click 1"). taxes.cpp toggles on every return-1, so
        // the panel opened then instantly closed again - the "tax info
        // disappears and I have to click the invisible button again" report.
        // s_clickConsumed latches until the button is actually released, so
        // the second signal is swallowed and one press = one toggle.
        if (ReadablePtr(g_exeBase + RVA_CLICK_LATCH, 1) &&
            *(const BYTE*)(g_exeBase + RVA_CLICK_LATCH) != 0)
            latch = 1;
        if (ReadablePtr(g_frame.game + G_WIDGET_BUSY, sizeof(int)))
            busy = *(int*)(g_frame.game + G_WIDGET_BUSY);   // diagnostics only

        // The engine latch can outlive the press that set it (the natives'
        // own consumers may not have run this frame), so re-arm only on a
        // real release: LButton up AND the engine latch cleared.
        if (!isLDown && !latch) s_clickConsumed = 0;

        if (hover && winLive && !s_clickConsumed && (latch || physEdge)) {
            if (latch)
                *(BYTE*)(g_exeBase + RVA_CLICK_LATCH) = 0;   // consume
            if (physEdge) s_wasLButtonDown = true;           // one-shot
            s_clickConsumed = 1;      // v3.7: one toggle per press
            if (o_PlaySound) o_PlaySound(g_frame.game, "gui_click", 1);
            clicked = 1;
        }

        int held = isLDown ||
                   (ReadablePtr(g_exeBase + RVA_MOUSE_DOWN, 1) &&
                    *(const BYTE*)(g_exeBase + RVA_MOUSE_DOWN) != 0);
        float sink = (active || clicked || (hover && held))
                         ? dpi * 2.0f : 0.0f;

        C3DRect rect;
        rect.left   = (int)bx;
        rect.top    = (int)(y + sink);
        rect.right  = (int)(bx + width);
        rect.bottom = (int)(y + sink + size);
        // v3.2/v3.8 NATIVE recipe - Draw with the rect coming from the
        // globals, then Draw(0,0,uv,uv,0,true).
        // v3.8 CRITICAL: the last parameter is `bool centered`, and 83 of the
        // 84 native C3D_PANEL2D::Draw calls in the corpus pass TRUE. With
        // centered == true the engine draws the quad CENTERED on the point in
        // the rect globals, i.e. the visual box is
        //   [cx - size/2, cx + size/2] x [cy - size/2, cy + size/2].
        // The natives know this and subtract the half-size themselves -
        // ch_builder_full.c:560 passes Y as `local_b08 + 0.0` where local_b08
        // already includes `- dpi*70`, and ch_tab1.c:832 passes
        // `fVar43 - dpi*fVar35`. v3.7 passed the TOP-LEFT and got a tile
        // painted a half-size up and left of the rect the hit-test and the
        // label use - the visible tile / invisible hotspot split reported
        // twice. So we pass the CENTRE and keep (bx, y) as the top-left for
        // Collision, the label and CursorOverTile. Sink stays on the Y global
        // so the pressed state shows in both draw paths.
        *(float*)(g_exeBase + RVA_P2D_RECT_X) = bx + width * 0.5f;
        *(float*)(g_exeBase + RVA_P2D_RECT_Y) = y + sink + size * 0.5f;
        float uv = LayoutF(RVA_UV_FULL);
        if (!(uv > 0.0f && uv <= 4.0f)) uv = 1.0f;
        if (o_DrawPlain && bound) {
            o_DrawPlain((void*)(g_exeBase + RVA_PANEL2D),
                        0.0f, 0.0f, uv, uv, 0.0f, true);
        } else if (o_Draw9Patch) {
            o_Draw9Patch((void*)(g_exeBase + RVA_PANEL2D), rect,
                         dpi * LayoutF(RVA_9P_BORDER),
                         dpi * LayoutF(RVA_9P_BORDER),
                         LayoutF(RVA_9P_UV_A), LayoutF(RVA_9P_UV_B),
                         LayoutF(RVA_9P_UV_A), LayoutF(RVA_9P_UV_B));
        }

        // v3.1 label: proven left printer + math center X first (the same
        // import the text rows use live), NoArg then PrintCenter behind it.
        void* mgr  = ResolveButtonFontMgr();
        void* font = ResolveButtonFont();
        PrintButtonLabel(mgr, font, bx, y + sink, width, size, label,
                         &lblX, &lblY);
    } __except (FaultFilter("ui_panels widget button",
                            GetExceptionInformation())) {
        // v3.1: a fault mid-draw still consumes the row so the window
        // chrome stays consistent (no overlapping next row).
    }

    // v3.4 bounded diagnostics: first frames + state changes, hard cap.
    // Answers definitively which gate is dead if the tile misbehaves:
    // the engine hover (Collision) vs the cursor fallback, the rect vs the
    // mouse space, live/busy/latch/edge, and the label geometry - so a
    // residual label/tile mismatch is one log line, not another blind
    // iteration.
    if (s_btnProbe < BTN_PROBE_MAX) {
        if (s_btnProbe < 3 || hover != s_btnPrevHover ||
            winLive != s_btnPrevLive || latch != s_btnPrevLatch ||
            busy != s_btnPrevBusy || clicked)
        {
            s_btnProbe++;
            s_btnPrevHover = hover; s_btnPrevLive = winLive;
            s_btnPrevLatch = latch; s_btnPrevBusy = busy;
            void* fnt = ResolveButtonFont();
            Logf("ui_panels  btn probe: rect (%.0f,%.0f)+%.0f  mouse (%.1f,%.1f)"
                 "  screen (%ld,%ld)  client (%ld,%ld)  chrome (%ld,%ld)"
                 "  engine %d  hover %d  live %d  busy %d  latch %d  edge %d"
                 "  click %d  tex %d  font %d  lx %.0f ly %.0f  used %d"
                 "  tab %d",
                 bx, y, size, m.x, m.y, (long)sxp, (long)syp,
                 (long)cxp, (long)cyp, (long)chromeX, (long)chromeY,
                 engineHover, hover, winLive, busy, latch, physEdge,
                 clicked, bound, fnt ? 1 : 0, lblX, lblY, s_clickConsumed,
                 0);
            // v5.2: raw window geometry dump so the toolbar dock can be
            // computed exactly instead of guessed. Prints once per probe
            // tick; the numbers pin W_Y/W_OFFY/W_X/W_OFFX/dpi for the
            // window the tile is docking in.
            if (g_frame.win &&
                ReadablePtr(g_frame.win + W_Y, sizeof(float)) &&
                ReadablePtr(g_frame.win + W_OFFY, sizeof(float)) &&
                ReadablePtr(g_frame.win + W_X, sizeof(float)) &&
                ReadablePtr(g_frame.win + W_OFFX, sizeof(float)))
            {
                Logf("ui_panels  btn geom : small %d  dpi %.3f  "
                     "W_Y %.1f W_OFFY %.1f  W_X %.1f W_OFFX %.1f  "
                     "(target y %.0f x %.0f)",
                     small, dpi,
                     *(float*)(g_frame.win + W_Y),
                     *(float*)(g_frame.win + W_OFFY),
                     *(float*)(g_frame.win + W_X),
                     *(float*)(g_frame.win + W_OFFX),
                     y, bx);
            }
        }
    }

    // v6.x: buttons are content rows - always advance window+0x250 by two
    // pitches so they sit cleanly below the rest of the panel content.
    g_frame.y = y + 2.0f * dpi * g_frame.step;
    g_frame.rowsUsed += 2;
    if (ReadablePtr(g_frame.win + W_BOTTOM, sizeof(float)))
        *(float*)(g_frame.win + W_BOTTOM) = g_frame.y;
    return clicked;
}

static int WidgetButtonImpl(const wchar_t* label, int active)
{
    return WidgetButtonImplEx(label, active, 0);
}

static int WidgetButtonSmallImpl(const wchar_t* label, int active)
{
    return WidgetButtonImplEx(label, active, 1);
}

// v6.8: the wide bar - "a normal big button". Same tile machinery, height
// two row pitches, width from wide_button_width.
static int WidgetWideButtonImpl(const wchar_t* label, int active)
{
    return WidgetButtonImplEx(label, active, 0, g_wideButtonW);
}

// v4.9: the widget pass for a window ui_panels does not itself hook.
// Mirrors AppendRowsImpl's readability + geometry checks but skips the
// row providers and row printing (widgets only). Called by
// private_construction from its own construction-panel hook so widget
// providers (enterprise_base's CON tile, the TAX tile on the City Hall
// chrome path) can draw on that window.
static void AppendWidgetsImpl(void* building, void* game, BYTE* w,
                              float x, float y, float dpi, float step,
                              void* fontMgr, void* font, int textRows);
static int AppendWidgetsToWindowImpl(void* game, void* win)
{
    if (!g_enabled || !o_Print || !win || !game) return 0;

    BYTE* w = (BYTE*)win;
    if (!ReadablePtr(w, W_BOTTOM + sizeof(float))) return 0;
    if (*(char*)(w + W_HIDDEN) != 0) return 0;

    void* building = *(void**)(w + W_BUILDING);
    if (!building || !ReadablePtr(building, 0x320)) return 0;
    if (g_widgetCount <= 0) return 0;

    float dpi = LayoutF(RVA_DPI);
    if (!(dpi > 0.2f && dpi < 8.0f)) dpi = 1.0f;

    float step   = LayoutF(RVA_ROW_STEP);
    float indent = LayoutF(RVA_X_INDENT);
    float back   = LayoutF(RVA_X_BACK);
    if (!(step   > 1.0f  && step   < 200.0f)) step   = 35.0f;
    if (!(indent >= 0.0f  && indent < 500.0f)) indent = 110.0f;
    if (!(back   >= 0.0f  && back   < 500.0f)) back   = 15.0f;

    float x = *(float*)(w + W_X) + *(float*)(w + W_OFFX)
            + dpi * (step + indent - back);
    float y = *(float*)(w + W_BOTTOM);
    if (!(y > 0.0f && y < 30000.0f)) return 0;   // no sane bottom edge yet

    void* fontMgr = g_exeBase + RVA_FONTMGR;
    void* font    = *(void**)(g_exeBase + g_fontRva);

    // v5.6: mark this as the FOREIGN append so the widget skips its ClipPush
    // (see s_foreignClip). The clip there would land outside the builder's
    // clip scope, stay unpopped, and block ESC.
    // v6.6: a foreign caller only exists for a building window
    // (private_construction's construction-site panel), so publish the kind
    // rather than inheriting whatever the last hook left behind.
    int prevKind = g_currentObjectKind;
    g_currentObjectKind = OBJ_BUILDING;
    s_foreignClip = 1;
    __try {
        AppendWidgetsImpl(building, game, w, x, y, dpi, step,
                          fontMgr, font, 0);
    }
    __except (FaultFilter("ui_panels foreign widget pass",
                          GetExceptionInformation()))
    {
        Logf("ui_panels  foreign widget pass faulted on %p - skipped",
             building);
    }
    s_foreignClip = 0;
    g_currentObjectKind = prevKind;
    return 1;
}

static const TsmUiPanelsApi kApi = {
    sizeof(TsmUiPanelsApi),
    RegisterRowProviderImpl,
    UnregisterRowProviderImpl,
    MarkDirtyImpl,
    RegisterWidgetProviderImpl,
    UnregisterWidgetProviderImpl,
    WidgetSliderImpl,
    WidgetCheckboxImpl,
    WidgetButtonImpl,
    WidgetButtonSmallImpl,
    AppendWidgetsToWindowImpl,
    RegisterPersonRowProviderImpl,     // v5
    UnregisterPersonRowProviderImpl,   // v5
    WidgetWideButtonImpl,              // v6
};

// Prefix views: truncated structSize stops before the later tails, so a
// consumer built against an older header reads exactly what it expects.
// v1 stops before the widget tail, v2 before the button tail, v3 before
// the v4 (toolbar tile + foreign-window append) tail, v4 before the v5
// person-surface row tail. A consumer's Has* helper reads structSize and
// never looks past it, so a v4 consumer is unaffected by v5 existing.
static TsmUiPanelsApi kApiV1;
static TsmUiPanelsApi kApiV2;
static TsmUiPanelsApi kApiV3;
static TsmUiPanelsApi kApiV4;
static TsmUiPanelsApi kApiV5;
static int            g_apiV1Ready = 0;
static int            g_apiV2Ready = 0;
static int            g_apiV3Ready = 0;
static int            g_apiV4Ready = 0;
static int            g_apiV5Ready = 0;

// ---------------------------------------------------------------- the append

// Layout constants live ahead of the widget helpers (WidgetCheckboxImpl
// reads the checkbox size through this).
static float LayoutF(DWORD rva) { return *(float*)(g_exeBase + rva); }

static void AppendWidgetsImpl(void* building, void* game, BYTE* w,
                              float x, float y, float dpi, float step,
                              void* fontMgr, void* font, int textRows)
{
    if (g_widgetCount <= 0) return;
    if (!building || !game || !w) return;

    // v6.6 HARD GATE: widget providers are building-only. This is the
    // single line that keeps a Person* out of the TsmPanelWidgetProvider
    // contract (see the object-kind note above RunHook). Belt and braces:
    // the caller already gates on the hook index, AND the object itself has
    // to look like a Building before any provider sees it. Both are cheap,
    // neither allocates, and a provider that would have faulted simply is
    // not called - which is what "the PRIV tile stopped appearing" needed.
    if (g_currentObjectKind != OBJ_BUILDING) {
        memset(&g_frame, 0, sizeof(g_frame));   // leave no stale frame live
        return;
    }
    if (!TsmLooksLikeBuilding(building)) {
        if (g_probe)
            Logf("ui_panels  widget pass refused: window+0x240 %p is not a "
                 "Building (kind %d) - providers not called", building,
                 g_currentObjectKind);
        memset(&g_frame, 0, sizeof(g_frame));
        return;
    }

    if (!ReadablePtr((BYTE*)game + TSM_GO_BLD_VEC_BEGIN, 16)) return;

    memset(&g_frame, 0, sizeof(g_frame));
    g_frame.active    = 1;
    g_frame.game      = (BYTE*)game;
    g_frame.win       = w;
    g_frame.left      = x;
    g_frame.y         = y;
    g_frame.dpi       = dpi;
    g_frame.step      = step;
    g_frame.fontMgr   = fontMgr;
    g_frame.font      = font;
    g_frame.rowsUsed  = textRows;
    g_frame.rowBudget = (g_maxRows < TSM_PANEL_MAX_ROWS) ? g_maxRows
                                                         : TSM_PANEL_MAX_ROWS;
    if (ReadablePtr(w + W_ACTIVE, 1))
        g_frame.live = (*(char*)(w + W_ACTIVE) != 0) ? 1 : 0;

    // v5.9: sample the engine's real ESC window-close state every frame this
    // panel is open (before the widget pass, so it is not budget-gated).
    // Read-only; hard-capped; gated on g_probe.
    EscCloseProbe();

    for (int i = 0; i < g_widgetCount; i++)
    {
        if (!g_widgets[i].cb) continue;

        // v6.6: capture the callback address BEFORE clearing the slot, so
        // the log names the provider that died instead of just its index -
        // a dropped provider is a silent, session-long feature loss, and
        // "provider 1" is not an answer a tester can act on.
        TsmPanelWidgetProvider cb = g_widgets[i].cb;
        __try { cb(building, g_widgets[i].ud); }
        __except (FaultFilter("ui_panels widget provider",
                              GetExceptionInformation()))
        {
            g_widgets[i].cb = NULL;
            s_widgetProviderFaults++;
            Logf("ui_panels  widget provider %d (cb %p) FAULTED on building "
                 "%p - disabled for the rest of the session (%d provider "
                 "fault(s) this session). This removes every widget it drew, "
                 "on every panel, until restart.",
                 i + 1, (void*)cb, building, s_widgetProviderFaults);
        }

        if (WidgetBudgetLeft() <= 0) break;
    }

    g_frame.active = 0;
}

static void AppendRowsImpl(void* game, void* win, int idx)
{
    if (!g_enabled || !o_Print || !win) return;

    BYTE* w = (BYTE*)win;
    if (!ReadablePtr(w, W_BOTTOM + sizeof(float))) return;
    if (*(char*)(w + W_HIDDEN) != 0) return;          // the builders' own first test

    // window+0x240 holds the Building* for building windows and the Person*
    // for the citizen panel. v6.6: the SURFACE is decided by the hook index
    // (published by RunHook) and decides which provider table runs, so this
    // pointer is only required to be readable - no provider is asked to
    // describe an object of the wrong kind any more.
    void* object = *(void**)(w + W_BUILDING);
    if (!object || !ReadablePtr(object, 8)) return;

    // Window kind (window+0x248) is DIAGNOSTIC ONLY. It is recorded so the
    // log can correlate panels with the dispatcher's routing; nothing gates
    // on it. (The comment that used to sit here claimed the wallet provider
    // gated on kind == 8, which was never true in code and never true in
    // the game - the kind-8 builder does not fire for citizens.)
    g_currentHookIdx = idx;
    if (ReadablePtr(w + 0x248, sizeof(int)))
        g_currentWindowKind = *(int*)(w + 0x248);
    else
        g_currentWindowKind = -1;

    // City Hall builder (0x72DBF0) - type 39 routed by the caller's
    // CMP ECX,0x27 (section 11.16). Keep the type filter so any other
    // window routed here stays byte-identical to vanilla: no rows, no
    // write back to window+0x250. The taxes provider self-gates too.
    if (idx == HOOK_CITYHALL && TsmBuildingType(object) != 39) return;

    // Gather: providers copy under their own lock and return plain numbers.
    // Static: the detour runs BEFORE the builder allocates its own 0xB80 /
    // 0x2568 frame, so this must not grow the stack either. The builders are
    // render-thread only, so one buffer is safe.
    static TsmPanelRows rows;
    memset(&rows, 0, sizeof(rows));
    rows.capacity = (g_maxRows < TSM_PANEL_MAX_ROWS) ? g_maxRows
                                                     : TSM_PANEL_MAX_ROWS;

    // v6.6 / API v5: run the table that matches the surface. A building
    // provider is never handed a Person* and a person provider is never
    // handed a Building* - the union at window+0x240 stops being a hazard
    // here rather than in six provider bodies.
    PanelProvider* table  = (g_currentObjectKind == OBJ_BUILDING)
                                ? g_providers : g_personProviders;
    int            nProv  = (g_currentObjectKind == OBJ_BUILDING)
                                ? g_providerCount : g_personProviderCount;
    if (g_currentObjectKind != OBJ_BUILDING && g_currentObjectKind != OBJ_PERSON)
    {
        nProv = 0;   // unknown surface: run nobody
    }

    for (int i = 0; i < nProv; i++)
    {
        if (!table[i].cb) continue;

        // v6.6: name the callback before clearing the slot - see the widget
        // loop for why "provider N" is not an actionable log line.
        TsmPanelRowProvider cb = table[i].cb;
        __try { cb(object, &rows, table[i].ud); }
        __except (FaultFilter("ui_panels provider", GetExceptionInformation()))
        {
            table[i].cb = NULL;
            s_rowProviderFaults++;
            Logf("ui_panels  %s row provider %d (cb %p) FAULTED on object %p "
                 "- disabled for the rest of the session (%d row fault(s)). "
                 "Every row it drew is gone until restart.",
                 (g_currentObjectKind == OBJ_BUILDING) ? "building" : "person",
                 i + 1, (void*)cb, object, s_rowProviderFaults);
        }

        if (rows.count >= rows.capacity) break;
    }

    // Built-in smoke row: proves the hook end-to-end before any provider exists.
    if (g_testRow && rows.count < rows.capacity)
    {
        TsmPanelRow* r = &rows.rows[rows.count++];
        wcscpy_s(r->label, TSM_PANEL_LABEL, L"ui_panels");
        wcscpy_s(r->value, TSM_PANEL_VALUE, L"row ok");
        r->colour = 0;
    }

    // v2.2: a collapsed widget-only panel pushes ZERO text rows (the City
    // Hall header toggle is a widget). Fall through to the widget pass;
    // if the widget providers also draw nothing the window stays
    // byte-identical to vanilla (helpers only touch +0x250 when a widget
    // is actually drawn).
    if (rows.count <= 0 && g_widgetCount <= 0) return;
    if (rows.count > rows.capacity) rows.count = rows.capacity;

    float dpi = LayoutF(RVA_DPI);
    if (!(dpi > 0.2f && dpi < 8.0f)) dpi = 1.0f;

    float step   = LayoutF(RVA_ROW_STEP);
    float indent = LayoutF(RVA_X_INDENT);
    float back   = LayoutF(RVA_X_BACK);
    if (!(step   > 1.0f  && step   < 200.0f)) step   = 35.0f;
    if (!(indent >= 0.0f  && indent < 500.0f)) indent = 110.0f;
    if (!(back   >= 0.0f  && back   < 500.0f)) back   = 15.0f;

    // Same expression the builders use for their own row x.
    float x = *(float*)(w + W_X) + *(float*)(w + W_OFFX)
            + dpi * (step + indent - back);
    float y = *(float*)(w + W_BOTTOM);
    if (!(y > 0.0f && y < 30000.0f)) {
        // v4.2: one-shot trace per hook (probe only). A persistent abort
        // here means the window's builder never laid out a bottom edge
        // (collapsed window / early-returning builder) and rows AND
        // widgets - including the PRIV tile - are skipped every frame.
        if (g_probe && !g_hooks[idx].appended) {
            g_hooks[idx].appended = 2;   // "aborted" trace; suppresses "appended"
            Logf("ui_panels  %s: +0x250 not sane (y %.1f) - rows/widgets skipped",
                 g_hooks[idx].name, y);
        }
        return;   // no sane bottom edge yet
    }

    void* fontMgr = g_exeBase + RVA_FONTMGR;
    void* font    = *(void**)(g_exeBase + g_fontRva);
    // v3.1: a NULL text font no longer aborts the whole pass - the text
    // rows are skipped but the widget pass still runs (widgetButton
    // resolves its own font through the fallback chain).
    float yStart = y;
    if (font) {
        for (int i = 0; i < rows.count; i++)
        {
            wchar_t line[TSM_PANEL_LABEL + TSM_PANEL_VALUE + 8];
            _snwprintf_s(line, _countof(line), _TRUNCATE, L"%ls: %ls",
                         rows.rows[i].label, rows.rows[i].value);
            unsigned long col = rows.rows[i].colour ? rows.rows[i].colour
                                                    : g_colour;
            o_Print(fontMgr, font, x, y, col, L"%ls", line);
            y += dpi * step;
        }
        // Give the window the rows back, so the frame grows instead of clipping.
        *(float*)(w + W_BOTTOM) = y;
    }

    // v2: run the widget pass at the new bottom edge. Helpers advance
    // window+0x250 themselves; read it back for the probe log below.
    // v3.1: textRows = 0 when the rows could not be printed (no space was
    // taken), so the widget budget stays honest.
    //
    // v6.6: PERSON SURFACES DO NOT GET A WIDGET PASS. The citizen panel's
    // window+0x240 is a Person*, and the widget contract only knows about
    // buildings - running it there is what disabled enterprise_base's PRIV
    // tile provider for a whole session. AppendWidgetsImpl enforces this
    // itself; this branch just makes the intent readable and skips the
    // geometry work.
    if (g_currentObjectKind == OBJ_BUILDING)
    {
        AppendWidgetsImpl(object, game, w, x, y, dpi, step,
                          fontMgr, font, font ? rows.count : 0);
        y = *(float*)(w + W_BOTTOM);
    }
    else if (g_probe && !g_hooks[idx].appended)
    {
        Logf("ui_panels  %s: person surface - %d text row(s) appended, "
             "widget pass skipped (providers are building-only)",
             g_hooks[idx].name, rows.count);
    }

    if (!g_hooks[idx].appended)
    {
        g_hooks[idx].appended = 1;
        if (g_probe)
            Logf("ui_panels  %s: appended %d row(s), y %.1f -> %.1f (x %.1f, dpi %.2f)",
                 g_hooks[idx].name, rows.count, yStart, y, x, dpi);
    }
}

// One detour body for every hook: run the game's own builder first (it
// rewrites +0x250 itself), probe it once, then append.
//
// `real` is how the builder is reached: the trampoline for an inline hook,
// or the plain function address for a call-site hook (v6.7 DO). Everything
// after the call is identical, which is the point - a new window costs a
// table row, not a second copy of the append logic.
static void RunHookBody(void* g, void* w, void* p3, int idx, t_PanelFn real)
{
    PanelHook* hk = &g_hooks[idx];
    if (!real) return;

    // v6.6: a hook disabled by a fault still has to run the GAME's builder
    // (the window has to draw); it just stops appending to it.
    if (hk->disabled)
    {
        real(g, w, p3);
        return;
    }

    float y0 = 0.0f;
    int probe = g_probe && !hk->probed && w && ReadablePtr(w, W_BOTTOM + sizeof(float));
    if (probe) y0 = *(float*)((BYTE*)w + W_BOTTOM);

    real(g, w, p3);

    if (probe)
    {
        float y1 = *(float*)((BYTE*)w + W_BOTTOM);
        hk->probed = 1;
        Logf("ui_panels  probe %s: builder bottom %.1f -> %.1f  %s",
             hk->name, y0, y1,
             (y1 > y0 || y0 <= 0.0f) ? "(maintains +0x250)"
                                     : "(DID NOT ADVANCE - rows may misplace)");
    }

    // v6.x DEBUG: throttled log of hook fire with window kind to debug
    // citizen hook. Only logs when a new window pointer appears (open),
    // not every frame (builders re-render ~20x/sec while open).
    BYTE* ww = (BYTE*)w;
    if (ww && ReadablePtr(ww + 0x248, sizeof(int)))
    {
        int kind = *(int*)(ww + 0x248);
        if (idx == HOOK_CITIZEN || kind == 8 || g_probe)
        {
            static void* s_lastDbgWin = nullptr;
            if (w != s_lastDbgWin)
            {
                s_lastDbgWin = w;
                uintptr_t obj = 0;
                if (ReadablePtr(ww + W_BUILDING, sizeof(void*)))
                    obj = (uintptr_t)*(void**)(ww + W_BUILDING);
                Logf("ui_panels  DEBUG hook=%s idx=%d kind=%d  object=0x%llx",
                     hk->name, idx, kind, obj);
            }
        }
    }

    // v6.6: publish WHICH surface this pass is for, and restore the previous
    // value afterwards - including across the __except below, which is the
    // whole point of doing it here rather than inside AppendRowsImpl (that
    // function has eight early returns, and a stale kind is exactly the
    // class of bug this is fixing).
    int prevKind = g_currentObjectKind;
    int prevIdx  = g_currentHookIdx;
    g_currentHookIdx    = idx;
    g_currentObjectKind = HookServesBuilding(idx) ? OBJ_BUILDING : OBJ_PERSON;

    __try { AppendRowsImpl(g, w, idx); }
    __except (FaultFilter("ui_panels append", GetExceptionInformation()))
    {
        // v6.6: a fault that reaches here used to switch the whole plugin
        // off (g_enabled = 0) - rows AND widgets, on every panel, for the
        // rest of the session. That turns any one bad provider into a
        // global outage. Disable only the append for the offending hook so
        // the other five keep working and the log names the culprit.
        g_hooks[idx].disabled = 1;
        Logf("ui_panels  append faulted on hook %s (idx %d) - that window "
             "gets no rows/widgets for this session, the other %d are "
             "untouched", hk->name, idx, MAX_HOOKS - 1);
    }

    g_currentHookIdx    = prevIdx;
    g_currentObjectKind = prevKind;
}

static void RunHook(void* g, void* w, void* p3, int idx)
{
    RunHookBody(g, w, p3, idx, (t_PanelFn)g_hooks[idx].trampoline);
}

// v6.7: the DO window. No trampoline exists because nothing was overwritten
// in the callee - the shim calls FUN_140741170 by address.
static void h_Do(void* g, void* w, void* p3)
{
    RunHookBody(g, w, p3, HOOK_DO, (t_PanelFn)(g_exeBase + RVA_DO_BUILDER));
}

// v6.11: verified type-5 builder route. RCX/RDX carry the same game/window
// pair used by the shared panel hook body; the builder ignores the spare R8.
static void h_Storage(void* g, void* w, void* p3)
{
    RunHookBody(g, w, p3, HOOK_STORAGE,
                (t_PanelFn)(g_exeBase + RVA_STORAGE_BUILDER));
}

static void h_Shop(void* g, void* w, void* p3)      { RunHook(g, w, p3, 0); }
static void h_Residence(void* g, void* w, void* p3) { RunHook(g, w, p3, 1); }
static void h_Pub(void* g, void* w, void* p3)       { RunHook(g, w, p3, 2); }
static void h_Factory(void* g, void* w, void* p3)   { RunHook(g, w, p3, 3); }
static void h_CityHall(void* g, void* w, void* p3)  { RunHook(g, w, p3, HOOK_CITYHALL); }
static void h_Citizen(void* g, void* w, void* p3)    { RunHook(g, w, p3, HOOK_CITIZEN); }

static void* s_detours[MAX_HOOKS] = {
    (void*)h_Shop, (void*)h_Residence, (void*)h_Pub, (void*)h_Factory,
    (void*)h_CityHall, (void*)h_Citizen, (void*)h_Do, (void*)h_Storage
};

// ---------------------------------------------------------------- v6.7 call-site hook
//
// Rewrite the rel32 of `call FUN_140741170` inside the window dispatcher so
// it lands in a nearby cave that does:
//
//     call FUN_140741170        ; the real builder, by absolute address
//     <h_Do body>               ; append rows / run widget providers
//     ret
//
// The `E8` opcode at the call site is preserved. Everything is verified
// before a byte is written and the site is re-verified under the write
// protection, exactly as private_logistics does on the transfer sites.
static bool InstallDoCallSiteHook(void)
{
    if (!ReadablePtr(g_exeBase + RVA_WINDOW_DISPATCH, 16) ||
        !ReadablePtr(g_exeBase + RVA_DO_BUILDER, 16)) {
        Logf("ui_panels  do: the dispatcher or the DO builder is not readable "
             "at the expected RVA - no tile on a distribution office");
        return false;
    }

    BYTE* const target = g_exeBase + RVA_DO_BUILDER;

    // Scan the dispatcher for an E8 whose rel32 resolves to the DO builder.
    // A match is exact, so a false positive is impossible; a missed call is
    // reported rather than guessed at.
    unsigned found = 0;
    DWORD    firstAt = 0;
    for (DWORD off = 0; off + 5 <= DISPATCH_SCAN_LEN; off++) {
        const BYTE* p = g_exeBase + RVA_WINDOW_DISPATCH + off;
        if (!ReadablePtr(p, 5)) break;
        if (p[0] != 0xE8) continue;
        const int32_t rel = *(const int32_t*)(p + 1);
        if (p + 5 + rel != target) continue;
        if (!found) firstAt = RVA_WINDOW_DISPATCH + off;
        found++;
    }

    if (!found) {
        Logf("ui_panels  do: no `call 0x%06X` found in the dispatcher "
             "0x%06X..+0x%X - the DO window still has no PRIV tile. Nothing "
             "was written. Need FUN_140741170's call site (or its prologue "
             "for an inline hook) from Ghidra.",
             RVA_DO_BUILDER, RVA_WINDOW_DISPATCH, DISPATCH_SCAN_LEN);
        return false;
    }

    if (found > 1)
        Logf("ui_panels  do: %u calls to 0x%06X found; hooking the first "
             "(0x%06X). If the tile draws twice, this is why.",
             found, RVA_DO_BUILDER, firstAt);

    BYTE* site = g_exeBase + firstAt;
    if (!ReadablePtr(site, 5)) {
        Logf("ui_panels  do: call site 0x%06X unreadable - nothing written", firstAt);
        return false;
    }

    BYTE* cave = AllocNear(site, 64);
    if (!cave) {
        Logf("ui_panels  do: no code cave near 0x%06X - nothing written", firstAt);
        return false;
    }

    // Cave: FF 25 00000000 <imm64 h_Do>   jmp qword ptr [rip+0]
    static const BYTE kJmpAbs[6] = { 0xFF, 0x25, 0x00, 0x00, 0x00, 0x00 };
    memcpy(cave, kJmpAbs, sizeof(kJmpAbs));
    void* detour = (void*)&h_Do;
    memcpy(cave + 6, &detour, sizeof(detour));
    FlushInstructionCache(GetCurrentProcess(), cave, 14);

    const int64_t toCave = (int64_t)(cave - (site + 5));
    if (toCave > 0x7FFFFFFFLL || toCave < -0x80000000LL) {
        Logf("ui_panels  do: cave out of rel32 range - nothing written");
        return false;
    }

    DWORD oldProtect = 0;
    if (!VirtualProtect(site, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        Logf("ui_panels  do: VirtualProtect failed at 0x%06X - nothing written", firstAt);
        return false;
    }

    // Re-verify under the protection: still a call to the DO builder?
    if (site[0] != 0xE8 || site + 5 + *(const int32_t*)(site + 1) != target) {
        VirtualProtect(site, 5, oldProtect, &oldProtect);
        Logf("ui_panels  do: the call site changed under us - nothing written");
        return false;
    }

    site[0] = 0xE8;                              // same opcode: still a call
    *(int32_t*)(site + 1) = (int32_t)toCave;
    FlushInstructionCache(GetCurrentProcess(), site, 5);
    VirtualProtect(site, 5, oldProtect, &oldProtect);

    s_doCave   = cave;
    s_doCallAt = firstAt;
    Logf("ui_panels  do: hook 0x%06X -> DO builder 0x%06X ok (cave %p). "
         "The PRIV tile now draws on a distribution office window.",
         firstAt, RVA_DO_BUILDER, (void*)cave);
    return true;
}


// v6.12 diag: dump every E8 in the dispatcher so the warehouse builder can be
// identified from a live log without a new Ghidra run. Warehouse_covered_combined
// (type 5) never fired 0x4CABD0 - 1 site at 0x6EA80C installed but zero
// "probe storage" in 1688- and 1949-line logs. It must use a second storage
// builder in the same 0x4Cxxxx region or a separate dispatcher arm. This helper
// logs each E8 target RVA and its first 8 bytes; the storage installer can then
// be extended to hook all storage-like candidates (0x4A0000..0x4E0000) instead
// of only 0x4CABD0 in the next build.
static void DumpDispatcherTargets(void)
{
    Logf("ui_panels  storage diag: dumping dispatcher 0x%06X..+0x%X E8 targets (warehouse variant hunt)", RVA_WINDOW_DISPATCH, DISPATCH_SCAN_LEN);
    unsigned total = 0, storageish = 0;
    for (DWORD off = 0; off + 5 <= DISPATCH_SCAN_LEN; off++) {
        const BYTE* p = g_exeBase + RVA_WINDOW_DISPATCH + off;
        if (!ReadablePtr(p, 5)) break;
        if (p[0] != 0xE8) continue;
        const int32_t rel = *(const int32_t*)(p + 1);
        BYTE* target = (BYTE*)p + 5 + rel;
        if (target < g_exeBase + 0x1000 || target >= g_exeBase + 0xA00000) continue;
        DWORD rva = (DWORD)(target - g_exeBase);
        BYTE head[8] = {0};
        if (ReadablePtr(target, sizeof(head))) memcpy(head, target, sizeof(head));
        const bool isStorageRange = (rva >= 0x4A0000 && rva <= 0x4E0000);
        if (isStorageRange) storageish++;
        total++;
        Logf("ui_panels  dispatcher E8 at 0x%06X -> 0x%06X  head %02X %02X %02X %02X %02X %02X %02X %02X%s", RVA_WINDOW_DISPATCH + off, rva, head[0], head[1], head[2], head[3], head[4], head[5], head[6], head[7], isStorageRange ? "  <- storage-range" : "");
        if (isStorageRange && ReadablePtr(target, 21)) {
            Logf("ui_panels  storage builder 0x%06X prologue 21: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X", rva, target[0],target[1],target[2],target[3],target[4],target[5],target[6],target[7],target[8],target[9],target[10],target[11],target[12],target[13],target[14],target[15],target[16],target[17],target[18],target[19],target[20]);
        }
    }
    Logf("ui_panels  storage diag: %u calls, %u in storage range 0x4A0000..0x4E0000", total, storageish);
    if (ReadablePtr(g_exeBase + RVA_STORAGE_BUILDER, 32)) {
        BYTE* s = g_exeBase + RVA_STORAGE_BUILDER;
        Logf("ui_panels  storage builder direct 0x%06X 32: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X", RVA_STORAGE_BUILDER, s[0],s[1],s[2],s[3],s[4],s[5],s[6],s[7],s[8],s[9],s[10],s[11],s[12],s[13],s[14],s[15],s[16],s[17],s[18],s[19],s[20],s[21],s[22],s[23],s[24],s[25],s[26],s[27],s[28],s[29],s[30],s[31]);
    }
    if (ReadablePtr(g_exeBase + RVA_WINDOW_DISPATCH, 32)) {
        BYTE* d = g_exeBase + RVA_WINDOW_DISPATCH;
        Logf("ui_panels  dispatcher prologue 0x%06X: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X", RVA_WINDOW_DISPATCH, d[0],d[1],d[2],d[3],d[4],d[5],d[6],d[7],d[8],d[9],d[10],d[11],d[12],d[13],d[14],d[15]);
    }
}

// v6.13: storage uses INLINE hook on 0x4CABD0 (21-byte prologue verified from live dump 40 55 57 41 54...), so warehouse_covered_combined is caught even when opened outside dispatcher. Falls back to call-site patch at 0x6EA80C if inline fails (e.g. already patched).
static bool TryInstallStorageInlineHook(void)
{
    // g_hooks[HOOK_STORAGE] now carries kStorageExpect (21 bytes, stolen=21)
    PanelHook* hk = &g_hooks[HOOK_STORAGE];
    if (!hk->expect || hk->stolen < 14) return false;
    return InstallInlineHook(g_exeBase + hk->rva, s_detours[HOOK_STORAGE], &hk->trampoline, hk->expect, hk->stolen, hk->name);
}
static bool InstallStorageCallSiteHook(void)
{
    if (!ReadablePtr(g_exeBase + RVA_STORAGE_BUILDER, 16)) {
        Logf("ui_panels  storage: builder unreadable at RVA 0x%06X - nothing written",
             RVA_STORAGE_BUILDER);
        return false;
    }
    BYTE* const target = g_exeBase + RVA_STORAGE_BUILDER;
    unsigned found = 0;
    DWORD firstAt = 0;
    for (DWORD off = 0; off + 5 <= DISPATCH_SCAN_LEN; off++) {
        const BYTE* p = g_exeBase + RVA_WINDOW_DISPATCH + off;
        if (!ReadablePtr(p, 5)) break;
        if (p[0] != 0xE8) continue;
        const int32_t rel = *(const int32_t*)(p + 1);
        if (p + 5 + rel != target) continue;
        if (!found) firstAt = RVA_WINDOW_DISPATCH + off;
        found++;
    }
    if (!found) {
        Logf("ui_panels  storage: no call 0x%06X found in dispatcher 0x%06X..+0x%X - storage panel will not get checkbox. Need builder call site from Ghidra.",
             RVA_STORAGE_BUILDER, RVA_WINDOW_DISPATCH, DISPATCH_SCAN_LEN);
        return false;
    }
    if (found > 1)
        Logf("ui_panels  storage: %u calls to 0x%06X found; hooking first (0x%06X). If checkbox draws twice, this is why.",
             found, RVA_STORAGE_BUILDER, firstAt);

    BYTE* site = g_exeBase + firstAt;
    if (!ReadablePtr(site, 5)) {
        Logf("ui_panels  storage: call site 0x%06X unreadable - nothing written", firstAt);
        return false;
    }
    // Diagnostic: keep the original exact-byte expectation for the known site
    static const BYTE expect[5] = { 0xE8, 0xBF, 0x03, 0xDE, 0xFF };
    if (firstAt == RVA_STORAGE_CALLSITE && memcmp(site, expect, sizeof(expect)) != 0) {
        Logf("ui_panels  storage: call site 0x%06X bytes mismatch (expected E8 BF 03 DE FF) - still hooking the rel32 target", firstAt);
    }
    if (site + 5 + *(const int32_t*)(site + 1) != target) {
        Logf("ui_panels  storage: call site target mismatch - nothing written");
        return false;
    }

    BYTE* cave = AllocNear(site, 64);
    if (!cave) {
        Logf("ui_panels  storage: no code cave near 0x%06X - nothing written",
             firstAt);
        return false;
    }
    static const BYTE kJmpAbs[6] = { 0xFF, 0x25, 0, 0, 0, 0 };
    memcpy(cave, kJmpAbs, sizeof(kJmpAbs));
    void* detour = (void*)&h_Storage;
    memcpy(cave + 6, &detour, sizeof(detour));
    FlushInstructionCache(GetCurrentProcess(), cave, 14);

    const int64_t rel = (int64_t)(cave - (site + 5));
    if (rel < -0x80000000LL || rel > 0x7FFFFFFFLL) {
        Logf("ui_panels  storage: cave outside rel32 range - nothing written");
        return false;
    }
    DWORD oldProtect = 0;
    if (!VirtualProtect(site, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        Logf("ui_panels  storage: VirtualProtect failed - nothing written");
        return false;
    }
    if (site[0] != 0xE8 || site + 5 + *(const int32_t*)(site + 1) != target) {
        VirtualProtect(site, 5, oldProtect, &oldProtect);
        Logf("ui_panels  storage: call site changed under us - nothing written");
        return false;
    }
    site[0] = 0xE8;
    *(int32_t*)(site + 1) = (int32_t)rel;
    FlushInstructionCache(GetCurrentProcess(), site, 5);
    VirtualProtect(site, 5, oldProtect, &oldProtect);

    s_storageCave = cave;
    s_storageCallAt = firstAt;
    Logf("ui_panels  storage: hook call 0x%06X -> builder 0x%06X installed (cave %p, %u site(s) found)",
         firstAt, RVA_STORAGE_BUILDER, (void*)cave, found);
    return true;
}

// ---------------------------------------------------------------- settings


// Comma/semicolon list, case-insensitive, whole-token match - "shop" must
// not match a hypothetical "shopwindow".
static int ListHas(const char* list, const char* name)
{
    if (!list || !name) return 0;
    size_t n = strlen(name);
    const char* p = list;
    while (*p)
    {
        while (*p == ' ' || *p == ',' || *p == ';' || *p == '\t') p++;
        if (!*p) break;
        const char* start = p;
        while (*p && *p != ',' && *p != ';' && *p != '\r' && *p != '\n') p++;
        const char* end = p;
        while (end > start && (end[-1] == ' ' || end[-1] == '\t')) end--;
        if ((size_t)(end - start) == n && _strnicmp(start, name, n) == 0) return 1;
    }
    return 0;
}

static void ReadSettings(void)
{
    const char* ini = "plugins\\ui_panels.ini";
    char v[160];

    g_enabled = H->configInt(ini, "ui_panels", "enabled", 1);
    g_probe   = H->configInt(ini, "ui_panels", "probe",   1);
    g_testRow = H->configInt(ini, "ui_panels", "test_row", 1);
    g_maxRows = H->configInt(ini, "ui_panels", "max_rows", 6);
    if (g_maxRows < 1)                    g_maxRows = 1;
    if (g_maxRows > TSM_PANEL_MAX_ROWS)   g_maxRows = TSM_PANEL_MAX_ROWS;

    H->configString(ini, "ui_panels", "wide_button_width", v, sizeof(v), "280");
    g_wideButtonW = strtof(v, NULL);
    if (!(g_wideButtonW >= 120.0f && g_wideButtonW <= 900.0f)) g_wideButtonW = 280.0f;

    H->configString(ini, "ui_panels", "colour", v, sizeof(v), "0xFF1A1A1A");
    g_colour = (unsigned long)strtoul(v, NULL, 0);
    if ((g_colour & 0xFF000000u) == 0) g_colour |= 0xFF000000u;   // force opaque

    H->configString(ini, "ui_panels", "font", v, sizeof(v), "0x994200");
    g_fontRva = (DWORD)strtoul(v, NULL, 0);
    if (!ReadablePtr(g_exeBase + g_fontRva, sizeof(void*)))
        g_fontRva = RVA_FONT_DEFAULT;

    H->configString(ini, "ui_panels", "enable", v, sizeof(v),
                    "shop,residence,pub,factory,cityhall,citizen,do,storage");
    for (int i = 0; i < MAX_HOOKS; i++)
        g_hooks[i].wanted = ListHas(v, g_hooks[i].name);

    // v6.1: toolbar-row dock for widgetButtonSmall (construction
    // privatisation tile). Native toolbar geometry from the builder loop
    // (constr_tbloop.asm): 40-unit squares, top at content+dpi*15.
    // v6.x: buttons now use content-flow placement (g_frame.y, g_frame.left);
    // toolbar_tile_size is kept for small-tile sizing only. The absolute X/Y
    // knobs below are deprecated - buttons no longer use absolute offsets.
    H->configString(ini, "ui_panels", "toolbar_tile_size", v, sizeof(v), "40");
    g_tbSize = strtof(v, NULL);
    if (!(g_tbSize >= 25.0f && g_tbSize <= 90.0f)) g_tbSize = 40.0f;

    // Deprecated positioning knobs - still read for backward compatibility
    // but no longer used; buttons flow natively via g_frame.y / g_frame.left.
    H->configString(ini, "ui_panels", "toolbar_tile_y", v, sizeof(v), "4");
    g_tbY = strtof(v, NULL);
    if (!(g_tbY >= -90.0f && g_tbY <= 300.0f)) g_tbY = 4.0f;
    H->configString(ini, "ui_panels", "toolbar_tile_x", v, sizeof(v), "438");
    g_tbX = strtof(v, NULL);
    if (!(g_tbX >= -1.0f && g_tbX <= 2000.0f)) g_tbX = 438.0f;
    H->configString(ini, "ui_panels", "toolbar_tile_x_base", v, sizeof(v), "25");
    g_tbBaseX = strtof(v, NULL);
    if (!(g_tbBaseX >= -40.0f && g_tbBaseX <= 120.0f)) g_tbBaseX = 25.0f;
    H->configString(ini, "ui_panels", "toolbar_tile_x_pitch", v, sizeof(v), "70");
    g_tbXPitch = strtof(v, NULL);
    if (!(g_tbXPitch >= 20.0f && g_tbXPitch <= 90.0f)) g_tbXPitch = 70.0f;
    H->configString(ini, "ui_panels", "icon_tile_x_pitch", v, sizeof(v), "80");
    g_iconXPitch = strtof(v, NULL);
    if (!(g_iconXPitch >= 40.0f && g_iconXPitch <= 140.0f)) g_iconXPitch = 80.0f;
    H->configString(ini, "ui_panels", "icon_tile_row_dy", v, sizeof(v), "188");
    g_iconRowDy = strtof(v, NULL);
    if (!(g_iconRowDy >= 40.0f && g_iconRowDy <= 300.0f)) g_iconRowDy = 188.0f;
    H->configString(ini, "ui_panels", "tab_tile_x_pitch", v, sizeof(v), "63");
    g_tabXPitch = strtof(v, NULL);
    if (!(g_tabXPitch >= 40.0f && g_tabXPitch <= 140.0f)) g_tabXPitch = 63.0f;
    H->configString(ini, "ui_panels", "tab_tile_row_dy", v, sizeof(v), "185");
    g_tabRowDy = strtof(v, NULL);
    if (!(g_tabRowDy >= 40.0f && g_tabRowDy <= 300.0f)) g_tabRowDy = 185.0f;
    H->configString(ini, "ui_panels", "toolbar_tile_x_slot", v, sizeof(v), "6");
    g_tbSlot = strtof(v, NULL);
    if (!(g_tbSlot >= 0.0f && g_tbSlot <= 14.0f)) g_tbSlot = 6.0f;
}

// ---------------------------------------------------------------- lifecycle

extern "C" __declspec(dllexport) unsigned TsmPluginApiVersion(void)
{
    return TSM_API_VERSION;
}

extern "C" __declspec(dllexport) int TsmPluginInit(const TsmHost* host,
                                                   TsmPluginInfo* info)
{
    TsmBind(host);
    info->name    = "ui_panels";
    info->version = "6.14";  // v6.12: dispatcher diagnostic for warehouse variant

    ReadSettings();
    if (!g_enabled) {
        Logf("ui_panels  disabled");
        return 1;
    }

    // Quad-publish: v1 prefix (text rows only), v2 prefix (slider +
    // checkbox widgets), v3 prefix (button tail), and the full v4 iface
    // (toolbar tile + foreign-window append). Same truncated-structSize
    // precedent as currency, citizen_labor and taxes.
    kApiV1 = kApi;
    kApiV1.structSize = (unsigned)offsetof(TsmUiPanelsApi, markDirty) +
                        sizeof(kApiV1.markDirty);
    kApiV2 = kApi;
    kApiV2.structSize = (unsigned)offsetof(TsmUiPanelsApi, widgetCheckbox) +
                        sizeof(kApiV2.widgetCheckbox);
    kApiV3 = kApi;
    kApiV3.structSize = (unsigned)offsetof(TsmUiPanelsApi, widgetButton) +
                        sizeof(kApiV3.widgetButton);
    kApiV4 = kApi;
    kApiV4.structSize = (unsigned)offsetof(TsmUiPanelsApi, appendWidgetsToWindow) +
                        sizeof(kApiV4.appendWidgetsToWindow);
    kApiV5 = kApi;
    kApiV5.structSize = (unsigned)offsetof(TsmUiPanelsApi, unregisterPersonRowProvider) +
                        sizeof(kApiV5.unregisterPersonRowProvider);
    g_apiV1Ready = 1;
    g_apiV2Ready = 1;
    g_apiV3Ready = 1;
    g_apiV4Ready = 1;
    g_apiV5Ready = 1;
    H->provide(TSM_SERVICE_UI_PANELS, TSM_UI_PANELS_VERSION_V1, &kApiV1);
    H->provide(TSM_SERVICE_UI_PANELS, TSM_UI_PANELS_VERSION_V2, &kApiV2);
    H->provide(TSM_SERVICE_UI_PANELS, TSM_UI_PANELS_VERSION_V3, &kApiV3);
    H->provide(TSM_SERVICE_UI_PANELS, TSM_UI_PANELS_VERSION_V4, &kApiV4);
    H->provide(TSM_SERVICE_UI_PANELS, TSM_UI_PANELS_VERSION_V5, &kApiV5);
    H->provide(TSM_SERVICE_UI_PANELS, TSM_UI_PANELS_VERSION, &kApi);
    return 0;
}

extern "C" __declspec(dllexport) int TsmPluginStart(void)
{
    if (!g_enabled) return 1;

    if (void** slot = FindIatSlot(g_exe, DLL_ENGINE, SYM_PRINT_VARIADIC))
        o_Print = (t_PrintVar)*slot;
    if (!o_Print) {
        Logf("ui_panels  no PrintLeftUnicode import - inactive");
        return 1;
    }

    if (!ReadablePtr(g_exeBase + RVA_ROW_STEP, sizeof(float))) {
        Logf("ui_panels  the layout constants are not where this build puts "
             "them - inactive");
        return 1;
    }

    // v2 widgets: direct RVA calls (no import slot). cities.cpp runs the
    // same pair live on v1.1.1.9. Readability-checked so a game update
    // degrades to "no widgets" rather than a fault.
    if (ReadablePtr(g_exeBase + RVA_SLIDER, 16))
        o_Slider = (t_NativeSlider)(g_exeBase + RVA_SLIDER);
    if (ReadablePtr(g_exeBase + RVA_CHECKBOX, 16))
        o_Checkbox = (t_NativeCheckbox)(g_exeBase + RVA_CHECKBOX);
    if (!o_Slider || !o_Checkbox)
        Logf("ui_panels  WARN  native widgets unreadable - text rows only");

    // v3 button pieces: the four panel/input imports (all verified in
    // the exe IAT) plus two exe-internal helpers by RVA. The v3 iface is
    // ALWAYS published (Init ran first) - if anything failed to resolve,
    // g_buttonsReady stays 0 and widgetButton no-ops, so v3 consumers
    // degrade to "button returns 0" instead of faulting.
    if (void** slot = FindIatSlot(g_exe, DLL_ENGINE, SYM_DRAW_9PATCH))
        o_Draw9Patch = (t_Draw9Patch)*slot;
    if (void** slot = FindIatSlot(g_exe, DLL_ENGINE, SYM_COLLISION))
        o_Collision = (t_Collision)*slot;
    if (void** slot = FindIatSlot(g_exe, DLL_ENGINE, SYM_MOUSE_SOLID))
        o_MouseSolid = (t_GetMouseSolid)*slot;
    if (void** slot = FindIatSlot(g_exe, DLL_ENGINE, SYM_PRINT_CENTER))
        o_PrintCenter = (t_PrintCenter)*slot;
    // v3.1 optional pieces: solid-frame Draw + NoArg left printer (both
    // verified in the engine export table); PrintCenter optional since
    // PrintButtonLabel leads with the proven o_Print import.
    if (void** slot = FindIatSlot(g_exe, DLL_ENGINE, SYM_DRAW_PLAIN))
        o_DrawPlain = (t_DrawPlain)*slot;
    if (void** slot = FindIatSlot(g_exe, DLL_ENGINE, SYM_PRINT_LEFT_NOARG))
        o_PrintNoArg = (t_PrintNoArg)*slot;
    if (ReadablePtr(g_exeBase + FN_PLAY_GUI_SOUND, 16))
        o_PlaySound = (t_PlayGuiSound)(g_exeBase + FN_PLAY_GUI_SOUND);
    if (ReadablePtr(g_exeBase + FN_CLIP_PUSH, 16))
        o_ClipPush = (t_ClipPush)(g_exeBase + FN_CLIP_PUSH);
    // v3.2: readiness is the native Draw (or 9-patch fallback) + mouse
    // pair. PrintCenter, sound and clip all have fallback paths (or are
    // cosmetic), so a missing one degrades a feature, not the whole button.
    g_buttonsReady = ((o_DrawPlain || o_Draw9Patch) && o_MouseSolid) ? 1 : 0;
    if (!g_buttonsReady)
        Logf("ui_panels  WARN  button core pieces incomplete "
             "(draw %d, mouse %d) - widgetButton inactive",
             (o_DrawPlain != NULL) || (o_Draw9Patch != NULL),
             (o_MouseSolid != NULL));
    else if (!(o_Collision && o_PrintCenter && o_PlaySound && o_ClipPush))
        Logf("ui_panels  button degraded pieces: collision %d, center %d, "
             "sound %d, clip %d (fallbacks active)",
             (o_Collision != NULL), (o_PrintCenter != NULL),
             (o_PlaySound != NULL), (o_ClipPush != NULL));

    // v6.12 diag: log all dispatcher E8 targets before installing hooks so a
    // warehouse_covered_combined open can be correlated without a Ghidra run.
    // Costs one scan (~32 KiB) at startup; no hooks are added yet.
    DumpDispatcherTargets();

    int installed = 0;
    for (int i = 0; i < MAX_HOOKS; i++)
    {
        if (!g_hooks[i].wanted) {
            Logf("ui_panels  hook %-10s off in ini", g_hooks[i].name);
            continue;
        }
        bool ok = false;
        if (i == HOOK_DO) ok = InstallDoCallSiteHook();
        else if (i == HOOK_STORAGE) {
            // prefer inline (catches all callers), fallback to call-site patch
            ok = TryInstallStorageInlineHook();
            if (!ok) {
                Logf("ui_panels  storage: inline hook refused, trying call-site fallback at 0x6EA80C");
                ok = InstallStorageCallSiteHook();
            }
        } else ok = InstallInlineHook(g_exeBase + g_hooks[i].rva, s_detours[i], &g_hooks[i].trampoline, g_hooks[i].expect, g_hooks[i].stolen, g_hooks[i].name);
        if (ok)
        {
            g_hooks[i].installed = 1;
            installed++;
        }
        else {
            Logf("ui_panels  hook %s refused (see above) - that window gets "
                 "no rows", g_hooks[i].name);
        }
    }

    // Consume the currency service so the citizen wallet row can read the
    // current balance. v4 preferred, v3 fallback; null if currency is not
    // installed, in which case CitizenWalletRowProvider no-ops safely.
    g_walletCurrency = (const TsmCurrencyApi*)
        H->consume(TSM_SERVICE_CURRENCY, TSM_CURRENCY_VERSION);
    if (!g_walletCurrency)
        g_walletCurrency = (const TsmCurrencyApi*)
            H->consume(TSM_SERVICE_CURRENCY, TSM_CURRENCY_VERSION_V3);
    // v6.6 / API v5: the wallet row is a PERSON-surface provider. Registering
    // it in the building table (as pre-v6.6 did) worked only because the
    // provider self-gated on the hook index - it was still invoked with a
    // Building* on every shop/residence/pub/factory panel and returned
    // immediately, once per frame, forever.
    if (g_walletCurrency)
        RegisterPersonRowProviderImpl(CitizenWalletRowProvider, nullptr);

    if (!installed) {
        Logf("ui_panels  no panel hook installed - inactive");
        return 1;
    }

    Logf("ui_panels  live v6.14: %d hook(s), max_rows %d, font 0x%X, "
         "colour 0x%08lX, test_row %d, probe %d, buttons %s, "
         "toolbar size %.0f, "
         "ESC probe %s, wallet row %s",
         installed, g_maxRows, g_fontRva, g_colour, g_testRow, g_probe,
         g_buttonsReady ? "on (content-flow, black label)" : "off",
         g_tbSize,
         (g_probe && g_buttonsReady) ? "on (physical ESC + game+0xCB78/0xD428)"
                                     : "off",
         g_walletCurrency ? "on (currency consumed)" : "off (no currency service)");
    Logf("ui_panels  provider tables: %d building row, %d person row, "
         "%d widget (widgets are building-only by contract)",
         g_providerCount, g_personProviderCount, g_widgetCount);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID) { return TRUE; }
