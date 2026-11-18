# Repository cleanup — 2026-11-18

Branch `arena/5b956cb0-wrsre`, commit `81468b1`, pushed to origin.
**46 files changed, +534 / −145.** No plugin logic changed except one default.

## Verified after the change

| Check | Result |
|---|---|
| Python suites (`find tests -name "test_*.py"`) | **29/29 pass** |
| Almanac host runners (`run_host_tests`, `run_binary_sha256_tests`, `run_tax_policy_tests`) | all pass |
| `tools/peinfo.py selftest`, `tools/audit_economy_csv.py selftest` | PASS |
| Broken relative Markdown links | **0** (was 5) |
| Tracked `.md` files that are valid UTF-8 with no NUL | **all** |
| `.gitignore` / `.gitattributes` effective | verified by simulation |

## What changed, by phase

### 1. Documentation accuracy (highest user impact)

The panel gained Milestones 1–4 and no document followed.

| Was | Now |
|---|---|
| "read-only panel" | Documented the four tabs and the real write surface (`setTaxRate`, `setTaxEnabled`, `setSectorWage`, `setBankRate`, `SettleTaxesNow`) |
| `F8` toggles it (4 docs) | `F9` — matching `toggle_key = 0x78` and `VK_F9`, which the code always used |
| "What it does not do: interactive widgets, tabs that switch, live readout, mouse capture" | Accurate limitation list; the old text described the pre-Milestone prototype |
| `build.bat` "builds tesmioloader.dll by default" | Loader is opt-in (`build.bat loader`) |
| `almanac-engineering-status.md` "no build or test has occurred" | Added a dated supersession banner; historical text left intact per `docs/AGENTS.md` |

### 2. Staging safety

- **`build.bat` shares `build\plugins` between the monolithic and per-plugin
  targets.** It printed "copy `build\plugins\*`", which shipped every plugin
  twice. Now: building `wrsre` deletes stale standalone DLLs (INIs kept,
  because `wrsre.dll` reads them); building per-plugin DLLs deletes a stale
  `wrsre.dll`. Both directions warn.
- **`almanac_button` is now disabled by default** — INI *and* the missing-key
  fallback are `0`. Its fail-stop path calls `TerminateProcess`, so a
  default-on shipped prototype was a real hazard. Left buildable for tests.

### 3. Corrupt and dead files

- Three UTF-16LE `.md` exports → UTF-8 (they were declared `text eol=lf`, so git
  treated them as text; grep/diff could not read them).
- `test_schedule_observer.py` accepted only `utf-16`; now accepts either.
- Repaired NUL-swallowed text: `"RVA \x00x1AAD70"` → `"RVA 0x1AAD70"` (4 places
  across 2 files).
- Re-encoded a cp1252 em dash in `FOLLOW-UP.md`.
- Removed `RAW_EXPORTS_VEWICLE_DETAIL_RENDERER.md` (12-byte typo'd stub) and two
  0-byte `.asm` placeholders that their own reports document as never produced.
- Removed `metadata.json` and the no-op Node `package.json`.

### 4. Re-organisation

- **Restored `tools/ghidra/Xrefs.java` and `ListFuncs.java`** from the verbatim
  source quoted in `RAW_EXPORTS_34D590.md` §10.1/§10.4. They were cited ~150
  times by tracked documents and by `active/GHIDRA_REQUEST.md`, but absent.
- Added the three missing indexes: `export/README.md` (260 evidence files with
  no index), `Mods for analysis/README.md` (83 MB of vendored third-party mods,
  read-only, never stage), `Autosave bin files/README.md`.
- **Tasks lifecycle:** 6 completed records moved `active/` → `archive/`;
  `active/` now holds only 1 blocked and 3 pending records. Both indexes rewritten.
- **`docs/README.md`:** 28 of 70 root docs were unlisted → **0**.
- `.gitignore`: removed the leading `.kilo/` rule that made the four re-include
  rules below it dead code.
- `tools/peinfo.py`: removed duplicate imports.
- `.vscode`: fixed the `Win32`/x64 mislabel and the literal `<version>` compiler
  path; dropped the hardcoded `C:/WRSRE/Mods for analysis/...` CMake path.
- `.gitattributes`: `-text` for `Mods for analysis/**` and `*.asm` so vendored
  and tool-emitted bytes keep their recorded hashes.

### 5. GitHub housekeeping

Deleted **16 stale remote branches**: 15 already merged into `main`, plus
`Gogle` (420 commits behind `main`, 73K lines of deletions, whose only unique
commit added the `package.json` removed in this change). Remote now holds only
`main` and `Almanac`.

## Deliberately not done

**The 14 MB `VEHICLE_PANEL_FUEL_INVESTIGATION.md` was left intact.** It is a
genuine append-only 173K-line status log, not JSON bloat — only 0.2 MB of it is
long lines. Splitting it would break the many `file:line` citations throughout
`docs/`, and the 564 MB `.git` is dominated by 41 historical revisions of that
log, which a working-tree edit cannot reclaim anyway. If you want the history
shrunk, that needs a `git filter-repo` pass coordinated across the repo — a
separate decision with force-push consequences, not a cleanup commit.

Also left alone: the four spaced directory names. Renaming them would break the
external citations listed in `Mods for analysis/README.md` and the ~150 doc
references to `RAW_EXPORTS_*` paths.

## Suggested next steps

1. **Decide `main` vs `Almanac`.** `Almanac` is 21 commits ahead of `main` and
   is where the panel actually lives. Merge it or make it the default branch.
2. **Run one live in-game test of the write controls** (taxes, wages, bank
   rate). That is the only claim still at "host-tested, not runtime-proven".
3. **Consider `git filter-repo`** for the fuel-investigation log if the 564 MB
   clone is a problem.
