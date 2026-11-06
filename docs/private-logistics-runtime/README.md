# Private logistics — runtime observation and implementation

## Current source — 0.18.18-census-guard (2026-11-02)

The authorized safety follow-up guards the older daily census's exhausted slot before per-vehicle diagnostics and summarizes omitted visits. It keeps the 64-slot cap, existing identities, dispatch, newer observer and ledger/billing policy unchanged. **144 host tests pass**, including optimized/sanitized execution of the extracted census; the original census fails the same host sanitizer fixture at index -1. **Test15 (2026-11-03) now logs this version running in the user's Windows game**, following the reported successful build. Thirty daily summaries show ordinary census execution, not >64-slot exhaustion. The run exposes 411 omissions caused by the existing five-total-target automatic-connection cap, plus secondary-goods dispatch symptoms whose threshold/resource-selection cause is not yet established; late meat transport is logged after manual changes. Existing timestamps give coarse 20–59ms day-end work brackets, not a frame/phase benchmark. A four-record load with one interrupted/unpriced record is now observed, but exact restored amounts remain unchecked. Full log, screenshots and bounded findings are in the cumulative report. Earlier evidence below remains historical; no runtime fix, instrumentation or deployment is added by this investigation.

Start with [How distribution offices work in WRSRE](DO_OFFICES.md) and [the main design document, section 12](../WRSRE%20DESIGN%20DOC.md). PR #42 merged persistent dry assignment costs: closure-frozen cached quotes, original-target cancellation/replacement allocation, pointer-free snapshots, protected save/load and interrupted/unpriced active saves. Payments remain OFF; snapshots do not verify economic payers, and no actual debt/settlement is created. [Permissions and working-dispatch history](AUTONOMOUS_DO_PERMISSIONS_PLAN.md) covers the unchanged daily 10/30 connection policy.

Vehicle observation runs at most approximately once per real-world second through tick callbacks, not a full scan every tick or a replacement dispatch engine. Prices are cached for approximately 30 real-world seconds; weekly game-clock pricing/cache invalidation is a future idea only. Large-save performance is unmeasured. The guide separates these clocks, limits and remaining work.

The implementation has archived host-test support (141 Python tests; 29 fleet + 12 optimized/sanitized dry-ledger scenarios). Earlier live tests support dispatch, the accepted initial approximation/refill-like rises and pause continuity; Test12 named saving worked. **Test13 (2026-11-02) now provides live 0.18.17 dry-ledger evidence:** reported factory demolition closed once, retaining the original factory and a 1.8025194 RUB estimate, without adding later return fuel. Two earlier autosaves logged successful 3-record/616-byte writes, but neither includes this later cancellation. Cancelled-record persistence, exact restored row values, compiler/source-binary parity, payer linkage and large-save timing remain unverified. **Test14 now shows nonempty native restoration:** `autosave5` loads 3 records without missing/blocked/interrupted flags; two autosaves retain 3 rows / 616 bytes, and the named `26741 - My republic` save writes 4 rows / 812 bytes. Only a new assignment opening is logged, no closure: the fourth saved row is consistent with the active interrupted/unpriced-copy policy, not a new completed bill. Reloading that fourth row and comparing frozen amounts remain later checks; further loading can wait. No repeat simple-save/demolition/baseline/pause research is required. Complete original log/screenshots, helper source/results and handover remain in [the single cumulative report](VEHICLE_PANEL_FUEL_INVESTIGATION.md).

## Historical observation work package and reviews

**Everything below is historical context, not the current task list or branch status.** Earlier “next” prompts, exact-burn research gates, success-only billing proposals, agent/model/worktree assignments and session-retirement instructions are not active directions. Use current user authorization and repository workflow; the old disconnected journey core is not the durable fleet-cost book. Preserve these records without reactivating old agents, legacy billing or repeated evidence requests.

> **ACTUAL RAW CLOSURE RECEIVED AND HASH-MATCHED (2026-11-01).** Attachment
> 65056bytes/SHA188d6595…fcacf206;801 supplied instructions independently
> decoded. Two local source-debit/5F0-credit pairs are present. Supplied constants
> decode4.0,+0.5,-0.5: the negative floor retires the earlier speculative large
> undershoot/restoration concern. No more delivery-only or repeated raw exports.
> Remaining: observed-producer applicability and named-resource/price-unit bridge.
> Full correction review, complete lossless attachment, audit source/results and
> finite next task are cumulative. Display samples remain not an exact burn meter.
> Payments OFF; no new native query, production change, hook, build or game run.

