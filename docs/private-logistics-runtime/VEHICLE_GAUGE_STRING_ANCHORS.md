# Vehicle Gauge String Anchor Search — Results

**Target:** SOVIET64.exe v1.1.1.9, PE 0x6A3EB6AD, base 0x140000000 (file), ASLR ON
**Generated:** 2026-10-29 via scratch/ghidra_run.bat (Ghidra 12.1.4, JDK 21)

## 1. Search Coverage

Searched initialized executable data sections (`.rdata`, `.data`, `.rsrc`) for:
- ASCII and UTF-16LE string literals containing litre/unit text
- Numeric format strings with unit suffixes
- Vehicle instrument labels (speed, RPM, odometer, distance)
- Cross-references (code xrefs) to each matched string address

**Tools:** `scratch/StrAddrNeedles.java` (custom script, source in section 5), `StrAddr.java`, `DecompAt.java`, `ListFuncs.java`, `Xrefs.java`

**Search needles (ASCII + UTF-16LE):**
litre, liter, Litre, Liter, Fuel, fuel, km/h, mph, RPM, rpm, odometer, distance, ton, tons, cargo, capacity, tank, load, unload, km, mi, %.2f, %.1f, %d l, %.2f l, %.1f l, %.*l, l:, L:

## 2. String Literal Search Results — Fuel/Litre/Unit Text

### 2.1 Negative results (no ASCII or UTF-16 match)

| Needle | Result |
|--------|--------|
| litre | No match in either encoding |
| liter | No match in either encoding |
| Litre | No match in either encoding |
| Liter | No match in either encoding |
| Fuel | No match in either encoding |
| km/h | No match in either encoding |
| mph | No match in either encoding |
| RPM | No match in either encoding |
| rpm | No match in either encoding |
| odometer | No match in either encoding |
| distance | No match in either encoding |
| ton | No match in either encoding |
| tons | No match in either encoding |
| capacity | No match in either encoding |
| unload | No match in either match |
| %d l | No match in either encoding |
| %.2f l | No match in either encoding |
| %.1f l | No match in either encoding |
| %.*l | No match in either encoding |

### 2.2 "fuel" matches — all building/resource type strings, no vehicle gauge

| Address | String | Xrefs | Containing function |
|---------|--------|-------|---------------------|
| 0x14087B06F | 'nuclearfuel' | (no references) | — |
| 0x14087B07C | 'fuel' | (no references) | — |
| 0x14087B09F | 'nuclearfuel' | (no references) | — |
| 0x14087B1B0 | 'fuel' | (no references) | — |
| 0x1408875E4 | 'fuel' | [DATA] from 0x1400E436F | FUN_1400e40f0@0x1400E40F0 |
| 0x14088E67F | 'nuclearfuel' | (no references) | — |
| 0x14088E684 | 'fuel' | [DATA] from 0x140195F5D | FUN_140195040@0x140195040 |
| 0x1408A51B7 | 'nuclearfuel' | (no references) | — |
| 0x1408A5224 | 'fuel' | [READ] from 0x1402A5586 | FUN_1402a1dd0@0x1402A1DD0 |
| 0x1408A634C | 'fuel' | [DATA] from 0x1402A834C | FUN_1402a1dd0@0x1402A1DD0 |
| 0x1408A65D7 | 'nuclearfuel' | (no references) | — |
| 0x1408C5224 | 'fuel' | (no references) | — |

All "fuel" strings are building resource-type identifiers (e.g., nuclearfuel), not vehicle fuel gauge labels. No code references use them as format strings for vehicle instruments.

### 2.3 "cargo" matches — building type names only

| Address | String | Xrefs |
|---------|--------|-------|
| 0x1408770E5 | 'rail_station_cargo' | (no references) |
| 0x1408777D9 | 'heliport_cargo' | (no references) |
| 0x140877B0D | 'rail_station_cargo' | (no references) |
| 0x140877BD9 | 'heliport_cargo' | (no references) |
| 0x1408C3D11 | 'heliport_cargo' | (no references) |
| 0x1408C3F2D | 'rail_station_cargo' | (no references) |
| 0x1408C864E | 'vagon_cargo' | (no references) |
| 0x1408C87F6 | 'ships_cargo' | (no references) |
| 0x1408C881F | 'cabins_cargo' | (no references) |
| 0x1408C896C | 'helicargo' | (no references) |
| 0x1408C898B | 'aircargo' | (no references) |
| 0x1408C8A16 | 'ships_cargo' | (no references) |

