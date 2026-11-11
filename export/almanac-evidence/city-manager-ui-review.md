# W&R City Manager UI reference — static review

**Scope:** Read-only review of the files under `Mods for analysis/City manager`. I inspected source text, the README/LICENSE, and the supplied `previewimage.png`. I did not run the launcher, Python, JavaScript, or shortcut; install packages; or extract `assets/game_assets.zip`. This is a companion-app UI review, not evidence about the game's native UI or a recommendation to reuse its implementation.

## Short answer

The appeal is a consistent Workers & Resources-style visual language: warm paper panels, a dark red title/tab color, restrained red section labels, compact icon-led information, and clear separation between controls and data. The preview image (`Mods for analysis/City manager/previewimage.png`) shows that treatment in the desktop calculator. The same visual ideas could inform an Almanac, but the City Manager is a separate, always-on-top desktop application—not an in-game sidebar page.

The local README describes city/resource calculations, a building catalog, and a planner, in Ukrainian, English, and German (`README.md:1-5`). The Python window identifies itself as an always-on-top overlay calculator (`wr_calc.py:1-3`); it is built with PySide6 widgets, while the map planner is an HTML/JavaScript page (`wr_calc.py:97-106`, `wr_calc.py:2583-2589`, `planner/index.html:1-4`). The reviewed UI code does not show a native Almanac-style game panel integration.

## What makes the UI feel cohesive

### A small, disciplined visual system

- The color roles are consistent: warm ivory/paper backgrounds, dark-brown text, burgundy actions and selected tabs, muted gray inactive tabs, and red section headings/rules (`wr_calc.py:116`, `wr_calc.py:325-370`). The Python-rendered window has a textured body and a red gradient title bar, with a clean fallback when no texture is available (`wr_gui.py:13-22`, `wr_gui.py:71-116`, `wr_gui.py:120-145`, `wr_gui.py:177-196`).
- Controls share the same compact borders, padding, hover treatment, and selected state. Tabs and segmented switches visibly distinguish active from inactive choices (`wr_calc.py:336-350`). This is useful for communicating the current page/filter without adding visual clutter.
- The City Manager preview reinforces the game's paper-and-red aesthetic, while the tool's own headings and accent rules distinguish sections from values. This is a visual observation of the preview, not proof that the texture or artwork can be reused.

### Information is grouped for scanning

- The main shell keeps navigation, the page stack, a notification/toast area, and a footer as separate regions (`wr_calc.py:2578-2589`). The tabs and language choices are in a single consistent header row (`wr_calc.py:2605-2618`).
- City cards and the resource strip summarize data, while the scroll area contains the detailed city entries; totals are kept below a divider (`wr_calc.py:1719-1729`, `wr_calc.py:1737-1757`). This keeps summary information available while the list is longer.
- The building catalog uses a search field and a header outside the scrolling rows, so column labels remain visible (`wr_calc.py:2003-2048`). Sortable headings provide a compact way to inspect a dense list (`wr_calc.py:2030-2035`, `wr_calc.py:2065-2088`).
- Icon-plus-text labels and short explanatory hints make controls recognizable without relying only on color (`wr_calc.py:2023-2028`, `planner/index.html:15-27`).

### The planner is a separate, capable workspace

The planner uses a top action bar, left-side tools/layers, a central SVG canvas, and a right-side block library (`planner/index.html:5-38`). It offers explicit grid/snap/magnet/measurement controls and zoom/undo/rotate actions (`planner/index.html:13-34`). The map supports selection, drag, resize/rotation, keyboard shortcuts, zoom and pan, and has a resize observer to keep its view positioned (`planner/app.js:144-153`, `planner/app.js:428-477`). It also supports JSON import/export and PNG/PDF output (`planner/app.js:340-347`, `planner/app.js:479-492`). Those are strong desktop-planner patterns, but they are not a button or page implementation for the game's sidebar.

## What can inform the Almanac design

Use these as **design principles**, not copied assets or code:

1. Keep the in-game visual language coherent: a clear title strip, warm/light content surface, restrained red section labels, and legible dark text, using the game's existing native styling and approved assets.
2. Separate navigation from the content area and make the active section unmistakable. Use a small set of consistent native buttons rather than introducing a desktop-style tab/window system inside the game.
3. Group financial values into short, scannable sections and align labels, values, units, and period/currency consistently. Put important totals or status where they remain easy to find. Do not use color as the only signal.
4. Keep the first page focused: finance overview and the requested VAT, profit-tax, and property-tax controls belong in the Almanac; the full map-planner tool is outside that scope.
5. Preserve the product requirement for a **same-row, own Almanac button** and native interaction. The attractive look of a standalone app does not solve the native row's placement, artwork, hit-testing, or page lifecycle.

## What does not transfer directly

- The main app is a frameless PySide6 desktop window, with a configurable always-on-top flag and OS window behavior (`wr_calc.py:2488-2514`). It has a resize grip, a persisted window size, and a desktop minimum size (`wr_calc.py:2524-2537`). These mechanics are not an in-game panel or a safe substitute for the game's button/input system.
- The planner is browser-rendered HTML/CSS/SVG. Its page uses fixed 290px and 260px side columns around a flexible canvas (`planner/style.css:1-14`) and browser storage in standalone mode (`planner/app.js:154-162`). Copying this whole layout or embedding a browser view is not a ready or approved Almanac approach.
- The Python “pipette” explicitly captures a screen area and uses OCR plus game-resource-icon templates to estimate production/needs (`pipette.py:1-8`, `pipette.py:44-74`, `wr_calc.py:2829-2863`). That companion workflow does not provide trustworthy, live tax/treasury data to an in-game page.
- Its window texture and many resource/building images come from game material distributed separately in `assets/game_assets.zip`; the README says those materials are not covered by the project license (`README.md:15-18`). Do not extract, copy, or ship those files for Almanac work.
- The companion's code and author-owned artwork are licensed CC BY-NC-ND 4.0, which explicitly forbids distributing modified versions; the game assets in the zip are excluded, and Arimo is separately identified as Apache 2.0 (`LICENSE.txt:4-14`, `LICENSE.txt:26-42`, `README.md:20-24`). Do not copy or adapt its source, artwork, or bundled materials. This review uses its UI only as a reference.

## Recommendation

Borrow the **hierarchy and restraint**, not the app: use native game controls and the game's own panel lifecycle; keep the Almanac title and sections visually clear; align values and labels; and use the familiar red/cream palette only through the game's existing native resources. Any new icon must be original or otherwise cleared for use, and must be drawn through a separately reviewed native UI path rather than reusing the final vanilla tile's artwork.

This source review does not establish the Almanac button's clickability, compatibility, layout, or safety. The exact in-game integration still requires the separate technical investigation and explicit test authorization described in the Almanac plan.
