# Almanac host contract

## Purpose

This document defines the mod-owned host contract for the future global Almanac
shell. The shell is hosted inside the global Economy & Trade UI. It uses
internal Almanac pages and does not add or mutate entries in the native panel
vector.

This is a host-contract task only. It does not authorize a production hook, native
panel-vector mutation, deployment, or game runtime validation.

## Scope and ownership

Owned files:

- `docs/almanac/01-host-contract.md`
- `plugins/almanac/host_contract.h`
- `plugins/almanac/host_validation.h`
- `plugins/almanac/host_validation.cpp`
- `tests/almanac/host_contract_test.cpp`
- `tests/almanac/test_host_contract.py`

No shared build, loader, address registration, public API header, or sibling
plugin was modified.

## Host interface

The page-facing contract is `almanac::AlmanacHostSurface` in
`plugins/almanac/host_contract.h`. It carries validated host state only:

- `capabilities`: host capability flags.
- `visible`: validated host visibility.
- `selected`: validated host selection for the Almanac surface.
- `geometry`: host-provided DPI and flow-layout origin/extent.
- `clip`: clipping capability and rectangle.
- `scroll`: scrolling capability and state.
- `pointer`: normalized pointer/input event.
- `frame`: frame/generation identity.
- `lifecycle`: lifecycle invalidation state.

Rules:

- Almanac pages receive `AlmanacHostSurface`, not a raw game pointer.
- Native panel indices are never exposed as page identifiers.
- Host selection is reduced to a boolean `selected` plus validated visibility.
- Coordinates are host flow-local origin/extent values, not absolute screen
  coordinates.

Capability flags:

- `AlmanacHostCapability_Visibility`
- `AlmanacHostCapability_SelectionValidation`
- `AlmanacHostCapability_Geometry`
- `AlmanacHostCapability_Dpi`
- `AlmanacHostCapability_Clipping`
- `AlmanacHostCapability_Scrolling`
- `AlmanacHostCapability_PointerInput`
- `AlmanacHostCapability_KeyboardInput`
- `AlmanacHostCapability_FrameIdentity`
- `AlmanacHostCapability_LifecycleInvalidation`
- `AlmanacHostCapability_FlowLayout`

Graceful degradation is mandatory. If a capability is absent, the affected
function is disabled by the corresponding `host_surface_can_*` helper instead of
using guessed offsets or absolute coordinates.

## Validation helpers

`plugins/almanac/host_validation.h` and `.cpp` provide portable synthetic
validation helpers:

- `checked_add_size`, `checked_mul_size`
- `validate_native_panel_vector`, `native_panel_vector_is_valid`
- `validate_native_selection`, `native_selection_is_member`
- `validate_bounded_name`, `name_is_known_host_panel`
- `validate_host_selection`
- `host_geometry_is_valid`, `host_clip_rect_is_valid`,
  `host_scroll_state_is_valid`, `host_pointer_event_is_valid`,
  `host_frame_identity_is_valid`, `host_lifecycle_is_valid`,
  `host_visibility_is_valid`, `host_is_open`
- `frame_identity_equal`, `generation_is_stale`, `lifecycle_invalidated`
- `host_surface_is_valid`, `host_surface_can_render`, `host_surface_can_clip`,
  `host_surface_can_scroll`, `host_surface_can_receive_pointer`

Synthetic native vector model:

- `begin` at `game+0xC608`
- `end` at `game+0xC610`
- stride `0x68`
- `Entry+0x00` active flag
- `Entry+0x01` NUL-terminated ASCII name
- `game+0xC650` selected entry

The helpers validate bounds, stride alignment, entry count, selected membership,
and bounded names without dereferencing synthetic invalid addresses. Tests pass
null, out-of-range, or synthetic descriptors rather than fabricated pointers.

## Claim ledger

### Supported facts

