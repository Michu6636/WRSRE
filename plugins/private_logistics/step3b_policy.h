#ifndef PRIVATE_LOGISTICS_STEP3B_POLICY_H
#define PRIVATE_LOGISTICS_STEP3B_POLICY_H

namespace private_logistics_step3b {

enum class BillingState { Off, DryRun, Live };
enum class ReceiverKind { Unknown, Private, State, Customs };
enum class PayerKind { None, PrivateCompany, Treasury };

struct PayerDecision {
    PayerKind payer = PayerKind::None;
    const char* reason = "direction_unknown";
    const char* treasuryLabel = nullptr;
};

inline BillingState BillingMode(bool billing, bool dryRun) {
    if (!billing) return BillingState::Off;
    return dryRun ? BillingState::DryRun : BillingState::Live;
}

inline const char* BillingStateName(bool billing, bool dryRun) {
    switch (BillingMode(billing, dryRun)) {
    case BillingState::Off: return "off";
    case BillingState::DryRun: return "dry_run";
    case BillingState::Live: return "live";
    }
    return "off";
}

inline PayerDecision ChoosePayer(ReceiverKind receiver) {
    switch (receiver) {
    case ReceiverKind::Private:
        return {PayerKind::PrivateCompany, "private_receiver_pays", nullptr};
    case ReceiverKind::State:
        return {PayerKind::Treasury, "state_receiver_treasury_pays", "b2b_state_do_fuel"};
    case ReceiverKind::Customs:
        return {PayerKind::None, "customs_export_deferred", nullptr};
    case ReceiverKind::Unknown:
        return {PayerKind::None, "direction_unknown", nullptr};
    }
    return {};
}

} // namespace private_logistics_step3b

#endif