> **EVIDENCE-GAPS PACKET REVIEWED (2026-11-01).** Exact call-site bytes and
> callee copies resolve the ABI: RCX/XMM1/R8B is ordinary Windows x64 for the
> reported signature; no hidden parameter is needed. New41-prefix input resolves
> the MOVAPS transcription. Root/member selection is supported. Earlier SIL/R14
> reachability, store-length and factor-name claims remain wrong or unproved.
> Early refill credit bytes/resource selection are still absent; one bounded
> continuation is the next native gap. Full review/input/audit/output/reference
> and raw-extraction task are cumulative. Payments OFF; no hook/build/game run.

> **TANK CONTRACT REVIEWED, NOT ACCEPTED (2026-11-01).** Actual report text is
> received; no resend needed. Its typed caller expressions imply +4F0/+514,
> not +13C/+145. Its loop includes the root despite claiming never-written.
> Independent decoding of29 supplied rows confirms8/9-byte stores and exposes
> a MOVAPS byte/mnemonic mismatch. Argument defaults are unproved; floor
> restoration is not automatically a refill. Full correction review, selected
> evidence, complete audit source/output and bounded next task are cumulative.
> Producer applicability/ABI/transaction boundary/refill resource/units remain
> blocked. No hook, build, game replay, code change or payment is authorized.

> **FUEL-COST INTEGRATION: RAW RECOVERY REVIEWED, CONTRACT BLOCKED (2026-11-01).**
> The recovery's actual raw exports and six index entries were received in chat;
> no repeated delivery-only request or unchanged game test is needed.
> They contradict key claims: effective table stride16, nested storage receiver
> rather than a proved truck tank, reused formula registers, and mistaken5F0
> writer/object classifications.6B3B50 really deducts5F0 in one branch; its alleged
> top-up is a local calculation.142430 has a real credit AND later cargo work.
> Next: targeted caller/receiver/floor/refill/unit evidence for those paths using
> the authorized local extractor and caches first. No unverified hook is approved.
> Complete parent review, selected raw excerpts, audit source/actual output and
> next prompt are in the cumulative handover. No production/version/build/run or
> payment changes; outside-assignment burn remains separate operating overhead.

> **TEST8 PASSES: actual v0.18.11 ledger paths (2026-10-31).**
> User confirms the first single-row phase is vehicle refuelling; no delivery
> ledger record opens for it. ID2opens53, is replaced by ID3at101; scalar edit121
> keeps ID3; removal152closes it once. Actual quiet last-sample fuel endpoints
> 100/151and boundary101/152match accepted records.16classifications/12ledger
> events checked;193fuel samples,5gaps/29discarded,4unchanged comparisons.
> Full final parser/results/evidence/lossless log in cumulative. No runtime fix
> or unchanged-probe retest requested. Billing OFF; costing remains unconnected.
> Earlier0.18.11Windows smoke-check-pending notices are historical for these paths.

> **v0.18.11 dry-run assignment ledger implemented (2026-10-31).**
> Selected-panel candidates open on observed appearance, close/reopen on target
> replacement, and close on removal without requiring empty cargo. Scalar edits
> and stop advances keep the same ID. Existing jobs are adopted with unknown
> start; invalid reads and identity/load changes never pretend to be completion.
> No costing/payment/tracker activation or new native read/default changes.
> 129tests PASS; ASan+UBSan28reader/23ledger/16runtime cases PASS. Test7replay
> opens57, replaces79, keeps91, closes111. Full source/results/guide in cumulative.
> New Windows0.18.11DLL build/smoke check pending; billing remains OFF.

> **TEST7 PASSES: v0.18.10 actual classification path (2026-10-31).**
> Deployed version/gates confirmed;15classification lines independently checked.
> Target replacement79 is structural; scalar edit91 coincides with index0->1
> but is NOT structural. Three index-only advances remain separate. Final
> clearing111 is correctly reported across a pause, while screenshot30 still
> shows0.30t electronics: empty schedule does not mean empty truck/success.
> Complete review/parser/actual output/evidence/lossless log in cumulative.
> No runtime changes, accounting activation or unchanged-probe rerun requested;
> next stage is bounded dry-run assignment accounting. Payments remain OFF.
> Earlier pending Windows smoke-check notices are historical for this path.

> **v0.18.10 captured-value classification implemented (2026-10-31).**
> Separate count/targets, availability, flags, exact scalars and active-index-only
> observations; office-only/load-unload labels are patterns, not native jobs.
> Test6 replay distinguishes replacement79 from scalar-only91; original gap
> safeguards retained.33schedule/104suite tests PASS; sanitized28reader+13runtime
> cases PASS. No native read/default/tracker/payment changes. Full final source,
> actual output and Windows guide in cumulative. No repeated full refueling test
> needed; new DLL build/smoke check pending. Earlier notices are historical.

