#ifndef WRSRE_B2B_SETTLEMENT_H
#define WRSRE_B2B_SETTLEMENT_H

#include <cstddef>
#include <cstdint>
#include <cmath>
#include <cstring>
#include <functional>

namespace wrsre {
namespace b2b {

static constexpr std::size_t PENDING_RING = 32;
static constexpr std::size_t VEH_CARGO_SLOTS = 4;
static constexpr std::size_t VEH_TRACK_MAX = 16384;

enum OriginKind : unsigned char {
    ORIGIN_UNKNOWN = 0,
    ORIGIN_STATE = 1,
    ORIGIN_PRIVATE = 2
};

struct PendingLoad {
    std::uintptr_t sourceBld = 0;
    std::uintptr_t res = 0;
    float tons = 0.0f; // positive magnitude
    double t = 0.0;
    bool active = false;
    bool sourceIsPriv = false;
};

struct PendingDelivery {
    std::uintptr_t destBld = 0;
    std::uintptr_t fleetOwner = 0;
    std::uintptr_t res = 0;
    float tons = 0.0f; // positive magnitude
    double t = 0.0;
    bool active = false;
};

struct VehCargoLot {
    std::uintptr_t res = 0;
    float tons = 0.0f;
    std::uintptr_t sellerBld = 0;
    std::uint64_t sellerKey = 0;
    OriginKind origin = ORIGIN_UNKNOWN;
};

struct VehCargoState {
    std::uintptr_t vehPtr = 0;
    VehCargoLot lots[VEH_CARGO_SLOTS];
};

struct MatchedDelivery {
    std::uintptr_t destBld = 0;
    std::uintptr_t fleetOwner = 0;
    std::uintptr_t res = 0;
    float tons = 0.0f;
    double t = 0.0;
    std::uintptr_t vehicle = 0;
    std::uint32_t vehSlotIdx = 0;
    std::uintptr_t vehicleHome = 0;
    std::uintptr_t carrierBld = 0;
    OriginKind origin = ORIGIN_UNKNOWN;
    std::uintptr_t sellerBld = 0;
    std::uint64_t sellerKey = 0;
    bool deliveryMatched = false;
    bool cargoFound = false;
};

struct SettlementLeg {
    void* payer = nullptr; // nullptr => State Treasury
    int payerCat = 0;
    void* payee = nullptr; // nullptr => State Treasury
    int payeeCat = 0;
    double amount = 0.0;
    const char* treasuryLabel = nullptr;
    bool isGoods = false;
    bool isFreight = false;
};

struct SettlementPlan {
    OriginKind origin = ORIGIN_UNKNOWN;
    std::uintptr_t sellerBld = 0;
    std::uint64_t sellerKey = 0;
    std::uintptr_t carrierBld = 0;
    std::uintptr_t buyerBld = 0;
    double goodsValue = 0.0;
    double freightValue = 0.0;
    bool internalGoods = false;
    bool selfHaul = false;
    bool chargeStateCarrier = true;
    int inFlightReloadPolicy = 0;
    bool allowBuyerOverdraft = false;
    double buyerOverdraftLimit = 0.0;

    bool hasGoodsLeg = false;
    SettlementLeg goodsLeg;
    bool hasFreightLeg = false;
    SettlementLeg freightLeg;

    double stateDelta = 0.0;
};

struct FundsTransferCallbacks {
    std::function<bool(void*)> isLivePrivate;
    std::function<double(void*)> getCapital;
    std::function<void(void*, double, int)> modifyCapital;
    std::function<double(int)> getTreasury;
    std::function<void(double, const char*)> moveTreasury;
    std::function<bool(void*, std::uint64_t)> checkSellerKey;
    bool allowBuyerOverdraft = false;
    double buyerOverdraftLimit = 0.0;
    double minPayment = 0.01;
    bool dryRun = false;
    unsigned* skipBuyerInsolvent = nullptr;
    unsigned* skipTreasury = nullptr;
};

class Engine {
public:
    Engine() { resetAll(); }

    void resetAll() {
        for (std::size_t i = 0; i < PENDING_RING; ++i) {
            pendingLoad[i] = PendingLoad{};
            pendingDeliv[i] = PendingDelivery{};
        }
        pendingLoadHead = 0;
        pendingDelivHead = 0;
        for (std::size_t i = 0; i < VEH_TRACK_MAX; ++i) {
            vehCargo[i].vehPtr = 0;
            for (std::size_t j = 0; j < VEH_CARGO_SLOTS; ++j) {
                vehCargo[i].lots[j] = VehCargoLot{};
            }
        }
    }