### 2.4 "tank" matches — building types only

| Address | String | Xrefs |
|---------|--------|-------|
| 0x1403E7250 | 'oiltank' | (no references) |
| 0x1408C879E | 'cementtank' | (no references) |
| 0x1408C87AB | 'oiltank' | (no references) |
| 0x1408C89D3 | 'oiltank' | (no references) |
| 0x1408C89DE | 'cementtank' | (no references) |

### 2.5 "load" matches — terrain/building only

| Address | String | Xrefs |
|---------|--------|-------|
| 0x1408960D2 | 'terrain_reload' | (no references) |
| 0x14089611A | 'terrain_reload' | (no references) |
| 0x1408B113A | 'terrain_reload' | (no references) |
| 0x1408B131A | 'terrain_reload' | (no references) |
| 0x1408B133F | 'media_soviet/terrain_reload' | (no references) |
| 0x1408CDF07 | 'media_soviet/terrain_reload' | (no references) |
| 0x1408CDF1A | 'terrain_reload' | (no references) |
| 0x1408F2FC1 | 'vehicle_buttoncanload' | (no references) |

## 3. Numeric Format String Search Results

### 3.1 ASCII `%.2f` — referenced from general UI formatter

| Address | String | Xrefs | Containing function |
|---------|--------|-------|---------------------|
| 0x14087F518 | 'en %.2f %.2f' | (no references) | — |
| 0x14087F578 | 'st %.2f %.2f' | (no references) | — |
| 0x140887D20 | 'Christmass light load position error: %.2f %.2f %.2f; %.2f %.2f %.2f' | (no references) | — |
| 0x140887DF1 | 'Christmass light load error duplicate: %.2f %.2f %.2f; %.2f %.2f %.2f' | (no references) | — |
| 0x140887E76 | 'Christmass lights load error can not exist: %.2f %.2f %.2f; %.2f %.2f %.2f' | (no references) | — |
| 0x140887EE0 | 'Christmass light save position error: %.2f %.2f %.2f; %.2f %.2f %.2f' | (no references) | — |
| 0x1408DAC63 | 'failed church %.2f %.2f' | (no references) | — |
| 0x1408E35E8 | '%.2f' | [DATA] from 0x1405A11E9 | FUN_14059f1f0@0x14059F1F0 |
| 0x1408E35F0 | '%.2f' | [DATA] from 0x1405A1232 | FUN_14059f1f0@0x14059F1F0 |
| 0x1408E35F8 | '%.2f' | [DATA] from 0x1405A124F | FUN_14059f1f0@0x14059F1F0 |
| 0x1408E37CC | '%.2f' | [DATA] from 0x1405B117C | FUN_14059f1f0@0x14059F1F0 |
| 0x1408E37D4 | '%.2f' | (no references) | — |

### 3.2 UTF-16 `%.2f` — referenced from large UI render function FUN_14009dc80

