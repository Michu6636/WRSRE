# Retail Purchase Block Investigation — FUN_140171E10 (shop retail tick)

**Target:** `SOVIET64.exe` v1.1.1.9, base `0x140000000`, file-based RVA `0x140` + RVA.
**Status:** INVESTIGATION ONLY — no `.cpp`/`.h`/`.ini` files were modified.
**Deliverable:** `docs/Retail_Purchase_Block_Investigation.md` (UTF-8, no BOM).

---

## 1. Bottom line (recommendation)

Block the give at the engine's own checkout fulfillment gate inside `FUN_140171E10`, **before** shelf inventory or citizen demand are mutated, and only when the citizen cannot afford the per-frame dispensed amount:

> At the fulfillment gate (all three native conditions already passed), compute
> `frameCost = amount × pricePerTon(res, cur) × marketMult` and compare against
> `g_currency->getWalletBalance(person)`. If `wallet < frameCost`, jump to the
> loop-continue target `0x14017293a` — shelf and demand are left untouched, the
> citizen leaves empty-handed, and the engine's native hunger/health/happiness
> penalties apply naturally. If affordable, fall through to the normal write path.

Why here and not in `citizen_retail::ProcessOneDemand`: the engine (`FUN_140171E10`) gives the goods (decrements shelf + satisfies demand) **before** the plugin charges (`citizenToStatePartial`). A broke citizen (`taken == 0`) still leaves fully fed. A post-checkout rollback or "charge then claw back" is fragile; a pre-give affordability gate is the clean native block.
## 2. Phase B structure (decompiled `scratch/sol_171e10.c`, lines ~218–363)

`FUN_140171E10(longlong param_1, longlong param_2)` where `param_2` is the shop `Building*`.

Phase B = the customer loop:

| Construct | Meaning |
|---|---|
| outer `local_138` loop | iterates shop customer list `bld + 0xBD8 .. 0xBE0` |
| `lVar8 = *(longlong *)(local_128 + bld + 0xbd8)` | `lVar8` = current `Person*` (customer) |
| `pfVar15 = (float *)(lVar8 + 0x118)` | start of person's demand array |
| inner demand loop stride `0x80` | `person + 0x118 + d * 0x80` per demand (`TSM_PERSON_DEMAND_STRIDE 0x80`, cap 7) |
| `*(int *)(person + 0x110)` | demand count |
| demand record layout | `+0x00` remaining amount (float), `+0x04` rate (float), `+0x08` kind (int 1/2/3), `+0x10` resource pointer (qword) |

The **fulfillment branch** (where goods actually change hands) is decompiled at lines 252–263:

```c
if ((*(longlong *)(lVar1 + lVar14) == *(longlong *)(pfVar15 + 4)) &&   // shelf res id == demand res id
    (fVar19 = *(float *)(lVar1 + 8 + lVar14), 0.0 < fVar19) &&          // shelf inventory > 0
    (pfVar15[1] * fVar18 < *pfVar15))                                   // demand_remaining > rate*time
{
    ...
    *(float *)(lVar1 + 8 + lVar14) = *(float *)(lVar1 + 8 + lVar14) - fVar19;   // SHELF -= amount
    *pfVar15 = *pfVar15 - fVar19;                                               // DEMAND SATISFIED -= amount
    ...
}
```

**Critical finding:** there is **no wallet check anywhere in Phase B**. `person + 0x71C` (tourist flag: 0 resident / 1 USD / 2 RUB) is read at lines 283, 302, 328 only to route a satisfaction/fee accumulator (`+0x734` spent, `+0x738` satisfaction, treasury `+0x580`/`+0x588`) — it never gates the physical give. Goods are dispensed blindly once the three native conditions pass. `citizen_retail.cpp` then charges in `ProcessOneDemand` (line 480: `citizenToStatePartial(person, totalCost, ...)`), which returns 0 for a broke citizen — too late, the goods are already gone.

## 3. Exact checkout fulfillment gate (disassembly)

Range disassembled: `0x140172459`–`0x140172a00` → `scratch/shop_checkout_gate.asm`.

The three gate conditions are consecutive compare/branch pairs; all three jump to `0x14017293a` (skip fulfillment → continue to next demand/customer). When **none** is taken, execution falls into the write path at `0x1401725de`.