    static std::uint32_t vehSlot(std::uintptr_t vehicle) {
        if (!vehicle) return 0;
        std::uint32_t idx = 0;
        std::memcpy(&idx, (const char*)(void*)vehicle + 0x10, sizeof(idx));
        if (idx < VEH_TRACK_MAX) return idx;
        // pointer-hash fallback
        std::uintptr_t h = (std::uintptr_t)vehicle;
        h ^= h >> 33;
        h *= 0xff51afd7ed558ccdULL;
        h ^= h >> 33;
        return (std::uint32_t)(h % VEH_TRACK_MAX);
    }

    void recordMode0Load(std::uintptr_t sourceBld, std::uintptr_t res, float absTons, double nowSec, bool sourceIsPriv = false) {
        PendingLoad& e = pendingLoad[pendingLoadHead % PENDING_RING];
        e.sourceBld = sourceBld;
        e.res = res;
        e.tons = (absTons < 0.0f) ? -absTons : absTons;
        e.t = nowSec;
        e.active = true;
        e.sourceIsPriv = sourceIsPriv;
        pendingLoadHead = (pendingLoadHead + 1) % (int)PENDING_RING;
    }

    bool matchMode1Load(std::uintptr_t vehicle, std::uint32_t vehSlotIdx, std::uintptr_t res,
                        float tons, double nowSec, bool sourceIsPriv, std::uint64_t sellerKey) {
        if (vehSlotIdx >= VEH_TRACK_MAX) vehSlotIdx = vehSlot(vehicle);
        const float mag = (tons < 0.0f) ? -tons : tons;
        // scan newest-first
        int start = (int)(pendingLoadHead == 0 ? PENDING_RING - 1 : pendingLoadHead - 1);
        // pendingLoadHead points next slot; newest is head-1
        for (int i = 0; i < (int)PENDING_RING; ++i) {
            int k = (start - i);
            while (k < 0) k += (int)PENDING_RING;
            k %= (int)PENDING_RING;
            PendingLoad& pl = pendingLoad[k];
            if (!pl.active) continue;
            if (pl.res != res) continue;
            if (nowSec - pl.t > 2.0) continue;
            const float tol = (mag > 0.01f) ? mag * 0.005f : 0.0005f;
            if (std::fabs(pl.tons - mag) > tol) continue;
            // match
            pl.active = false;
            // stamp VehCargoState
            VehCargoState& vcs = vehCargo[vehSlotIdx];
            if (vcs.vehPtr != vehicle) {
                vcs.vehPtr = vehicle;
                for (std::size_t j = 0; j < VEH_CARGO_SLOTS; ++j) vcs.lots[j] = VehCargoLot{};
            } else {
                vcs.vehPtr = vehicle;
            }
            // find lot
            int slot = -1;
            for (std::size_t j = 0; j < VEH_CARGO_SLOTS; ++j) {
                if (vcs.lots[j].res == res) { slot = (int)j; break; }
            }
            if (slot < 0) {
                for (std::size_t j = 0; j < VEH_CARGO_SLOTS; ++j) {
                    if (vcs.lots[j].res == 0 && vcs.lots[j].tons == 0.0f) { slot = (int)j; break; }
                }
            }
            if (slot < 0) slot = 0;
            VehCargoLot& lot = vcs.lots[slot];
            if (lot.res != 0 && lot.res != res) {
                // overwrite
                lot = VehCargoLot{};
            }
            if (lot.res == 0) lot.res = res;
            lot.tons += mag;
            if (sourceIsPriv || pl.sourceIsPriv) {
                lot.origin = ORIGIN_PRIVATE;
                lot.sellerBld = pl.sourceBld;
                lot.sellerKey = sellerKey ? sellerKey : pl.sourceBld;
            } else {
                if (pl.sourceBld != 0) {
                    lot.origin = ORIGIN_STATE;
                    lot.sellerBld = 0;
                    lot.sellerKey = 0;
                } else {
                    lot.origin = ORIGIN_UNKNOWN;
                    lot.sellerBld = 0;
                    lot.sellerKey = 0;
                }
            }
            (void)vehicle;
            return true;
        }
        return false;
    }