> **TEST6 PASSES: v0.18.9 sampled-list/pause reporting (2026-10-31).**
> Actual game log confirms version/gates and correct gap clearing with prior
> rows, unchanged pause comparisons, same-count replacement and final clearing.
> Scalar-only adjustment at sample91 also raises the raw content flag: do not
> treat every field change as a new trip. Full review/script/results/log archive
> in cumulative. No unchanged-probe retest or native research needed this round;
> next is change classification before accounting. No runtime edits; paymentsOFF.

> **v0.18.9 changed diagnostic implemented (2026-10-31).** Last-valid
> endpoint comparison now reports content differences across pause/drop gaps
> with continuity_valid=0, preserving existing baseline safeguards. No native
> read/hook or billing changes.27schedule/98suite testsPASS; sanitized host
> checks and test5 endpoint replayPASS. Full final source/results and Windows
> build/test instructions in cumulative. Windows DLL build and in-game test
> are still pending; verify0.18.9-schedule-gap-endpoints before testing.

> **LIVE TEST5: existing probe reads the screenshot task lists (2026-10-31).**
> Empty->three rows->changed three rows->empty captured; target/setting changes
> occur with unchanged storage. Index-only changes remain separate. Paused
> coalescing rebaselines suppress replacement/clearing delta flags, despite
> raw rows capturing them. Full checkpoint analysis/source/results/input archive
> in cumulative. Next: descriptive endpoint comparison across gaps, not new
> native reads or a jobID investigation. No production changes; paymentsOFF.

> **AWAITING APPROVAL: 911b5f1 publication confirmed (2026-10-31).**
> Corrected report and cached helper are visible; no more research/publication
> round needed. Illustrative vector reader still uses the wrong base and loses
> per-row coverage. Coordinator corrected contract is in the cumulative; do not
> implement the draft verbatim. No new runtime reads or production changes
> authorized. Stop agent work pending explicit user implementation decision.

> **ACTIVE: aadf4b9 delivery confirmed (2026-10-31).** All four report
> deliverables are published; do not repeat that mapping. Native installer
> bytes checked and corrected contract supplied directly in cumulative. Draft
> still omits vectors and has fallback/relocation/ABA/bounds/gap errors. Next
> is an explicit implementation decision, not another research round. No
> runtime/code/config/billing/build/game changes or approval inferred.

> **ACTIVE: df23d12 per-row report independently reviewed (2026-10-31).**
> Published receipt confirmed, but negative C8/04/CC and vector display-read
> claims rejected against the same cached renderer. Coordinator supplied actual
> reads; omitted inputs are config+08/+D0 vectors and conditional+190, not a new
> config-entry identity. Full source/actual audit output/bytes/corrected map and
> SAME-parent continuation in cumulative. Existing partial observer stays usable.
> No new runtime reads/hooks/config/billing/build/game changes. Newest notice
> supersedes older historical ACTIVE notices below.

> **ACTIVE: published23225b2task-list report reviewed (2026-10-31).** Saved
> receipt/report confirmed. Useful existing observer mapping, but no new detector
> implementation or fleet-wide coverage proof. Kind7road chain, C80job-close labels
> and storage=>wholeplan classifier rejected; neutral rules supplied directly.
> Next new evidence is per-truck-row resource/filter renderer mapping, not office
> task storage or anotherD39loop. Full receipt/bytes/audit/source/results/prompt
> consolidated in cumulative. PaymentsOFF; no native/runtime changes.

> **ACTIVE: task-list change investigation resumed (2026-10-31).** User wants
> changes to the vehicle panel’s task/connections list. Earlier closure covers only
> the failedC80/D39line. Reuse existing0.18.8schedule observer; missing visible-field
> coverage and a connected simulation plan writer are the next targets. New host
>3->1->3/same-count replacement regression;19testsPASS, not native/game validation.
> Full final source/results and SAME-parent continuation in cumulative. No new
> runtime hooks/config/billing/native build/game changes; paymentsOFF.

> **Final closeout confirmed (2026-10-31):** both final canonical corrections
> received and accepted. Static round CLOSED with the three lifecycle questions
> UNTRACED; no further work requested and no billing boundary proved. Full record
> remains in the cumulative. PaymentsOFF; no runtime changes.

> **Final call-table receipt accepted; static round CLOSED (2026-10-31):**
> Latest chat table matches canonical31sites. QualifiedUNTRACEDresult accepted,
> not a proved lifecycle/billing boundary. Minor summary fixes:252650has6sites;
> unresolved PREstore effects are after the inner guard(e.g.6635E0at6B8FFD).
> Canonical source/results and final closure prompt preserved in cumulative.
> No further native query/trace requested; paymentsOFF, runtime unchanged.

