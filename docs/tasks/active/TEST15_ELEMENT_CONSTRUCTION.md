# Test15 delegated element construction — evidence task completed

**Review status (2026-11-04): COMPLETE for cached count-one pointer transfer.** The packet is synced at `2ffc5fdb04b4cda921e2160aa6953e785515a7e7:TEST15_ELEMENT_CONSTRUCTION_PACKET.md`. Independent decoding and objdump verify the 27-byte helper, closing growth/append calls at 0x1E212/0x1E2DF. No new commit/push prompt, repackaging, R9 research or insertion extraction is needed. See the cumulative report and 1E8C0.md. This is not target-cap approval. The original request below is retained as task history; do not rerun it.

Proposed ready-to-paste task for the existing Windows Kilo parent session. Read AGENTS.md, INSTRUCTIONS.md and KILO_AGENT_WORKFLOW.md; keep native access serialized. No runtime/config/build/deployment, commit or push. Do not repeat packet packaging, 0x1E140 extraction, R9 research, creator/ctor/dispatcher/rail exports or gameplay tests.

The reviewer independently decoded 5bff8da's insertion packet: 5/5 encoded artifacts and 7/7 source slices pass. Recovered 621 contiguous bytes for 0x1E140 (one identical boundary row deduplicated); independent objdump confirms decisive instructions. R9 incoming value is dead; spare-capacity middle insertion directly stores window* at preferred VA 0x14001E380. Those questions are closed for the supplied cached body.

Only this transfer edge remains: growth and append call FUN_14001E8C0 at preferred VAs 0x14001E212 and 0x14001E2DF. Its body was not included. Find existing cached body/index evidence first. If genuinely absent, obtain only read-only decompilation and instruction bytes needed to establish its count-one source load and destination store; follow a delegated store only if required. RVA 0x1E8C0, preferred VA 0x14001E8C0. Do not infer semantics from the helper name or caller signature.

Known calls: growth RCX=newbuffer+insertion_index*8, EDX=1, R8=fifth argument (&window* local); append RCX=oldend, EDX=1 for valid position==oldend, R8=&callee local at stack+0x78 containing window*. Prove what 0x1E8C0 reads/writes and returns for count one. Supply exact addresses/bytes and source/destination linkage. If a necessary cache is missing, identify only that precise next edge.

Corrections to the earlier narrative (use raw instructions, do not re-export):
- 0x14001E36C is indirect memmove, NOT a call to 0x1E8C0.
- 0x14001E39C is MOV RAX,R14. 0x14001E39F writes slot ADDRESS to output pointer, not window* to the table.
- Valid append skips 0x1E323's fill loop (count zero); middle loop at 0x1E380 writes one slot, not remaining table entries.
- The old packet's readable excerpts are reproducible but mislabeled. Its embedded 20/20 output checks old V2, not the new insertion exports. No replacement packet is needed.

Deliver only the new original helper evidence losslessly, with paths/counts/hashes and mechanically selected address-based excerpts; include extraction/verification source and actual results for THESE new artifacts. Label readable views as normalized, with explicit hash domains. Append findings/raw evidence/results to the cumulative report in small checked chunks, and update the function index. Do not merely copy an earlier verification log.

Successful transfer closure is not authorization to raise the cap: total-target UI/lifecycle safety, 100 physical slots, /49 display and batch limits remain distinct. Commodity row+0x28/+0x60 writers stay a separate unresolved task; thresholds, permissions and billing unchanged.
