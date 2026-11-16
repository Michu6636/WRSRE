# Almanac UI direction and follow-up evidence

## User decision

Use native controls for additions within supported native panels. Use in-game overlay panels for Almanac and other macro interfaces. An overlay is drawn within the game window; it is not a separate desktop popup. The native-sidebar launcher remains a separate integration task, with the existing eight buttons preserved. This design decision does not authorize speculative native patching, builds, deployment or gameplay tests.

## Beam report: corrections checked against cached raw assembly

Source: `export/140317db0_full_disasm.asm`. Beam's supplied report is not sufficient to approve the proposed interception at preferred VA 0x1403204EB.

- At 0x1403204E0 the code loads XMM9 from 0x14090AA28, reported as **50.0**, not 68.0. The post-loop live-register claim is wrong on the normal fallthrough path.
- There is a direct empty-vector branch at 0x14031F64E to 0x1403204EB. It bypasses the loop and the fallthrough register resets. Therefore RSI=0, R15D=count and the stated XMM values cannot be assumed collectively from the normal-exit path without analyzing this incoming edge. In particular R15D is initialized to zero before the empty branch; loop-only loads need not have executed.
- The JLE/JZ branches to 0x14032031C are internal branches, not incoming edges to 0x1403204EB or evidence of the proposed patch span's safety.
- The first instruction at 0x1403204EB is RIP-relative. A copied trampoline must relocate its memory reference; our current non-relocating inline-hook helper cannot simply copy it. The report supplies no complete verified stolen span or branch-target audit.
- The following native section immediately consumes XMM1, XMM8 and then XMM13/XMM11, and uses the shared panel object. A helper call/draw may clobber volatile registers, flags and panel/render state. “Required restoration: none” is not justified.
- The decrement at 0x14031F9CD lies within conditional click/selection handling. It is not an unconditional per-iteration loop-counter decrement. Actual iteration increment is R15D at 0x1403204B5.
- A zero literal-address search for writes to 0x1409BE2F0 does not rule out indirect or helper writes. It also does not establish that this store is needed for launcher placement.

Keep the reported geometry/constants as useful leads, but do not hardcode index 8 or approve Y=618*scale until actual entry eligibility/count, origin paths and available space are established. The vector construction/ownership questions remain unanswered by this report.

## Space Race initial source triage

Fetched `Almanac` commit `44d0040b997a083c3d7bea91be99ad711ee3944b`; inspected an ignored archive of `Mods for analysis/wrsr_spacerace-main`. No branch switch, execution, build or deployment.

- Its AGENTS.md records game 1.1.1.9 and Tesmio API 3/4 hosting. A matching version label is a useful lead, not matching-binary proof.
- The visible programme windows in `mod/plugins/spacerace/data/programme/race.tmpl` use scenario VM calls `Scenario_WindowWithImageLeft` and `Scenario_WindowExists` (for example around lines 535–590), not the Earthworks ImGui rendering path.
- `mod/plugins/spacerace/spacerace.cpp` around lines 1336 onward copies programme script/images and generates scenario content. This is useful evidence for native scenario notifications, not proof of arbitrary editable macro-panel controls or native sidebar insertion.
- Initial targeted source searches found no ImGui/Present launcher in the plugin sources. This is preliminary UI triage, not a complete audit of the mod.
- The source includes GPLv3 licensing. Preserve applicable attribution/licensing if reusing it. Its repository-specific branching/deployment instructions do not govern our session or authorize operations here.

## Next bounded work

1. Stop treating Beam's post-loop hook proposal as approved. Request a correction focused on actual incoming paths, live state and a safe launcher boundary—not another summary of constants.
2. Continue Space Race review only where it answers a concrete UI/lifecycle or workflow question; do not divert into unrelated rocket/economy systems.
3. Define a minimal shared overlay/input service for macro panels using Earthworks as a reference, excluding terrain/facility hooks. Preserve existing Finance snapshots/editor and generation-based lifecycle policies. Account for DPI, native/overlay input arbitration, overlay coexistence, failure handling and licensing before implementation.
4. Independent review remains required before high-risk integration. No child-review capability was available for this static pass.