| ID | Claim | Source |
|---|---|---|
| F1 | `FUN_140317DB0`, RVA `0x317DB0`, size 84,735 bytes, receives game in RCX and uses `game+0xADB0` as drawing context. | `docs/UI investigation.md` sections 13.4 and 15 |
| F2 | Documented 18- and 23-byte position-independent prologue candidates end before `_alloca`. | `docs/UI investigation.md` section 13.4 |
| F3 | `game+0xFCE` selects the drawing path; it is not an early-return gate. | `docs/UI investigation.md` section 14.1 |
| F4 | Native panel vector: `game+0xC608` begin, `game+0xC610` end, stride `0x68`, `Entry+0x00` active flag, `Entry+0x01` name. | `docs/UI investigation.md` section 15.1 |
| F5 | `game+0xC650` is the selected entry. | `docs/UI investigation.md` sections 13.2 and 15.1 |
| F6 | Candidate names include `importexport`, `trade`, `loanusd`, and `loanrub`. | `docs/UI investigation.md` section 14.2 |
| F7 | `game+0xC650` was not observed being nulled. | `docs/UI investigation.md` section 14.1 |
| F8 | `FUN_140296110` is a texture loader, not the panel builder. | `docs/UI investigation.md` section 11.12 |
| F9 | `FUN_14072DBF0` is the City Hall type-39 panel, not the global Economy & Trade builder. | `docs/UI investigation.md` section 11.16 |

### Interpretations

| ID | Interpretation | Basis |
|---|---|---|
| I1 | The global Economy & Trade UI can host one mod-owned Almanac surface without adding native panel entries. | F1, F4, and the absence of a need to mutate the vector |
| I2 | Selection must be validated by live vector membership because `game+0xC650` is not observed being nulled. | F5, F7 |
| I3 | `game+0xFCE` is the authoritative host visibility flag for the shell. | F3 |
| I4 | Native names are validation inputs only; they are not page identifiers. | F6 and contract rule |
| I5 | Host flow geometry is supplied by the host rather than guessed from absolute screen coordinates. | Project UI rules and F1 |

### Unresolved runtime questions

| ID | Question |
|---|---|
| U1 | Exact binary identity and build-stamp verification for the target host. |
| U2 | Exact 18/23-byte prologue instruction boundaries and incoming edges for `FUN_140317DB0`. |
| U3 | Which vector entries are populated at runtime and in what order. |
| U4 | Whether `game+0xC650` is written on tab click or only during internal cycling. |
| U5 | Exact flow geometry, clipping, scroll origin, and extent inside the master UI. |
| U6 | DPI source and scaling inside the master UI draw context. |
| U7 | Input routing and hit-testing contract for the master UI. |
| U8 | Lifecycle invalidation triggers and frame/generation ownership. |
| U9 | Fallback behavior when geometry, clipping, input, selection, or lifecycle data is unavailable. |

## Production-readiness checklist

A future production integration must verify all of the following before any hook
is installed:

- Binary identity: target `SOVIET64.exe` v1.1.1.9, build stamp `0x6A3EB6AD`.
- Exact instructions and incoming edges for the proposed stolen span.
- Selection: live vector membership, stride alignment, sane bounds, and name
  validation.
- Geometry: host-provided flow origin and extent.
- DPI: host-provided scalar and row scaling.
- Clipping: capability flag and valid clip rectangle.
- Input: normalized pointer events and bounds.
- Lifecycle: frame/generation identity and invalidation handling.
- Fallback: closed host, missing vector, stale or misaligned selection, unknown
  or unterminated names, and missing capabilities disable the affected
  functionality without guessed offsets or absolute screen coordinates.

## Contract consumed by 05-almanac-ui-shell

The shell consumes `almanac::AlmanacHostSurface` and the validation helpers. It
must:

- Render only when `host_surface_can_render` is true. This requires validated
  visibility, selection, geometry, DPI, flow layout, frame identity, and
  lifecycle capabilities; geometry alone is insufficient.
- Use `geometry` origin/extent and `dpi` for flow layout.
- Apply `clip` only when `host_surface_can_clip` is true. An enabled invalid or
  unsupported clip prevents rendering rather than being ignored.
- Scroll only when `host_surface_can_scroll` is true.
- Consume pointer input only when `host_surface_can_receive_pointer` is true.
  The surface must be renderable and the non-empty event must belong to the
  current frame; hidden surfaces and stale-frame events cannot commit input.
- Invalidate cached page state when `lifecycle_invalidated` is true.
- Never receive or dereference the raw game pointer.
- Never use native panel indices as page identifiers.
- Never mutate the native panel vector.

## Remaining production blockers

- No production hook is implemented or authorized.
- No native runtime validation has occurred.
- Master UI geometry, DPI, clipping, scrolling, and input remain unresolved.
- Runtime vector population and selection write behavior remain unresolved.
- Lifecycle invalidation triggers remain unresolved.
