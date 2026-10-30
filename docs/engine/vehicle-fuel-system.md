# Vehicle Fuel System — Ghidra Reverse Engineering Report

**Target:** `SOVIET64.exe` v1.1.1.9 (PE `0x6A3EB6AD`), base `0x140000000`, ASLR ON. All addresses are RVAs; file-based addresses = `0x140` + RVA.
**Status:** Historical attribution, NOT verified for the actual road truck.

> **2026-10-30 correction:** the real-game road-truck test does not validate
> this report's fuel-slot, activity-gate, native clamp or writer attribution.
> Do not use its addresses as verified truck burn/refill hooks. The original
> text below is retained as historical candidate evidence. Current findings,
> missing raw artifacts and implementation status are in
> [the single cumulative handover](../private-logistics-runtime/VEHICLE_PANEL_FUEL_INVESTIGATION.md).

---

## 0. Executive Summary

| Question | Answer |
|---|---|
| Tank field offset | `veh+0x3B8` cargo vector — fuel is a `{Resource*, float tons}` slot whose `Resource* == *(game+0xC3F0)`; `slot+0x08` = fuel level in tonnes. No dedicated standalone tank field exists. |
| `veh+0x5F0` role | Derived mirror of fuel level. Written by `FUN_140142430` (refuel) and `FUN_14014b920` (burn adjustment). **Not clamped** — can go negative (−0.0302 t observed). Not authoritative for tariff purposes. |
| Consumption formula | `consumption = (slot_rate / bld+0xE30) × base_rate × (tick_delta / DAT_14090AA78) × fVar24`, where `fVar24 = DAT_140909F58 / ((bld+0xE68 − bld+0xE60)/0xC)`. Units: **tonnes per tick**. |
| Does load matter? | **NO.** `fVar24` is building-level; cargo weight does not enter burn. `litres_per_tonne = 0`. |
| Refuel path | `FUN_140420350` (RVA `0x420350`) called from `FUN_14016E3B0` at `0x14016F831`: `transfer(game, veh+0x4F8, mode=0, res=*(game+0xC330), −tons, &out)`. Fuel flows from home building storage to vehicle cargo slot. **No money changes hands.** |
| Tank capacity | Per-vehicle: `*(veh+0x1708) + 0x8684` (refuel clamp in `FUN_140142430`). Building-level clamp component: `fVar24 = (bld+0xE30 / DAT_14090A968) × DAT_140909E54`. Constant `456.0f` (`0x43E40000`) **not found** anywhere in binary. |

---

## 1. Methodology

All findings derived from sequential Ghidra headless runs via `scratch\ghidra_run.bat`, JDK 21, `.java` scripts only, one run at a time, 120 s per call. Scripts used:

| Script | Purpose |
|---|---|
| `scratch\FindDisp.java` | Scan `.text` for instructions containing a given 32-bit displacement |
| `scratch\DecompAt.java` | Decompile a single function at a given address |
| `scratch\DisasmMany.java` | Disassemble an address range |

Key artifacts produced during this investigation:

| File | Content |
|---|---|
| `scratch/write_5f0.txt` | Every instruction touching `+0x5F0` (218 hits) |
| `scratch/write_5f4.txt` | Every instruction touching `+0x5F4` (104 hits) |
| `scratch/decomp_142430.c` | `FUN_140142430` — refuel writer of `+0x5F0` |
| `scratch/decomp_14b920.c` | `FUN_14014b920` — building upkeep, touches `+0x5F0` |
| `scratch/decomp_183580.c` | `FUN_140183580` — building-level burn |
| `scratch/d_g2_upkeep.c` | `FUN_14016E3B0` — building upkeep: burn + refuel |
| `scratch/asm_burn_disasm.txt` | Full disassembly `0x14016E3B0:0x14016FA00` |
| `scratch/disasm_14b920_5f0.txt` | `FUN_14014b920` around `+0x5F0` store |
| `scratch/search_1c8.txt` | `0x1C8` (456) hits — 524, all structure offsets, no capacity constant |
| `scratch/search_456f.txt` | `456.0f` (`0x43E40000`) — **0 hits** |

