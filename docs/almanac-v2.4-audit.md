# Almanac v2.4 Code Audit

Audit of the v2.4 almanac_button plugin against available native evidence.
Branch: Almanac at f05f2ce. Source: plugins/almanac_button/almanac_button.cpp.
No source edits were made in this audit phase.

## 1. Mouse ABI and debounce (v2.1, v2.2)

### 1.1 GetMouseSolid ABI

- **Claim (C6):** `void (*)(void* input, C3DVec3* out)` — this in RCX, hidden result pointer in RDX.
- **Source (v2.4):** `typedef void (*t_GetMouseSolid)(void* input, C3DVec3* out);`
  Called as `g_getMouseSolid(input, &mouse)` where `mouse` is `C3DVec3 { float x, y, z; }`.
- **Verdict:** Matches the corrected ABI. The earlier `float* (*)(void*)` form (v2.0) was fixed in v2.1.
  The mangled name `?GetMouseSolid@C3D_INPUT@@QEAA?AVC3DVECTOR3@@XZ` confirms
  `?AVC3DVECTOR3@@` = return by value (hidden sret pointer in RDX per MSVC x64).
  **Status: consistent with evidence.**

### 1.2 Debounce reset (v2.2)

- **Source (v2.4):** In `UpdateAlmanacState`, the debounce reset is gated on
  `!ReadClickLatch()` (click latch clear). When the latch is clear, debounce is
  reset to 0, allowing the next click to toggle.
- **Prior bug (v2.1 and earlier):** The reset condition was
  `InterlockedCompareExchange(&g_almanacDebounce, 0, 0) == 0`, which only reset
  when debounce was already 0 — i.e., never reset from 1. This was fixed in v2.2.
- **Verdict:** The current code correctly resets debounce only when the click
  latch is clear, preventing re-trigger while the mouse is held. The fix is
  consistent with the latch semantics (latch set while held, clear on release).
  **Status: consistent with evidence.**

## 2. Custom texture (v2.3)

### 2.1 Middlepoint

- **Claim (C4):** Middlepoint must be `Base() + 0x9EACD0` (compile-time constant).
- **Source (v2.4):** `#define ALM_RVA_MIDDLEPOINT 0x9EACD0u` and
  `void* middlepoint = (void*)(Base() + ALM_RVA_MIDDLEPOINT);`
- **Verdict:** Correct. The earlier wrong computation (`exeBase + 0x9D4F10 + 0xB048`)
  was removed. The middlepoint is now the compile-time constant 0x9EACD0.
  **Status: consistent with evidence.**

### 2.2 TexBind vtable offset

- **Claim (C7):** vtable+0x70 = TexBind. Status: **contested** (inferred, not
  verified against actual C3DAPI_TEXTURE vtable).
- **Source (v2.4):** `#define ALM_TEX_BIND_OFFSET 0x70u` with comment
  "vtable[0x70] = TexBind (confirmed by native code at FUN_140358A20:141)".
- **Evidence:** Native code at FUN_140358A20 line 141 calls
  `(**(code **)(*plVar5 + 0x70))(plVar5, 0, DAT_1409ead08)` where plVar5 is
  a C3DAPI_TEXTURE* from CreateManagedTexture. This confirms offset 0x70 is used
  by native code for the same object type.
- **Verdict:** The offset 0x70 is supported by native code usage, but the method
  name "TexBind" and its exact semantics are inferred. The claim status should
  be upgraded from "contested" to "supported (offset)" / "inferred (semantics)".
  **Status: offset supported by native usage; semantics inferred.**

### 2.3 Texture loading and binding

- **Source (v2.4):** Lazy-loads texture via CreateManagedTexture on first draw.
  Binds pre-Draw via vtable[0x70] with panel_state = `Base() + TSM_RVA_DEFAULT_PANEL_STATE`
  (0x9EAD08). SEH-wrapped; failure sets g_textureLoaded=0 (falls back to native artwork).
- **Issues identified:**
  1. **Load (vtable+0x10) is NOT called.** The native sequence is
     Create -> Load(vtable+0x10) -> TexBind(vtable+0x70). The plugin only calls
     TexBind, skipping Load. The texture may not have image data loaded.
     This is a significant gap — the texture object exists but may be unpopulated.
  2. **Texture lifetime:** The texture is loaded once and cached in g_almanacTexture.
     It is never freed. No cleanup path exists.
  3. **Panel-state argument:** Uses `Base() + 0x9EAD08` (correct per native code).
     The earlier wrong value (`game+0x9D4F10+0xEAD08`) was fixed.
  4. **SEH wrapping:** The binding is SEH-wrapped, non-fatal on failure. Good.
  5. **No restore of prior binding:** After the ninth Draw, the bound texture
     remains as the plugin's texture. The next frame's eighth-tile draw re-binds
     its own texture, so this is likely safe. But within the same frame, any
     subsequent draw using the bound texture would use the plugin's texture.
- **Verdict:** The binding approach is structurally correct (pre-Draw, correct
  panel_state), but the missing Load call (vtable+0x10) means the texture may
  not display correctly. This needs runtime verification.
  **Status: partially correct; Load call missing.**