| RVA | Bytes | Instruction | Meaning (gate condition) |
|---|---|---|---|
| `0x1401725a9` | `48 39 04 1A` | `CMP qword ptr [RDX+RBX],RAX` | shelf slot `+0x0` (res pointer) == demand `+0x10` (res pointer) |
| `0x1401725ad` | `0F 85 87 03 00 00` | `JNZ 0x14017293a` | ≠ → skip (no match) |
| `0x1401725ba` | `45 0F 2F C3` | `COMISS XMM8,XMM11` | shelf inventory `+0x8` vs `0.0` (XMM11 = 0.0) |
| `0x1401725be` | `0F 86 76 03 00 00` | `JBE 0x14017293a` | shelf ≤ 0 → skip (empty shelf) |
| `0x1401725d5` | `0F 2F C1` | `COMISS XMM0,XMM1` | demand remaining vs `rate × time` (XMM1) |
| `0x1401725d8` | `0F 86 5C 03 00 00` | `JBe 0x14017293a` | demand fully covered this frame → skip |
| **`0x1401725de`** | — | **← fulfillment entry (all gates passed)** | **hook point** |

First game-memory mutation (the actual give):
| RVA | Bytes | Instruction |
|---|---|---|
| `0x14017262b` | `F3 0F 11 44 1A 08` | `MOVSS [RDX+RBX+0x8],XMM0`  ← **SHELF inventory decremented** |
| `0x14017263f` | `F3 0F 11 0F` | `MOVSS [RDI],XMM1`          ← **DEMAND remaining decremented (satisfied)** |

`0x14017293a` is the loop-continue target (next shelf slot / next demand / next customer). Jumping there leaves shelf and demand untouched — exactly the desired "leaves empty-handed" outcome.

## 4. Register mapping at the gate (hook point `0x1401725de`)

All three native conditions have passed; none of the shelf/demand writes have executed yet.

| Register | Holds | Notes |
|---|---|---|
| `R14` | `Building*` (shop) | `param_2`; source of `+0xBD8/0xBE0` customer list, `+0x970/0x978` shelf slots, `+0x108`, `+0x318` |
| `RSI` | `Person*` (customer) | from customer list; `+0x110` demand count, `+0x118` demands, `+0x71C` tourist flag, `+0x734` spent, `+0x738` satisfaction |
| `RDI` | `Person* + 0x118 + d*0x80` = current demand record | `+0x00` remaining, `+0x04` rate, `+0x08` kind, `+0x10` res pointer |
| `RAX` | `Resource*` (res pointer) | = `[RDI + 0x10]`; set at `0x1401725a5`, **still live at `0x1401725de`** (not clobbered before the hook point) |
| `RDX` | shelf-record array base | `=[RCX]` where `RCX = [R14+0x970] + R15` |
| `RBX` | shelf slot offset (stride `0x10`) | `[RDX+RBX+0x0]` = res pointer, `[RDX+RBX+0x8]` = shelf count (float) |
| `RCX` | shelf slot descriptor ptr | `[R14 + 0x970] + R15` |
| `R13` | `param_1` (game/world state) | used for `+0x11420`, `+0x580`, `+0xc2c8` |
| `XMM0` | demand remaining (`[RDI]`) | later reused as `shelf - amount` |
| `XMM1` | `rate × time` (`XMM2 × XMM9`) | |
| `XMM2` | demand rate (`[RDI+0x4]`) | at `0x1401725eb` becomes `rate × time` via `MULSS XMM2,XMM15` |
| `XMM8` | shelf inventory (`[RDX+RBX+0x8]`) | becomes `amount = min(shelf, rate×time)` |
| `XMM11` | `0.0` | comparator for empty-shelf test |

**`amount` (tons dispensed this frame)** is not a single live register at `0x1401725de`; it is computed as `min(XMM8 [shelf], XMM2 [rate × time])`. After executing the steal region (see §5), `XMM2 = rate × time` and `XMM8 = shelf`, so `amount = MINSS XMM8, XMM2`.

## 5. Hook feasibility

### 5.1 Inline hook — YES (≥ 14 bytes available)

Clean, register-only steal region with **no incoming jump targets inside** and **no game-memory writes**:

