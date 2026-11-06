# Proposal: B2B Wholesale Counterparty Settlement ("Ship 2")

- **Status:** Draft RFC / Implementation Spec (non-invasive — no `private_logistics` or `private_shops` files modified by this proposal)
- **Target Subsystems:** `plugins/private_logistics/private_logistics.cpp` (primary), `plugins/private_shops/private_shops.cpp` (cutover coordination)
- **Aligns With:** `docs/implementation-plan-per-trip-fuel.md` (v0.18 Option A deferred mode-1 billing) and `docs/review-per-trip-fuel-plan.md`
- **Service ABI Impact:** **Zero new ABI versions required** (`enterprise_base` v8, `currency` v4, and `resource_pricing` v3 already provide every required primitive; optional lightweight query flag noted in §7.3 for automatic `private_shops` cutover)

---

## 1. Executive Summary & Economic Motivation

Today, WRSRE's B2B economy routes every physical movement of goods through the **State Treasury as an involuntary clearinghouse**:

1. **In `private_shops.cpp` (`WholesaleSlot` / `RunWholesaleBilling`, lines 830–985):**
   - A 1 Hz storage-delta scanner watches every private shop, pub, factory, and warehouse.
   - When an output buffer drops, `private_shops` credits the producer (`TSM_CAP_PRODUCTION`) and debits the State Treasury (`moveTreasury(-credit, "procurement")`).
   - When an input/shelf buffer rises, `private_shops` debits the buyer (`chargeWholesaleDelivery`) and credits the State Treasury (`moveTreasury(+cost, "wholesale")`).
2. **In `private_logistics.cpp` (`Pay()`, lines 1005–1065, currently gated behind `billing = 0`, `dry_run = 1`):**
   - At load RVA `0x694F40` (`mode = 0, tons < 0`), `Pay()` credits the source factory (`TSM_CAP_PRODUCTION`) from State Treasury (`"b2b_sale"`).
   - At delivery RVA `0x68F792` (`mode = 0, tons > 0`), `Pay()` credits the fleet owner (`TSM_CAP_WHOLESALE`) from State Treasury (`"b2b_freight"`).
   - **The buyer is never debited in `private_logistics`, and turning `billing = 1` while `private_shops` `wholesale_enabled = 1` double-pays the factory from State Treasury.**

### Why State-Clearinghouse B2B Hurts Gameplay & Accounting

Even when wholesale prices look symmetric on paper, routing private-to-private trade through the State Treasury produces four structural distortions:

1. **Inventory Float Subsidized by Taxpayers:** Between the day a private farm produces 100 t of crops into a warehouse and the day a private food factory or shop buys them, the State fronts 100% of the working capital. Overproduction by private farms drains the State Treasury even if nobody ever buys the goods.
2. **Spoilage, Demolition, and Border Leaks:** Any private output that decays, sits unsold in a full warehouse, or disappears on demolition was already paid for by the State.
3. **Freight Tariff Socialization:** Under `Pay()`, the State pays private Distribution Offices (`b2b_freight`) to haul goods between two private companies (`Private Factory A -> Private DO -> Private Shop B`). Taxpayers subsidize 100% of private B2B trucking.
4. **Distorted Fiscal Readouts & Affordability Deadlocks:** Currency flow ledgers (`currency_api.h` observers) Mix billions of roubles of private gross B2B turnover into State budget lines (`procurement` / `wholesale`), and if the State Treasury hits `0 RUB` during a public construction project, `Pay()`'s treasury affordability gate (`private_logistics.cpp:1042–1053`) halts **private-to-private** commerce even when both private companies are flush with cash.

### Target Architecture ("Ship 2": Vehicle-Provenance Counterparty Settlement)

Use the **physical vehicle (`p2` on `mode == 1` of `FUN_140420350`)** as the ground-truth bill of lading linking Seller, Carrier, and Buyer:

- **Private Seller <-> Private Buyer:** Buyer pays Seller directly (`modifyCompanyCapital`), with **zero State Treasury involvement**.
- **State Seller <-> Private Buyer (or Private Seller <-> State Buyer):** Counterparty settles against State Treasury (`moveTreasury`), because the State genuinely *is* the counterparty on that leg (e.g., a State farm supplying a private distillery, or a private brickworks supplying a State warehouse/construction depot).
- **Freight Tariff (`trip_tariff`):** Paid by the **receiving Buyer** (or State Treasury if the destination is State-owned) to the **Carrier (`vehicleHome`)**. Self-haul (`carrier == buyer` or `carrier == seller` on own-account delivery) skips external freight billing while still deducting fuel operating cost (`v0.18`).

---