| Address | String | Xrefs | Containing function |
|---------|--------|-------|---------------------|
| 0x14087F5D0 | '%.2f' | [DATA] from 0x14009E714 | FUN_14009dc80@0x14009DC80 |
| 0x14087F8F0 | '%.2f' | [DATA] from 0x1400A615F | FUN_14009dc80@0x14009DC80 |
| 0x14087FAB8 | '%.2f' | [DATA] from 0x1400A99CB | FUN_14009dc80@0x14009DC80 |
| 0x14087FE20 | '%.2f' | [DATA] from 0x1400AA1CA | FUN_14009dc80@0x14009DC80 |
| 0x14087FE60 | '%.2f' | [DATA] from 0x1400AA2A1 | FUN_14009dc80@0x14009DC80 |
| 0x140880030 | '%.2f' | [DATA] from 0x1400AD320 | FUN_14009dc80@0x14009DC80 |
| 0x1408800B0 | 's%.2f' | [DATA] from 0x1400AE58A | FUN_14009dc80@0x14009DC80 |
| 0x1408800C0 | '%.2f' | [DATA] from 0x1400AE896 | FUN_14009dc80@0x14009DC80 |
| 0x1408800D0 | '%.2f' | [DATA] from 0x1400ADF57 | FUN_14009dc80@0x14009DC80 |
| 0x1408800E0 | '%.2f' | [DATA] from 0x1400AE256 | FUN_14009dc80@0x14009DC80 |
| 0x140880148 | '%.2f' | [DATA] from 0x1400AEB96 | FUN_14009dc80@0x14009DC80 |
| 0x140880158 | '%.2f' | [DATA] from 0x1400ADC54 | FUN_14009dc80@0x14009DC80 |

### 3.3 ASCII `%.1f`

| Address | String | Xrefs |
|---------|--------|-------|
| 0x140887DA6 | 'Christmass light load error too long: %.2f %.2f %.2f; %.2f %.2f %.2f; %.1f' | (no references) |

### 3.4 UTF-16 `%.1f`

| Address | String | Xrefs |
|---------|--------|-------|
| 0x14087D318 | '%.1f' | [DATA] from 0x14008C7D7, 0x14008C7DE | FUN_14008c3c0@0x14008C3C0 |
| 0x14087D374 | '%s%.1f' | (no references) |
| 0x1408AEFFC | '%.1f %.1f %.1f' | (no references) |
| 0x1408B65D4 | 'Strength: %.1f' | (no references) |
| 0x1408B7E94 | '%.1f, %.1f' | (no references) |
| 0x1408C0F2E | '%.1f / %.1f' | (no references) |
| 0x1408C0F46 | '%.1f / %.1f' | (no references) |
| 0x1408D6D3E | '%s %.1f' | (no references) |
| 0x1408D6D50 | '%s: %.1f' | (no references) |
| 0x1408D6D80 | '%s: %.1f' | (no references) |
| 0x1408D6DB8 | '%s: %.1f' | (no references) |
| 0x1408D75EE | '%s %.1f' | (no references) |

### 3.5 `l:` / `L:` labels — no code references

| Address | Encoding | String | Xrefs |
|---------|----------|--------|-------|
| 0x1404C5BFA | ASCII | 'l:' | (no references) |
| 0x1404C5C83 | ASCII | 'l:' | (no references) |
| 0x1404C60F6 | ASCII | 'l:' | (no references) |
| 0x1404C610E | ASCII | 'l:' | (no references) |
| 0x1405ED514 | ASCII | 'l:' | (no references) |
| 0x1405ED5B8 | ASCII | '5l:' | (no references) |
| 0x1406ADF5B | ASCII | '=]l:' | (no references) |
| 0x14087637A | UTF16 | 'l:' | (no references) |
| 0x14026CDF0 | ASCII | 'L:' | (no references) |

### 3.6 `km` / `mi` distance units

