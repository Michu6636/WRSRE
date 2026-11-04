#include "b2b_settlement.h"
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>

using namespace wrsre::b2b;

static bool approx(double a, double b, double tol = 1e-6) {
    return std::fabs(a - b) <= tol;
}

struct MockWorld {
    double        cap[16]     = {};
    bool          live[16]    = {};
    bool          dropCredit[16] = {};
    std::uint64_t keys[16]    = {};
    double        treasury    = 100000.0;
    unsigned      skipInsolv  = 0;
    unsigned      skipTreas   = 0;

    static bool isLivePrivate(void* ud, void* b) {
        auto* w = static_cast<MockWorld*>(ud);
        auto idx = reinterpret_cast<std::uintptr_t>(b);
        return idx > 0 && idx < 16 && w->live[idx];
    }
    static double getCapital(void* ud, void* b) {
        auto* w = static_cast<MockWorld*>(ud);
        auto idx = reinterpret_cast<std::uintptr_t>(b);
        return (idx > 0 && idx < 16) ? w->cap[idx] : 0.0;
    }
    static void modifyCapital(void* ud, void* b, double d, int) {
        auto* w = static_cast<MockWorld*>(ud);
        auto idx = reinterpret_cast<std::uintptr_t>(b);
        if (idx == 0 || idx >= 16 || !w->live[idx]) return;
        if (d > 0.0 && w->dropCredit[idx]) return; // simulate silent ApplyCapital drop
        w->cap[idx] += d;
    }
    static double getTreasury(void* ud, int) {
        return static_cast<MockWorld*>(ud)->treasury;
    }
    static void moveTreasury(void* ud, double d, const char*) {
        static_cast<MockWorld*>(ud)->treasury += d;
    }
    static bool checkSellerKey(void* ud, void* b, std::uint64_t key) {
        auto* w = static_cast<MockWorld*>(ud);
        auto idx = reinterpret_cast<std::uintptr_t>(b);
        if (idx == 0 || idx >= 16) return false;
        return w->keys[idx] == key;
    }

    FundsTransferCallbacks makeCallbacks(bool allowOverdraft, double overdraftLimit, bool dryRun = false) {
        FundsTransferCallbacks cb;
        cb.userData            = this;
        cb.isLivePrivate       = &MockWorld::isLivePrivate;
        cb.getCapital          = &MockWorld::getCapital;
        cb.modifyCapital       = &MockWorld::modifyCapital;
        cb.getTreasury         = &MockWorld::getTreasury;
        cb.moveTreasury        = &MockWorld::moveTreasury;
        cb.checkSellerKey      = &MockWorld::checkSellerKey;
        cb.allowBuyerOverdraft = allowOverdraft;
        cb.buyerOverdraftLimit = overdraftLimit;
        cb.minPayment          = 0.01;
        cb.dryRun              = dryRun;
        cb.skipBuyerInsolvent  = &skipInsolv;
        cb.skipTreasury        = &skipTreas;
        return cb;
    }
};

static Engine g_testEngine;

