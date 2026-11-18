# Test15 packet repair — existing artifacts only

Use the same Windows Kilo session that created TEST15_NATIVE_CLOSURE_PACKET.md. Read root AGENTS.md, INSTRUCTIONS.md and KILO_AGENT_WORKFLOW.md. One parent owns this task. No runtime/config/build/deployment changes, Git commit/push, EXE upload or Ghidra rerun is requested. Keep scratch ignored.

The reviewer found the synced packet in DO-fuel-cost-calculatotr commit f1c9d2b7e9fa508d3dd799bd953079e0620556e5. Its Git blob is 215658 bytes, SHA256 f83153c40a3672bf2de6c37056d3716981e80a3e6939350c414e3b8fb6d9970e. No attachment problem remains. The packet itself fails the evidence-completeness check:

- The raw rail section has no `0x310 + ...` consumer and no `/49` literal, despite its ledger citing both. It jumps from a partially rendered row to the `/20` else branch. Provide the complete existing rail file, not reconstructed missing lines.
- Creator row VA 0x1406EE83F supplies eight bytes `48 C7 43 18 00 00 00 00`, ending at 0x1406EE847, but the next row starts at 0x1406EE843. These overlap by four bytes.
- Ctor rows around 0x14025C7E8 / 0x14025C7E5 run backward and repeat an address.
- At ctor VA 0x14025C58B, bytes `48 89 67 08` decode to MOV [RDI+8],RSP, but the supplied mnemonic says RSI. Do not silently change either side to match an expected zeroing narrative.
- `[RSP+0x20]` immediately before CALL is argument five, not R9's home slot. The value stored there is an address of a stack local holding window*, not window* directly. Preserve the R9 limitation unless reaching definitions in existing raw output resolve it. Window field +0x34 values 8/9 are not by themselves road/rail building types 43/52.

## Deliver one mechanically packaged file

Preserve the current packet. Create TEST15_NATIVE_CLOSURE_PACKET_V2.md at the repository root. Locate each original artifact; filenames may differ. Include the full ledger/index and the creator, ctor, dispatcher and rail raw files, plus the existing insertion corroboration file. Mark genuinely missing originals MISSING; do not recreate them from the old packet, chat or narrative.

Use a small local packaging script that reads original bytes. For every source include its actual path, exact byte count/SHA256 and a lossless Base64 representation (optionally gzip first). Split long writes into small coherent chunks and verify each. Add readable deciding excerpts separately, explicitly labelled partial; never manually retype full exports. Embed the packager/verification script source and actual output in the Markdown.

Verify by decoding every packaged artifact and comparing it byte-for-byte to its original. Distinguish a full-file hash from an excerpt hash. Report the assembly inconsistencies above even if they already exist in the original cached files; do not claim packaging repairs native evidence. If R9 or an export remains unresolved, revise dependent READY statements rather than asserting closure.

No new native query is authorized by this packaging task. If an original cache is itself inconsistent or missing, identify the smallest exact missing artifact/range and stop. Open the final packet in VS Code and give its absolute path.