| Address | Encoding | String | Xrefs | Containing function |
|---------|----------|--------|-------|---------------------|
| 0x140233E05 | ASCII | 'km' | (no references) | — |
| 0x1402C0978 | ASCII | 'km' | (no references) | — |
| 0x1402C098D | ASCII | 'km' | (no references) | — |
| 0x1402C09A4 | ASCII | 'km' | (no references) | — |
| 0x1402C09BB | ASCII | 'km' | (no references) | — |
| 0x1402C09D0 | ASCII | 'km' | (no references) | — |
| 0x1402C0A14 | ASCII | 'akm' | (no references) | — |
| 0x1402C0A71 | ASCII | 'km' | (no references) | — |
| 0x140072BAF | ASCII | 'mi' | (no references) | — |
| 0x1402739E4 | ASCII | 'mi' | (no references) | — |
| 0x140273ADA | ASCII | 'mi' | (no references) | — |
| 0x140273C25 | ASCII | 'mi' | (no references) | — |
| 0x14032728D | ASCII | 'mi' | (no references) | — |
| 0x1403272B9 | ASCII | 'mi' | (no references) | — |
| 0x14032731D | ASCII | '@mi' | (no references) | — |
| 0x1408B4874 | UTF16 | 'mi' | [READ] from 0x14032D8BB | FUN_14032d240@0x14032D240 |
| 0x1408B4938 | UTF16 | 'mi' | [READ] from 0x14032E04B | FUN_14032d240@0x14032D240 |
| 0x1408B4AF8 | UTF16 | 'mi' | [READ] from 0x14032E7DB | FUN_14032d240@0x14032D240 |
| 0x1408B4BB8 | UTF16 | 'mi' | [READ] from 0x14032EF6B | FUN_14032d240@0x14032D240 |
| 0x1408B4D28 | UTF16 | 'mi' | [READ] from 0x140330DCB | FUN_140330690@0x140330690 |
| 0x1408B4EE8 | UTF16 | 'mi' | [READ] from 0x14033155B | FUN_140330690@0x140330690 |
| 0x1408B4FA8 | UTF16 | 'mi' | [READ] from 0x140331CEB | FUN_140330690@0x140330690 |
| 0x1408B5168 | UTF16 | 'mi' | [READ] from 0x14033247B | FUN_140330690@0x140330690 |

## 4. Function Analysis — Callers of Format String References

### 4.1 FUN_14009dc80 (0x14009DC80, size 73724 bytes)

**Xrefs:**
- [DATA] from 0x140a5aa8c (NON_FUNCTION)
- [DATA] from 0x140928f40 (NON_FUNCTION)
- [UNCONDITIONAL_CALL] from 0x140106080 in FUN_140105a90@0x140105A90

**Role:** Monolithic UI rendering function (73KB). Called from the main game loop/frame update `FUN_140105a90` at 0x140106080 during the render pipeline (when `DAT_14099528b != 0`). References 12 UTF-16 `%.2f` format strings for general numeric formatting. Not vehicle-instrument specific.

**FUN_140105a90 (0x140105A90)** — main frame update loop:
- Handles Steam API, Alt-Tab, window pause/fullscreen toggles, time scaling
- Calls render pipeline functions including FUN_14009dc80 when `DAT_14099528b != 0`
- Also calls FUN_1403d85a0, FUN_14030c790, FUN_14047d320, FUN_1402f0f10, FUN_14030d1a0, FUN_1406257e0

### 4.2 FUN_14059f1f0 (0x14059F1F0, size 47977 bytes)

**Xrefs:** References ASCII `%.2f` format strings at 0x1408E35E8–0x1408E37D4.
**Role:** Another monolithic UI/formatting function (~48KB). Not vehicle-instrument specific.

### 4.3 FUN_14032d240 (0x14032D240, size 8713 bytes) — NOT vehicle-related

**Role:** Random quote/decree generator. Takes context pointer + char flag. Reads random strings from `C3D_LANGUAGE::GetString` with text IDs 29000–29099, concatenates random digits and UTF-16 string constants from `DAT_1408b4xxx` (including `mi` fragments). The `mi` references are part of random text assembly, not vehicle instrument formatting.

### 4.4 FUN_140330690 (0x140330690, size 8919 bytes) — NOT vehicle-related

**Role:** Random quote/decree generator (same pattern as FUN_14032d240). Takes context pointer + C3DVECTOR3 position + char flag. Finds nearest object from an array, then generates random text/decrees. The `mi` references are part of random text assembly, not vehicle instrument formatting.

## 5. Conclusion

**No direct string/reference anchor for individual road-vehicle fuel display was found.**

### Search summary

