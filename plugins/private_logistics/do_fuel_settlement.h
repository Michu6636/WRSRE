#ifndef PRIVATE_LOGISTICS_DO_FUEL_SETTLEMENT_H
#define PRIVATE_LOGISTICS_DO_FUEL_SETTLEMENT_H

#include "step3b_policy.h"
#include <cmath>
#include <cstdint>

namespace private_logistics_step3b {

struct DoFuelCallbacks {
    void* userData = nullptr;
    bool (*isLivePrivate)(void*, void*) = nullptr;
    double (*getCapital)(void*, void*) = nullptr;
    void (*modifyCapital)(void*, void*, double, int) = nullptr;
    double (*getTreasury)(void*, int) = nullptr;
    void (*moveTreasury)(void*, double, const char*) = nullptr;
    bool (*partyCurrent)(void*, std::uintptr_t, std::uint64_t, ReceiverKind) = nullptr;
    bool (*generationCurrent)(void*) = nullptr;
    bool (*commit)(void*, PayerKind, double) = nullptr;
    bool allowBuyerOverdraft = false;
    double buyerOverdraftLimit = 0.0;
    double minPayment = 0.01;
    int payerCategory = 1;
    int payeeCategory = 1;
    unsigned* skipBuyerInsolvent = nullptr;
    unsigned* skipTreasury = nullptr;
};

struct DoFuelResult {
    bool paid = false;
    const char* reason = "direction_unknown";
    double payerDelta = 0.0;
    double payeeDelta = 0.0;
    bool payerRollbackVerified = true;
    bool payeeRollbackVerified = true;
};

inline bool DoFuelWithin(double actual, double expected) {
    return std::isfinite(actual) && std::fabs(actual - expected) <= 0.05;
}

inline DoFuelResult ExecuteDoFuelTransfer(
    const DoFuelCallbacks& cb, const PayerDecision& decision,
    std::uintptr_t receiver, std::uint64_t receiverKey,
    std::uintptr_t carrier, std::uint64_t carrierKey,
    double amount, bool live)
{
    DoFuelResult result;
    result.reason = decision.reason;
    if (decision.payer == PayerKind::None) return result;
    if (!std::isfinite(amount) || amount < cb.minPayment) {
        result.reason = "invalid_amount";
        return result;
    }

    result.reason = "missing_callbacks";
    if (!receiver || !carrier || !cb.partyCurrent ||
        !cb.generationCurrent || !cb.isLivePrivate || !cb.getCapital ||
        !cb.modifyCapital) return result;
    if (decision.payer == PayerKind::Treasury &&
        (!cb.getTreasury || !cb.moveTreasury)) {
        if (cb.skipTreasury) ++*cb.skipTreasury;
        result.reason = "treasury_unavailable";
        return result;
    }
    if (decision.payer == PayerKind::PrivateCompany &&
        !cb.getCapital) return result;
    if (live && !cb.commit) return result;

    result.reason = "receiver_changed";
    const ReceiverKind receiverKind = decision.payer == PayerKind::PrivateCompany
        ? ReceiverKind::Private : ReceiverKind::State;
    if (!cb.partyCurrent(cb.userData, receiver, receiverKey, receiverKind)) return result;
    if (decision.payer == PayerKind::PrivateCompany &&
        !cb.isLivePrivate(cb.userData, reinterpret_cast<void*>(receiver))) return result;
    result.reason = "do_unavailable";
    if (!cb.isLivePrivate(cb.userData, reinterpret_cast<void*>(carrier)) ||
        !cb.partyCurrent(cb.userData, carrier, carrierKey, ReceiverKind::Private)) return result;
    if (!live) {
        result.reason = decision.reason;
        return result;
    }

    double payerBefore = 0.0;
    if (decision.payer == PayerKind::PrivateCompany) {
        payerBefore = cb.getCapital(cb.userData, reinterpret_cast<void*>(receiver));
        if (!std::isfinite(payerBefore)) {
            result.reason = "payer_unavailable";
            return result;
        }
        const double available = cb.allowBuyerOverdraft
            ? payerBefore + cb.buyerOverdraftLimit : payerBefore;
        if (!std::isfinite(available) || available + 1e-9 < amount) {
            if (cb.skipBuyerInsolvent) ++*cb.skipBuyerInsolvent;
            result.reason = "private_insufficient";
            return result;
        }
    } else {
        payerBefore = cb.getTreasury(cb.userData, 0);
        if (!std::isfinite(payerBefore)) {
            if (cb.skipTreasury) ++*cb.skipTreasury;
            result.reason = "treasury_unavailable";
            return result;
        }
        if (payerBefore + 1e-9 < amount) {
            if (cb.skipTreasury) ++*cb.skipTreasury;
            result.reason = "treasury_insufficient";
            return result;
        }
    }
    double payeeBefore = cb.getCapital(cb.userData, reinterpret_cast<void*>(carrier));
    if (!std::isfinite(payeeBefore)) {
        result.reason = "do_unavailable";
        return result;
    }

    const auto payerRead = [&]() {
        return decision.payer == PayerKind::PrivateCompany
            ? cb.getCapital(cb.userData, reinterpret_cast<void*>(receiver))
            : cb.getTreasury(cb.userData, 0);
    };
    const auto mutatePayer = [&](double delta) {
        if (decision.payer == PayerKind::PrivateCompany)
            cb.modifyCapital(cb.userData, reinterpret_cast<void*>(receiver),
                             delta, cb.payerCategory);
        else
            cb.moveTreasury(cb.userData, delta,
                decision.treasuryLabel ? decision.treasuryLabel : "b2b_state_do_fuel");
    };
    const auto rollbackPayer = [&]() {
        const double current = payerRead();
        if (!std::isfinite(current)) return false;
        const double restore = payerBefore - current;
        if (!std::isfinite(restore)) return false;
        (void)cb.generationCurrent(cb.userData);
        mutatePayer(restore);
        const double restored = payerRead();
        return DoFuelWithin(restored, payerBefore);
    };
    const auto rollbackPayee = [&]() {
        const double current = cb.getCapital(cb.userData, reinterpret_cast<void*>(carrier));
        if (!std::isfinite(current)) return false;
        const double restore = payeeBefore - current;
        if (!std::isfinite(restore)) return false;
        (void)cb.generationCurrent(cb.userData);
        cb.modifyCapital(cb.userData, reinterpret_cast<void*>(carrier),
                         restore, cb.payeeCategory);
        const double restored = cb.getCapital(cb.userData, reinterpret_cast<void*>(carrier));
        return DoFuelWithin(restored, payeeBefore);
    };

    result.reason = "generation_changed_before_debit";
    if (!cb.generationCurrent(cb.userData)) return result;
    if (!cb.partyCurrent(cb.userData, receiver, receiverKey, receiverKind) ||
        (decision.payer == PayerKind::PrivateCompany &&
         !cb.isLivePrivate(cb.userData, reinterpret_cast<void*>(receiver))) ||
        !cb.isLivePrivate(cb.userData, reinterpret_cast<void*>(carrier)) ||
        !cb.partyCurrent(cb.userData, carrier, carrierKey, ReceiverKind::Private)) {
        result.reason = "identity_changed_before_debit";
        return result;
    }
    payerBefore = payerRead();
    if (!std::isfinite(payerBefore)) {
        if (decision.payer == PayerKind::Treasury && cb.skipTreasury)
            ++*cb.skipTreasury;
        result.reason = decision.payer == PayerKind::Treasury
            ? "treasury_unavailable" : "payer_unavailable";
        return result;
    }
    if (decision.payer == PayerKind::PrivateCompany) {
        const double available = cb.allowBuyerOverdraft
            ? payerBefore + cb.buyerOverdraftLimit : payerBefore;
        if (!std::isfinite(available) || available + 1e-9 < amount) {
            if (cb.skipBuyerInsolvent) ++*cb.skipBuyerInsolvent;
            result.reason = "private_insufficient";
            return result;
        }
    } else if (payerBefore + 1e-9 < amount) {
        if (cb.skipTreasury) ++*cb.skipTreasury;
        result.reason = "treasury_insufficient";
        return result;
    }
    if (!cb.generationCurrent(cb.userData) ||
        !cb.partyCurrent(cb.userData, receiver, receiverKey, receiverKind) ||
        (decision.payer == PayerKind::PrivateCompany &&
         !cb.isLivePrivate(cb.userData, reinterpret_cast<void*>(receiver))) ||
        !cb.isLivePrivate(cb.userData, reinterpret_cast<void*>(carrier)) ||
        !cb.partyCurrent(cb.userData, carrier, carrierKey, ReceiverKind::Private)) {
        result.reason = "identity_changed_before_debit";
        return result;
    }
    mutatePayer(-amount);
    const double payerAfter = payerRead();
    result.payerDelta = std::isfinite(payerAfter) ? payerBefore - payerAfter : 0.0;
    if (!DoFuelWithin(result.payerDelta, amount)) {
        result.payerRollbackVerified = rollbackPayer();
        result.reason = result.payerRollbackVerified
            ? "payer_debit_mismatch" : "payer_rollback_unverified";
        return result;
    }

    result.reason = "identity_changed_after_debit";
    if (!cb.generationCurrent(cb.userData) ||
        !cb.partyCurrent(cb.userData, receiver, receiverKey, receiverKind) ||
        (decision.payer == PayerKind::PrivateCompany &&
         !cb.isLivePrivate(cb.userData, reinterpret_cast<void*>(receiver))) ||
        !cb.isLivePrivate(cb.userData, reinterpret_cast<void*>(carrier)) ||
        !cb.partyCurrent(cb.userData, carrier, carrierKey, ReceiverKind::Private)) {
        result.payerRollbackVerified = rollbackPayer();
        if (!result.payerRollbackVerified) result.reason = "payer_rollback_unverified";
        return result;
    }

    result.reason = "generation_changed_before_credit";
    if (!cb.generationCurrent(cb.userData)) {
        result.payerRollbackVerified = rollbackPayer();
        if (!result.payerRollbackVerified) result.reason = "payer_rollback_unverified";
        return result;
    }
    payeeBefore = cb.getCapital(cb.userData, reinterpret_cast<void*>(carrier));
    if (!std::isfinite(payeeBefore)) {
        result.payerRollbackVerified = rollbackPayer();
        result.reason = result.payerRollbackVerified
            ? "do_unavailable_after_debit" : "payer_rollback_unverified";
        return result;
    }
    if (!cb.generationCurrent(cb.userData) ||
        !cb.partyCurrent(cb.userData, receiver, receiverKey, receiverKind) ||
        (decision.payer == PayerKind::PrivateCompany &&
         !cb.isLivePrivate(cb.userData, reinterpret_cast<void*>(receiver))) ||
        !cb.isLivePrivate(cb.userData, reinterpret_cast<void*>(carrier)) ||
        !cb.partyCurrent(cb.userData, carrier, carrierKey, ReceiverKind::Private)) {
        result.payerRollbackVerified = rollbackPayer();
        result.reason = result.payerRollbackVerified
            ? "identity_changed_after_debit" : "payer_rollback_unverified";
        return result;
    }
    cb.modifyCapital(cb.userData, reinterpret_cast<void*>(carrier),
                     amount, cb.payeeCategory);
    const double payeeAfter = cb.getCapital(cb.userData, reinterpret_cast<void*>(carrier));
    result.payeeDelta = std::isfinite(payeeAfter) ? payeeAfter - payeeBefore : 0.0;
    if (!DoFuelWithin(result.payeeDelta, amount)) {
        result.payeeRollbackVerified = rollbackPayee();
        result.payerRollbackVerified = rollbackPayer();
        result.reason = !result.payeeRollbackVerified || !result.payerRollbackVerified
            ? "credit_rollback_unverified" : "do_credit_mismatch";
        return result;
    }

    result.reason = "final_identity_changed";
    if (!cb.generationCurrent(cb.userData) ||
        !cb.partyCurrent(cb.userData, receiver, receiverKey, receiverKind) ||
        (decision.payer == PayerKind::PrivateCompany &&
         !cb.isLivePrivate(cb.userData, reinterpret_cast<void*>(receiver))) ||
        !cb.isLivePrivate(cb.userData, reinterpret_cast<void*>(carrier)) ||
        !cb.partyCurrent(cb.userData, carrier, carrierKey, ReceiverKind::Private)) {
        result.payeeRollbackVerified = rollbackPayee();
        result.payerRollbackVerified = rollbackPayer();
        if (!result.payeeRollbackVerified || !result.payerRollbackVerified)
            result.reason = "final_rollback_unverified";
        return result;
    }
    result.reason = "receipt_or_generation_changed";
    if (!cb.commit(cb.userData, decision.payer, amount)) {
        result.payeeRollbackVerified = rollbackPayee();
        result.payerRollbackVerified = rollbackPayer();
        if (!result.payeeRollbackVerified || !result.payerRollbackVerified)
            result.reason = "final_rollback_unverified";
        return result;
    }
    result.paid = true;
    result.reason = "paid";
    return result;
}

} // namespace private_logistics_step3b

#endif
