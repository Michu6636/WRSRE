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
    float  tons                 = 0.0f; // positive magnitude
    double t                    = 0.0;
    double unitPrice            = 0.0; // global market quote locked at loading
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
    float  tons              = 0.0f;
    double unitPrice         = 0.0; // never repriced during transit
    std::uintptr_t sellerBld = 0;
    std::uint64_t  sellerKey = 0;
    OriginKind     origin    = ORIGIN_UNKNOWN;
};

struct VehCargoState {
    std::uintptr_t vehPtr = 0;
    VehCargoLot    lots[VEH_CARGO_SLOTS];
};

struct DeliveryPart {
    float  tons                  = 0.0f;
    double unitPrice             = 0.0;
    OriginKind origin            = ORIGIN_UNKNOWN;
    std::uintptr_t sellerBld     = 0;
    std::uint64_t  sellerKey     = 0;
};

struct MatchedDelivery {
    std::uintptr_t destBld         = 0;
    std::uintptr_t fleetOwner      = 0;
    std::uintptr_t res             = 0;
    float  tons                    = 0.0f;
    double t                       = 0.0;
    std::uintptr_t vehicle         = 0;
    std::uint32_t  vehSlotIdx      = 0;
    std::uintptr_t vehicleHome     = 0;
    std::uintptr_t carrierBld      = 0;
    OriginKind     origin          = ORIGIN_UNKNOWN;
    std::uintptr_t sellerBld       = 0;
    std::uint64_t  sellerKey       = 0;
    double         unitPrice       = 0.0;
    DeliveryPart   parts[VEH_CARGO_SLOTS]{};
    unsigned       partCount       = 0;
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
};