> **Bounded call-table receipt (2026-10-31):** qualified UNTRACED result accepted;
> the31-row transcription still had9bad rows. Canonical table mechanically generated
> from pinned complete bytes; input/source/actual results and bounded correction/
> qualified closure are consolidated in the handover. No further speculative trace
> or native query requested; matrix formatting remains CLOSED. No runtime changes.

> **Further trace rejected against complete cached bytes (2026-10-31):**
> The alleged call-free post-store interval contains31CALLsites.6BAC66is an entry
> bailout, not post-store tail; updater6480/6486instructions are mistranscribed.
> D39memory preservation remains unproved, not disproved merely by calls. Full
> reproducible source/actual stdout and bounded correction in the cumulative.
> Matrix7x8format stays accepted; no new native queries/runtime changes.

> **Published10719a1 matrix received; same-truck trace reviewed (2026-10-31):**
> Actual7single-line rows x8cells: formatting/publication issue CLOSED, superseding
> older notices below. Existing complete6B8EB0bytes exclude the proposed inner-
> D39nonzero path from reaching the association stores. Actual-store normal return
> goes throughMOVAL,1, so caller520=0; laterD39effects/lifecycle still unresolved.
> Full scoped review, reproducible audit source/actual stdout, matrix/index receipts
> and SAME-parent continuation are in the cumulative. No native/runtime changes.

> **Published6f7b04b corrections checked (2026-10-31):** gate-skip and cleanup/
> timing wording improved; index count27+README confirmed. Published matrix still
> has wrong physical cell counts and contradictions around HIGH-byte confidence,
> unproved528writer and internal continuity. A newer local version may differ.
> Exact receipt/diff/audit/source/results and SAME-parent continuation are in the
> cumulative handover. No native execution or runtime changes.

> **Published c7ced75 matrix/gate claims reviewed (2026-10-31):** all seven
> case labels received, but Markdown cells/continuations do not form a valid
> eight-column table. Cached bytes disprove gate-false enqueue:1C68EB jumps69B8
> past insertion.3FBDC0 call68BD is before gate/consumer; earlierC80guard is not
> a post-write invariant. Complete743-instruction cached body re-decode passes;
> native lifecycle and proposed logging remain unproved. Full review/evidence/
> audit/source/results and SAME-parent correction prompt in cumulative handover.

> **Published e7f92ef recovery checked (2026-10-31):** final matrix heading
> and valid eight-column header/separator received; zero case rows. Exact440-byte
> append preserves the prior handover. Newly published C report/index also reviewed:
> receiver join untraced, stale cleanup/verified-index claims and verifier failures
> require reconciliation. Full receipt/audit/source/results and one SAME-parent
> continuation prompt in the cumulative handover. Continue verified single-row
> writes automatically; this is not a completed native lifecycle contract.

> **Reusable team workflow guide added (2026-10-31):**
> [KILO_AGENT_WORKFLOW.md](../../KILO_AGENT_WORKFLOW.md) is the maintained guide
> for future Kilo task prompts, resource ownership and automated coordination.
> Root AGENTS.md and INSTRUCTIONS.md direct future AI sessions to it. The guide
> includes a reusable parent-prompt skeleton without current investigation offsets.
> No Kilo settings/permissions or runtime behavior changed.

> **Kilo workflow revised (2026-10-31):** official docs confirm normal Code
> supports task subagents; dedicated Orchestrator is deprecated. Use ONE Code
> parent: sole Ghidra operator/shared writer, with read-only task children for
> independent causal-end and receiver-identity analysis. Parent collects results,
> queries missing edges serially and delegates follow-ups; no manual user relay.
> Swarm is optional communication, not a scheduler. This supersedes separately
> managed A/B/C prompts. Full research/source URLs and unified parent prompt are
> at the end of VEHICLE_PANEL_FUEL_INVESTIGATION.md. No Kilo configuration,
> sessions, native evidence or runtime behavior changed by this workflow review.

> **Published1252253 reviewed (2026-10-31):** A/B reports, five B disassembly
> exports and updated INSTRUCTIONS are received. The prior publication gap was
> expected under no-commit instructions, not an agent failure. Export audit:
> 5635unique instructions/26956bytes; zero decode-size/target/conflict/overlap
> errors;299shared prior-report instructions match. False road cleanup chain
> retracted; complete contract and replacement zero-target close rule unproved.
> Missing A exports/verifier source, causal removal and receiver join remain open.
> Full review/artifacts/audit source+actual output/instructions and updated A/B/C
> prompts are appended to VEHICLE_PANEL_FUEL_INVESTIGATION.md (single handover).
> Historical separate-agent plan superseded by the single-parent Kilo workflow
> above. No sessions started/stopped or runtime/hooks/billing/build/game changes.