---

## 2. Q1 — The Tank Field

### 2.1 The cargo vector

Every vehicle instance has a cargo vector at:

| Offset | Size | Type | Meaning |
|---|---|---|---|
| `veh+0x3B8` | 8 bytes | pointer | Cargo vector **begin** |
| `veh+0x3C0` | 8 bytes | pointer | Cargo vector **end** |
| Stride | — | `0x10` | Per cargo slot |
| Slot layout | `{+0x00 Resource*, +0x08 float tons}` | — | Proven by burn slot lookup at `0x14016F6A0` |

This layout is corroborated by three independent plugin sources:
- `private_logistics\private_logistics.cpp` `CargoTons()` walks `veh+0x3B8/0x3C0` reading `beg + i*16 + 8` as float tons.
- `scratch/d_q1_owner.c` iterates `(*(lVar13+0x3c0) − *(lVar13+0x3b8)) >> 4` slots, reading `float*` at `lVar5+8`.
- `scratch/d_q2_3b8.txt` lists dozens of `+0x3B8` references across the codebase.

### 2.2 Which slot is fuel?

Inside `FUN_14016E3B0`, the burn loop locates the fuel slot by matching the slot's `Resource*` against `*(game+0xC3F0)`:

```
1016 | 14016f69d:  MOV R9,qword ptr [R8]          ; R9 = cargo vector begin
1017 | 14016f6a0:  MOV R8,qword ptr [R15 + 0xc3f0] ; R8 = game+0xC3F0 (fuel resource pointer)
1020 | 14016f6b0:  SHL RAX,0x4                    ; RAX = slot_index * 0x10
1021 | 14016f6b4:  ADD RAX,R9                     ; RAX = &cargo_slot[slot_index]
### 2.3 `veh+0x5F0` — derived mirror, not authoritative

`veh+0x5F0` behaves like fuel (falls during driving, jumps at refill, tops out near 0.46 t) but is **not** the authoritative store:

1. **Burn does NOT write to `+0x5F0`.** `FUN_14016E3B0` writes only to `slot+0x08`. Confirmed: none of the `MOVSS [reg+0x5f0]` stores in `write_5f0.txt` are inside `FUN_14016E3B0`. The burn function only *reads* `+0x5F0` at `0x1401704AB` (`COMISS XMM0,[R12+0x5f0]` in `FUN_14016FCD0`).

2. **`+0x5F0` can go negative.** Observed: −0.0302 t on day 13, −0.0502 t on day 13, six more times in 51 days. A physical tank cannot be negative.

3. **Two functions write `+0x5F0`:**
   - **Refuel:** `FUN_140142430` (RVA `0x140142430`) — adds fuel from home building, clamped by capacity.
   - **Burn adjustment:** `FUN_14014b920` (RVA `0x14014b920`) — `ADDSS XMM1,[R14+0x5f0]` then store back; the added term `XMM8 × [veh+0xDE0] × [R8+0x428]` is negative during burn.

**G1 Answer:** `+0x5F0` is category (b) — a derived copy/adjustment, not a direct subtraction inside the burn loop. The authoritative fuel store is `slot+0x08` (cargo slot matching `game+0xC3F0`).
1022 | 14016f6b7:  CMP qword ptr [RAX],R8         ; CMP slot.Resource*, game+0xC3F0
1023 | 14016f6ba:  JNZ 0x14016f6c7                  ; not fuel → next slot
1024 | 14016f6bc:  MOVSS XMM0,dword ptr [RAX + 0x8] ; XMM0 = slot.tons ← FUEL LEVEL
```

**Burn store** (decrement) at lines 1032–1035:
```
1032 | 14016f6d6:  COMISS XMM0,XMM8              ; compare slot.tons vs consumption
1033 | 14016f6da:  JBE 0x14016f790               ; if tons <= consumption → empty tank
1034 | 14016f6e0:  SUBSS XMM0,XMM8               ; tons = tons − consumption   ← BURN
1035 | 14016f6e5:  MOVSS dword ptr [RAX + 0x8],XMM0 ; slot.tons = XMM0        ← STORE BACK
```

