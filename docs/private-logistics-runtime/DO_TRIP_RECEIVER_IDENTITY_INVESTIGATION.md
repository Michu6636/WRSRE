# Native DO-Truck Receiver Identity Investigation (Agent C)

## Mission statement (from INSTRUCTIONS.md)

Agent C owns pointer provenance, executor applicability, and an identity-only observation fallback. The narrow missing proof: whether the type-43 office-fleet truck receiving a plan (Chain 1) is the same native object processed by the proposed simulation consumer (Chain 2: FUN_14066D530/FUN_140699420).

Agent A owns assignment/start/work scope and integrates the final rule. Agent B owns removal/cancellation/replacement. Agent C does NOT interpret job starts/ends, repeat gate reviews, or classify C80.

## Deliverable scope

- Owns ONLY docs/private-logistics-runtime/DO_TRIP_RECEIVER_IDENTITY_INVESTIGATION.md (new) and scratch/newC_* artifacts.
- Does NOT edit A's DO_ASSIGNMENT_INTERVAL_INVESTIGATION.md, B's DO_ASSIGNMENT_INTERVAL_INDEPENDENT_REVIEW.md, README, cumulative handover, or others' artifacts.
- Provides A/B a compact receiver-chain handoff map from office-fleet candidate to executor and exact field receivers.

## Executable identity — verified

Verified directly from PE:

| Property | Value |
|---|---|
| File | C:\Workers Resources Soviet Republic\SOVIET64.exe |
| Size | 10,308,608 bytes |
| SHA-256 | 296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8 |
| PE timestamp | 0x6A3EB6AD |
| Image base | 0x140000000 |
| Machine/magic | AMD64 / PE32+ |

All identity values match Agent A and Agent B records.

## Report structure

1. Chain 1 recap (office fleet -> plan builder) — proven by A and B, byte-verified.
2. Chain 2 decoding (enumeration -> 66D530 -> 699420) — decoded directly from PE.
3. Candidate callers of 66D530 — exact argument mapping.
4. The alias question: does any Chain 2 caller process the same object as Chain 1?
5. +0x4A0 discriminator semantics — list-membership guard, not truck identity.
6. Conclusion: proved applicable chain / disproved old path / remaining alias edge.
7. Compact handoff map for A and B.
8. Fallback two-ended identity observation proposal.
9. Validation script source, commands, and actual stdout.
