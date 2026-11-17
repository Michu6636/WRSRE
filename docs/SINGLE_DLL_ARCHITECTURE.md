# WRSRE Single-DLL Architecture (`wrsre.dll`)

WRSRE can be built and staged as a **single native x64 DLL (`build\plugins\wrsre.dll`)** loaded by `tesmioloader.dll`, while keeping every module's source code isolated in `plugins/<name>/<name>.cpp`.

---

## Why a Single DLL?

Previously, each WRSRE subsystem was compiled into a separate `/MT` DLL (15+ DLLs):
- `tesmioloader.dll` loaded them in filesystem enumeration order (`FindFirstFileA`), making `TsmPluginStart` and `sim_clock` callback registration order depend on NTFS alphabetical order.
- Each `/MT` DLL carried its own static CRT heap, making cross-DLL memory ownership (such as `persistence::readFile()`) fragile.
- `tesmioloader.dll` is an upstream loader binary and should not be modified or rebuilt by WRSRE.

Consolidating WRSRE into **`wrsre.dll`** solves these issues entirely inside the mod:
1. **Explicit, deterministic Init/Start order** independent of filesystem enumeration.
2. **Shared `/MT` CRT heap** across all 15 WRSRE modules.
3. **Zero changes to `tesmioloader.cpp`**, and `tesmioloader.dll` is no longer built by default.
4. **Zero edits to individual plugin `.cpp` files** — standalone per-plugin builds (`build.bat <name>`) still work unchanged.

---

## How It Works

### 1. Zero-Edit Symbol Shim (`src/tesmio_plugin.h`)
When a plugin `.cpp` is compiled with `/DWRSRE_MONOLITHIC_MODULE=<name>`, `src/tesmio_plugin.h` renames its four DLL entry points at compile time:

- `TsmPluginApiVersion` → `WrsreApiVersion_<name>`
- `TsmPluginInit` → `WrsreInit_<name>`
- `TsmPluginStart` → `WrsreStart_<name>`
- `DllMain` → `WrsreDllMain_<name>`

All other symbols inside each plugin `.cpp` are `static`, so all 15 object files link together cleanly without collisions.

### 2. Central Orchestrator (`src/wrsre_entry.cpp`)
`src/wrsre_entry.cpp` (compiled *without* `WRSRE_MONOLITHIC_MODULE`) exports the single `TsmPluginApiVersion`, `TsmPluginInit`, `TsmPluginStart`, and `DllMain` for `wrsre.dll`.

It runs `Init` for all enabled modules, then `Start` for all modules whose `Init` returned `0`, in an explicit dependency order:

1. **Level 0 (Infrastructure):** `sim_clock`, `persistence`, `currency`
2. **Level 1 (Pricing, Firms, Banking, Fiscal, Ledger):** `resource_pricing`, `enterprise_base`, `republic_bank`, `taxes`, `state_ledger`
3. **Level 2 (Labor, Retail, Shops, Logistics, Construction):** `citizen_labor`, `citizen_retail`, `private_shops`, `private_logistics`, `private_construction`
4. **UI:** `ui_panels`, `ui_overlay`

Each module's `Init` and `Start` is wrapped in SEH (`__try / __except`), and individual modules can still be disabled via `[plugins] <name> = 0` in `tesmioloader.ini`.

### 3. Cross-CRT Persistence Contract (`src/persistence_api.h`)
For any external plugin DLL that consumes `persistence` across a DLL boundary, `TsmPersistenceApi` now appends:
- `void (*freeBuffer)(void* buf)` (implemented by `FreeBufferImpl` in `plugins/persistence/persistence.cpp`).
- `TsmPersistenceFree(const TsmPersistenceApi* api, void* buf)` inline helper, which checks `structSize` and calls `api->freeBuffer(buf)` when present (falling back to `free(buf)` only against older providers).

---

## Building & Staging

### Build
- **VS Code:** Press **`Ctrl+Shift+B`** (runs `build.bat wrsre`).
- **PowerShell:** Run `.\build.bat wrsre` (or `.\build.bat clean wrsre`).
- **Separate plugins (optional):** `.\build.bat` builds individual plugin DLLs (without `tesmioloader.dll`). `.\build.bat <name>` builds one plugin.

### Stage in Game
1. Copy `build\plugins\wrsre.dll` and all `build\plugins\*.ini` files into your game's `tesmioloader\plugins\` folder.
2. **Remove** the 15 old individual WRSRE plugin DLLs (`sim_clock.dll`, `persistence.dll`, `currency.dll`, etc.) so they are not loaded twice.
3. Optional standalone plugins (such as `almanac_overlay_dev.dll`) can remain alongside `wrsre.dll`.
4. Launch the game and verify `tesmioloader.log` contains:
   ```text
   wrsre    15/15 modules active
   ```