    void recordMode0Delivery(std::uintptr_t destBld, std::uintptr_t fleetOwnerP9, std::uintptr_t res, float tons, double nowSec) {
        PendingDelivery& e = pendingDeliv[pendingDelivHead % PENDING_RING];
        e.destBld = destBld;
        e.fleetOwner = fleetOwnerP9;
        e.res = res;
        e.tons = (tons < 0.0f) ? -tons : tons;
        e.t = nowSec;
        e.active = true;
        pendingDelivHead = (pendingDelivHead + 1) % (int)PENDING_RING;
    }

    bool matchMode1Delivery(std::uintptr_t vehicle, std::uint32_t vehSlotIdx, std::uintptr_t vehicleHome,
                            std::uintptr_t res, float absTons, double nowSec, MatchedDelivery* out) {
        if (!out) return false;
        if (vehSlotIdx >= VEH_TRACK_MAX) vehSlotIdx = vehSlot(vehicle);
        *out = MatchedDelivery{};
        out->vehicle = vehicle;
        out->vehSlotIdx = vehSlotIdx;
        out->res = res;
        out->vehicleHome = vehicleHome;

        const float mag = (absTons < 0.0f) ? -absTons : absTons;

        // scan pending delivery newest-first
        int start = (pendingDelivHead == 0) ? (int)PENDING_RING - 1 : (int)pendingDelivHead - 1;
        bool deliveryMatched = false;
        PendingDelivery pd{};
        for (int i = 0; i < (int)PENDING_RING; ++i) {
            int k = start - i;
            while (k < 0) k += (int)PENDING_RING;
            k %= (int)PENDING_RING;
            PendingDelivery& cand = pendingDeliv[k];
            if (!cand.active) continue;
            if (cand.res != res) continue;
            if (nowSec - cand.t > 2.0) continue;
            const float tol = (mag > 0.01f) ? mag * 0.005f : 0.0005f;
            if (std::fabs(cand.tons - mag) > tol) continue;
            cand.active = false;
            pd = cand;
            deliveryMatched = true;
            break;
        }

        out->deliveryMatched = deliveryMatched;
        if (deliveryMatched) {
            out->destBld = pd.destBld;
            out->fleetOwner = pd.fleetOwner;
            out->tons = mag;
            out->t = pd.t;
        }

        std::uintptr_t carrierBld = vehicleHome ? vehicleHome : (deliveryMatched ? pd.fleetOwner : 0);
        out->carrierBld = carrierBld;

        // pop cargo from veh state
        VehCargoState& vcs = vehCargo[vehSlotIdx];
        if (vcs.vehPtr != 0 && vcs.vehPtr != vehicle) {
            // slot reuse guard: reset
            vcs.vehPtr = vehicle;
            for (std::size_t j = 0; j < VEH_CARGO_SLOTS; ++j) vcs.lots[j] = VehCargoLot{};
        } else {
            vcs.vehPtr = vehicle;
        }

        OriginKind origin = ORIGIN_UNKNOWN;
        std::uintptr_t sellerBld = 0;
        std::uint64_t sellerKey = 0;
        float remaining = mag;
        for (std::size_t j = 0; j < VEH_CARGO_SLOTS; ++j) {
            VehCargoLot& lot = vcs.lots[j];
            if (lot.res != res || lot.tons <= 0.0f) continue;
            origin = lot.origin;
            sellerBld = lot.sellerBld;
            sellerKey = lot.sellerKey;
            float take = (lot.tons < remaining) ? lot.tons : remaining;
            lot.tons -= take;
            remaining -= take;
            if (lot.tons <= 0.000001f) {
                lot = VehCargoLot{};
            }
            if (remaining <= 0.000001f) break;
        }
        if (remaining > 0.000001f) {
            // no matching cargo lot
            origin = ORIGIN_UNKNOWN;
            sellerBld = 0;
            sellerKey = 0;
        } else {
            // cargo found if we deducted all
        }
        bool cargoFound = (remaining <= 0.000001f) && (mag > 0.0f);
        // if no lot existed, remaining==mag>0 => not found
        if (mag <= 0.0f) cargoFound = false;
        if (remaining >= mag - 0.000001f && mag > 0.0f) {
            // didn't pop anything
            cargoFound = false;
            origin = ORIGIN_UNKNOWN;
            sellerBld = 0;
            sellerKey = 0;
        }

        out->origin = origin;
        out->sellerBld = sellerBld;
        out->sellerKey = sellerKey;
        out->cargoFound = cargoFound;
        if (!deliveryMatched) {
            out->origin = ORIGIN_UNKNOWN;
        }
        return deliveryMatched;
    }

