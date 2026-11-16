# Arena check of Beam S0 (4589d3a)

Fetched `origin/Almanac` and read the published report, manifest and all three raw exports using `git show 4589d3a:...`. No matching binaries are available locally; executable/engine hashes remain operator-reported, not freshly recomputed here. Independent review is unavailable.

## Accepted evidence

`c3d_import_d3d11.txt` identifies slot VA `0x18016B750` (RVA `0x16B750`) as `D3D11CreateDeviceAndSwapChain`; the assembly shows the indirect call at `0x1800148BC`. This supports Earthworks' symbol-based engine import route. Raw xrefs show data references, not a proven startup call chain. Init/Start-versus-device-creation ordering remains unknown.

## Critical correction: output argument identities

Beam's prose swaps several Win64 stack argument meanings. Reading its raw assembly against the D3D11 signature gives these **caller-RSP** offsets at the call:

| Offset | D3D11 parameter | Value established by assembly |
|---|---|---|
| +0x20 | pFeatureLevels | address of stack array `[RBP+0x190]` |
| +0x28 | FeatureLevels count | 3 |
| +0x30 | SDKVersion | 7 |
| +0x38 | pSwapChainDesc | address `[RBP-0x50]` |
| +0x40 | ppSwapChain | address `0x1801D7730` |
| +0x48 | ppDevice | address `0x1801D76C0` |
| +0x50 | pFeatureLevel | address `0x1801CDBE4` |
| +0x58 | ppImmediateContext | address `0x1801D76C8` |

The post-success virtual call at offset `0x128` is through the **device**, consistent with ID3D11Device::GetFeatureLevel; it does NOT prove a swap-chain method. Do not dereference `0x1801D76C0` as a swap chain. The new drawing candidate takes a supplied IDXGISwapChain and uses none of these globals.

## Other limits/corrections

- The exports do not include raw PE import descriptor/name/hint dumps for all reported imports, nor captured hash-command output. The selected slot symbol supports the creation route, not every table claim independently.
- Failure retries are supported by the excerpt; it ends at `0x18001491C`, not the whole Initialize function. No complete device lifecycle is proven.
- Local `src/tesmioloader.cpp:194` confirms PatchIat stores the previous slot and ignores protection-restoration failure. That failure still returns success. It is not a safely rolled-back transaction.
- The statement that patchIat has 'no effect' on DLL residency is unsafe: the patched slot points into the detour's DLL, which must remain resident while reachable. Partial installation cannot justify unloading.
- Implicit saving of the previous pointer does not itself prove race-free ownership, ABI-correct forwarding or runtime overlay coexistence.

## Implementation disposition

Proceed only with isolated drawing-component source preparation. Native observer/interception installation still needs a bounded, reviewed ownership/startup mechanism. No permission for a game run, build or deployment is inferred from Beam's S0 completion. The user has confirmed the target is an Earthworks-style ImGui Almanac, with F8 development activation, not native-text HUD replacement.