int main() {
    // 1. Load join + single-consumption (two identical trucks never steal one load)
    {
        g_testEngine.resetAll();
        g_testEngine.recordMode0Load(0x100, 0x200, 10.0f, 1.0, true, 0xAAAA1111ULL);
        assert(g_testEngine.matchMode1Load(0x300, 0, 0x200, 10.0f, 1.05, false, 0));
        // Second truck with identical tonnage cannot match the already-consumed load:
        assert(!g_testEngine.matchMode1Load(0x301, 1, 0x200, 10.0f, 1.06, false, 0));

        // Fresh load matches second truck cleanly:
        g_testEngine.recordMode0Load(0x101, 0x200, 10.0f, 2.0, true, 0xBBBB2222ULL);
        assert(g_testEngine.matchMode1Load(0x301, 1, 0x200, 10.0f, 2.04, false, 0));
        std::printf("1. load join + single consumption OK\n");
    }

    // 2. Delivery join + 0.5% float rounding tolerance on cargo pop (9.998t -> 10.000t)
    {
        g_testEngine.resetAll();
        g_testEngine.recordMode0Load(1, 5, 9.998f, 1.0, true, 0xCAFE0001ULL);
        assert(g_testEngine.matchMode1Load(10, 0, 5, 9.998f, 1.05, false, 0));
        g_testEngine.recordMode0Delivery(2, 3, 5, 10.000f, 20.0);
        MatchedDelivery md{};
        assert(g_testEngine.matchMode1Delivery(10, 0, 3, 5, 10.000f, 20.03, &md));
        assert(md.deliveryMatched);
        assert(md.cargoFound);
        assert(md.origin == ORIGIN_PRIVATE);
        assert(md.sellerBld == 1);
        assert(md.sellerKey == 0xCAFE0001ULL);
        assert(md.destBld == 2);
        assert(md.carrierBld == 3);
        std::printf("2. delivery join + float tolerance OK\n");
    }

    // 3. Closed-loop Private Farm (1) -> Private DO (3) -> Private Factory (2) -> Private Shop (4)
    {
        g_testEngine.resetAll();
        MockWorld w;
        for (int i = 1; i <= 4; ++i) {
            w.live[i] = true;
            w.cap[i]  = 50000.0;
            w.keys[i] = 0x9000ULL + static_cast<std::uint64_t>(i);
        }
        const double openPrivSum = w.cap[1] + w.cap[2] + w.cap[3] + w.cap[4];
        const double openState   = w.treasury;

        for (int trip = 0; trip < 50; ++trip) {
            const double t0 = trip * 10.0;
            g_testEngine.recordMode0Load(1, 77, 12.5f, t0, true, w.keys[1]);
            assert(g_testEngine.matchMode1Load(500, 12, 77, 12.5f, t0 + 0.02, false, 0));

            g_testEngine.recordMode0Delivery(2, 3, 77, 12.5f, t0 + 5.0);
            MatchedDelivery md{};
            assert(g_testEngine.matchMode1Delivery(500, 12, 3, 77, 12.5f, t0 + 5.03, &md));

            SettlementPlan plan = planSettlement(
                md.origin,
                md.sellerBld, true, md.sellerKey,
                md.carrierBld, true,
                md.destBld, true,
                412.50, 38.20,
                true, 5000.0, true, 0);
            assert(approx(plan.stateDelta, 0.0));
            auto cb = w.makeCallbacks(true, 5000.0);
            TransferExecutionSummary ex = executeAtomicTransfer(plan, cb);
            assert(ex.ok);
            assert(approx(ex.goodsPaid, 412.50));
            assert(approx(ex.freightPaid, 38.20));
            assert(approx(ex.stateDelta, 0.0));
        }

        const double closePrivSum = w.cap[1] + w.cap[2] + w.cap[3] + w.cap[4];
        assert(approx(closePrivSum, openPrivSum, 1e-6));
        assert(approx(w.treasury, openState, 1e-6));
        std::printf("3. closed-loop private conservation (50 trips) OK\n");
    }

    // 4. Intra-company transfer (Seller == Buyer) and Self-haul (Carrier == Buyer) move 0.00
    {
        SettlementPlan plan = planSettlement(
            ORIGIN_PRIVATE,
            7, true, 0x777ULL,
            7, true,
            7, true,
            250.0, 40.0,
            false, 0.0, true, 0);
        assert(plan.internalGoods);
        assert(plan.selfHaul);
        assert(!plan.hasGoodsLeg);
        assert(!plan.hasFreightLeg);
        assert(approx(plan.stateDelta, 0.0));
        std::printf("4. intra-company + self-haul zero OK\n");
    }

    // 5. Mixed State/Private legs (including State Carrier -> Private Buyer!)
    {
        MockWorld w;
        w.live[2] = true; w.cap[2] = 2000.0; w.keys[2] = 0x222ULL;
        w.live[3] = true; w.cap[3] = 500.0;  w.keys[3] = 0x333ULL;
        const double openTotal = w.treasury + w.cap[2] + w.cap[3];

        // 5a: State Seller -> State DO (carrier=99, !carrierIsPriv) -> Private Buyer (2)
        SettlementPlan p1 = planSettlement(
            ORIGIN_STATE,
            88, false, 0,
            99, false,
            2, true,
            300.0, 25.0,
            true, 5000.0, true, 0);
        assert(p1.hasGoodsLeg && p1.hasFreightLeg);
        assert(approx(p1.stateDelta, +325.0));
        auto cb = w.makeCallbacks(true, 5000.0);
        auto ex1 = executeAtomicTransfer(p1, cb);
        assert(ex1.ok);
        assert(approx(w.cap[2], 2000.0 - 325.0));
        assert(approx(w.treasury + w.cap[2] + w.cap[3], openTotal));

        // 5b: Private Seller (2) -> Private DO (3) -> State Warehouse (buyer=55, !buyerIsPriv)
        SettlementPlan p2 = planSettlement(
            ORIGIN_PRIVATE,
            2, true, w.keys[2],
            3, true,
            55, false,
            400.0, 30.0,
            true, 5000.0, true, 0);
        assert(p2.hasGoodsLeg && p2.hasFreightLeg);
        assert(approx(p2.stateDelta, -430.0));
        auto ex2 = executeAtomicTransfer(p2, cb);
        assert(ex2.ok);
        assert(approx(w.treasury + w.cap[2] + w.cap[3], openTotal));
        std::printf("5. mixed State/Private legs + State carrier OK\n");
    }

    // 6. Demolished seller / sellerKey mismatch / silent payee drop rollback
    {
        MockWorld w;
        w.live[1] = true;  w.cap[1] = 1000.0; w.keys[1] = 0x111ULL;
        w.live[2] = false; w.cap[2] = 200.0;  w.keys[2] = 0x222ULL; // decayed!
        auto cb = w.makeCallbacks(false, 0.0);
        assert(!TransferFunds(cb, reinterpret_cast<void*>(1), CAP_CAT_WHOLESALE,
                              reinterpret_cast<void*>(2), CAP_CAT_PRODUCTION,
                              100.0, nullptr, 0x222ULL));
        assert(approx(w.cap[1], 1000.0));

        // Seller replaced by different building at same pointer (sellerKey mismatch):
        w.live[2] = true; w.keys[2] = 0x9999ULL;
        assert(!TransferFunds(cb, reinterpret_cast<void*>(1), CAP_CAT_WHOLESALE,
                              reinterpret_cast<void*>(2), CAP_CAT_PRODUCTION,
                              100.0, nullptr, 0x222ULL));
        assert(approx(w.cap[1], 1000.0));
        assert(approx(w.cap[2], 200.0));

        // Silent credit drop inside modifyCompanyCapital triggers immediate rollback:
        w.keys[2] = 0x222ULL;
        w.dropCredit[2] = true;
        assert(!TransferFunds(cb, reinterpret_cast<void*>(1), CAP_CAT_WHOLESALE,
                              reinterpret_cast<void*>(2), CAP_CAT_PRODUCTION,
                              100.0, nullptr, 0x222ULL));
        assert(approx(w.cap[1], 1000.0));
        assert(approx(w.cap[2], 200.0));
        std::printf("6. decay + key mismatch + silent-drop rollback OK\n");
    }

    // 7. Buyer overdraft floor (-5000.0) with Freight-first priority + partial principal
    {
        MockWorld w;
        w.live[1] = true; w.cap[1] = -4850.0; w.keys[1] = 0x1ULL; // 150.0 credit room left!
        w.live[2] = true; w.cap[2] = 0.0;     w.keys[2] = 0x2ULL; // Seller
        w.live[3] = true; w.cap[3] = 0.0;     w.keys[3] = 0x3ULL; // Carrier

        SettlementPlan plan = planSettlement(
            ORIGIN_PRIVATE,
            2, true, w.keys[2],
            3, true,
            1, true,
            400.0, 60.0, // wants 60 freight + 400 goods = 460, only 150 room
            true, 5000.0, true, 0);
        auto cb = w.makeCallbacks(true, 5000.0);
        TransferExecutionSummary ex = executeAtomicTransfer(plan, cb);
        assert(approx(ex.freightPaid, 60.0)); // Carrier paid in full FIRST
        assert(approx(ex.goodsPaid, 90.0));   // Seller gets remaining 90.0 capacity
        assert(approx(w.cap[1], -5000.0));
        assert(approx(w.cap[3], 60.0));
        assert(approx(w.cap[2], 90.0));
        assert(approx(w.cap[1] + w.cap[2] + w.cap[3], -4850.0)); // exact conservation!
        assert(w.skipInsolv == 1);
        std::printf("7. overdraft floor + freight-first partial principal OK\n");
    }

    // 8. resetAll() on save load + ORIGIN_UNKNOWN policy (0=skip vs 1=state)
    {
        g_testEngine.resetAll();
        g_testEngine.recordMode0Load(1, 5, 8.0f, 1.0, true, 0x123ULL);
        assert(g_testEngine.matchMode1Load(10, 0, 5, 8.0f, 1.1, false, 0));
        g_testEngine.resetAll(); // simulate TSM_SAVE_LOADING

        g_testEngine.recordMode0Delivery(2, 3, 5, 8.0f, 2.0);
        MatchedDelivery md{};
        assert(g_testEngine.matchMode1Delivery(10, 0, 3, 5, 8.0f, 2.05, &md));
        assert(md.deliveryMatched);
        assert(!md.cargoFound);
        assert(md.origin == ORIGIN_UNKNOWN);

        SettlementPlan pSkip = planSettlement(
            md.origin, 0, false, 0, 3, true, 2, true, 200.0, 25.0, true, 5000.0, true, 0);
        assert(!pSkip.hasGoodsLeg);
        assert(pSkip.hasFreightLeg); // Freight still settles normally!

        SettlementPlan pState = planSettlement(
            md.origin, 0, false, 0, 3, true, 2, true, 200.0, 25.0, true, 5000.0, true, 1);
        assert(pState.hasGoodsLeg);
        assert(pState.hasFreightLeg);
        assert(approx(pState.stateDelta, +200.0));
        std::printf("8. resetAll + ORIGIN_UNKNOWN policy OK\n");
    }

    // 9. Runtime delivery-site routing: p9 == vehicleHome must not suppress
    // the vehicle half at 0x68F7F9, while genuine staging elsewhere stays out.
    {
        assert(ShouldTrackMode1Cargo(true, 1, true, true, false,
                                     DELIVERY_VEHICLE_SITE_RVA));
        assert(!ShouldTrackMode1Cargo(true, 1, true, true, false, 0x006953C4));
        assert(ShouldTrackMode1Cargo(true, 1, true, false, false, 0x006953C4));
        assert(!ShouldTrackMode1Cargo(false, 1, true, true, false,
                                      DELIVERY_VEHICLE_SITE_RVA));
        assert(!ShouldTrackMode1Cargo(true, 0, true, true, false,
                                      DELIVERY_VEHICLE_SITE_RVA));
        assert(!ShouldTrackMode1Cargo(true, 1, false, true, false,
                                      DELIVERY_VEHICLE_SITE_RVA));
        assert(!ShouldTrackMode1Cargo(true, 1, true, true, true,
                                      DELIVERY_VEHICLE_SITE_RVA));

        // Exact 1.650 t pair from the two-save runtime log, including cargo
        // attribution and single consumption of the pending delivery.
        g_testEngine.resetAll();
        g_testEngine.recordMode0Load(1, 5, 1.650f, 1.0, true, 0xD311E1ULL);
        assert(g_testEngine.matchMode1Load(10, 9, 5, 1.650f, 1.03, false, 0));
        g_testEngine.recordMode0Delivery(2, 3, 5, 1.650f, 2.0);
        MatchedDelivery md{};
        assert(g_testEngine.matchMode1Delivery(10, 9, 3, 5, 1.650f, 2.03, &md));
        assert(md.deliveryMatched && md.cargoFound);
        assert(md.origin == ORIGIN_PRIVATE);
        assert(md.sellerBld == 1 && md.sellerKey == 0xD311E1ULL);
        assert(md.destBld == 2 && md.carrierBld == 3);
        assert(!g_testEngine.matchMode1Delivery(10, 9, 3, 5, 1.650f, 2.04, &md));

        // A wrong resource cannot consume a pending delivery; an excessively
        // old pair and an ambiguous partial-load tonnage remain safe skips.
        g_testEngine.recordMode0Delivery(2, 3, 6, 0.500f, 4.0);
        assert(!g_testEngine.matchMode1Delivery(10, 9, 3, 5, 0.500f, 4.03, &md));
        assert(g_testEngine.matchMode1Delivery(10, 9, 3, 6, 0.500f, 4.05, &md));
        g_testEngine.recordMode0Delivery(2, 3, 7, 0.500f, 10.0);
        assert(!g_testEngine.matchMode1Delivery(10, 9, 3, 7, 0.500f, 12.011, &md));
        g_testEngine.recordMode0Load(1, 8, 10.0f, 20.0, true, 0x1234ULL);
        assert(!g_testEngine.matchMode1Load(10, 9, 8, 6.0f, 20.03, false, 0));

        // One cargo lot can legitimately satisfy multiple partial deliveries.
        g_testEngine.resetAll();
        g_testEngine.recordMode0Load(1, 5, 10.0f, 1.0, true, 0xBEEFULL);
        assert(g_testEngine.matchMode1Load(10, 9, 5, 10.0f, 1.03, false, 0));
        g_testEngine.recordMode0Delivery(2, 3, 5, 4.0f, 2.0);
        assert(g_testEngine.matchMode1Delivery(10, 9, 3, 5, 4.0f, 2.03, &md));
        assert(md.cargoFound);
        g_testEngine.recordMode0Delivery(2, 3, 5, 6.0f, 3.0);
        assert(g_testEngine.matchMode1Delivery(10, 9, 3, 5, 6.0f, 3.03, &md));
        assert(md.cargoFound && md.sellerKey == 0xBEEFULL);
        std::printf("9. vehicle-delivery gate + safe partial joins OK\n");
    }

    std::printf("ALL B2B SCENARIOS PASS\n");
    return 0;
}