// Goods only. Observed-assignment fuel uses TransferFunds directly; it is not
// a delivery-value or freight component and cannot consume buyer credit twice.
struct SettlementPlan {
    OriginKind     origin              = ORIGIN_UNKNOWN;
    std::uintptr_t sellerBld           = 0;
    std::uint64_t  sellerKey           = 0;
    std::uintptr_t buyerBld            = 0;
    bool           sellerIsPriv        = false;
    bool           buyerIsPriv         = false;
    double         goodsValue          = 0.0;
    bool           internalGoods       = false;
    bool           allowBuyerOverdraft = false;
    double         buyerOverdraftLimit = 0.0;
    bool           hasGoodsLeg         = false;
    SettlementLeg  goodsLeg;
    double         stateDelta          = 0.0;
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
    bool   ok         = true;
    double goodsPaid  = 0.0;
    double stateDelta = 0.0;
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
                         std::uint64_t sellerKey = 0,
                         double unitPrice = 0.0) {
        PendingLoad& e = pendingLoad_[pendingLoadHead_ % PENDING_RING];
        e.sourceBld    = sourceBld;
        e.res          = res;
        e.tons         = (absTons < 0.0f) ? -absTons : absTons;
        e.t            = nowSec;
        e.unitPrice    = (std::isfinite(unitPrice) && unitPrice >= 0.0) ? unitPrice : 0.0;
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

            VehCargoState& vcs = vehCargo_[vehSlotIdx];
            if (vcs.vehPtr != vehicle) {
                vcs.vehPtr = vehicle;
                for (std::size_t j = 0; j < VEH_CARGO_SLOTS; ++j)
                    vcs.lots[j] = VehCargoLot{};
            }

            const bool isPriv = sourceIsPriv || pl.sourceIsPriv;
            const std::uint64_t effKey = sellerKey ? sellerKey
                                       : (pl.sellerKey ? pl.sellerKey : pl.sourceBld);

            const OriginKind origin = (isPriv && pl.sourceBld != 0) ? ORIGIN_PRIVATE
                                    : (pl.sourceBld != 0 ? ORIGIN_STATE : ORIGIN_UNKNOWN);
            const std::uintptr_t lotSeller = origin == ORIGIN_PRIVATE ? pl.sourceBld : 0;
            const std::uint64_t lotKey = origin == ORIGIN_PRIVATE ? effKey : 0;

            // Resource alone is not a lot identity. Preserve seller and the
            // global quote locked at loading, including residual cargo after
            // task replacement. A full book fails closed instead of overwriting.
            int slot = -1;
            for (std::size_t j = 0; j < VEH_CARGO_SLOTS; ++j) {
                const VehCargoLot& lot = vcs.lots[j];
                if (lot.res == res && lot.origin == origin &&
                    lot.sellerBld == lotSeller && lot.sellerKey == lotKey &&
                    lot.unitPrice == pl.unitPrice) {
                    slot = static_cast<int>(j);
                    break;
                }
            }
            if (slot < 0) {
                for (std::size_t j = 0; j < VEH_CARGO_SLOTS; ++j) {
                    if (vcs.lots[j].res == 0 && vcs.lots[j].tons <= 0.0f) {
                        slot = static_cast<int>(j);
                        break;
                    }
                }
            }
            if (slot < 0) return false;

            pl.active = false;

            VehCargoLot& lot = vcs.lots[slot];
            lot.res       = res;
            lot.origin    = origin;
            lot.sellerBld = lotSeller;
            lot.sellerKey = lotKey;
            lot.unitPrice = pl.unitPrice;
            lot.tons     += mag;
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

        float remaining = mag;
        float popped    = 0.0f;

        for (std::size_t j = 0; j < VEH_CARGO_SLOTS; ++j) {
            VehCargoLot& lot = vcs.lots[j];
            if (lot.res != res || lot.tons <= 0.0f) continue;
            const float take = (lot.tons < remaining) ? lot.tons : remaining;
            if (take > 0.0f && out->partCount < VEH_CARGO_SLOTS) {
                DeliveryPart& part = out->parts[out->partCount++];
                part.tons       = take;
                part.unitPrice  = lot.unitPrice;
                part.origin     = lot.origin;
                part.sellerBld  = lot.sellerBld;
                part.sellerKey  = lot.sellerKey;
            }
            lot.tons  -= take;
            remaining -= take;
            popped    += take;
            if (lot.tons <= TonnageTolerance(mag)) {
                lot = VehCargoLot{};
            }
            if (remaining <= 0.000001f) break;
        }

        const bool cargoFound = (mag > 0.0f) && (popped > 0.0f) &&
                                (remaining <= tol) && out->partCount > 0;
        if (cargoFound) {
            const DeliveryPart& first = out->parts[0];
            out->origin     = first.origin;
            out->sellerBld  = first.sellerBld;
            out->sellerKey  = first.sellerKey;
            out->unitPrice  = first.unitPrice;
        } else {
            out->partCount  = 0; // partial/ambiguous delivery: no financial attribution
            out->origin     = ORIGIN_UNKNOWN;
            out->sellerBld  = 0;
            out->sellerKey  = 0;
            out->unitPrice  = 0.0;
        }
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
    std::uintptr_t buyerBld, bool buyerIsPriv,
    double goodsValue)
{
    SettlementPlan plan;
    plan.origin=origin;plan.sellerBld=sellerBld;plan.sellerKey=sellerKey;
    plan.buyerBld=buyerBld;
    plan.sellerIsPriv=sellerIsPriv && sellerBld!=0;
    plan.buyerIsPriv=buyerIsPriv && buyerBld!=0;
    plan.goodsValue=std::isfinite(goodsValue) && goodsValue>0.0?goodsValue:0.0;

    if(origin==ORIGIN_UNKNOWN) return plan; // fail closed, never guess a seller
    if(origin==ORIGIN_PRIVATE) {
        if(!plan.sellerIsPriv) return plan;
        if(plan.buyerIsPriv && sellerBld==buyerBld) {
            plan.internalGoods=true;return plan;
        }
        plan.hasGoodsLeg=plan.goodsValue>0.0;
        plan.goodsLeg.payer=plan.buyerIsPriv?reinterpret_cast<void*>(buyerBld):nullptr;
        plan.goodsLeg.payerCat=plan.buyerIsPriv?CAP_CAT_WHOLESALE:0;
        plan.goodsLeg.payee=reinterpret_cast<void*>(sellerBld);
        plan.goodsLeg.payeeCat=CAP_CAT_PRODUCTION;
        plan.goodsLeg.treasuryLabel=plan.buyerIsPriv?nullptr:"b2b_state_buy";
    } else if(origin==ORIGIN_STATE) {
        if(!plan.buyerIsPriv) return plan;
        plan.hasGoodsLeg=plan.goodsValue>0.0;
        plan.goodsLeg.payer=reinterpret_cast<void*>(buyerBld);
        plan.goodsLeg.payerCat=CAP_CAT_WHOLESALE;
        plan.goodsLeg.payee=nullptr;
        plan.goodsLeg.payeeCat=0;
        plan.goodsLeg.treasuryLabel="b2b_state_sell";
    }
    plan.goodsLeg.amount=plan.hasGoodsLeg?plan.goodsValue:0.0;
    if(plan.goodsLeg.payer==nullptr) plan.stateDelta-=plan.goodsLeg.amount;
    if(plan.goodsLeg.payee==nullptr) plan.stateDelta+=plan.goodsLeg.amount;
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

// Executes the single goods leg with an optional buyer overdraft floor.
inline TransferExecutionSummary executeAtomicTransfer(const SettlementPlan& plan,
                                                      const FundsTransferCallbacks& cb,
                                                      double dryBuyerAlreadySpent = 0.0)
{
    TransferExecutionSummary res;
    if(!plan.hasGoodsLeg) return res;
    void* const buyer=plan.buyerIsPriv?reinterpret_cast<void*>(plan.buyerBld):nullptr;
    double amount=plan.goodsLeg.amount;
    if(buyer && cb.getCapital) {
        const double have=cb.getCapital(cb.userData,buyer) -
            (cb.dryRun && dryBuyerAlreadySpent>0.0?dryBuyerAlreadySpent:0.0);
        const double available=(plan.allowBuyerOverdraft?
            have+plan.buyerOverdraftLimit:have);
        if(available+1e-9<amount) {
            amount=available>=cb.minPayment?available:0.0;
            if(cb.skipBuyerInsolvent) ++*cb.skipBuyerInsolvent;
        }
    }
    if(amount<cb.minPayment) {
        if(plan.goodsLeg.amount>=cb.minPayment) res.ok=false;
        return res;
    }
    const std::uint64_t key=plan.goodsLeg.payee?plan.sellerKey:0;
    if(!TransferFunds(cb,plan.goodsLeg.payer,plan.goodsLeg.payerCat,
                      plan.goodsLeg.payee,plan.goodsLeg.payeeCat,amount,
                      plan.goodsLeg.treasuryLabel,key)) {
        res.ok=false;return res;
    }
    res.goodsPaid=amount;
    if(plan.goodsLeg.payer==nullptr) res.stateDelta-=amount;
    if(plan.goodsLeg.payee==nullptr) res.stateDelta+=amount;
    return res;
}

} // namespace b2b
} // namespace wrsre

#endif // WRSRE_B2B_SETTLEMENT_H