    // pending ring access for tests
    PendingLoad* pendingLoadPtr() { return pendingLoad; }
    PendingDelivery* pendingDelivPtr() { return pendingDeliv; }
    VehCargoState* vehCargoPtr() { return vehCargo; }

private:
    PendingLoad pendingLoad[PENDING_RING];
    int pendingLoadHead = 0;
    PendingDelivery pendingDeliv[PENDING_RING];
    int pendingDelivHead = 0;
    static VehCargoState vehCargo[VEH_TRACK_MAX];
};

// static member definition
inline VehCargoState Engine::vehCargo[VEH_TRACK_MAX] = {};

// default engine
inline Engine& defaultEngine() {
    static Engine e;
    return e;
}

// free functions delegating to default engine
inline void resetAll() { defaultEngine().resetAll(); }
inline void recordMode0Load(std::uintptr_t sourceBld, std::uintptr_t res, float absTons, double nowSec, bool sourceIsPriv = false) {
    defaultEngine().recordMode0Load(sourceBld, res, absTons, nowSec, sourceIsPriv);
}
inline bool matchMode1Load(std::uintptr_t vehicle, std::uint32_t vehSlotIdx, std::uintptr_t res, float tons, double nowSec, bool sourceIsPriv, std::uint64_t sellerKey) {
    return defaultEngine().matchMode1Load(vehicle, vehSlotIdx, res, tons, nowSec, sourceIsPriv, sellerKey);
}
inline void recordMode0Delivery(std::uintptr_t destBld, std::uintptr_t fleetOwnerP9, std::uintptr_t res, float tons, double nowSec) {
    defaultEngine().recordMode0Delivery(destBld, fleetOwnerP9, res, tons, nowSec);
}
inline bool matchMode1Delivery(std::uintptr_t vehicle, std::uint32_t vehSlotIdx, std::uintptr_t vehicleHome, std::uintptr_t res, float absTons, double nowSec, MatchedDelivery* out) {
    return defaultEngine().matchMode1Delivery(vehicle, vehSlotIdx, vehicleHome, res, absTons, nowSec, out);
}

// Plan settlement
inline SettlementPlan planSettlement(
    OriginKind origin,
    std::uintptr_t sellerBld, std::uint64_t sellerKey,
    std::uintptr_t carrierBld,
    std::uintptr_t buyerBld,
    double goodsValue, double freightValue,
    bool allowBuyerOverdraft, double buyerOverdraftLimit,
    bool chargeStateCarrier, int inFlightReloadPolicy) {

    SettlementPlan plan;
    plan.origin = origin;
    plan.sellerBld = sellerBld;
    plan.sellerKey = sellerKey;
    plan.carrierBld = carrierBld;
    plan.buyerBld = buyerBld;
    plan.goodsValue = goodsValue;
    plan.freightValue = freightValue;
    plan.allowBuyerOverdraft = allowBuyerOverdraft;
    plan.buyerOverdraftLimit = buyerOverdraftLimit;
    plan.chargeStateCarrier = chargeStateCarrier;
    plan.inFlightReloadPolicy = inFlightReloadPolicy;

    const bool sellerPriv = (sellerBld != 0);
    const bool buyerPriv = (buyerBld != 0);
    const bool carrierPriv = (carrierBld != 0);

    // Goods principal leg
    if (origin == ORIGIN_PRIVATE) {
        if (sellerBld != 0 && sellerBld == buyerBld) {
            plan.internalGoods = true;
            plan.hasGoodsLeg = false;
        } else if (sellerPriv && buyerPriv) {
            // Private A -> Private B
            plan.hasGoodsLeg = true;
            plan.goodsLeg.payer = (void*)buyerBld;
            plan.goodsLeg.payerCat = 1; // TSM_CAP_WHOLESALE
            plan.goodsLeg.payee = (void*)sellerBld;
            plan.goodsLeg.payeeCat = 9; // TSM_CAP_PRODUCTION
            plan.goodsLeg.amount = goodsValue;
            plan.goodsLeg.treasuryLabel = nullptr;
            plan.goodsLeg.isGoods = true;
        } else if (sellerPriv && !buyerPriv) {
            // Private A -> State: debit State, credit Seller
            plan.hasGoodsLeg = true;
            plan.goodsLeg.payer = nullptr;
            plan.goodsLeg.payerCat = 0;
            plan.goodsLeg.payee = (void*)sellerBld;
            plan.goodsLeg.payeeCat = 9;
            plan.goodsLeg.amount = goodsValue;
            plan.goodsLeg.treasuryLabel = "b2b_state_buy";
            plan.goodsLeg.isGoods = true;
        } else {
            // seller private but buyer? already; seller state? no
            plan.hasGoodsLeg = false;
        }
    } else if (origin == ORIGIN_STATE) {
        if (buyerPriv) {
            // State -> Private B: debit Buyer, credit State
            plan.hasGoodsLeg = true;
            plan.goodsLeg.payer = (void*)buyerBld;
            plan.goodsLeg.payerCat = 1;
            plan.goodsLeg.payee = nullptr;
            plan.goodsLeg.payeeCat = 0;
            plan.goodsLeg.amount = goodsValue;
            plan.goodsLeg.treasuryLabel = "b2b_state_sell";
            plan.goodsLeg.isGoods = true;
        } else {
            // State -> State: none
            plan.hasGoodsLeg = false;
        }
    } else { // ORIGIN_UNKNOWN
        if (inFlightReloadPolicy == 1 && buyerPriv) {
            plan.hasGoodsLeg = true;
            plan.goodsLeg.payer = (void*)buyerBld;
            plan.goodsLeg.payerCat = 1;
            plan.goodsLeg.payee = nullptr;
            plan.goodsLeg.payeeCat = 0;
            plan.goodsLeg.amount = goodsValue;
            plan.goodsLeg.treasuryLabel = "b2b_legacy_transit";
            plan.goodsLeg.isGoods = true;
        } else {
            plan.hasGoodsLeg = false; // skip
        }
    }

    // Freight leg
    plan.selfHaul = (carrierBld != 0 && carrierBld == buyerBld);
    if (carrierBld == 0 || carrierBld == buyerBld) {
        plan.hasFreightLeg = false;
    } else {
        if (carrierPriv && buyerPriv) {
            plan.hasFreightLeg = true;
            plan.freightLeg.payer = (void*)buyerBld;
            plan.freightLeg.payerCat = 1;
            plan.freightLeg.payee = (void*)carrierBld;
            plan.freightLeg.payeeCat = 1;
            plan.freightLeg.amount = freightValue;
            plan.freightLeg.treasuryLabel = nullptr;
            plan.freightLeg.isFreight = true;
        } else if (carrierPriv && !buyerPriv) {
            // Private C -> State
            plan.hasFreightLeg = true;
            plan.freightLeg.payer = nullptr;
            plan.freightLeg.payerCat = 0;
            plan.freightLeg.payee = (void*)carrierBld;
            plan.freightLeg.payeeCat = 1;
            plan.freightLeg.amount = freightValue;
            plan.freightLeg.treasuryLabel = "b2b_freight";
            plan.freightLeg.isFreight = true;
        } else if (!carrierPriv && buyerPriv) {
            // State carrier -> Private B
            if (chargeStateCarrier) {
                plan.hasFreightLeg = true;
                plan.freightLeg.payer = (void*)buyerBld;
                plan.freightLeg.payerCat = 1;
                plan.freightLeg.payee = nullptr;
                plan.freightLeg.payeeCat = 0;
                plan.freightLeg.amount = freightValue;
                plan.freightLeg.treasuryLabel = "b2b_state_freight";
                plan.freightLeg.isFreight = true;
            } else {
                plan.hasFreightLeg = false;
            }
        } else {
            // State -> State: none
            plan.hasFreightLeg = false;
        }
    }

    return plan;
}

inline bool TransferFunds(const FundsTransferCallbacks& cb,
                          void* payerBld, int payerCat,
                          void* payeeBld, int payeeCat,
                          double amount, const char* treasuryLabel,
                          std::uint64_t sellerKey = 0, void* sellerForCheck = nullptr) {
    if (!(amount >= (double)cb.minPayment)) return false;
    const bool payerPriv = (payerBld != nullptr);
    const bool payeePriv = (payeeBld != nullptr);
    if (!payerPriv && !payeePriv) return false;

    // pre-validate
    if (payerPriv) {
        if (!cb.isLivePrivate || !cb.isLivePrivate(payerBld)) return false;
        if (cb.getCapital) {
            double have = cb.getCapital(payerBld);
            if (!cb.allowBuyerOverdraft) {
                if (have < amount) {
                    if (cb.skipBuyerInsolvent) (*cb.skipBuyerInsolvent)++;
                    return false;
                }
            } else {
                if (have + cb.buyerOverdraftLimit < amount) {
                    if (cb.skipBuyerInsolvent) (*cb.skipBuyerInsolvent)++;
                    return false;
                }
            }
        }
    } else {
        if (cb.getTreasury) {
            double bal = cb.getTreasury(0);
            if (bal < amount) {
                if (cb.skipTreasury) (*cb.skipTreasury)++;
                return false;
            }
        }
    }
    if (payeePriv) {
        if (!cb.isLivePrivate || !cb.isLivePrivate(payeeBld)) return false;
        if (sellerKey != 0 && cb.checkSellerKey) {
            if (!cb.checkSellerKey(payeeBld, sellerKey)) return false;
        }
        (void)sellerForCheck;
    }

    if (cb.dryRun) return true;

    // execute payer debit
    if (payerPriv) {
        double before = cb.getCapital ? cb.getCapital(payerBld) : 0.0;
        if (cb.modifyCapital) cb.modifyCapital(payerBld, -amount, payerCat);
        double after = cb.getCapital ? cb.getCapital(payerBld) : 0.0;
        if (std::fabs((before - after) - amount) > 0.05) {
            return false;
        }
    } else {
        if (cb.moveTreasury) cb.moveTreasury(-amount, treasuryLabel ? treasuryLabel : "b2b");
    }

    // payee credit
    if (payeePriv) {
        double payeeBefore = cb.getCapital ? cb.getCapital(payeeBld) : 0.0;
        if (cb.modifyCapital) cb.modifyCapital(payeeBld, amount, payeeCat);
        double payeeAfter = cb.getCapital ? cb.getCapital(payeeBld) : 0.0;
        if (std::fabs((payeeAfter - payeeBefore) - amount) > 0.05) {
            // rollback payer
            if (payerPriv) {
                if (cb.modifyCapital) cb.modifyCapital(payerBld, +amount, payerCat);
            } else {
                if (cb.moveTreasury) cb.moveTreasury(+amount, treasuryLabel ? treasuryLabel : "b2b");
            }
            return false;
        }
    } else {
        if (cb.moveTreasury) cb.moveTreasury(+amount, treasuryLabel ? treasuryLabel : "b2b");
    }
    return true;
}

// Execute atomic transfer for a plan with freight-first priority and overdraft handling.
inline bool executeAtomicTransfer(const SettlementPlan& plan, const FundsTransferCallbacks& cb,
                                 unsigned* skipBuyerInsolventOut = nullptr) {
    bool ok = true;
    unsigned skipLocal = 0;

    // Freight first
    if (plan.hasFreightLeg) {
        double amt = plan.freightLeg.amount;
        void* buyer = (void*)plan.buyerBld;
        if (buyer && plan.allowBuyerOverdraft && cb.getCapital) {
            double have = cb.getCapital(buyer);
            double cap = have + plan.buyerOverdraftLimit;
            if (cap < amt) amt = (cap >= cb.minPayment) ? cap : 0.0;
        }
        if (amt >= cb.minPayment) {
            bool r = TransferFunds(cb, plan.freightLeg.payer, plan.freightLeg.payerCat,
                                   plan.freightLeg.payee, plan.freightLeg.payeeCat,
                                   amt, plan.freightLeg.treasuryLabel);
            if (!r) { ok = false; skipLocal++; }
        } else {
            if (skipBuyerInsolventOut) (*skipBuyerInsolventOut)++;
            skipLocal++;
        }
    }

    // Goods
    if (plan.hasGoodsLeg) {
        double amt = plan.goodsLeg.amount;
        void* buyer = (void*)plan.buyerBld;
        // buyer pays goods if payer == buyer
        bool buyerPays = (plan.goodsLeg.payer == buyer) && (buyer != nullptr);
        if (buyerPays && plan.allowBuyerOverdraft && cb.getCapital) {
            double have = cb.getCapital(buyer);
            double cap = have + plan.buyerOverdraftLimit;
            if (cap < amt) amt = (cap >= cb.minPayment) ? cap : 0.0;
        }
        if (amt >= cb.minPayment) {
            bool r = TransferFunds(cb, plan.goodsLeg.payer, plan.goodsLeg.payerCat,
                                   plan.goodsLeg.payee, plan.goodsLeg.payeeCat,
                                   amt, plan.goodsLeg.treasuryLabel,
                                   plan.sellerKey, (void*)plan.sellerBld);
            if (!r) { ok = false; skipLocal++; }
        } else {
            if (skipBuyerInsolventOut) (*skipBuyerInsolventOut)++;
            skipLocal++;
        }
    }

    if (skipBuyerInsolventOut) *skipBuyerInsolventOut += skipLocal;
    return ok;
}

} // namespace b2b
} // namespace wrsre

#endif