## 2. Ground-Truth Engine Call Ordering (And a Latent Bug in v0.17)

Every domestic loading and unloading operation in `SOVIET64.exe` invokes `FUN_140420350` **twice in chronological sequence** on the simulation thread: first `mode == 0` (the building storage side), then `mode == 1` (the vehicle cargo side, ~0–70 ms later in the same tick/frame).

| Physical Event | Call #1 (`mode == 0`, Building Side) | Call #2 (`mode == 1`, Vehicle Side) | What Each Call Knows |
|---|---|---|---|
| **Vehicle Loads at Source** | RVA `0x694F40`: `tons < 0`, `p2 = sourceBld`, `p9 = NULL`, `vhome = NULL` | RVA `0x6953C4`: `tons > 0`, `p2 = vehicle`, `vhome = *(veh + 0x1F8)` | Call #1 knows **`sourceBld`**; Call #2 knows **`vehicle`** and **`vehicleHome`**. |
| **Vehicle Unloads at Dest** | RVA `0x68F792`: `tons > 0`, `p2 = destBld`, `p9 = fleetOwner`, `vhome = NULL` | RVA `0x68F7F9`: `tons < 0`, `p2 = vehicle`, `vhome = *(veh + 0x1F8)` | Call #1 knows **`destBld`** and **`p9` (`fleetOwner`)**; Call #2 knows **`vehicle`**. |

### Critical Bug in Existing v0.17 `CarrierNote` Join (`private_logistics.cpp:1201–1226`)

Look closely at `DetourTransfer` in `plugins/private_logistics/private_logistics.cpp:1201–1226`:

```cpp
if (committed && mode != 0 && vehicleHome) {
    CarrierNote& n = g_carrier[g_carrierHead % CARRIER_RING];
    ... // mode 1 pushes into g_carrier
}

if (committed && mode == 0 && tons < 0.0f && !owner) {
    // The load leg: param_9 is NULL, so find the vehicle that took these
    // goods from the mode-1 call that follows within a few milliseconds.
    ... // BUG: searches g_carrier DURING mode 0, BEFORE mode 1 has fired!
}
```

Because `0x694F40` (`mode == 0`) executes **before** `0x6953C4` (`mode == 1`), searching `g_carrier` inside the `mode == 0` branch can **never** see the `mode == 1` note for the current load! At best it falsely matches a stale note from a *previous* truck that loaded the exact same tonnage of the same resource within 2.0 seconds; otherwise `carrierMatched` is `false`.

### Unified Deferred Mode-1 Join Pattern (Aligns Directly With v0.18 Fuel Plan Option A)

`docs/implementation-plan-per-trip-fuel.md` (Option A) and `docs/review-per-trip-fuel-plan.md` already establish the exact fix for the **delivery** leg (`PendingDelivery`: stash at `0x68F792` `mode == 0`, consume and settle at `0x68F7F9` `mode == 1` when `p2 = vehicle` arrives).

Applying the **exact same symmetric pattern** to the **load** leg (`PendingLoad`: stash at `0x694F40` `mode == 0`, consume at `0x6953C4` `mode == 1` when `p2 = vehicle` arrives) solves **both** the v0.17 `CarrierNote` ordering bug and vehicle cargo provenance in one stroke:

```
LOAD AT SOURCE:
  [1] 0x694F40 (mode=0, -tons, p2=sourceBld)
        -> push PendingLoad { sourceBld, res, abs(tons), t }
  [2] 0x6953C4 (mode=1, +tons, p2=vehicle, vhome=carrierBld)
        -> pop matching PendingLoad (same res, |tons| within 0.5%, dt <= 2.0s)
        -> stamp VehCargoLot on VehSlot(vehicle): seller = EffectiveSeller(sourceBld, res)

UNLOAD AT DESTINATION:
  [3] 0x68F792 (mode=0, +tons, p2=destBld, p9=fleetOwner)
        -> push PendingDelivery { destBld, p9, res, tons, t }   // same struct as v0.18 Option A!
  [4] 0x68F7F9 (mode=1, -tons, p2=vehicle, vhome=carrierBld)
        -> pop matching PendingDelivery (same res, |tons| within 0.5%, dt <= 2.0s)
        -> read & deduct VehCargoLot from VehSlot(vehicle)
        -> execute atomic B2B Settlement(seller, carrier, buyer=destBld, res, tons)
        -> execute v0.18 per-trip fuel deduction on `vehicle` (same hook point!)
```

Notice how cleanly this composes with `v0.18`: **`0x68F7F9` (`mode == 1, tons < 0`) becomes the single, unified settlement commit point** where Seller (`VehCargoLot`), Carrier (`vehicleHome` / `pd.fleetOwner`), Vehicle (`p2`, for fuel burn & Odo), and Buyer (`pd.destBld`) are **all simultaneously known**.

