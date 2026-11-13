# Independent review: sidebar write trace

The Windows agent's [report and six excerpts](../export/almanac-evidence/sidebar-write-trace-20261109/) from `c8a0471` are preserved byte-for-byte. This review compares those excerpts with existing host and row disassemblies; [selected host control-flow context](../export/almanac-evidence/sidebar-write-independent/host-context.asm) is retained separately. The source report does not establish runtime execution or an exhaustive write search. Its `.asm` excerpts are UTF-16, not corrupted.

## What the excerpts support

- The excerpt shows `0x1400843f3` writing `RDI` to `[R14+0xc650]`. The host calls `0x14007c400` with `RCX=R14` at `0x14031bb22`; the agent asserts the callee preserves that argument in `R14`, though the supplied excerpt does not include the prologue. This is a candidate write, **not** proven to be the write behind the observed sidebar change. The preceding name tests and branch are conditional, and the excerpt alone does not establish that the write always runs or that this function handles every switch.
- The original row disassembly and new excerpt agree that `0x140374356` writes `EBX` into `[RSI+0xc65c]` on a guarded row-interaction path. At `0x14037434d`, the equality branch jumps **to the write**, not past it: only the flag clear is skipped for an unchanged value; the store and following `0xffffffff` counter assignment still occur. This corrects section 4 of the report.
- The host's named `trade` dispatch uses the nonzero/zero selector gate to choose `0x140373ea0` or `0x140361bb0`, but a selector snapshot does not prove execution. `0x140358a20` at `0x14031ee76` is a separate comparison branch, not the `importexport` fallback. The report's table itself lists `0x1403472a0` and `0x14034b470` under a later `importexport` comparison; it also says the comparison strings were not resolved. Do not assert more than the evidence proves.

## Missing host writes invalidate the two-site conclusion

The existing `FUN_140317DB0` disassembly itself contains direct writes to **both** candidate fields. Examples:

- `0x14031f9a4` stores the selected record pointer to `[R14+0xc650]`. Earlier in the same displayed path, collision and additional state checks guard execution (`0x14031f8a3` onward). `0x14031acd9` and `0x1403282f9` are other direct pointer stores in the host.
- The code at `0x14031fb77–7b` sets `EDI` to `-1` and writes it to `[R14+0xc65c]`. Later, after a six-byte name comparison with data at `0x1408b21e4`, `0x14031fc76` writes `R9D`, previously cleared at `0x14031fb6d`, into the same field. Other direct item-index stores appear at `0x14031fde9`, `0x14031fe38`, and `0x14031ff62`.

These instructions falsify the report's claim that parent switching only writes the pointer and item handling only writes the index, or that there are only two write sites. They offer candidate explanations for the snapshot's `-1` and `0` values, but the matching section strings, full conditions, and actual executed runtime path must be verified before associating those stores with that observation.

## Default and other unresolved issues

`XOR EDI,EDI` at `0x1400842fc`, followed by a store to `[R14+0xc8e8]`, does not itself choose the first row item or prove a return default. It is not sufficient evidence for the report's chain to `0x1400843f3`: the necessary intervening control flow and `RDI` provenance are not preserved in the excerpt. The observed `1 → 0` on native return is real as a reported snapshot, but its cause remains unproven.

## Follow-up `28f891f`: correction and remaining issues

The agent's [follow-up report and excerpts](../export/almanac-evidence/master-write-trace-20261109/) are preserved unchanged. Its correction of the missed host stores and `0x14037434d` is supported by the existing disassembly. These writes are found within a **limited three-function search**, not an exhaustive list across the executable. In particular its prose claims eight and seven writes while listing four and six, respectively. Indirect/aliased stores and unsearched functions remain unknown.

The agent reports having resolved `0x1408b21e4` as `trade` via PE mapping, but it did not save the actual bytes, section mapping or hash in those particular evidence files. Its subsequent report supplies the bytes and mapping; see the [button approach review](almanac-button-approach-review.md#reported-string-evidence). The bytes decode to `trade\0`, but the source executable and reported hash cannot be independently authenticated here. The six-byte loop at `0x14031fbb0–c5` would compare five characters plus a terminator if the constant is `trade\0`.

There are two consequential control-flow errors in the new summary:

1. The seven-byte name match at `0x14031f960–73` calls `0x140336e50` and then falls through to `0x14031f97d`. That block re-reads the selected pointer and compares it with the candidate: it skips the write only if the pointers are equal at `0x14031f991–97`. A name match by itself does not prove the write is skipped. Nor does a name mismatch guarantee a write, because it reaches the same pointer equality check.
2. The name mismatch branches at `0x14031fbbb` to `0x14031fcbe`. After a full match, `0x14031fbdc` tests a **separate vector count**, so `JZ 0x14031fc76` skips the loop only when that count is zero. When nonzero, a loop runs and eventually reaches the zero store. The report incorrectly labels the latter `JZ` as the string comparison and says its not-taken case branches to the name-mismatch block; neither is supported by the instructions.

The host path at `0x14031acd5` derives the `RCX` stored at `0x14031acd9` with `LEA [RDI+RSI]` after a `0x68`-stride record scan, rather than merely a prior load. The report's `0x1403282f9` source label `R15 = panel pointer` is not substantiated by its excerpts; the zero/other value should be traced through the full predecessor paths instead. One should not infer semantics from a register name.

No safe content suppression, hook, input handling, ninth-item fit, or Almanac close/return has been established. The observed switch/return is consistent with these candidate paths but does not prove they executed. Next analysis should evaluate the native row's layout and click handling as one system, specifically the observed eight-to-nine reordering and whether a separately drawn dynamic button can coexist with native controls, while documenting what needs an explicitly authorized integration test.