> **Fresh-team prompts prepared (2026-10-31):** three non-overlapping roles:
> A acceptance/work scope + integration; B causal termination/cancel/replacement;
> C fleet-to-executor identity. Full self-contained prompts in the cumulative
> handover end. Replaces prior two-role continuation plan; no agents launched.
> Historical C→A→B query plan superseded: one designated Ghidra orchestrator only.
> No runtime/hooks/billing/game changes authorized.

> **Replacement return8b7d5eb reviewed (2026-10-31):** committed decompilation
> supports both698resetsto0; initial acceptance versus refresh still unproved.
> No classified trip close yet. Correct post-call observation/correlation before
> proposing probes;429A80/42A0D0 tail callees are unclassified next-ordering leads.
> Full review/audit/source/results/report/decomp/xrefs at the single handover end.
> No hooks, runtime, build, save or game changes.

> **Replacement Agent1 handoff prepared (2026-10-31):** latest actual reports
> are1ae50c3/ee2faf5. Complete self-contained takeover prompt and unchanged reports
> at the cumulative handover end. Start/work scope remains open; termination-clear
> semantics remain proposed. Raw start-workscope scratch exports are not tracked.
> No runtime changes, hooks, billing, builds or game replay authorized.

> **Latest e9c5d8e/37bcb65 reviewed (2026-10-31):** source/byte-check repairs
> partly accepted; no assignment-to-removal interval proved. Agent1's collection
> base is P=[RBX+398], not RBX; P is also a direct simulation receiver. Independent
> gate listing still has three byte/address conflicts and wrong branch polarity;
> universal before-loading acceptance is unsupported. Complete review, seven-case
> matrix, final audit/source/stdout and both unchanged reports at the handover end.
> No runtime changes, hooks, billing, build or game replay. Earlier notices historical.

> **Both faadf28 returns reviewed (2026-10-31):** agent1 adds a conditional
> stored698 increment, not a700 increment. No assignment/removal rule yet.
> Fresh independent report's commitment claim is not accepted; missing source,
> inconsistent PASS output, wrong jump address and wrong logging-site receiver
> must be corrected. Full comparison/seven-case matrix/audit/source/stdout and
> both unchanged reports are embedded at the single handover end. No hooks,
> billing, runtime changes or game replay. Older sole-agent1 notices are historical.

> **Two-investigator continuation requested (2026-10-31):** the handover ends
> with full prompts for agent1 (execution/removal) and a fresh independent agent
> (commitment), using the same acceptance matrix. This supersedes sole-agent1
> guidance; older agents2/3 remain retired. Separate output files, explicit
> serial Ghidra handoff; no runtime changes or game replay authorized.

> **Agent1 4170a55 reviewed (2026-10-31):** conditional association/home reset
> evidenced, no assignment/removal rule. The700 block increments a register
> copy, not the stored field; D39 proof corrected. Existing test4 raw4a0=0
> snapshots need not be replayed; fleet→simulation identity is still missing.
> Full review/audit/results/unchanged report are at the single handover end.

> **Next sole-agent1 prompt prepared:** the final handover section contains the
> complete execution/removal-focused continuation after c9bb065. Reuse known
> route builders; trace internal stops versus actual removal/replacement and
> resolve post-callD39 effects. No runtime changes or game test requested.

> **Agent1 c9bb065 reviewed:** concrete candidate-route/config rebuild path now
> identified; assignment/removal boundaries still unproved. New complete-body
> byte checks and Markdown fence tracking pass. The call-exclusivity claim still
> needs post-callD39 proof and680A20 raw blocks. Review/audit/results/unchanged
> report are appended to the single handover. No hooks, billing or game replay.

> **Agent1 follow-up31e6e41 reviewed:** useful negative evidence, lifecycle still
> incomplete. The D39-exclusive-path claim skips callee effects; existing bytes
> name680A20 and candidate-buffer→1E5350 as bounded next edges. Full review,
> independent audit/results and unchanged report are at the handover end.
> Continue only agent1; no hooks, billing or unchanged game replay.

> **Sole-agent1 follow-up prepared:** the final section of the single handover
> contains the complete next prompt. Continue only agent1, using its already
> delivered complete bodies to resolve actual lifecycle boundaries; do not
> repeat the same exports or continue agents2/3. External sessions must be
> stopped/left idle by their operator; none was stopped from this checkout.

> **Agent1 updated return b730880 reviewed (2026-10-30):** complete updater/helper
> bodies are now delivered and pass independent byte-structure checks. This closes
> the missing-body gap; assignment/removal semantics remain unresolved. Full
> review, audit source/stdout and unchanged report are appended to the single
> handover. Do not request these same bodies again or enable billing/hooks.
> Earlier notices saying agent1 is still pending describe the prior state.