If `tons <= consumption`:
```
1067 | 14016f790:  MOVAPS XMM8,XMM0
1068 | 14016f794:  MOV dword ptr [RAX + 0x8],0x0 ; slot.tons = 0 (clamped at zero)

---

## 3. Q2 — Consumption Formula

### 3.1 Function and context

`FUN_14016E3B0` (RVA `0x16E3B0`) — building upkeep, called once per building per tick. Signature:

```
void FUN_14016E3B0(longlong param_1 /* game */, longlong param_2 /* building */, float param_3 /* tick delta */)
```

For each vehicle in `bld+0xC70/+0xC78` (fleet vector, stride 8):

```
443 | lVar13 = *(longlong *)(*(longlong *)(param_2 + 0xc70) + lVar20 * 8);  ; vehicle*
446 | if (*(int *)(lVar13 + 0x50c) == 0x42) {                              ; fuel consumer
```

### 3.2 Step-by-step formula

| Step | Expression | Source |
|---|---|---|
| Slot rate | `slot_rate = *(float *)(*(veh+0x1708) + 0x85E0 + 4 + slot_index * 0x10)` | Order-array metadata |
| Base rate | `base_rate = DAT_140909FD8` (or `DAT_140909E54` if `veh+0x50C == 0x42`) | Global constants |
| Building divisor | `bld_divisor = *(float *)(bld+0xE30)` | Building field |
| Rate | `rate = (slot_rate / bld_divisor) * base_rate` | `0x14016F593–0x14016F5A2` |
| Tick scale | `tick_scale = param_3 / DAT_14090AA78` | `DAT_14090AA78` at `0x14090AA78` |
| Building multiplier | `fVar24 = DAT_140909F58 / ((*(bld+0xE68) − *(bld+0xE60))/0xC)` | Building-level, lines 116–121 |
| **Consumption** | `consumption = rate * tick_scale * fVar24` | `0x14016F57E–0x14016F582` |
| New tank level | `new_tons = max(0, slot.tons − consumption)` | `0x14016F6E0–0x14016F6E5` |

**Units:** `slot+0x08` is `float tons`. Consumption is **tonnes per tick**.

### 3.3 Does load matter? — NO

`fVar24` is computed once per building per tick from building-level data only:
- `DAT_140909F58` (global)
- `(bld+0xE68 − bld+0xE60) / 0xC` (count of stride-`0xC` entries in building struct)

Cargo weight, speed, distance, and load factor do **not** appear in the burn formula. A full truck burns identical fuel to an empty one.

**Tariff implication:** `litres_per_tonne = 0`. Fuel cost is per-tick (or per-trip), not per-tonne-km.

### 3.4 Burn gate (G3)

Beyond `veh+0x50C == 0x42`, burn is gated by vehicle state:

| Address | Instruction | Field | Required Value |
|---|---|---|---|
| `0x14016F515` | `MOVSXD R10,[RCX + 0x90]` | `veh+0x448` (integer state) | `∈ {1, 2}` |
| `0x14016F517–0x14016F523` | `LEA EAX,[R10−1]; TEST EAX,0xFFFFFFEE` | same | `EAX & 0xFFFFFFEE == 0` |
| `0x14016F538` | `CMP dword ptr [RCX + 0x88],0x9` | `veh+0x440` (mode) | `== 0x9` |
| `0x14016F556` | `CMP dword ptr [[[order+0x30]+0x318]+0x360],0x5` | Order facility type | `== 0x5` |

If `veh+0x448 ∉ {1, 2}`, the vehicle is **entirely skipped** — no burn, no refuel. Parked/idle vehicles do not burn.
```

---

## 4. Q3 — Refuel Path

### 4.1 Refuel occurs in `FUN_14016E3B0`

After burn, a stochastic refuel trigger fires:

```
1084 | 14016f7e4:  COMISS XMM9,XMM8              ; roll vs threshold
1085 | 14016f7e8:  JC 0x14016f7f9                ; if roll > threshold → refuel branch
```

Fuel is drawn from the vehicle's home building via the ordinary freight transfer:

```
1096 | 14016f81d:  MOV R9,qword ptr [R15 + 0xc330]   ; R9 = game+0xC330 (fuel resource)
1098 | 14016f827:  MOV RDX,qword ptr [RBX + 0x4f8]   ; RDX = veh+0x4F8 (home building)
1100 | 14016f831:  CALL 0x140420350                   ; transfer(game, home_bld, mode=0, fuel, −tons, &out)
```

### 4.2 Transfer arguments (proven)

| Param | Register | Value | Meaning |
|---|---|---|---|
| param_1 | RCX | `R15` = game | Game pointer |
| param_2 | RDX | `*(RBX + 0x4F8)` | **Vehicle's home building** (source of fuel) |
| param_3 | R8 | `0` | Mode 0 |
| param_4 | R9 | `*(game + 0xC330)` | Fuel resource |
| param_5 | stack | negative tons | Draw **from** building |
| param_6 | stack+0x28 | `&out` | Actual tons delivered |

`FUN_140420350` (RVA `0x420350`) is the same transfer used for all freight. Negative tons = resource flows **from** building **to** vehicle.

### 4.3 After transfer: add to tank and clamp

```
1074 | 14016f7b5:  MOV RAX,qword ptr [R13]     ; RAX = fuel slot pointer
1075 | 14016f7b9:  ADDSS XMM8,dword ptr [RAX + 0x8]; XMM8 += current slot.tons
1076 | 14016f7bf:  MOVSS dword ptr [RAX + 0x8],XMM8 ; slot.tons = new level
1077 | 14016f7c5:  COMISS XMM8,XMM12           ; compare vs clamp (fVar24)
```

### 4.4 Fuel resource globals

| Global | RVA | Use |
|---|---|---|
| `game+0xC330` | `0xC330` | Transfer resource at refuel (`0x14016F81D`) |
| `game+0xC3F0` | `0xC3F0` | Slot-match resource at burn (`0x14016F6A0`) |

Likely the same `Resource*` (fuel), used at two call sites. Confidence: likely.

### 4.5 Who pays? — Nobody

After transfer, shortfall booked via `FUN_1402fdb00` to `game+0x11408` (statistics) and city ledger. **No currency changes hands.** The engine only moves fuel tonnage and records statistics. The company's `cat=2 FOREIGN` diesel debits are a mod-side construct, not engine behavior.

### 4.6 Tank capacity

Two capacity mechanisms:
1. **Per-vehicle:** `*(veh+0x1708) + 0x8684` — used as refuel clamp in `FUN_140142430` (`scratch/decomp_142430.c`, line 60):
   ```c
   if ((param_1 < *(float *)(lVar24 + 0x8684)) && ...)  // +0x5F0 < capacity
   ```
2. **Building-level:** `fVar24 = (bld+0xE30 / DAT_14090A968) × DAT_140909E54` — used as clamp in burn function at `0x14016F7C5`.

**`456.0f` (`0x43E40000`) not found anywhere in binary** (`scratch/search_456f.txt`: 0 hits). Capacity is not hardcoded as `456.0f`.

---

## 5. G1 — What writes `veh+0x5F0`?

### 5.1 Not the burn function

Confirmed: `FUN_14016E3B0` (burn) writes only to `slot+0x08`, never to `veh+0x5F0`.

### 5.2 Refuel writer: `FUN_140142430` (RVA `0x140142430`)

Decompiled (`scratch/decomp_142430.c`, lines 53–89):

```c
lVar4 = *(longlong *)(param_2 + 0x4f8);      // veh+0x4F8 = home building
param_1 = *(float *)(param_2 + 0x5f0);       // READ current +0x5F0
if ((param_1 < *(float *)(lVar24 + 0x8684)) &&  // capacity at *(veh+0x1708)+0x8684
    (fVar28 = *(float *)(lVar24 + 0x8684) - param_1, 0.0 < fVar28)) {
    // ... find matching entry in home building's vector (stride 0xE0)
    fVar27 = *pfVar21;                        // amount available
    if (0.0 < fVar27) {
        param_1 = fVar27;
        if (fVar28 <= fVar27) param_1 = fVar28;   // clamp to available space
        *pfVar21 = fVar27 - param_1;              // deduct from source
        param_1 = param_1 + *(float *)(param_2 + 0x5f0);  // new = added + current
        *(float *)(param_2 + 0x5f0) = param_1;    // WRITE to veh+0x5F0
    }
}
```

**Expression:** `new_level = min(home_building_amount, capacity − current) + current`. Category (b) — copy from cargo-slot-derived value.

### 5.3 Burn adjustment writer: `FUN_14014b920` (RVA `0x14014b920`)

Disassembly (`scratch/disasm_14b920_5f0.txt`, lines 23–26):

```
14014bf53:  MOVAPS XMM1,XMM8              ; XMM1 = rate value
14014bf57:  MULSS XMM1,dword ptr [R14 + 0xde0]   ; XMM1 *= [veh+0xDE0]
14014bf60:  MULSS XMM1,dword ptr [R8 + 0x428]    ; XMM1 *= [R8+0x428]
14014bf69:  ADDSS XMM1,dword ptr [R14 + 0x5f0]   ; XMM1 += [veh+0x5F0]
14014bf72:  MOVSS dword ptr [R14 + 0x5f0],XMM1   ; [veh+0x5F0] = XMM1
```

**Expression:** `new_level = (XMM8 × [veh+0xDE0] × [R8+0x428]) + current_level`. The first term is negative during burn, positive during refuel.

### 5.4 G1 Answer

| Aspect | Answer |
|---|---|
| Authoritative store? | **No.** Burn clamps `slot+0x08` at 0 but does not clamp `+0x5F0`. |
| Category | (b) — copy/adjustment of cargo-slot-related value. |
| Recommendation | Read `slot+0x08` for fuel level; treat `+0x5F0` as a consistency check only. |

---

## 6. G2 — Where does `R9` point at `0x14016F6B4`?

### 6.1 Register chain

From `scratch/asm_burn_disasm.txt`, lines 1003–1021:

| Address | Instruction | Meaning |
|---|---|---|
| `0x14016F66B` | `MOVSXD RAX,R12D` | `RAX = order_index` |
| `0x14016F66E` | `IMUL RAX,RAX,0x68` | `RAX = order_index × 0x68` |
| `0x14016F675` | `MOV RAX,[RAX + R8 + 0x38]` | `RAX = order_entry[0x38]` (vector of vehicle pointers) |
| `0x14016F67A` | `MOV R8,[RAX + RCX×8]` | `R8 = order_entry[0x38][inner]` = `veh+0x3B8` for that vehicle |
| `0x14016F69D` | `MOV R9,[R8]` | `R9 = *(veh+0x3B8)` = cargo vector **begin** pointer |
| `0x14016F6B0` | `SHL RAX,0x4` | `RAX = slot_index × 0x10` |
| `0x14016F6B4` | `ADD RAX,R9` | `RAX = R9 + slot_index×0x10` = `&cargo_slot[slot_index]` |

### 6.2 G2 Answer

| Aspect | Answer |
|---|---|
| `R9` at `0x14016F6B4` | **`*(veh+0x3B8)`** — cargo vector begin pointer |
| Slot-array base expression | `*(order_entry+0x38)[inner_counter]` = `veh+0x3B8` |
| Does `order+0x38[j]` decay to `*(veh+0x3B8)`? | **No.** `order+0x38` is a **vector of `veh+0x3B8` pointers** (stride 8). An element equals `veh+0x3B8`. |

---

## 7. G3 — What gates the burn?

---

## 8. G4 — What is `bld+0xE30`, and tank capacity?

### 8.1 `bld+0xE30`

From `scratch/asm_burn_disasm.txt` lines 739–741:

```
14016f1fc:  MOVSS XMM12,dword ptr [RSI + 0xe30]   ; XMM12 = bld+0xE30
14016f205:  DIVSS XMM12,XMM15                      ; XMM12 /= DAT_14090A968
14016f20a:  MULSS XMM12,XMM7                       ; XMM12 *= DAT_140909E54
```

`fVar24 = (bld+0xE30 / DAT_14090A968) × DAT_140909E54`

Used as:
1. **Burn-rate divisor:** `slot_rate / bld+0xE30 × base_rate`
2. **Refuel clamp component:** compared against new tank level at `0x14016F7C5`

### 8.2 Tank capacity

| Source | Expression | Scope |
|---|---|---|
| Refuel clamp (`FUN_140142430`) | `*(veh+0x1708) + 0x8684` | Per-vehicle |
| Burn clamp (`FUN_14016E3B0`) | `(bld+0xE30 / DAT_14090A968) × DAT_140909E54` | Building-level |

**`456.0f` (`0x43E40000`) not found** — 0 hits in `scratch/search_456f.txt`. Capacity is not hardcoded as `456.0f`.

**Per-model capacity needed:** Yes. Capacity is per-vehicle (`*(veh+0x1708) + 0x8684`), so a mixed fleet needs per-vehicle reads.

---

## 9. G5 — The pair `+0x5F0` / `+0x5F4`

### 9.1 Writers

From `scratch/write_5f0.txt` and `scratch/write_5f4.txt`:

| Offset | Writers (sample) |
|---|---|
| `+0x5F0` | `FUN_140142430`, `FUN_14014b920`, `FUN_140183580`, `FUN_140183d30`, `FUN_140184930`, `FUN_1401856f0`, `FUN_140664e00`, `FUN_14068b330`, `FUN_1406b3b50`, `FUN_1406c81d0`, `FUN_1406de4a0` |
| `+0x5F4` | `FUN_1401edc40`, `FUN_1402bade0`, `FUN_1406b46e0`, `FUN_14083d9d0` |

### 9.2 G5 Answer

- `+0x5F0` and `+0x5F4` are **not** written by the same instruction pair. No adjacent `MOVSS` pair writes both.
- They're bit-identical at refill because refuel code sets both to the same post-refuel value.
- They drift during burn (0–0.002 difference) because different functions decrement them with different timing/rounding.
- `+0x5F0` goes negative (not clamped); `+0x5F4` behavior may differ.

---

## 10. G6 — Slot layout and fuel resource globals

### 10.1 Cargo slot layout

From `scratch/asm_burn_disasm.txt` lines 1011–1024:

```
14016f689:  MOV RDX,qword ptr [R8 + 0x8]   ; vector end
14016f68d:  SUB RDX,qword ptr [R8]         ; - vector begin
14016f690:  SAR RDX,0x4                    ; / 16 (stride 0x10)
14016f69d:  MOV R9,qword ptr [R8]          ; R9 = cargo vector begin
14016f6a0:  MOV R8,qword ptr [R15 + 0xc3f0] ; R8 = game+0xC3F0
14016f6b7:  CMP qword ptr [RAX],R8         ; CMP slot.Resource*, game+0xC3F0
14016f6bc:  MOVSS XMM0,dword ptr [RAX + 0x8] ; slot.tons
```

| Field | Offset | Type | Meaning |
|---|---|---|---|
| Slot array base | `veh+0x3B8` | pointer | Cargo vector `{begin@+0x00, end@+0x08}` |
| Stride | — | `0x10` | Per slot |
| Slot `+0x00` | `slot+0x00` | `Resource*` | Compared against `game+0xC3F0` |
| Slot `+0x08` | `slot+0x08` | `float tons` | **Authoritative fuel level** |
| Slot `+0x0C`+ | — | — | Not read in fuel lookup |

### 10.2 Fuel resource globals

| Global | RVA | Use |
|---|---|---|
| `game+0xC330` | `0xC330` | Transfer resource at refuel (`0x14016F81D`) |
| `game+0xC3F0` | `0xC3F0` | Slot-match resource at burn (`0x14016F6A0`) |

Likely the same `Resource*` (fuel), used at two call sites.

---

## 11. Consolidated Answers to Original Three Questions

### Q1 — The tank field

**`veh+0x3B8` cargo vector.** Fuel is a `{Resource*, float tons}` slot whose `Resource* == *(game+0xC3F0)`, and `slot+0x08` = fuel level in tonnes. No dedicated standalone tank field exists. `veh+0x5F0` is a derived mirror that can drift negative.

### Q2 — Consumption formula

```
consumption = (slot_rate / bld+0xE30) × base_rate × (tick_delta / DAT_14090AA78) × fVar24
fVar24 = DAT_140909F58 / ((bld+0xE68 − bld+0xE60)/0xC)
```

Units: **tonnes per tick**. **Load does NOT matter** — `fVar24` is building-level. `litres_per_tonne = 0`.

### Q3 — Refuel path

`FUN_140420350` (RVA `0x420350`) called from `FUN_14016E3B0` at `0x14016F831`:

```
transfer(game, veh+0x4F8, mode=0, res=*(game+0xC330), −tons, &out)
```

Fuel flows from home building storage to vehicle cargo slot `slot+0x08`. **No money changes hands.**

---

## 12. Verification Checklist for Plugin

The `[VEH CHG]` daily scan walks `veh+0x000..0x800`. To confirm the tank:

| Check | Expected Result |
|---|---|
| `slot+0x08` falls while truck drives | Yes — burn in `FUN_14016E3B0` |
| `slot+0x08` jumps at refuel | Yes — transfer at `0x14016F831` |
| `slot+0x08` unchanged when parked | Yes — burn gated by `veh+0x448 ∈ {1,2}` |
| `veh+0x5F0` can go negative | Yes — not clamped |
| Only fuel slot (`Resource* == game+0xC3F0`) changes | Yes |
| Cargo vector stride is `0x10` | Yes |

**Plugin action needed:** The `[VEH CHG]` scanner currently skips `+0x3A0..0x3C8` (cargo vectors). Narrow to `+0x3A0..0x3B8` (trailers only) so `+0x3B8..0x3C8` cargo slots (including fuel) are scanned, or explicitly track the slot matching `game+0xC3F0`.

---

## 13. Evidence Index

| File | Ghidra Command | Content |
|---|---|---|
| `scratch/write_5f0.txt` | `FindDisp.java write_5f0.txt 5f0` | All `+0x5F0` accesses (218 hits) |
| `scratch/write_5f4.txt` | `FindDisp.java write_5f4.txt 5f4` | All `+0x5F4` accesses (104 hits) |
| `scratch/decomp_142430.c` | `DecompAt.java decomp_142430.c 0x140142430` | Refuel writer of `+0x5F0` |
| `scratch/decomp_14b920.c` | `DecompAt.java decomp_14b920.c 0x14014b920` | Burn adjustment writer of `+0x5F0` |
| `scratch/decomp_183580.c` | `DecompAt.java decomp_183580.c 0x140183580` | Building-level burn |
| `scratch/d_g2_upkeep.c` | `DecompAt.java d_g2_upkeep.c 0x14016E3B0` | Building upkeep: burn + refuel |
| `scratch/asm_burn_disasm.txt` | `DisasmMany.java ... 0x14016E3B0:0x14016FA00` | Full burn function disassembly |
| `scratch/disasm_14b920_5f0.txt` | `DisasmMany.java ... 0x14014BF00:0x14014C000` | `+0x5F0` store in `FUN_14014b920` |
| `scratch/search_1c8.txt` | `FindDisp.java search_1c8.txt 1c8` | `0x1C8` hits — no capacity constant |
| `scratch/search_456f.txt` | `FindDisp.java search_456f.txt 43e40000` | `456.0f` — 0 hits |

All constants quoted in this document appear verbatim in the disassembly output.