---

## 3. Counterparty Settlement Matrix

Let `IsPriv(B)` mean `B != nullptr && IsPrivate(B) && !TsmIsDecayed(B)`.
Let `State` denote the Republic Treasury (`g_currency->moveTreasury`), and `Cap(B)` denote private company capital (`g_enterprise->modifyCompanyCapital`).

At delivery commit (`0x68F7F9`), for cargo quantity $Q$ tons of resource $R$:
- **Commodity Principal:** $V_{\text{goods}} = Q \times P_{\text{wholesale}}(R)$, where $P_{\text{wholesale}}(R) = \text{getPrice}(R, \text{cur}) \times \text{getMarketMultiplier}(R) \times \text{wholesale\_Factor}$ (using existing `resource_pricing` v3 live price + market multiplier so `private_logistics` and retail margins stay consistent).
- **Freight Tariff:** $V_{\text{freight}} = \text{TariffFor}(Q, \dots)$ (existing v0.7/v0.18 formula, or `0.0` if `carrier == nullptr` or `carrier == buyer` self-haul).

### 3.1 Goods Principal Leg ($V_{\text{goods}}$: Buyer -> Seller)

| Seller (`lot.seller`) | Buyer (`destBld`) | Economic Meaning | Payer Action | Payee Action |
|---|---|---|---|---|
| **Private A** | **Private B** (`A == B`) | Intra-company transfer (vertical integration) | **None** (`0.00` moved; log `[B2B INTERNAL]`) | **None** |
| **Private A** | **Private B** (`A != B`) | Pure B2B private wholesale | Debit `Cap(B)` by $-V_{\text{goods}}$ (`TSM_CAP_WHOLESALE`) | Credit `Cap(A)` by $+V_{\text{goods}}$ (`TSM_CAP_PRODUCTION`) |
| **Private A** | **State** | State procurement from private producer | Debit `State` by $-V_{\text{goods}}$ (`"b2b_state_buy"`) | Credit `Cap(A)` by $+V_{\text{goods}}$ (`TSM_CAP_PRODUCTION`) |
| **State** (or unknown/import origin) | **Private B** | Private firm buying from State depot / customs | Debit `Cap(B)` by $-V_{\text{goods}}$ (`TSM_CAP_WHOLESALE`) | Credit `State` by $+V_{\text{goods}}$ (`"b2b_state_sell"`) |
| **State** | **State** | Pure State-sector transfer | **None** | **None** |

### 3.2 Freight Leg ($V_{\text{freight}}$: Buyer -> Carrier)

| Carrier (`carrierBld`) | Buyer (`destBld`) | Economic Meaning | Payer Action | Payee Action |
|---|---|---|---|---|
| **Private C** | **Private B** (`C == B`) | Buyer's own DO / own truck (self-haul) | **None** (C already pays fuel via v0.18) | **None** |
| **Private C** | **Private B** (`C != B`) | Third-party private carrier | Debit `Cap(B)` by $-V_{\text{freight}}$ (`TSM_CAP_WHOLESALE`) | Credit `Cap(C)` by $+V_{\text{freight}}$ (`TSM_CAP_WHOLESALE`) |
| **Private C** | **State** | Private DO hauling for State building | Debit `State` by $-V_{\text{freight}}$ (`"b2b_freight"`) | Credit `Cap(C)` by $+V_{\text{freight}}$ (`TSM_CAP_WHOLESALE`) |
| **State DO / State Truck** | **Private B** | State DO hauling for private buyer *(configurable via `charge_state_carrier`, default `1`)* | Debit `Cap(B)` by $-V_{\text{freight}}$ (`TSM_CAP_WHOLESALE`) | Credit `State` by $+V_{\text{freight}}$ (`"b2b_state_freight"`) |
| **State DO / State Truck** | **State** | Pure State logistics | **None** | **None** |

> **Why Buyer pays Freight (CIF / Delivered Pricing):** Charging freight to the receiving enterprise (`destBld`) means a private shop far from its suppliers naturally faces higher landed cost than a well-located shop, and a vertically integrated company that buys its own DO (`C == B`) internalizes freight and pays only fuel + driver wages.

### 3.3 Why Zero `enterprise_base` ABI Changes Are Needed

Inspecting `ApplyCategory` in `plugins/enterprise_base/enterprise_base.cpp:1140–1187` (v9.18) confirms every P&L bucket already behaves **purely locally** (none of these categories call `moveTreasury` inside `enterprise_base`):