> **Agent3 summary review (2026-10-30):** the proposed one-frame interval is
> planning cadence, not proved assignment lifetime. Transfer-function and receiver
> labels need correction; timer units/constants are not independently verified.
> Full review, correction prompt and supplied summary are appended to the same
> handover. Agent3 native exports were not delivered in the fetched branch.

> **Agent2 comparison (2026-10-30):** its added function names are investigation
> leads, not new task-boundary proof. Its delivered raw instruction evidence is
> entirely shared with the corrected agent1 report, and its field table contains
> route/fleet and home/target mislabels. Full comparison/audit and the unchanged
> agent2 report are appended to the single handover below. Agent1's ongoing
> latest-prompt work remains pending. No runtime or test change is requested.

> **Next-agent prompt prepared:** the end of the single handover Markdown now
> contains an instructions-first, wrapper-aware continuation prompt. It requires
> complete byte-checked type43 caller/helper evidence before lifecycle claims;
> no new runtime code or unchanged-build test is requested.

> **Corrected report reviewed (2026-10-30), actual remote commita068d19.** The
> byte/address correction passes independent checks and the opaque BB0 value is
> no longer mistaken for a truck pointer. This is association evidence, not a
> proved task interval. Full new helper/caller CFG evidence is not embedded in
> the supplied report. The second review, complete audit/results and preserved
> updated report are appended to the single handover Markdown below. No runtime
> changes, billing or unchanged-build game test are requested. The preceding
> report's review below is historical.

> **Returned native report independently reviewed (2026-10-30).** Fetched the
> actual remote commit7154fda. Complete helper listings are useful, but the
> proposed association install has unresolved type43 road-DO provenance and an
> inconsistent addressed instruction excerpt. No new hooks or billing are
> enabled. The full review, original report, audit output and targeted correction
> prompt are appended to the single handover Markdown below.

> **Latest supplied game test: v0.18.8 (2026-10-30).** The new schedule observer
> ran in the user's Windows game: 197 paired fuel/object records, 21 valid schedule
> snapshots and all six screenshot values correlate. The run exposed a three-row
> destination/config change without an observed empty intermediate table, negative
> raw values at zero display, and refill-like upward steps. These are not yet
> native assignment or exact-burn/settlement proof. Billing stayed OFF. No further
> unchanged-build or empty-tank test is needed while awaiting the investigation.
> Full review, audit source, actual results and future test guidance are appended
> to the single [`VEHICLE_PANEL_FUEL_INVESTIGATION.md`](VEHICLE_PANEL_FUEL_INVESTIGATION.md).
> The following implementation checkpoint predates this supplied game run.

> **Current source: 0.18.8-schedule-observations (2026-10-30).**
> Added fingerprint-gated read-only observations of the selected road-DO truck’s
> raw schedule rows/config/index, using the existing panel hook and queue.
> This is NOT whole-job logging: job identity, buyer and completion reason remain
> unverified. Billing/permissions/dispatch and the loaded-only core are unchanged.
> Source/INI default `schedule_observer=1`; billing remains `0`.
> 238 Python regressions and six optimized + ASan/UBSan host suites PASS.
> No Windows DLL build or real-game run of 0.18.8 is claimed. Full findings,
> complete final source and actual output are appended to the single
> [`VEHICLE_PANEL_FUEL_INVESTIGATION.md`](VEHICLE_PANEL_FUEL_INVESTIGATION.md).