### 2.4 Texture asset

- **INI:** `texture_asset = icon_inspectnumbers.png` (default).
- **Verdict:** Asset availability unverified. If the file doesn't exist in the
  game's media directory, CreateManagedTexture returns NULL and the plugin
  falls back to native artwork. No crash risk.
  **Status: unverified at runtime.**

## 3. Hit-testing

### 3.1 Ninth-tile click detection

- **Source (v2.4):** `CheckNinthClick` tests mouse position against the ninth-tile
  rectangle (x, y, w, h) and reads the click latch (read-only, not consumed).
- **Verdict:** The rectangle test + latch read is the current approach. The
  proposed Collision-based enhancement (using C3D_PANEL2D::Collision) remains
  an open investigation item. The current approach is read-only and doesn't
  consume the latch, which is correct per the read-only policy.
  **Status: consistent with evidence; Collision enhancement unverified.**

### 3.2 Debounce

- See section 1.2 above. **Status: consistent.**

## 4. State restoration

### 4.1 0x5E6 write-on-open / write-zero-on-close

- **Source (v2.4):** `UpdateAlmanacState` writes 1 to game+0x5E6 when Almanac
  is open, 0 when closed. Gated by INI `content_suppression` (default 1).
- **Finding (C43, C44):** game+0x5E6 is NOT visual content suppression.
  It provides a partial, mouse-dependent input effect (disabling click handling
  + tint change). Native content still draws. Writing 0 on close does not
  necessarily restore the previous native state (native code toggles this flag
  on clicks).
- **Verdict:** The 0x5E6 write is safe (SEH-protected, single byte, INI-gated)
  but its effectiveness is unverified and its semantics are now understood to be
  limited. The INI default of 1 may give a false impression of content suppression.
  **Recommendation:** Either (a) gate behind a clearly-named INI key
  (`input_suppression` or `native_click_disable`) instead of `content_suppression`,
  or (b) remove the 0x5E6 writes entirely until a real suppression mechanism
  is established.
  **Status: safe but misleading; semantics corrected in docs.**

### 4.2 Almanac open state (g_almanacOpen)

- **Source (v2.4):** `g_almanacOpen` is a file-scope static volatile LONG.
  Not exposed to other plugins.
- **Verdict:** The open state is internal to the plugin. A future Almanac content
  renderer (in another plugin or the same plugin) would need to read this state.
  No service or exported accessor exists yet.
  **Status: needs exposure mechanism for future content rendering.**

### 4.3 lastSelection tracking

- **Source (v2.4):** `g_lastSelection` tracks the last native selection index.
  Updated every frame in `UpdateAlmanacState`. Initialized to 0.
- **Potential issue:** On first frame, if the native selection is 0, the
  close-on-native-click logic won't trigger (0 != 0 is false). If the user
  opens Almanac while selection 0 is active, then clicks selection 0 again,
  the close won't trigger (already selected). This is the "already-selected
  button" case documented in the technical findings.
- **Verdict:** Known limitation, documented. The click latch is consumed by
  native code even when selection doesn't change, but the plugin's comparison
  doesn't detect this.
  **Status: known limitation.**

## 5. Assets

- **Source (v2.4):** `texture_asset = icon_inspectnumbers.png` (default).
  No font asset (empty by default). No City Manager assets used.
- **Verdict:** The texture asset is a game-bundled PNG. No external or
  third-party assets are referenced. The asset availability is unverified.
  **Status: consistent with policy (original/cleared art only).**

## 6. Summary

| Item | Status | Action needed |
|------|--------|---------------|
| GetMouseSolid ABI | Consistent | None |
| Debounce reset | Consistent | None |
| Middlepoint computation | Consistent | None |
| TexBind vtable offset 0x70 | Offset supported, semantics inferred | Verify semantics at runtime |
| Texture Load (vtable+0x10) | MISSING | Add Load call before TexBind |
| Texture lifetime/cleanup | No cleanup path | Add cleanup on plugin unload |
| 0x5E6 writes | Safe but misleading | Rename INI key or remove |
| g_almanacOpen exposure | Not exposed | Add service or accessor |
| lastSelection tracking | Known limitation | Document; consider latch-based detection |
| Content suppression (visual) | NOT achieved | Needs different mechanism |
| Hook point 0x1403745B6 | Unsafe | No hook approved |

## 7. Recommended next steps (docs/design phase)

1. **Design decision document:** Choose the visibility/suppression mechanism.
   Options: (a) opaque overlay (needs F1-F8 gates), (b) render-path hook
   (needs safety proof), (c) accept partial suppression + document limitation.
2. **Add Load call:** Before runtime testing, add `vtable+0x10 Load` call to
   populate texture data. This is a small, safe change.
3. **Rename INI key:** Change `content_suppression` to `native_input_suppression`
   to accurately reflect what it does.
4. **Expose open state:** Add a minimal service or global accessor for
   `g_almanacOpen` so future content renderers can gate on it.
5. **Resolve texture lifetime:** Add cleanup path for `g_almanacTexture`.