| RVA | Bytes | Instruction | Length |
|---|---|---|---|
| `0x1401725de` | `F3 0F 10 44 1A 08` | `MOVSS XMM0,[RDX+RBX+0x8]` | 6 |
| `0x1401725e4` | `4D 8D 85 20 14 01 00` | `LEA R8,[R13+0x11420]` | 7 |
| `0x1401725eb` | `F3 41 0F 59 D7` | `MULSS XMM2,XMM15` | 5 |

- **Total: 18 bytes** (`0x1401725de` … `0x1401725f0`, exclusive) — satisfies the ≥14-byte stolen-bytes rule.
- Region ends exactly at instruction boundary `0x1401725f0` (`MOV byte ptr [RSP+0x30],0`).
- **No memory writes** in the stolen region (register-only), so on the "unaffordable → skip" path we simply never execute `0x1401725ff` (`[RBP+0x88]=1`, bVar3) / `0x14017260b` (`[RBP+0x80]=1`, bVar4). Those flags stay `0`, which is **correct** (no fulfillment occurred → no spurious `FUN_140171d40` restock notification at `0x140172a34`).
- XMM0/XMM8/XMM2 are all dead at the skip target `0x14017293a` (overwritten before next use), so clobbering them is safe.

### 5.2 Call detour — also viable

A `jmp [rel32]` detour (5 bytes) is smaller than the 18-byte region, so a pure inline hook works without a trampoline for the jump itself; a trampoline is still needed to re-execute the 18 stolen bytes and to call into the currency/pricing API (which lives outside a 2 GB ± range of the hook site). Either implementation is safe here; the inline-hook + trampoline is preferred because the entire 18 bytes are available contiguously.

## 6. Resolving wallet and price inside the hook

The wallet is **not a field on `Person`** (confirmed in `src/currency_api.h` — "the base game has no money field on a Person"). It lives in the `currency` plugin's map keyed by a citizen ID derived from the person object:

- Wallet read: `g_currency->getWalletBalance(person)` → `float` (currency plugin; also used by `citizen_retail.cpp` line 527). Read-only lookup, safe to call from the shop tick (no D3D, no render-state dependency).
- Citizen ID: `TsmCitizenID(person)` (`src/currency_api.h`, §~93–95).
- Price per ton: `g_pricing->getPrice(res, cur)` where `cur = TsmCurrencyActive(g_currency)` and `res = RAX`.
- Market multiplier: `TsmPricingMarketMult(g_pricing, res)`.
- Frame cost: `frameCost = amount × pricePerTon × marketMult`, with `amount = min(shelf, rate × time)`.

All three (`g_currency`, `g_pricing`, `TsmCitizenID`) are already Level-1 dependencies of `citizen_retail`, so a checkout hook living in that plugin can resolve them without crossing layer boundaries (see `docs/DEVELOPMENT_README.md` level rules).

## 7. Recommended implementation strategy

1. Install an inline hook at `0x1401725de` stealing 18 bytes (`0x1401725de`–`0x1401725f0`).
2. Trampoline re-executes the 18 stolen bytes, then:
   - `amount = min(XMM8 [shelf], XMM2 [rate × time])`
   - `wallet = g_currency->getWalletBalance(RSI)`
   - `res = RAX` (resource pointer)
   - `price = g_pricing->getPrice(res, cur) × TsmPricingMarketMult(g_pricing, res)`
   - `frameCost = amount × price`
   - **if `wallet < frameCost`:** `jmp 0x14017293a` → skip give; shelf & demand untouched; bVar3/bVar4 remain 0; customer proceeds to next demand/customer and ultimately leaves the store unsatisfied (native hunger/complaint decay applies).
   - **else:** `jmp 0x1401725f0` → resume normal fulfillment (sets bVar3/bVar4, writes shelf at `0x14017262b`, demand at `0x14017263f`).
3. Log the block occasionally (probe-gated) for tuning: resource, amount, wallet, frameCost.

### Why this satisfies the three goals
- **Shelf not decremented:** we skip `0x14017262b`.
- **Demand not satisfied:** we skip `0x14017263f`, so `person + 0x118 + d*0x80 + 0x00` (remaining) is unchanged; the citizen remains hungry/unclothed.
- **Citizen leaves empty-handed + native penalties:** the loop continues normally; no satisfaction (`+0x738`) or spent (`+0x734`) is accrued for this unpaid demand; the engine's hunger/health/happiness tick runs independently of the shop.

## 8. Alternatives considered and rejected