> **Latest supplied game run, 2026-10-30: v0.18.7 diagnostics validated.**
> All six new screenshot fuel values match the same DO-fleet truck. Zero
> queue overflow was reported; 253 paused snapshots were explicitly coalesced.
> Model/fallback labels are correct; billing stayed OFF and both deliveries ran.
> Keep the working DO simple and unchanged. Future assigned-job costing is now
> the user's preferred design direction, not a newly implemented native adapter.
> Full review, policy distinctions, save/exit clarification, analysis source and
> actual results are appended to the single
> [`VEHICLE_PANEL_FUEL_INVESTIGATION.md`](VEHICLE_PANEL_FUEL_INVESTIGATION.md).
> The following pre-test checkpoint is historical; its diagnostics were exercised
> by this new user-supplied run. Exact native fuel/job accounting is still pending.
>
> **2026-10-30 implementation checkpoint:** working source is
> **0.18.7-diagnostic-provenance**. Tariff/reader provenance and paused-queue
> gap/coalescing fixes are implemented; 220 host regressions pass. No MSVC build
> or game run of this revision is claimed. Simulation deduction/refill/clamp
> tracing is blocked on the missing raw writer evidence or matching executable;
> the native journey adapter remains unwired and billing OFF. **Not ready for
> the requested end-to-end adapter game test.** Exact unblock request, complete
> final changed source and actual results are appended to the single
> [`VEHICLE_PANEL_FUEL_INVESTIGATION.md`](VEHICLE_PANEL_FUEL_INVESTIGATION.md).
>
> **2026-10-30 game-test result:** the panel observer ran in Windows and
> all five screenshot fuel values matched the same actual DO-fleet truck.
> This passes display-source/identity correlation, NOT exact fuel burn or
> journey settlement. The latest full review and next work are at the end of
> [`VEHICLE_PANEL_FUEL_INVESTIGATION.md`](VEHICLE_PANEL_FUEL_INVESTIGATION.md).
> Billing remains OFF; the journey core remains disconnected. The following
> implementation-stage notice is historical; its build/game-test gate has now
> been exercised for this observer path by the user's supplied run.
>
> **2026-10-29:** The actual `%.2fl` display chain and its **1000.0f**
> multiplier are traced. A bounded read-only observer is implemented in the
> existing private_logistics plugin (**0.18.6-panel-observer**), awaiting a
> Windows DLL build and real-game validation. It compares actual panel-object
> pointers with candidate-home road-DO fleet entries; it is NOT an exact burn
> meter. See [`VEHICLE_PANEL_FUEL_INVESTIGATION.md`](VEHICLE_PANEL_FUEL_INVESTIGATION.md)
> for the latest review, complete observer/plugin source, actual test output,
> limitations and the proposed next game test. No observer build has been deployed.
>
> The tested C++ journey core remains **disconnected from runtime events**;
> its source/results are in [`JOURNEY_TRACKER_CORE.md`](JOURNEY_TRACKER_CORE.md).
> Billing remains OFF, source10/destination30 dispatch is preserved.
> `RUNTIME_PROTOTYPE.md`, older agent assignments and proposal sections below
> describe historical work, not the new observer or a validated fuel tracker.

## Current goal

**Latest user policy amendment (2026-10-30):** future accounting covers actual
DO-task assignment → removal, **regardless of success/cancellation**. Internal
stop changes stay within the interval; travel after task removal is excluded.
Earlier success-only requirements and blanket return-travel proposals below
are superseded by this rule. Native presence/removal/replacement and physical
fuel accounting still require verification; current runtime billing stays OFF.
The exact targeted Ghidra-agent prompt and changed-build diagnostic test guidance
are appended to the single handover Markdown linked above.

Preserve the working simple DO and billing OFF. The latest supplied v0.18.7 run
supports two deliveries and validates diagnostic cleanup, not native accounting.

**Future preferred envelope (user proposal, 2026-10-30): actual per-vehicle
assignment start → verified terminal outcome**, preserving the same job
across its loading/unloading stop transitions. A stop advance or list clear is NOT
a verified terminal outcome; success must be distinguished from cancel/replacement.
Empty outbound travel can belong
to this envelope; whether return to DO belongs must be explicit. This differs
from the older loaded-only policy in the still-disconnected tracker core.

Later fuel-only invoice = verified consumed tonnes × settlement-time in-game
price per tonne, paid by the correctly identified buyer to the carrier once.
Task identity/lifetime, cancellation, refills, physical unit conversion, coverage
and buyer settlement remain unverified/unimplemented. Endpoint gauge changes
can only be estimates under verified no-refuel/no-other-change assumptions.
See the latest single-file review for the complete future contract. No payment
or runtime policy was changed by this review.

## Status and provenance

- Current Arena branch: `arena/99cc5008-wrsre`; source is merged v0.18.8 (read-only schedule observations); latest supplied real-game observer test is v0.18.8. See the latest single-file review. Older agent/build reports below are historical.
- Old Agent 1 report fetched at `80a4404d8d3fa5161bcaaf8de8cfd0a7fc7a72b7`, branch `reverse-engineering-and-verified-transport-associa`. Clean of conflict markers; not accepted as a verified road load/burn/unload implementation. Still contains task-stride/prototype inconsistencies.
- Old Agent 2 model fetched at `94d304dfbd182aef6464ff35315de8d16ae1a694`, branch `executable-accounting-model-and-deterministic-test`. Independently ran 27 tests, all pass. Additional probes: CLOSED prevents a legitimate second trip; UNSUPPORTED can be reactivated by a repeated load. Not production-ready.
- Review exports exist only in ignored `.cache/agent-review/` in this checkout. Remote reports/model have NOT been merged here. New agents must obtain the exact artifacts without blindly merging old branches.
- Old fuel-branch commit `75c1126` contains a conflict-corrupted report: do not use it as canonical.
- Prior delivery-to-delivery architecture is superseded. Preserve historical files; do not delete/rewrite all old docs or expand the Python model again.