| Search category | Needles tested | Matches relevant to vehicle fuel |
|-----------------|---------------|----------------------------------|
| Litre/liter unit labels | litre, liter, Litre, Liter, Fuel, fuel | None — "fuel" matches are building resource types only |
| Speed units | km/h, mph | None |
| Engine RPM | RPM, rpm | None |
| Odometer/distance | odometer, distance, km, mi | "mi" matches are in random text generators, not instrument formatting |
| Mass units | ton, tons | None |
| Volume/mass labels | cargo, capacity, tank, load, unload | None relevant — cargo/tank/load are building/terrain type names |
| Numeric format with unit | %.2f l, %.1f l, %d l, %.*l | None |
| Generic format strings | %.2f, %.1f, l:, L: | Format strings exist but are used in general UI rendering (FUN_14009dc80, FUN_14059f1f0), not vehicle-specific |

### Key finding

The `%.2f` and `%.1f` format strings exist in the executable data but are consumed by monolithic UI rendering functions (FUN_14009dc80 at 73KB, FUN_14059f1f0 at 48KB) called from the main game loop. No string literal combines a numeric format with a litre/unit suffix (e.g., `%.2f l`, `Fuel: %.2f`). No `litre`, `liter`, `km/h`, `mph`, `RPM`, `rpm`, or `odometer` string literal exists in the executable data.

If vehicle fuel display uses localization text IDs (via `C3D_LANGUAGE::GetString`) rather than literal format strings, those text IDs would need to be traced separately through UI caller analysis — this is outside the scope of this string-anchor search.

### Recommendation

No vehicle fuel display string anchor exists in the executable data. Further investigation would require tracing UI callers that render vehicle-specific panels and checking whether they use localized text IDs for fuel/litre labels, rather than searching for string literals.

## 6. Search Script Source

### 6.1 StrAddrNeedles.java

```java
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryAccessException;
import ghidra.program.model.mem.MemoryBlock;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

public class StrAddrNeedles extends GhidraScript {
    @Override
    public void run() throws Exception {
        String outFile = getScriptArgs()[0];
        String[] needles = {
            "%.2f", "%.1f", "%d l", "%.2f l", "%.1f l", "%.*l",
            "l:", "L:", " Fuel", " fuel", "Litre", "litre",
            "Liter", "liter", "km/h", "mph", "RPM", "rpm",
            "odometer", "distance", " ton", "tons", "cargo",
            "capacity", "tank", "load", "unload", "km", "mi"
        };
        Memory mem = currentProgram.getMemory();
        List<MemoryBlock> blocks = new ArrayList<>();
        for (MemoryBlock b : mem.getBlocks()) {
            if (b.isInitialized()) blocks.add(b);
        }
        try (PrintWriter w = new PrintWriter(new FileWriter(outFile))) {
            for (String needle : needles) {
                byte[] asciiN = needle.getBytes("ASCII");
                byte[] utf16N = needle.getBytes("UTF-16LE");
                w.printf("== needle '%s'%n", needle);
                for (int enc = 0; enc < 2; enc++) {
                    byte[] nbytes = (enc == 0) ? asciiN : utf16N;
                    if (enc == 1 && asciiN.length == utf16N.length) continue;
                    int hits = 0;
                    for (MemoryBlock b : blocks) {
                        long base = b.getStart().getOffset();
                        long size = b.getSize();
                        if (size < nbytes.length + 2 || size > 64L * 1024 * 1024) continue;
                        byte[] buf = new byte[(int) size];
                        try {
                            mem.getBytes(b.getStart(), buf);
                        } catch (MemoryAccessException e) {
                            continue;
                        }
                        for (int i = 0; i + nbytes.length < buf.length; i++) {
                            boolean ok = true;
                            for (int j = 0; j < nbytes.length; j++) {
                                if (buf[i + j] != nbytes[j]) { ok = false; break; }
                            }
                            if (!ok) continue;
                            int term = (enc == 0) ? 1 : 2;
                            boolean terminated = true;
                            for (int t = 0; t < term; t++) {
                                if (buf[i + nbytes.length + t] != 0) { terminated = false; break; }
                            }
                            if (!terminated) continue;
                            long addrVal = base + i;
                            int s = i;
                            int step = (enc == 0) ? 1 : 2;
                            while (s - step >= 0 && buf[s - step] >= 0x20 && buf[s - step] < 0x7f
                                   && (step == 1 || buf[s - 1] == 0)) s -= step;
                            int len = i + nbytes.length - s;
                            if (len <= 0 || len > 160) continue;
                            String full = (enc == 0)
                                ? new String(buf, s, len, "ASCII")
                                : new String(buf, s, len, "UTF-16LE");
                            Address addr = currentProgram.getAddressFactory()
                                .getDefaultAddressSpace().getAddress(addrVal);
                            w.printf("  [%s] 0x%X  '%s'%n",
                                (enc == 0 ? "ASCII" : "UTF16"), addrVal, full);
                            ReferenceIterator refs = currentProgram
                                .getReferenceManager().getReferencesTo(addr);
                            int rc = 0;
                            while (refs.hasNext() && rc < 25) {
                                Reference ref = refs.next();
                                Address from = ref.getFromAddress();
                                Function func = currentProgram.getFunctionManager()
                                    .getFunctionContaining(from);
                                String fn = (func != null)
                                    ? String.format("%s@0x%X", func.getName(),
                                        func.getEntryPoint().getOffset())
                                    : "NON_FUNCTION";
                                w.printf("    [%s] from 0x%X in %s%n",
                                    ref.getReferenceType(), from.getOffset(), fn);
                                rc++;
                            }
                            if (rc == 0) w.printf("    (no references found)%n");
                            hits++;
                            if (hits >= 12) break;
                            i += nbytes.length;
                        }
                        if (hits >= 12) break;
                    }
                    if (hits == 0) w.printf("  (no %s match)%n",
                        (enc == 0 ? "ASCII" : "UTF16"));
                }
                w.printf("%n");
            }
        }
        println("StrAddrNeedles: written to " + outFile);
    }
}
```

