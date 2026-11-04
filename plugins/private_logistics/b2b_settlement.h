#ifndef WRSRE_B2B_SETTLEMENT_H
#define WRSRE_B2B_SETTLEMENT_H

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace wrsre {
namespace b2b {

static constexpr std::size_t PENDING_RING = 32;
static constexpr std::size_t VEH_CARGO_SLOTS = 4;
static constexpr std::size_t VEH_TRACK_MAX = 16384;

// At this verified native call site, mode 1 / negative tons is the vehicle
// half of the delivery recorded at 0x68F792.  Its p9 is the same fleet owner
// as the vehicle home on ordinary deliveries, so p9 == vehicleHome is NOT
// evidence of depot staging here.  The Windows detour supplies the native
// RVA; this pure predicate lets the routing rule be tested without Windows.
static constexpr unsigned DELIVERY_VEHICLE_SITE_RVA = 0x0068F7F9u;

inline bool ShouldTrackMode1Cargo(bool committed, unsigned mode,
                                  bool hasVehicle, bool isStaging,
                                  bool isRefuel, unsigned siteRva) {
    if (!committed || mode == 0 || !hasVehicle || isRefuel) return false;
    return !isStaging || siteRva == DELIVERY_VEHICLE_SITE_RVA;
}

// Matches TsmCapitalCategory in src/enterprise_base_api.h without pulling in
// platform headers so unit tests compile anywhere with -std=c++17.
static constexpr int CAP_CAT_WHOLESALE  = 1;
static constexpr int CAP_CAT_PRODUCTION = 9;

enum OriginKind : unsigned char {
    ORIGIN_UNKNOWN = 0,
    ORIGIN_STATE   = 1,
    ORIGIN_PRIVATE = 2
};

struct PendingLoad {
    std::uintptr_t sourceBld    = 0;
    std::uintptr_t res          = 0;
    float          tons         = 0.0f; // positive magnitude
    double         t            = 0.0;
    std::uint64_t  sellerKey    = 0;
    bool           active       = false;
    bool           sourceIsPriv = false;
};

struct PendingDelivery {
    std::uintptr_t destBld    = 0;
    std::uintptr_t fleetOwner = 0;
    std::uintptr_t res        = 0;
    float          tons       = 0.0f; // positive magnitude
    double         t          = 0.0;
    bool           active     = false;
};

struct VehCargoLot {
    std::uintptr_t res       = 0;
    float          tons      = 0.0f;
    std::uintptr_t sellerBld = 0;
    std::uint64_t  sellerKey = 0;
    OriginKind     origin    = ORIGIN_UNKNOWN;
};

struct VehCargoState {
    std::uintptr_t vehPtr = 0;
    VehCargoLot    lots[VEH_CARGO_SLOTS];
};

struct MatchedDelivery {
    std::uintptr_t destBld         = 0;
    std::uintptr_t fleetOwner      = 0;
    std::uintptr_t res             = 0;
    float          tons            = 0.0f;
    double         t               = 0.0;
    std::uintptr_t vehicle         = 0;
    std::uint32_t  vehSlotIdx      = 0;
    std::uintptr_t vehicleHome     = 0;
    std::uintptr_t carrierBld      = 0;
    OriginKind     origin          = ORIGIN_UNKNOWN;
    std::uintptr_t sellerBld       = 0;
    std::uint64_t  sellerKey       = 0;
    bool           deliveryMatched = false;
    bool           cargoFound      = false;
};

struct SettlementLeg {
    void*       payer         = nullptr; // nullptr => State Treasury
    int         payerCat      = 0;
    void*       payee         = nullptr; // nullptr => State Treasury
    int         payeeCat      = 0;
    double      amount        = 0.0;
    const char* treasuryLabel = nullptr;
    bool        isGoods       = false;
    bool        isFreight     = false;
};

struct SettlementPlan {
    OriginKind     origin              = ORIGIN_UNKNOWN;
    std::uintptr_t sellerBld           = 0;
    std::uint64_t  sellerKey           = 0;
    std::uintptr_t carrierBld          = 0;
    std::uintptr_t buyerBld            = 0;
    bool           sellerIsPriv        = false;
    bool           carrierIsPriv       = false;
    bool           buyerIsPriv         = false;
    double         goodsValue          = 0.0;
    double         freightValue        = 0.0;
    bool           internalGoods       = false;
    bool           selfHaul            = false;
    bool           chargeStateCarrier  = true;
    int            inFlightReloadPolicy = 0;
    bool           allowBuyerOverdraft = false;
    double         buyerOverdraftLimit = 0.0;

    bool          hasGoodsLeg   = false;
    SettlementLeg goodsLeg;
    bool          hasFreightLeg = false;
    SettlementLeg freightLeg;

    double stateDelta = 0.0;
};

// Pure POD callback table (zero heap allocations, trivially destructible so
// MSVC /EHsc + SEH __try/__except functions never see C++ unwind objects).
struct FundsTransferCallbacks {
    void*  userData = nullptr;
    bool   (*isLivePrivate)(void* userData, void* bld) = nullptr;
    double (*getCapital)(void* userData, void* bld) = nullptr;
    void   (*modifyCapital)(void* userData, void* bld, double delta, int cat) = nullptr;
    double (*getTreasury)(void* userData, int currency) = nullptr;
    void   (*moveTreasury)(void* userData, double delta, const char* label) = nullptr;
    bool   (*checkSellerKey)(void* userData, void* bld, std::uint64_t key) = nullptr;

    bool      allowBuyerOverdraft = false;
    double    buyerOverdraftLimit = 0.0;
    double    minPayment          = 0.01;
    bool      dryRun              = false;
    unsigned* skipBuyerInsolvent  = nullptr;
    unsigned* skipTreasury        = nullptr;
};

struct TransferExecutionSummary {
    bool   ok          = true;
    double goodsPaid   = 0.0;
    double freightPaid = 0.0;
    double stateDelta  = 0.0;
};

static inline float TonnageTolerance(float mag) {
    return (mag > 0.01f) ? (mag * 0.005f) : 0.0005f;
}

class Engine {
public:
    void resetAll() {
        for (std::size_t i = 0; i < PENDING_RING; ++i) {
            pendingLoad_[i]  = PendingLoad{};
            pendingDeliv_[i] = PendingDelivery{};
        }
        for (std::size_t i = 0; i < VEH_TRACK_MAX; ++i) {
            vehCargo_[i] = VehCargoState{};
        }
        pendingLoadHead_ = 0;
        pendingDelivHead_ = 0;
    }

    static std::uint32_t hashVehiclePtr(std::uintptr_t vehicle) {
        std::uintptr_t h = vehicle;
        h ^= h >> 33;
        h *= 0xff51afd7ed558ccdULL;
        h ^= h >> 33;
        return static_cast<std::uint32_t>(h % VEH_TRACK_MAX);
    }

    void recordMode0Load(std::uintptr_t sourceBld, std::uintptr_t res,
                         float absTons, double nowSec,
                         bool sourceIsPriv = false,
                         std::uint64_t sellerKey = 0) {
        PendingLoad& e = pendingLoad_[pendingLoadHead_ % PENDING_RING];
        e.sourceBld    = sourceBld;
        e.res          = res;
        e.tons         = (absTons < 0.0f) ? -absTons : absTons;
        e.t            = nowSec;
        e.sellerKey    = sellerKey;
        e.active       = true;
        e.sourceIsPriv = sourceIsPriv;
        pendingLoadHead_ = (pendingLoadHead_ + 1) % static_cast<int>(PENDING_RING);
    }

    bool matchMode1Load(std::uintptr_t vehicle, std::uint32_t vehSlotIdx,
                        std::uintptr_t res, float tons, double nowSec,
                        bool sourceIsPriv = false,
                        std::uint64_t sellerKey = 0) {
        if (!vehicle) return false;
        if (vehSlotIdx >= VEH_TRACK_MAX) vehSlotIdx = hashVehiclePtr(vehicle);
        const float mag = (tons < 0.0f) ? -tons : tons;
        const float tol = TonnageTolerance(mag);

        const int start = (pendingLoadHead_ == 0)
                        ? static_cast<int>(PENDING_RING) - 1
                        : pendingLoadHead_ - 1;
        for (int i = 0; i < static_cast<int>(PENDING_RING); ++i) {
            int k = start - i;
            while (k < 0) k += static_cast<int>(PENDING_RING);
            k %= static_cast<int>(PENDING_RING);

            PendingLoad& pl = pendingLoad_[k];
            if (!pl.active) continue;
            if (pl.res != res) continue;
            const double dt = nowSec - pl.t;
            if (dt < -0.001 || dt > 2.0) continue;
            if (std::fabs(pl.tons - mag) > tol) continue;

            pl.active = false;

            VehCargoState& vcs = vehCargo_[vehSlotIdx];
            if (vcs.vehPtr != vehicle) {
                vcs.vehPtr = vehicle;
                for (std::size_t j = 0; j < VEH_CARGO_SLOTS; ++j)
                    vcs.lots[j] = VehCargoLot{};
            }

            const bool isPriv = sourceIsPriv || pl.sourceIsPriv;
            const std::uint64_t effKey = sellerKey ? sellerKey
                                       : (pl.sellerKey ? pl.sellerKey : pl.sourceBld);

            int slot = -1;
            for (std::size_t j = 0; j < VEH_CARGO_SLOTS; ++j) {
                if (vcs.lots[j].res == res) { slot = static_cast<int>(j); break; }
            }
            if (slot < 0) {
                for (std::size_t j = 0; j < VEH_CARGO_SLOTS; ++j) {
                    if (vcs.lots[j].res == 0 && vcs.lots[j].tons <= 0.0f) {
                        slot = static_cast<int>(j);
                        break;
                    }
                }
            }
            if (slot < 0) slot = 0;

            VehCargoLot& lot = vcs.lots[slot];
            if (lot.res != 0 && lot.res != res) {
                lot = VehCargoLot{};
            }
            lot.res = res;
            lot.tons += mag;
            if (isPriv && pl.sourceBld != 0) {
                lot.origin    = ORIGIN_PRIVATE;
                lot.sellerBld = pl.sourceBld;
                lot.sellerKey = effKey;
            } else if (pl.sourceBld != 0) {
                lot.origin    = ORIGIN_STATE;
                lot.sellerBld = 0;
                lot.sellerKey = 0;
            } else {
                lot.origin    = ORIGIN_UNKNOWN;
                lot.sellerBld = 0;
                lot.sellerKey = 0;
            }
            return true;
        }
        return false;
    }

    void recordMode0Delivery(std::uintptr_t destBld, std::uintptr_t fleetOwnerP9,
                             std::uintptr_t res, float tons, double nowSec) {
        PendingDelivery& e = pendingDeliv_[pendingDelivHead_ % PENDING_RING];
        e.destBld    = destBld;
        e.fleetOwner = fleetOwnerP9;
        e.res        = res;
        e.tons       = (tons < 0.0f) ? -tons : tons;
        e.t          = nowSec;
        e.active     = true;
        pendingDelivHead_ = (pendingDelivHead_ + 1) % static_cast<int>(PENDING_RING);
    }

    bool matchMode1Delivery(std::uintptr_t vehicle, std::uint32_t vehSlotIdx,
                            std::uintptr_t vehicleHome, std::uintptr_t res,
                            float absTons, double nowSec,
                            MatchedDelivery* out) {
        if (!out) return false;
        *out = MatchedDelivery{};
        if (!vehicle) return false;
        if (vehSlotIdx >= VEH_TRACK_MAX) vehSlotIdx = hashVehiclePtr(vehicle);

        out->vehicle     = vehicle;
        out->vehSlotIdx  = vehSlotIdx;
        out->res         = res;
        out->vehicleHome = vehicleHome;

        const float mag = (absTons < 0.0f) ? -absTons : absTons;
        const float tol = TonnageTolerance(mag);

        const int start = (pendingDelivHead_ == 0)
                        ? static_cast<int>(PENDING_RING) - 1
                        : pendingDelivHead_ - 1;
        bool deliveryMatched = false;
        PendingDelivery pd{};
        for (int i = 0; i < static_cast<int>(PENDING_RING); ++i) {
            int k = start - i;
            while (k < 0) k += static_cast<int>(PENDING_RING);
            k %= static_cast<int>(PENDING_RING);

            PendingDelivery& cand = pendingDeliv_[k];
            if (!cand.active) continue;
            if (cand.res != res) continue;
            const double dt = nowSec - cand.t;
            if (dt < -0.001 || dt > 2.0) continue;
            if (std::fabs(cand.tons - mag) > tol) continue;

            cand.active = false;
            pd = cand;
            deliveryMatched = true;
            break;
        }

        out->deliveryMatched = deliveryMatched;
        if (!deliveryMatched) {
            out->carrierBld = vehicleHome;
            out->origin     = ORIGIN_UNKNOWN;
            out->cargoFound = false;
            return false;
        }

        out->destBld    = pd.destBld;
        out->fleetOwner = pd.fleetOwner;
        out->tons       = mag;
        out->t          = pd.t;
        out->carrierBld = vehicleHome ? vehicleHome : pd.fleetOwner;

        // Pop cargo from vehicle slot only after confirming a real delivery match.
        VehCargoState& vcs = vehCargo_[vehSlotIdx];
        if (vcs.vehPtr != 0 && vcs.vehPtr != vehicle) {
            vcs.vehPtr = vehicle;
            for (std::size_t j = 0; j < VEH_CARGO_SLOTS; ++j)
                vcs.lots[j] = VehCargoLot{};
        } else {
            vcs.vehPtr = vehicle;
        }

        OriginKind     origin    = ORIGIN_UNKNOWN;
        std::uintptr_t sellerBld = 0;
        std::uint64_t  sellerKey = 0;
        float          remaining = mag;
        float          popped    = 0.0f;

        for (std::size_t j = 0; j < VEH_CARGO_SLOTS; ++j) {
            VehCargoLot& lot = vcs.lots[j];
            if (lot.res != res || lot.tons <= 0.0f) continue;
            if (popped <= 0.0f) {
                origin    = lot.origin;
                sellerBld = lot.sellerBld;
                sellerKey = lot.sellerKey;
            }
            const float take = (lot.tons < remaining) ? lot.tons : remaining;
            lot.tons  -= take;
            remaining -= take;
            popped    += take;
            if (lot.tons <= TonnageTolerance(mag)) {
                lot = VehCargoLot{};
            }
            if (remaining <= 0.000001f) break;
        }

        const bool cargoFound = (mag > 0.0f) && (popped > 0.0f) && (remaining <= tol);
        if (!cargoFound) {
            origin    = ORIGIN_UNKNOWN;
            sellerBld = 0;
            sellerKey = 0;
        }

        out->origin     = origin;
        out->sellerBld  = sellerBld;
        out->sellerKey  = sellerKey;
        out->cargoFound = cargoFound;
        return true;
    }

    PendingLoad*     pendingLoadPtr()  { return pendingLoad_; }
    PendingDelivery* pendingDelivPtr() { return pendingDeliv_; }
    VehCargoState*   vehCargoPtr()     { return vehCargo_; }

private:
    PendingLoad     pendingLoad_[PENDING_RING]   = {};
    int             pendingLoadHead_             = 0;
    PendingDelivery pendingDeliv_[PENDING_RING]  = {};
    int             pendingDelivHead_            = 0;
    VehCargoState   vehCargo_[VEH_TRACK_MAX]     = {};
};

inline SettlementPlan planSettlement(
    OriginKind origin,
    std::uintptr_t sellerBld, bool sellerIsPriv, std::uint64_t sellerKey,
    std::uintptr_t carrierBld, bool carrierIsPriv,
    std::uintptr_t buyerBld, bool buyerIsPriv,
    double goodsValue, double freightValue,
    bool allowBuyerOverdraft, double buyerOverdraftLimit,
    bool chargeStateCarrier, int inFlightReloadPolicy)
{
    SettlementPlan plan;
    plan.origin               = origin;
    plan.sellerBld            = sellerBld;
    plan.sellerKey            = sellerKey;
    plan.carrierBld           = carrierBld;
    plan.buyerBld             = buyerBld;
    plan.sellerIsPriv         = sellerIsPriv && (sellerBld != 0);
    plan.carrierIsPriv        = carrierIsPriv && (carrierBld != 0);
    plan.buyerIsPriv          = buyerIsPriv && (buyerBld != 0);
    plan.goodsValue           = (goodsValue > 0.0) ? goodsValue : 0.0;
    plan.freightValue         = (freightValue > 0.0) ? freightValue : 0.0;
    plan.allowBuyerOverdraft  = allowBuyerOverdraft;
    plan.buyerOverdraftLimit  = (buyerOverdraftLimit > 0.0) ? buyerOverdraftLimit : 0.0;
    plan.chargeStateCarrier   = chargeStateCarrier;
    plan.inFlightReloadPolicy = inFlightReloadPolicy;

    // 1. Goods principal leg (Section 3.1: Buyer -> Seller)
    if (origin == ORIGIN_PRIVATE && plan.sellerIsPriv) {
        if (plan.buyerIsPriv && sellerBld == buyerBld) {
            plan.internalGoods = true;
            plan.hasGoodsLeg   = false;
        } else if (plan.buyerIsPriv) {
            plan.hasGoodsLeg            = (plan.goodsValue > 0.0);
            plan.goodsLeg.payer         = reinterpret_cast<void*>(buyerBld);
            plan.goodsLeg.payerCat      = CAP_CAT_WHOLESALE;
            plan.goodsLeg.payee         = reinterpret_cast<void*>(sellerBld);
            plan.goodsLeg.payeeCat      = CAP_CAT_PRODUCTION;
            plan.goodsLeg.amount        = plan.goodsValue;
            plan.goodsLeg.treasuryLabel = nullptr;
            plan.goodsLeg.isGoods       = true;
        } else {
            // Private A -> State Buyer
            plan.hasGoodsLeg            = (plan.goodsValue > 0.0);
            plan.goodsLeg.payer         = nullptr;
            plan.goodsLeg.payerCat      = 0;
            plan.goodsLeg.payee         = reinterpret_cast<void*>(sellerBld);
            plan.goodsLeg.payeeCat      = CAP_CAT_PRODUCTION;
            plan.goodsLeg.amount        = plan.goodsValue;
            plan.goodsLeg.treasuryLabel = "b2b_state_buy";
            plan.goodsLeg.isGoods       = true;
        }
    } else if (origin == ORIGIN_STATE) {
        if (plan.buyerIsPriv) {
            // State Seller -> Private Buyer
            plan.hasGoodsLeg            = (plan.goodsValue > 0.0);
            plan.goodsLeg.payer         = reinterpret_cast<void*>(buyerBld);
            plan.goodsLeg.payerCat      = CAP_CAT_WHOLESALE;
            plan.goodsLeg.payee         = nullptr;
            plan.goodsLeg.payeeCat      = 0;
            plan.goodsLeg.amount        = plan.goodsValue;
            plan.goodsLeg.treasuryLabel = "b2b_state_sell";
            plan.goodsLeg.isGoods       = true;
        }
    } else {
        // ORIGIN_UNKNOWN (mid-transit reload)
        if (inFlightReloadPolicy == 1 && plan.buyerIsPriv) {
            plan.hasGoodsLeg            = (plan.goodsValue > 0.0);
            plan.goodsLeg.payer         = reinterpret_cast<void*>(buyerBld);
            plan.goodsLeg.payerCat      = CAP_CAT_WHOLESALE;
            plan.goodsLeg.payee         = nullptr;
            plan.goodsLeg.payeeCat      = 0;
            plan.goodsLeg.amount        = plan.goodsValue;
            plan.goodsLeg.treasuryLabel = "b2b_legacy_transit";
            plan.goodsLeg.isGoods       = true;
        }
    }

    // 2. Freight leg (Section 3.2: Buyer -> Carrier)
    plan.selfHaul = (carrierBld != 0 && carrierBld == buyerBld);
    if (carrierBld != 0 && !plan.selfHaul && plan.freightValue > 0.0) {
        if (plan.carrierIsPriv && plan.buyerIsPriv) {
            plan.hasFreightLeg            = true;
            plan.freightLeg.payer         = reinterpret_cast<void*>(buyerBld);
            plan.freightLeg.payerCat      = CAP_CAT_WHOLESALE;
            plan.freightLeg.payee         = reinterpret_cast<void*>(carrierBld);
            plan.freightLeg.payeeCat      = CAP_CAT_WHOLESALE;
            plan.freightLeg.amount        = plan.freightValue;
            plan.freightLeg.treasuryLabel = nullptr;
            plan.freightLeg.isFreight     = true;
        } else if (plan.carrierIsPriv && !plan.buyerIsPriv) {
            // Private Carrier -> State Buyer
            plan.hasFreightLeg            = true;
            plan.freightLeg.payer         = nullptr;
            plan.freightLeg.payerCat      = 0;
            plan.freightLeg.payee         = reinterpret_cast<void*>(carrierBld);
            plan.freightLeg.payeeCat      = CAP_CAT_WHOLESALE;
            plan.freightLeg.amount        = plan.freightValue;
            plan.freightLeg.treasuryLabel = "b2b_freight";
            plan.freightLeg.isFreight     = true;
        } else if (!plan.carrierIsPriv && plan.buyerIsPriv && chargeStateCarrier) {
            // State Carrier -> Private Buyer
            plan.hasFreightLeg            = true;
            plan.freightLeg.payer         = reinterpret_cast<void*>(buyerBld);
            plan.freightLeg.payerCat      = CAP_CAT_WHOLESALE;
            plan.freightLeg.payee         = nullptr;
            plan.freightLeg.payeeCat      = 0;
            plan.freightLeg.amount        = plan.freightValue;
            plan.freightLeg.treasuryLabel = "b2b_state_freight";
            plan.freightLeg.isFreight     = true;
        }
    }

    double stateNet = 0.0;
    if (plan.hasGoodsLeg) {
        if (plan.goodsLeg.payer == nullptr) stateNet -= plan.goodsLeg.amount;
        if (plan.goodsLeg.payee == nullptr) stateNet += plan.goodsLeg.amount;
    }
    if (plan.hasFreightLeg) {
        if (plan.freightLeg.payer == nullptr) stateNet -= plan.freightLeg.amount;
        if (plan.freightLeg.payee == nullptr) stateNet += plan.freightLeg.amount;
    }
    plan.stateDelta = stateNet;
    return plan;
}

inline bool TransferFunds(const FundsTransferCallbacks& cb,
                          void* payerBld, int payerCat,
                          void* payeeBld, int payeeCat,
                          double amount, const char* treasuryLabel,
                          std::uint64_t sellerKey = 0)
{
    if (!(amount >= cb.minPayment)) return false;
    const bool payerPriv = (payerBld != nullptr);
    const bool payeePriv = (payeeBld != nullptr);
    if (!payerPriv && !payeePriv) return false;

    // 1. Pre-validate liveness, seller identity, and affordability BEFORE mutating.
    if (payerPriv) {
        if (!cb.isLivePrivate || !cb.isLivePrivate(cb.userData, payerBld)) return false;
        if (cb.getCapital) {
            const double have = cb.getCapital(cb.userData, payerBld);
            const double avail = cb.allowBuyerOverdraft ? (have + cb.buyerOverdraftLimit) : have;
            if (avail + 1e-9 < amount) {
                if (cb.skipBuyerInsolvent) (*cb.skipBuyerInsolvent)++;
                return false;
            }
        }
    } else {
        if (cb.getTreasury) {
            const double bal = cb.getTreasury(cb.userData, 0);
            if (bal + 1e-9 < amount) {
                if (cb.skipTreasury) (*cb.skipTreasury)++;
                return false;
            }
        }
    }

    if (payeePriv) {
        if (!cb.isLivePrivate || !cb.isLivePrivate(cb.userData, payeeBld)) return false;
        if (sellerKey != 0 && cb.checkSellerKey) {
            if (!cb.checkSellerKey(cb.userData, payeeBld, sellerKey)) return false;
        }
    }

    if (cb.dryRun) return true;

    // 2. Execute Payer Debit and verify it landed.
    if (payerPriv) {
        const double before = cb.getCapital ? cb.getCapital(cb.userData, payerBld) : 0.0;
        if (cb.modifyCapital) cb.modifyCapital(cb.userData, payerBld, -amount, payerCat);
        const double after = cb.getCapital ? cb.getCapital(cb.userData, payerBld) : 0.0;
        if (std::fabs((before - after) - amount) > 0.05) {
            return false;
        }
    } else {
        if (cb.moveTreasury) cb.moveTreasury(cb.userData, -amount, treasuryLabel ? treasuryLabel : "b2b");
    }

    // 3. Execute Payee Credit and verify; roll back Payer immediately if Payee dropped!
    if (payeePriv) {
        const double before = cb.getCapital ? cb.getCapital(cb.userData, payeeBld) : 0.0;
        if (cb.modifyCapital) cb.modifyCapital(cb.userData, payeeBld, amount, payeeCat);
        const double after = cb.getCapital ? cb.getCapital(cb.userData, payeeBld) : 0.0;
        if (std::fabs((after - before) - amount) > 0.05) {
            if (payerPriv) {
                if (cb.modifyCapital) cb.modifyCapital(cb.userData, payerBld, +amount, payerCat);
            } else {
                if (cb.moveTreasury) cb.moveTreasury(cb.userData, +amount, treasuryLabel ? treasuryLabel : "b2b");
            }
            return false;
        }
    } else {
        if (cb.moveTreasury) cb.moveTreasury(cb.userData, +amount, treasuryLabel ? treasuryLabel : "b2b");
    }

    return true;
}

// Executes atomic settlement for a delivery plan with Freight-first priority
// and buyer overdraft floor capping (Section 6.2).
inline TransferExecutionSummary executeAtomicTransfer(const SettlementPlan& plan,
                                                      const FundsTransferCallbacks& cb)
{
    TransferExecutionSummary res;
    void* const buyerPtr = plan.buyerIsPriv ? reinterpret_cast<void*>(plan.buyerBld) : nullptr;
    double dryBuyerSpent = 0.0;

    auto capBuyerAmount = [&](double want, bool& truncatedOrBlocked) -> double {
        truncatedOrBlocked = false;
        if (!buyerPtr || !cb.getCapital) return want;
        const double have = cb.getCapital(cb.userData, buyerPtr) - (cb.dryRun ? dryBuyerSpent : 0.0);
        const double avail = plan.allowBuyerOverdraft ? (have + plan.buyerOverdraftLimit) : have;
        if (avail + 1e-9 >= want) return want;
        truncatedOrBlocked = true;
        if (avail >= cb.minPayment) return avail;
        return 0.0;
    };

    // 1. Freight Leg FIRST
    if (plan.hasFreightLeg) {
        const bool buyerPays = (buyerPtr != nullptr && plan.freightLeg.payer == buyerPtr);
        bool capped = false;
        const double amt = buyerPays ? capBuyerAmount(plan.freightLeg.amount, capped)
                                     : plan.freightLeg.amount;
        if (capped && cb.skipBuyerInsolvent) (*cb.skipBuyerInsolvent)++;

        if (amt >= cb.minPayment) {
            if (TransferFunds(cb, plan.freightLeg.payer, plan.freightLeg.payerCat,
                              plan.freightLeg.payee, plan.freightLeg.payeeCat,
                              amt, plan.freightLeg.treasuryLabel, 0)) {
                res.freightPaid = amt;
                if (cb.dryRun && buyerPays) dryBuyerSpent += amt;
                if (plan.freightLeg.payer == nullptr) res.stateDelta -= amt;
                if (plan.freightLeg.payee == nullptr) res.stateDelta += amt;
            } else {
                res.ok = false;
            }
        } else if (plan.freightLeg.amount >= cb.minPayment) {
            res.ok = false;
        }
    }

    // 2. Goods Principal Leg SECOND (receives remaining credit capacity)
    if (plan.hasGoodsLeg) {
        const bool buyerPays = (buyerPtr != nullptr && plan.goodsLeg.payer == buyerPtr);
        bool capped = false;
        const double amt = buyerPays ? capBuyerAmount(plan.goodsLeg.amount, capped)
                                     : plan.goodsLeg.amount;
        if (capped && cb.skipBuyerInsolvent) (*cb.skipBuyerInsolvent)++;

        const std::uint64_t keyToCheck = (plan.goodsLeg.payee != nullptr) ? plan.sellerKey : 0;
        if (amt >= cb.minPayment) {
            if (TransferFunds(cb, plan.goodsLeg.payer, plan.goodsLeg.payerCat,
                              plan.goodsLeg.payee, plan.goodsLeg.payeeCat,
                              amt, plan.goodsLeg.treasuryLabel, keyToCheck)) {
                res.goodsPaid = amt;
                if (cb.dryRun && buyerPays) dryBuyerSpent += amt;
                if (plan.goodsLeg.payer == nullptr) res.stateDelta -= amt;
                if (plan.goodsLeg.payee == nullptr) res.stateDelta += amt;
            } else {
                res.ok = false;
            }
        } else if (plan.goodsLeg.amount >= cb.minPayment) {
            res.ok = false;
        }
    }

    return res;
}

} // namespace b2b
} // namespace wrsre

#endif // WRSRE_B2B_SETTLEMENT_H
