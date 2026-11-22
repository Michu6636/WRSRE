# Testing the Almanac panel

Short version:

```cmd
build.bat
```

That one command builds the loader and every plugin into `build\plugins`, so
there is a single folder to copy. Copy the **contents** of `build\plugins` into
the `plugins` folder of a disposable game install — the folder next to
`tesmioloader.dll`, which is what the loader scans:

```
build\plugins\tesmioloader.dll
build\plugins\almanac_overlay_dev.dll
build\plugins\almanac_overlay_dev.ini
...every other plugin DLL and INI
```

Launch the game through Tesmio. The panel starts hidden, so press `F9` once to
show it; `F9` again hides it.

## Expected loader log, in order

```
plugin   almanac_overlay_dev <version>  from almanac_overlay_dev.dll
almanac overlay  init: key 0x77, show_on_start 0, font 100%, host API 4
almanac overlay  startup handed to the installer thread; ...
almanac overlay  creation import hooked; waiting for the engine's swap chain
almanac overlay  Present hooked on the game's swap chain (hwnd ...)
almanac overlay  panel initialized; waiting for the first visible frame
almanac overlay  panel visible; key 0x77 hides and reopens it
```

That is a working session. Read it top to bottom; the first missing line names
the problem, and each case below is distinguishable.

## If it does not appear

| First line missing or different | Meaning | What to do |
|---|---|---|
| No `plugin almanac_overlay_dev` line at all | The loader did not see the DLL | It is not in `<loader dir>\plugins`; the DLL and `.ini` were copied somewhere else, or `almanac_overlay_dev = 0` in `tesmioloader.ini [plugins]` |
| `declined to install` | `enabled = 0` in the INI, or the host lacks the required API | Check `almanac_overlay_dev.ini`; this plugin has no hash/build gate to fail |
| `C3DDLL64 does not import D3D11CreateDeviceAndSwapChain` | This engine build gets its device another way | Not fatal on its own — the next line shows whether the fallback worked |
| `engine creation import already replaced by another mod` | Another overlay hooked it first | Expected; the shared-slot fallback takes over |
| `Present hooked through the shared DXGI vtable` | The fallback is in use | The panel can still appear; if no chain is adopted either, that is not the cause |
| `no Present hook could be installed` | Both paths failed | Nothing in this plugin can be fixed from here; report the log |
| `no swap chain seen in 20s` / no `Present hooked` after 20s | The engine created its device **before** the plugin was injected | Nothing inside the process can recover this; the loader is injecting too late. Report it — do not try to work around it |
| `panel initialization failed` | The chain is not D3D11, is not this process's window, or the font atlas failed | Report the log line; it distinguishes which |
| `frame failed …` | The device was removed or the back buffer was unavailable | Read the game's own graphics state; the plugin stops drawing rather than guessing |

`Present hooked` + `panel initialized` without `panel visible` means the panel is
hidden, which is the normal start with `show_on_start = 0`: press `F9` once,
making sure the game window has focus.

## Visual check on a disposable install

1. The panel is a paper-and-ink window anchored near the top-left, titled
   `Almanac`, over the game view. Not a separate desktop window.
2. **Defect to remove (owner rule D8, 2026-11-22):** the current build can still fall back to `SYNTHETIC`/placeholder rows when live data is missing. That fallback is forbidden. The target behaviour is a visible `NO DATA: <service> unavailable` / `ERROR <reason>` in warning colour, never sample numbers. Treat any sample value on screen as a failure.
3. The first `F9` shows it; the next `F9` hides it. Holding `F9` must not flicker:
   it takes exactly one press per change.
4. Alt-tab away and back: the panel hides while unfocused and needs a fresh `F9`
   press afterwards. Holding the key through the alt-tab must not reopen it.
5. Minimise, restore and resize the game window: the panel re-anchors and stays
   readable; nothing crashes.
6. Click around the panel: **the game** must receive every click. The panel is
   `NoInputs`, so it must never swallow a click, a key or the cursor.
7. The footer shows a rising `frame` counter. A frozen number means the game is
   not presenting, not that the panel is stale.

## What a pass does not prove

- Not GPU-driver or HDR/scaling behaviour on your monitor.
- Not coexistence with another ImGui overlay beyond "nothing crashed".
- Not a product UI: no interaction, no live Finance data, no native sidebar entry.

Do not test on a live install. The plugin reads one key and writes nothing to
game state, but it is still a development hook in the render path.