### 6.2 Raw result files

- `scratch/straddr_batch1.txt` — first batch (litre, liter, Fuel, fuel, km/h, mph, RPM, rpm, odometer, distance)
- `scratch/straddr_batch2.txt` — second batch ( ton, tons, cargo, capacity, tank, load, unload)
- `scratch/straddr_needles.txt` — combined results from StrAddrNeedles.java (all needles)
- `scratch/decomp_32d240.c` — FUN_14032d240 decompilation
- `scratch/decomp_330690.c` — FUN_140330690 decompilation
- `scratch/decomp_105a90.c` — FUN_140105a90 decompilation (caller of FUN_14009dc80)
- `scratch/xrefs_9dc80.txt` — cross-references to FUN_14009dc80

### 6.3 Commands run

```
scratch/ghidra_run.bat StrAddr.java scratch/straddr_batch1.txt litre liter Fuel fuel km/h mph RPM rpm odometer distance
scratch/ghidra_run.bat StrAddr.java scratch/straddr_batch2.txt ' ton' 'tons' 'cargo' 'capacity' 'tank' 'load' 'unload'
scratch/ghidra_run.bat StrAddrNeedles.java scratch/straddr_needles.txt
scratch/ghidra_run.bat ListFuncs.java scratch/funcs_candidates.txt 0x14009D000 0x14009F000
scratch/ghidra_run.bat ListFuncs.java scratch/funcs_candidates2.txt 0x14032D000 0x140331000
scratch/ghidra_run.bat ListFuncs.java scratch/funcs_59f1f0.txt 0x14059F000 0x1405A1000
scratch/ghidra_run.bat DecompAt.java scratch/decomp_32d240.c 0x14032D240
scratch/ghidra_run.bat DecompAt.java scratch/decomp_330690.c 0x140330690
scratch/ghidra_run.bat DecompAt.java scratch/decomp_105a90.c 0x140105A90
scratch/ghidra_run.bat Xrefs.java scratch/xrefs_9dc80.txt 0x14009DC80
```

### 6.4 Limitations

- StrAddrNeedles.java limits to 12 hits per needle per encoding (sufficient for this search as most needles had 0–13 hits)
- String literals constructed at runtime (e.g., via localization text IDs) would not appear as data-section literals
- No runtime code execution, hooks, or game testing was performed
