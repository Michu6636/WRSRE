# Earthworks → Almanac port map

## Scope and source identity

Static source comparison plus isolated styling extraction; no native interception installed or deployable backend built. Source: `Mods for analysis/wrsr-Earthworks-main`, frozen in WRSRE base commit `670eb5315f9d3240bb3467e47af9f1bef8f12a40`. Exact overlay source SHA-256 and licensing are recorded in `prototypes/almanac_overlay/earthworks/NOTICE.md`. This is locally accessible source, not fresh binary/runtime verification. Independent child review is unavailable; this is not integration approval.

Earthworks documents game **1.1.1.13**, Steam build 24922914, TesmioLoader b0.3.7 in `docs/NATIVE_BINDINGS.md`. That is its baseline, not proof of compatibility with our matching executable. Its dependency lock names API 4 and RepublicMP commit `630775724abd5e3346eb59f9c66e2873128d74a4`. The Tesmio API header hashes have now been corrected against pinned API 4 commit `7d6bb8943ca5f6af817d8e7e6cf5be2c7faa818a`; the Windows build remains unverified. GitHub returned 404 for the RepublicMP-pinned checkout in the earlier dependency investigation; no cause is assumed. The current build uses separately pinned upstream Dear ImGui v1.91.9b instead, while retaining RepublicMP source attribution. API version equality alone is insufficient.

## Source-traced route

Paths below are relative to the Earthworks snapshot.

| Stage | Source | Almanac treatment |
|---|---|---|
| Startup | `src/plugin/plugin.cpp:42–73`: Init checks API/host prefix; Start calls InstallRuntime, then StartOverlay | Do NOT copy InstallRuntime: it couples unrelated terrain, world and input patches to graphics. Need matching loader timing evidence. |
| Creation observer | `src/ui/overlay.cpp:2009–2030`: patch engine import `d3d11.dll!D3D11CreateDeviceAndSwapChain`; call original first | Candidate route, not installed. Need matching engine import and original-target/patchIat semantics. No hard-coded RVA here. |
| Present interception | `overlay.cpp:1990–2023`: write COM vtable slot 8 after successful creation | Slot 8 is an interface index, NOT a game-version address. Direct write is outside loader hook tracking; requires ownership/chaining review. |
| Context/resources | `overlay.cpp:1861–1947`: visible current-process HWND, device/context, fonts/style, backends, WndProc, underlay | Reuse D3D11/ImGui architecture, not monolithic initializer. Omit WndProc and native underlay from initial draw-only mode. Add transactional cleanup before native candidate. |
| Drawing | `overlay.cpp:1949–1988`: select context, NewFrame, DrawInterface, Render, backend draw | Substitute synthetic `DrawOverlayReadOnlyPrototype`; no Earthworks snapshot/providers/cursor/terrain preview. Preserve previous ImGui context on every exit. |
| Back buffer | `overlay.cpp:1861–1866, 1960, 1978–1986`: create RTV, save/restore OM target/depth, release RTV each successful frame | Useful no-retained-backbuffer pattern. Must audit all OM slots, backend state, exceptional paths and device loss; not just normal return. |
| Input | `overlay.cpp:290–520`; `src/runtime/runtime.cpp:2042–2068` | Not copied. WndProc is only one route; Earthworks also filters native imports and DirectInput. Initial draw-only context receives no game input. F8 adapter remains separate. |
| Style | `overlay.cpp:1889–1934` | Extracted into `prototypes/almanac_overlay/earthworks/panel_style.cpp`, GPL-3.0-only, with attribution/license. No native dependencies. |
| Shutdown | No matching StopOverlay, backend Shutdown or DestroyContext found in `src` by symbol search | No safe unload contract established. Keep hooks absent until residency/cleanup policy reviewed. |

The architecture therefore is **not primarily an address-porting problem**. The core graphics bootstrap is symbol/interface based. Fixed native input/world/underlay bindings belong to optional functionality we must not import wholesale.

## Changes required before copying the native wrapper

- **Initialization rollback:** Initialize acquires device/context and creates an ImGui context before backend/WndProc success. Failure paths return without corresponding cleanup in this function; Render can try initialization again while gameChain remains null. Port as an explicit transaction, not a verbatim copy.
- **Hook ownership:** PatchSlot stores whatever target it reads and writes a shared vtable slot directly. It has no expected-owner comparison/unpatch record and ignores protection-restoration failure. Loader bookkeeping does not cover that slot. Need a bounded ownership/coexistence policy.
- **Re-entry:** Present's recursive branch uses Present1 and can return S_OK if QueryInterface fails. That is not ordinary forwarding to the saved Present; do not adopt without review and overlay-coexistence proof.
- **Exceptional rendering:** cleanup/context restoration/OM restoration are manual after draw operations. The outer catch resets interaction flags but does not itself perform all render cleanup. C++ catch(...) is not proof of native access-violation recovery.
- **Input and affinity:** recursive mutex serializes Earthworks WndProc/Render access but does not prove device-context affinity or native input ownership. Do not enable its ImGui Win32 message handler for our initial noninteractive mode. Define the F8 observer separately.
- **Late load/device replacement:** creation-only interception does not catch a device created before installation. gameChain selection retains the first initialized chain identity; no full replacement/device-loss path is established by this excerpt. Default unavailable rather than fabricate an alternative bootstrap.

## Next evidence and implementation boundary

Beam S0 should establish current module identities, import/caller/creation outputs, startup ordering and loader patch semantics. Reconcile those against this map. Then scope an explicit-opt-in native candidate outside `plugins/`, with original-call chaining, rollback, state restoration, chain/device lifetime and disabled-by-default failure behavior reviewed before any build/deployment/runtime step. No native addresses have been changed or declared verified here.

For any actual address port, preserve original module/version/RVA, current module/version/RVA and raw matching-function/layout evidence together. Do not fill unknown current addresses from Earthworks' baseline. The Earthworks native underlay, input latches, terrain/facility hooks and gameplay service bindings are excluded from this port.