## Retire old agents safely

Tell both old agents: stop new work; return current branch, full HEAD, `git status --short`, last test commands/results and any uncommitted artifact locations. Keep commits/worktrees until replacement agents verify access. Do not delete/reset worktrees or apply all their changes to a shared branch. Do not merge the prior conflicting report. Existing pushed artifacts are references, not accepted integration.

## Scheduling / ownership

Suggested allocation (not a claim of benchmarked model capability):

| Agent | Suggested model | Start | Exclusive writes |
|---|---|---|---|
| A: road-path evidence | Nemotron 3 ultra | Now | `docs/private-logistics-runtime/evidence/` |
| B: diagnostic scaffold | Nemotron 3.5 super vl | Now, no hooks | `plugins/private_logistics_probe/`, `docs/private-logistics-runtime/probe/` |
| C: manual test and log checker | Poolside Laguna S 2.1, bounded context | Now | `tests/private_logistics_probe/`, `docs/private-logistics-runtime/validation/` |
| D: gated integration/review | Nemotron 3 ultra | After A/B/C | B's probe paths only after B stops; review docs under `docs/private-logistics-runtime/integration/` |

Give every agent its own Kilo worktree and the corresponding prompt. In Arena, never switch/create a branch: use the fixed session branch. In Kilo, remain on the assigned existing worktree branch. No two agents edit the same files concurrently. B hands ownership to D; C owns the final checker/schema changes. Agents do not merge/close PRs or apply other worktrees themselves.

## Gates

1. **Static evidence:** A supplies exact-byte verification, vehicle provenance and a safe proposed observation mechanism. Unsupported paths stay unsupported.
2. **Offline readiness:** B builds a disabled-by-default diagnostic scaffold; C tests the log checker with explicitly synthetic fixtures. Host tests are not game evidence.
3. **Integration:** D reviews A's evidence and adds only supported observation paths. Hook code changes require ABI/register/stack/return correctness and exact-byte guards. Do not invent a loader hook API.
4. **Deployment approval:** user approves the build, diagnostic configuration, coexistence/exclusion with existing private_logistics, and rollback steps BEFORE DLL copying or game execution. This package authorizes investigation and an opt-in test build, not automatic installation.
5. **Runtime:** actual controlled game run, raw logs, independent cargo/tank comparisons and clear pass/fail/inconclusive results. No observation that changes the game's transfer/fuel behaviour. No new reimbursements; disable existing payment paths for the controlled run with documented config verification.
6. **Later:** only after observation is verified should C++ accounting/settlement implementation be proposed.

## Minimal diagnostic contract (v0, proposal)

B proposes a versioned structured log schema; C implements against that same frozen schema. Record raw and interpreted events separately. Minimum: schema version, run/session generation, process-wide event/call sequence, thread/depth where relevant, simulation tick if verified (else null plus monotonic wall timestamp), static site RVA, raw args/return only where supported, vehicle/DO/resource identity with confidence, before/after cargo or fuel values, actual deducted tonnes, sequence gaps/drops/errors, boundary reason, settlement-time price/currency and trip summary. Raw addresses are session-scoped identities, not persistent IDs. Full buffers/gaps invalidate affected trip results; never block the game thread on disk IO.

No guessed typed dereferences, no universal mode/sign classifier, no silent fallback when evidence fails. Duplicate observations and genuine extra loads must not be conflated solely by equal trip/carrier/resource. Prefer explicit raw observation identity and a verified complete-load boundary. Partial cargo transfer calls are not automatically complete pickup/delivery.

## First in-game scenario

Controlled copy of a save, one road DO, one fuel-consuming truck, one resource, one source, one destination, no extra pickup/destination. Two consecutive full loaded journeys. First choose no refuel during a loaded leg: compare summed deductions against authoritative tank decrease, allowing logged numeric tolerance and tick-order effects. Use actual cargo state changes as an independent boundary check. Then separately test refuel, partial unload, session reset and invalid evidence. Do not manipulate inventory merely to make expected results pass.

## Artifact hygiene and final reports

No executable, DLL, Ghidra project DB, save, caches or huge logs in Git. Small scripts/excerpts and synthetic fixtures only. Keep large runtime artifacts outside Git with manifest/hash and location. No credential capture in logs. Each agent returns one consolidated Markdown with complete relevant evidence/source references, exact commands, actual output, limitations, branch/full commit SHA, and successful push confirmation. Run tests/build if available, `git diff --check`, scan conflict markers and inspect staged files. Do not claim a build/runtime result that was not executed.