- `modifyCompanyCapital(buyer, -V_goods, TSM_CAP_WHOLESALE)` -> adds `+V_goods` to `buyer.monthlyWholesaleCosts` and `buyer.periodWholesaleCosts` (deducted from profit before corporate tax!).
- `modifyCompanyCapital(seller, +V_goods, TSM_CAP_PRODUCTION)` -> adds `+V_goods` to `seller.monthlyGrossSales` and `seller.periodGrossSales`.
- `modifyCompanyCapital(buyer, -V_freight, TSM_CAP_WHOLESALE)` -> adds `+V_freight` to `buyer.monthlyWholesaleCosts` and `buyer.periodWholesaleCosts`.
- `modifyCompanyCapital(carrier, +V_freight, TSM_CAP_WHOLESALE)` -> adds `+V_freight` to `carrier.monthlyGrossSales` and `carrier.periodGrossSales`.

Corporate profit tax (`taxes.cpp`) automatically taxes `periodGrossSales - periodWages - periodWholesaleCosts - periodFuelCosts`, so B2B principal and freight are **tax-deductible COGS for the buyer** and **taxable revenue for the seller and carrier** out of the box!

---

## 4. Handling Intermediate Warehouses & Distribution Buffers

A key city-builder edge case in _Workers & Resources_ is **multi-hop logistics**:
`Farm A -> Warehouse W -> Food Factory B` (or `Food Factory B -> Warehouse W -> Shop S`).

There are two cases depending on who owns Warehouse `W`:

### Case 1: `W` is a Private Warehouse / Trading Post (`IsPriv(W) == true`)
- **Hop 1 (`A -> W`):** `W` is a private enterprise (`destBld = W`). `W` **buys** the goods from `A` at delivery (`W` pays `A` $V_{\text{goods}}$ + Carrier $V_{\text{freight}}$). `W` now owns the inventory cleanly on its own balance sheet!
- **Hop 2 (`W -> B`):** When a truck later loads at `W`, `EffectiveSeller(W)` is simply `W`! When delivered to `B`, `B` pays `W` at the live wholesale price.
- **No warehouse provenance table needed** for private warehouses: `W` acts as a genuine merchant wholesaler that absorbs inventory carry risk and earns/loses on price spread.