| Alternative | Why rejected |
|---|---|
| Hook `citizen_retail::ProcessOneDemand` after `citizenToStatePartial` | Too late — engine already gave the goods; a `taken == 0` result cannot un-give shelf inventory or un-satisfy demand. |
| Post-checkout rollback (give, then if unpaid restore shelf/re-open demand) | Fragile: must undo shelf write (`0x14017262b`), demand write (`0x14017263f`), `FUN_1402fdb00` transfers (`0x140172672`/`0x1401727f2` region), and satisfaction accumulators; easy to desync and re-entrancy risky inside the tick loop. |
| Zero the demand amount (`person + 0x118 + 0x00 = 0`) after the give | Goods already given; citizen still fed. Does not block the give. |
| Add wallet as a 4th native gate condition | Would require moving/resizing three existing `JNZ`/`JBE` branches with 6-byte rel32 offsets; more invasive than the 18-byte inline hook at `0x1401725de`. |

### Coordination note
`citizen_retail::ProcessOneDemand` charges `totalCost = amountNeeded × price × marketMult` (the **full** remaining demand, line 469), while the engine dispenses a **per-frame** `amount = min(shelf, rate × time)`. When the gate blocks, nothing is given, so there is nothing to charge. If `citizen_retail` independently re-walks the same demand later in the frame, its `ShopHasResourceInStock` check still sees stock (we did not decrement) and could attempt a charge for goods never dispensed. The hook should therefore be paired with citizen_retail skipping the charge when the engine gate blocked (e.g. via the existing `g_interceptor` hook at citizen_retail.cpp line 473–478, which already lets an interceptor short-circuit the sale). That cross-plugin coordination is outside this investigation's scope but is the natural place to wire it.

## 9. Verification checklist (before any code is written)

- [ ] Re-confirm `0x1401725de`–`0x1401725f0` instruction boundaries and that no jump lands inside (no targets in `0x1401725de`–`0x1401725f0`; observed targets are `0x1401725a2`, `0x140172580`, `0x140172530`, `0x140172932/0x14017293a`, `0x140172970`, `0x1401729b3`, `0x1401729d9`, `0x1401729fc`, `0x140172a32`, `0x140172a73`).
- [ ] Confirm `getWalletBalance(person)` is a pure read (no wallet mutation on lookup) so a blocked give does not perturb the currency map.
- [ ] Confirm `res` (`[RDI+0x10]`) is a resource pointer matching `g_pricing->getPrice` input (same offset used by `citizen_retail::ProcessOneDemand` at `demand + 0x10`).
- [ ] Build the hook as a Level-1 `citizen_retail` addition; verify `LastWriteTime` of `build\plugins\citizen_retail.dll` after `cmd /c build.bat citizen_retail`.
- [ ] In-game test: a citizen with 0 RUB at a shop — shelf count must not drop, demand remaining must not drop, citizen leaves unfed and accrues hunger complaints; a citizen with sufficient RUB must be served normally.

## 10. Sources

- `scratch/sol_171e10.c` — Ghidra decompile of `FUN_140171e10` (lines 218–363 = Phase B customer/demand loop).
- `scratch/shop_checkout_gate.asm` — `DisasmMany.java` range `0x140172459`–`0x140172a00`.
- `plugins/citizen_retail/citizen_retail.cpp` — `ProcessOneDemand` (lines 427–492), `WalkShop` (line 499+), price computation (lines 463–469), charge (line 480), interceptor (lines 473–478).
- `src/currency_api.h` — wallet map keyed by citizen ID, no money field on Person; `getWalletBalance`, `citizenToStatePartial`.
- `plugins/currency/currency.cpp` — `Api_GetWalletBalance`/`WalletGet` (line 222), `Api_CitizenToStatePartial` (line 232).
- `docs/Citizen_SoL_Investigation.md`, `scratch/citizen_sol_investigation.txt` — `TSM_PERSON_DEMAND_STRIDE 0x80`, `TSM_PERSON_TOURIST 0x71C` (0/1/2 resident/USD/RUB), `+0x734` spent, `+0x738` satisfaction.
- `docs/CPI_Survival_Basket_Investigation.md` — price = `getPrice(res, cur) × getMarketMultiplier(res)`.
- `docs/DEVELOPMENT_README.md` — plugin leveling rules; `.clinerules` — inline hook ≥ 14 stolen bytes rule, UTF-8-no-BOM INI rule (not applicable: no .ini touched).