### Case 2: `W` is a State Warehouse (`isStateStorage` / `IsPriv(W) == false`)
In mixed economies, players frequently use a central State Warehouse (especially with `private_logistics`'s `State Storage` permission widget, v0.17) as a shared rail/road hub between private farms and private factories:
- **Hop 1 (`Private Farm A -> State Warehouse W`):** Under the base matrix (§3.1 row 3), `State` buys from `A` (`-V_goods` from Treasury, `+V_goods` to `A`).
- **Hop 2 (`State Warehouse W -> Private Factory B`):** Under the base matrix (§3.1 row 4), `B` buys from `State` (`-V_goods` from `B`, `+V_goods` to Treasury).
- **Result:** Even through a State hub, **both legs settle automatically without any per-warehouse lot tracking**, and the State only carries working-capital float on goods *explicitly routed into a State-owned storage building* (which the player opted into via the State Storage toggle!).
- **Optional Consignment Mode (`state_warehouse_consignment = 0` default):** If desired later, `State` warehouses with `statePermission == 0` can refuse State-funded purchases from private producers unless `B` pulls directly, which `AutoTargetsOnDay` (v0.17) already enforces via `statePermission`!

---

## 5. Exact Data Structures & Reference Implementation (`private_logistics.cpp`)

All buffers follow `private_logistics.cpp`'s strict **fixed-size static allocation** rule (zero heap allocations on hot hooks, SEH-compatible).

### 5.1 Fixed-Size Tables (`PendingLoad`, `PendingDelivery`, `VehCargo`)

Reuse the vehicle slot index convention from `docs/implementation-plan-per-trip-fuel.md` (`VehSlot(void* veh)` reading `*(uint32_t*)((BYTE*)veh + 0x10)` bounded to `VEH_MAX`, with fallback pointer hash if needed):

```cpp
#define PENDING_RING 32
#define VEH_CARGO_SLOTS 4   // W&R road/rail vehicles carry 1-4 distinct resource slots
#define VEH_TRACK_MAX 16384 // matches engine vehicle pool scale; ~640 KB static BSS

enum OriginKind : unsigned char {
    ORIGIN_UNKNOWN = 0, // pre-existing cargo at save load or unobserved load RVA
    ORIGIN_STATE   = 1, // loaded from a valid non-private building (State depot/farm/border)
    ORIGIN_PRIVATE = 2  // loaded from a private enterprise (`seller` pointer + stable key)
};

struct PendingLoad {
    void*  sourceBld;
    void*  res;
    float  tons;        // positive magnitude
    double t;           // NowSeconds()
    bool   active;
};

struct PendingDelivery {
    void*  destBld;
    void*  fleetOwner;  // p9 from 0x68F792 (backup if vehicle+0x1F8 is null)
    void*  res;
    float  tons;        // positive magnitude
    double t;           // NowSeconds()
    bool   active;
};

struct VehCargoLot {
    void*      res;
    float      tons;
    void*      sellerBld;     // valid when origin == ORIGIN_PRIVATE
    uint64_t   sellerKey;     // building UniqueId/composite key to guard pointer reuse
    OriginKind origin;
};

struct VehCargoState {
    void*       vehPtr;       // guards slot reuse if vehicle is sold/replaced
    VehCargoLot lots[VEH_CARGO_SLOTS];
};

static PendingLoad     g_pendingLoad[PENDING_RING];
static int             g_pendingLoadHead = 0;
static PendingDelivery g_pendingDeliv[PENDING_RING];
static int             g_pendingDelivHead = 0;
static VehCargoState   g_vehCargo[VEH_TRACK_MAX];
```

### 5.2 Load Leg Join (`0x694F40` -> `0x6953C4`)

1. **At `mode == 0 && committed && tons < 0.0f && building` (e.g. `0x694F40`):**
   Under `g_lock`, write `{ building, res, fabsf(tons), NowSeconds(), true }` into `g_pendingLoad[g_pendingLoadHead++ % PENDING_RING]`. *(Do **not** pay Seller here anymore when `counterparty_settlement = 1`! Goods are still on the truck, not sold yet — or see §6.2 if FOB vs. CIF is toggled.)*
2. **At `mode == 1 && committed && tons > 0.0f && p2` (e.g. `0x6953C4`, `p2` is `vehicle`):**
   Under `g_lock`, scan `g_pendingLoad` newest-first for `active && pl.res == res && (now - pl.t) <= 2.0 && fabsf(pl.tons - tons) <= tol`:
   - Mark `pl.active = false` (consumed — prevents a second truck of identical tonnage from matching the same load event!).
   - Stamp `VehCargoState` for `vehicle`:
     - If `IsPrivate(pl.sourceBld)`: `origin = ORIGIN_PRIVATE`, `sellerBld = pl.sourceBld`, `sellerKey = BuildingStableKey(pl.sourceBld)`.
     - Else (`pl.sourceBld != nullptr`): `origin = ORIGIN_STATE`, `sellerBld = nullptr`.
     - Accumulate `tons` into matching `lots[i]` for `res` (if mixing cargo of the same resource from two different sellers on one multi-stop pickup, keep most recent or weighted-first lot; in W&R DO tasks, 99%+ of loads are single-source per resource).

### 5.3 Unload Leg Join (`0x68F792` -> `0x68F7F9`)

1. **At `mode == 0 && committed && tons > 0.0f && building` (e.g. `0x68F792`):**
   Under `g_lock`, write `{ building, p9, res, tons, NowSeconds(), true }` into `g_pendingDeliv[g_pendingDelivHead++ % PENDING_RING]`.
2. **At `mode == 1 && committed && tons < 0.0f && p2` (e.g. `0x68F7F9`, `p2` is `vehicle`):**
   Under `g_lock`, scan `g_pendingDeliv` newest-first for `active && pd.res == res && (now - pd.t) <= 2.0 && fabsf(pd.tons - fabsf(tons)) <= tol`:
   - Mark `pd.active = false`.
   - Pop `fabsf(tons)` from `VehCargoState` for `(vehicle, res)` to obtain `(origin, sellerBld, sellerKey)`.
   - Determine `carrierBld = vehicleHome ? vehicleHome : pd.fleetOwner`.
   - Release `g_lock` and run:
     1. **`SettleDelivery(origin, sellerBld, sellerKey, carrierBld, pd.destBld, res, fabsf(tons))`**
     2. **v0.18 Per-Trip Fuel Deduction** (`ChargeVehicleTripFuel(vehicle, carrierBld, ...)`).

---

## 6. Critical Edge Cases & Money-Conservation Invariants

### 6.1 The Silent-Noop Trap in `modifyCompanyCapital` (Money Conservation!)

Look at `ApplyCapital` in `plugins/enterprise_base/enterprise_base.cpp:1268–1286`:
- `TsmEnterpriseBaseApi::modifyCompanyCapital` returns **`void`**.
- Inside `enterprise_base`, if a building was demolished/decayed (`TsmIsDecayed(b)`), or unregistered and missing `[P]`, `ApplyCapital` **silently returns `0` without moving capital**.
- If `TransferPrivateToPrivate(buyer, seller, amount, ...)` debits `buyer` (succeeds) and credits `seller` (silently dropped because `seller` burned down while the truck was driving!), **`amount` RUB vanishes from the Republic money supply**. Conversely, if `buyer`'s debit silently drops while `seller`'s credit lands, **money is conjured out of thin air**.

**Required Atomic Transfer Helper (`TransferFunds`):**
Mirror the H33 `getTreasuryBalance` before/after verification already proven in `private_logistics.cpp:1058–1075`:

```cpp
// Atomically moves `amount` (> 0) from Payer to Payee.
// Either Payer or Payee (not both) may be NULL to denote State Treasury.
// Guarantees strict conservation of money: if the Payer leg fails or is
// unaffordable, the Payee leg NEVER executes; if the Payee leg fails to
// land on a valid live enterprise, the Payer leg is rolled back immediately.
static bool TransferFunds(void* payerBld, TsmCapitalCategory payerCat,
                          void* payeeBld, TsmCapitalCategory payeeCat,
                          double amount, const char* treasuryLabel)
{
    if (!(amount >= (double)g_minPayment)) return false;

    const bool payerPriv = (payerBld != nullptr);
    const bool payeePriv = (payeeBld != nullptr);
    if (!payerPriv && !payeePriv) return false; // State -> State: no-op

    // 1. Pre-validate live status & affordability BEFORE mutating anything.
    if (payerPriv) {
        if (!IsPrivate(payerBld) || TsmIsDecayed(payerBld)) return false;
        const double have = g_enterprise->getCompanyCapital(payerBld);
        // Optional overdraft policy: see §6.2. Default strict floor >= amount:
        if (!g_allowBuyerOverdraft && have < amount) {
            EnterCriticalSection(&g_lock); g_skipBuyerBroke++; LeaveCriticalSection(&g_lock);
            return false;
        }
    } else {
        const int cur = TsmCurrencyActive(g_currency);
        if (g_currency->getTreasuryBalance(cur) < amount) {
            EnterCriticalSection(&g_lock); g_skipTreasury++; LeaveCriticalSection(&g_lock);
            return false;
        }
    }

    if (payeePriv && (!IsPrivate(payeeBld) || TsmIsDecayed(payeeBld))) {
        // Seller burned down or was nationalized while truck was in transit:
        // route windfall/salvage to State or skip cleanly without debiting buyer.
        return false;
    }

    if (g_dryRun) return true;

    // 2. Execute Payer Debit and verify it landed.
    double payerBefore = 0.0, payerAfter = 0.0;
    if (payerPriv) {
        payerBefore = g_enterprise->getCompanyCapital(payerBld);
        g_enterprise->modifyCompanyCapital(payerBld, -amount, payerCat);
        payerAfter = g_enterprise->getCompanyCapital(payerBld);
        if (fabs((payerBefore - payerAfter) - amount) > 0.05) {
            // Debit refused inside enterprise_base; abort before crediting payee!
            return false;
        }
    } else {
        g_currency->moveTreasury(-amount, treasuryLabel);
    }

    // 3. Execute Payee Credit and verify; roll back Payer if Payee dropped!
    if (payeePriv) {
        const double payeeBefore = g_enterprise->getCompanyCapital(payeeBld);
        g_enterprise->modifyCompanyCapital(payeeBld, amount, payeeCat);
        const double payeeAfter = g_enterprise->getCompanyCapital(payeeBld);
        if (fabs((payeeAfter - payeeBefore) - amount) > 0.05) {
            // Roll back payer debit so zero RUB is destroyed!
            if (payerPriv) g_enterprise->modifyCompanyCapital(payerBld, +amount, payerCat);
            else           g_currency->moveTreasury(+amount, treasuryLabel);
            return false;
        }
    } else {
        g_currency->moveTreasury(+amount, treasuryLabel);
    }
    return true;
}
```

### 6.2 What Happens When a Private Buyer Cannot Afford a Delivery? (`buyer_insolvency_policy`)

In `Workers & Resources`, the physical cargo transfer inside `FUN_140420350` has **already happened** by the time `DetourTransfer` logs `committed == true` (or if we vetoed the engine call, trucks would freeze or loop endlessly at unloading bays).

How should settlement handle a broke private buyer (`getCompanyCapital(buyer) < V_goods + V_freight`)?

Recommend supporting an `.ini` policy (`buyer_overdraft_limit`, default e.g. `-5000.0` RUB, or `allow_buyer_overdraft = 1`):
1. **Why Limited Overdraft Beats Skipping Payment:**
   - If payment is simply skipped when `buyer` has `0 RUB`, the broke shop receives **free goods**, sells them to citizens for retail cash, and the honest supplier (`Seller`) gets stiffed for 100% of its shipment!
   - `enterprise_base` **already has a full insolvency & bankruptcy engine** (`negativeDays`, `insolvencyGraceDays`, automatic bailout/liquidation in `enterprise_base.cpp`) specifically designed for companies whose capital goes negative!
   - Allowing `modifyCompanyCapital(buyer, -amount, TSM_CAP_WHOLESALE)` to push `buyer`'s capital down to `-buyer_overdraft_limit` ensures:
     - **Seller and Carrier get paid in full** (up to the trade-credit floor),
     - **Money is strictly conserved** (`buyer` holds `-X`, `seller` holds `+X`, net sum `= 0.00`), and
     - Chronically unprofitable buyers naturally trigger `enterprise_base` bankruptcy + `AutoTargetsOnDay` (v0.17) can stop dispatching DOs to insolvent buyers (`getCompanyCapital(dest) < 0`) on the very next day!
2. **Payment Priority if Hard Floor Hits:** If `buyer` is already below `-buyer_overdraft_limit`, pay **Freight (`Carrier`) first** up to remaining credit capacity, then **Seller** for whatever partial principal remains, and increment `g_skipBuyerInsolvent` so the daily `[B2B DAY]` summary flags trade-credit defaults explicitly.

### 6.3 Save / Load & Mid-Transit Trucks (`ORIGIN_UNKNOWN`)

When a player loads a save (`TSM_SAVE_LOADING`):
1. Zero `g_pendingLoad`, `g_pendingDeliv`, and `g_vehCargo` under `g_lock` inside `AutoPersistenceEvent` (no new `.bin` save file required in Phase 1!).
2. What about the handful of trucks already mid-highway at the moment the save was written?
   - When those trucks unload 30 seconds after load, their `VehCargoLot` has `origin == ORIGIN_UNKNOWN`.
   - **Safe rule for `ORIGIN_UNKNOWN`:**
     - Freight (`V_freight`) still settles normally (`destBld` -> `carrierBld`), because both `destBld` and `carrierBld` are known at unload!
     - For Goods Principal (`V_goods`): if `IsPriv(destBld)`, debit `destBld` and credit `State` (`"b2b_legacy_transit"`, one-time transition for in-flight trucks on reload) **or** skip the principal leg (`in_flight_reload_policy = state | skip`, default `skip` so State Treasury is untouched). Within 2–3 minutes of gameplay every truck completes its reload cycle and 100% of lots carry exact `ORIGIN_PRIVATE` / `ORIGIN_STATE` provenance.

### 6.4 Direct Factory Connections, Conveyors, Pipes & Forklifts (Non-Vehicle Transfers)

Not all B2B transfers in _Workers & Resources_ use road trucks (`0x694F40` / `0x68F792`):
- **Passive factory crossings / pedestrian-style pulls / conveyors / pipes** either hit one of the currently unobserved `FUN_140420350` sites or bypass vehicle `mode == 1` entirely.
- This is why §7 below specifies a **surgical cutover** with `private_shops` rather than deleting `WholesaleSlot` blindly!

---

## 7. Cutover & Coexistence Protocol With `private_shops`

Currently, `plugins/private_shops/private_shops.cpp` (`RunWholesaleBilling`, lines 900–985) runs a 1 Hz storage-delta scanner across private buildings. If we simply turned on `private_logistics` counterparty settlement without coordinating `private_shops`, every truck delivery to a private shop/factory would be **charged twice** (once at `0x68F7F9` by `private_logistics`, and again up to 1 second later when `private_shops` sees storage rise!).

### 7.1 What `private_shops` Must Keep Doing Even After Cutover

Do **not** disable `private_shops` wholesale scanning wholesale without preserving three non-truck responsibilities embedded inside `RunWholesaleBilling` (`private_shops.cpp:860–975`):

1. **`TsmPricingRecordDemand(g_pricing, s.res, consumed)` (`private_shops.cpp:879`):**
   When a factory consumes input storage in production (`cur < s.lastTons`), `private_shops` records macro market demand in `resource_pricing` v2/v3. **Keep this active** regardless of who settles B2B deliveries, otherwise factory input demand disappears from Republic market multipliers!
2. **`RunServiceCharge` (Pubs / Service buildings) & `RestockSlot` (`foreign_restock_enabled`):**
   Keep untouched.
3. **Conveyor / Pipe Fallback (Optional Phase 2):**
   If two buildings transfer goods via direct factory connection/pipe without firing `0x68F792`, storage deltas occur without a `private_logistics` delivery event.

### 7.2 Clean Two-Stage Rollout Plan (Zero Merge-Conflict Path)

#### Stage 1 — Inside `private_logistics.cpp` Only (Zero touches to `private_shops` or headers)
1. Replace `CarrierNote` (`g_carrier`) with symmetric `PendingLoad` (`0x694F40 -> 0x6953C4`) and `PendingDelivery` (`0x68F792 -> 0x68F7F9`, shared directly with v0.18 Option A per-trip fuel!).
2. Populate `g_vehCargo[VehSlot(veh)]` on `0x6953C4` and consume it on `0x68F7F9`.
3. Add `[b2b]` config keys in `private_logistics.ini`:
   ```ini
   ; 0 = legacy v0.7 State-clearinghouse Pay() model
   ; 1 = Ship 2 vehicle-provenance counterparty settlement (Buyer <-> Seller + Carrier)
   counterparty_settlement = 1
   allow_buyer_overdraft   = 1
   buyer_overdraft_limit   = 5000.0
   ```
4. With `billing = 0, dry_run = 1` (shipped default during testing), log full counterparty dry-run lines at `0x68F7F9`:
   ```
   [B2B SETTLE] dry_run: Buyer "Univermag #2" [P] -> Seller "Food Plant #1" [P] 412.50 RUB (food 4.500t @ 91.67) + Carrier "Private DO #1" [P] 38.20 RUB freight | State delta = 0.00
   ```
   This lets the `private_logistics` engineer verify **live in-game** that Seller, Buyer, Carrier, tonnage, wholesale principal, freight tariff, and per-trip fuel all match 100% of truck deliveries before flipping `dry_run = 0`!

#### Stage 2 — Flipping `billing = 1, dry_run = 0` + `private_shops` Cutover
When ready to go live with real money:
- **Immediate zero-code config test:** Set `wholesale_enabled = 0` in `plugins/private_shops/private_shops.ini` and `billing = 1, dry_run = 0, counterparty_settlement = 1` in `plugins/private_logistics/private_logistics.ini`.
- **Clean code integration in `private_shops.cpp` (~15 lines):**
  Split `wholesale_enabled` in `RunWholesaleBilling` so that when `b2b_logistics_active = 1` (either via `.ini` or a tiny `TsmLogisticsApi` v1 service published by `private_logistics` with `int (*isCounterpartyBillingActive)(void)`):
  - `private_shops` **still runs** `TsmPricingRecordDemand` / `TsmPricingRecordSupply` on storage deltas,
  - `private_shops` **skips** `moveTreasury("procurement")` and `chargeWholesaleDelivery` on truck-serviced buildings so zero double-billing occurs.

---

## 8. Verification & Telemetry Checklist

Before enabling `dry_run = 0` by default, verify these 6 invariants in `tesmiolog.txt`:

1. **Load-to-Vehicle Join Rate (`0x694F40 -> 0x6953C4`):** `matchedLoads / committedMode0Loads >= 99%` across a full simulated month (replaces the broken 0% / false-positive v0.17 `g_carrier` reverse lookup).
2. **Delivery-to-Vehicle Join Rate (`0x68F792 -> 0x68F7F9`):** `matchedDeliveries / committedMode0Deliveries >= 99%` (shared metric with v0.18 per-trip fuel).
3. **Closed-Loop Private Money Conservation:** On a test save with `Private Farm -> Private DO -> Private Food Factory -> Private DO -> Private Shop`, verify over 10 days that:
   $$\Delta \text{StateTreasury}_{\text{B2B}} = 0.00\text{ RUB}$$
   and $\sum \Delta \text{Capital}_{\text{B2B}} = 0.00\text{ RUB}$ (every rouble debited from a buyer equals the exact roubles credited to seller + carrier, within `< 0.01` floating-point tolerance).
4. **Mixed State/Private Symmetry:**
   - `State Farm -> Private DO -> Private Factory`: Factory debited `V_goods + V_freight`; State Treasury credited `+V_goods`; Private DO credited `+V_freight`.
   - `Private Factory -> Private DO -> State Warehouse`: State Treasury debited `-(V_goods + V_freight)`; Private Factory credited `+V_goods`; Private DO credited `+V_freight`.
5. **Demolition / Decay Mid-Transit Safety:** Demolish a private seller building while a loaded truck is en route to a private shop; confirm `TransferFunds` detects `TsmIsDecayed(seller)` and neither debits the buyer nor crashes.
6. **Save/Load Mid-Transit Safety:** Save with 20+ loaded DO trucks on the road, reload, and verify zero crashes, zero negative-tonnage drift in `VehCargoState`, and clean resumption of `ORIGIN_PRIVATE` tracking on the next load cycle.
