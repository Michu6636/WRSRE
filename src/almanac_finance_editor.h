// Mod-owned Apply/Cancel policy. Single UI thread; no native hooks or service ABI.
#pragma once
#include "almanac_finance_snapshot.h"
#include <limits>

namespace almanac {
struct FinanceDraft {
    double value[3]{}; // VAT/profit in PERCENT [0,100]; property amount/enterprise.
    bool enabled[3]{};
};
enum class EditLoadStatus { Loaded, Busy, ServiceUnavailable, SnapshotUnavailable, CurrencyUnavailable };
enum class EditApplyStatus {
    Applied, NoChanges, NotLoaded, Busy, InvalidDraft, ServiceUnavailable,
    SnapshotUnavailable, Conflict, Rejected, PartiallyApplied,
    ReadbackFailed, VerificationMismatch
};
enum class EditWriteStatus { Unchanged, Accepted, Rejected, SkippedDependency };
struct FinanceApplyResult {
    EditApplyStatus status = EditApplyStatus::NotLoaded;
    EditWriteStatus value[3]{};
    EditWriteStatus enabled[3]{};
    unsigned invalidValues = 0; // bit 1<<TSM_TAX_* identifies invalid draft values
    unsigned accepted = 0, rejected = 0;
    FinanceSnapshot observed{}; // check observedValid before displaying as readback
    bool observedValid = false;
};

// Own for the UI session; never retain provider pointers. Caller keeps services
// alive during each operation. See tests/almanac_button/FINANCE_EDITOR.md.
class FinanceEditor {
public:
    FinanceEditor() = default;
    FinanceEditor(const FinanceEditor&) = delete;
    FinanceEditor& operator=(const FinanceEditor&) = delete;
    EditLoadStatus Load(const TsmCurrencyApi* currency, const TsmTaxesApi* taxes) {
        if (busy_) return EditLoadStatus::Busy;
        Guard guard(busy_);
        active_ = false;
        draft_ = {};
        baseline_ = {};
        if (!TsmTaxesHasPolicySnapshot(taxes)) return EditLoadStatus::ServiceUnavailable;
        const auto s = BuildFinanceSnapshot(currency, taxes);
        if (!ValidPolicy(s)) return EditLoadStatus::SnapshotUnavailable;
        if (!s.activeCurrency.Available()) return EditLoadStatus::CurrencyUnavailable;
        baseline_ = s;
        draft_.value[0] = s.vat.configuredFraction.value * 100.0;
        draft_.value[1] = s.profit.configuredFraction.value * 100.0;
        draft_.value[2] = s.property.baseAmountPerEnterprise.value;
        draft_.enabled[0] = s.vat.enabled.value;
        draft_.enabled[1] = s.profit.enabled.value;
        draft_.enabled[2] = s.property.enabled.value;
        active_ = true;
        return EditLoadStatus::Loaded;
    }
    bool Active() const { return active_; }
    FinanceDraft Draft() const { return draft_; }
    // Invalid numeric drafts are retained for UI error feedback, never clamped.
    bool SetValue(int kind, double value) {
        if (!active_ || busy_ || kind < 0 || kind > 2) return false;
        draft_.value[kind] = value;
        return true;
    }
    bool SetEnabled(int kind, bool enabled) {
        if (!active_ || busy_ || kind < 0 || kind > 2) return false;
        draft_.enabled[kind] = enabled;
        return true;
    }
    bool Cancel() {
        if (busy_) return false;
        active_ = false;
        draft_ = {};
        return true; // purely local: no service reads/writes
    }
    FinanceApplyResult Apply(const TsmCurrencyApi* currency, const TsmTaxesApi* taxes);
private:
    struct Guard {
        bool& flag;
        explicit Guard(bool& value) : flag(value) { flag = true; }
        ~Guard() { flag = false; }
    };
    static bool ValidPolicy(const FinanceSnapshot& s) {
        return s.vat.enabled.Available() && s.profit.enabled.Available() &&
            s.property.enabled.Available() && s.vat.configuredFraction.Available() &&
            s.profit.configuredFraction.Available() &&
            s.property.baseAmountPerEnterprise.Available() && s.property.rule.Available();
    }
    static double Configured(const FinanceSnapshot& s, int i) {
        return i == 0 ? s.vat.configuredFraction.value :
            i == 1 ? s.profit.configuredFraction.value : s.property.baseAmountPerEnterprise.value;
    }
    static bool Enabled(const FinanceSnapshot& s, int i) {
        return i == 0 ? s.vat.enabled.value : i == 1 ? s.profit.enabled.value : s.property.enabled.value;
    }
    FinanceSnapshot baseline_{};
    FinanceDraft draft_{};
    bool active_ = false, busy_ = false;
};
} // namespace almanac

namespace almanac {
inline FinanceApplyResult FinanceEditor::Apply(const TsmCurrencyApi* currency,
                                               const TsmTaxesApi* taxes) {
    FinanceApplyResult r;
    if (busy_) { r.status = EditApplyStatus::Busy; return r; }
    if (!active_) return r;
    Guard guard(busy_);
    float target[3]{};
    bool valueChanged[3]{}, enabledChanged[3]{};
    bool anyValue = false, anyEnabled = false;
    for (int i = 0; i < 3; ++i) {
        const double v = i < 2 ? draft_.value[i] / 100.0 : draft_.value[i];
        // Provider property setter currently caps NEW amounts at 1e6. A
        // higher existing configured amount can be left alone, not rewritten.
        if (!std::isfinite(v) || draft_.value[i] < 0 ||
            (i < 2 && v > 1) || v > std::numeric_limits<float>::max() ||
            (i == 2 && v > 1.0e6 && v != Configured(baseline_, i))) {
            r.invalidValues |= 1u << i;
            continue;
        }
        target[i] = static_cast<float>(v);
        valueChanged[i] = target[i] != static_cast<float>(Configured(baseline_, i));
        enabledChanged[i] = draft_.enabled[i] != Enabled(baseline_, i);
        anyValue = anyValue || valueChanged[i];
        anyEnabled = anyEnabled || enabledChanged[i];
    }
    if (r.invalidValues) { r.status = EditApplyStatus::InvalidDraft; return r; }
    if (!anyValue && !anyEnabled) { r.status = EditApplyStatus::NoChanges; return r; }
    if (!TsmTaxesHasPolicySnapshot(taxes) ||
        (anyValue && !taxes->setTaxRate) || (anyEnabled && !taxes->setTaxEnabled)) {
        r.status = EditApplyStatus::ServiceUnavailable;
        return r;
    }
    const auto current = BuildFinanceSnapshot(currency, taxes);
    if (!ValidPolicy(current) || !current.activeCurrency.Available()) {
        r.status = EditApplyStatus::SnapshotUnavailable;
        return r;
    }
    bool conflict = current.activeCurrency.value != baseline_.activeCurrency.value;
    for (int i = 0; i < 3; ++i) {
        if (valueChanged[i] || enabledChanged[i])
            conflict = conflict || Configured(current, i) != Configured(baseline_, i) ||
                Enabled(current, i) != Enabled(baseline_, i);
    }
    if (valueChanged[2] || enabledChanged[2])
        conflict = conflict || current.property.rule.value != baseline_.property.rule.value;
    if (conflict) {
        active_ = false; // reload explicitly instead of overwriting changed policy
        r.status = EditApplyStatus::Conflict;
        r.observed = current; r.observedValid = true;
        return r;
    }
    // Each field is a separate provider operation, NOT an atomic transaction.
    // Once writes start, require Load before another Apply (no blind retries).
    active_ = false;
    for (int i = 0; i < 3; ++i) {
        if (valueChanged[i]) {
            const bool ok = taxes->setTaxRate(i, target[i]) == 1;
            r.value[i] = ok ? EditWriteStatus::Accepted : EditWriteStatus::Rejected;
            if (ok) ++r.accepted; else ++r.rejected;
        }
        if (enabledChanged[i]) {
            if (r.value[i] == EditWriteStatus::Rejected) {
                r.enabled[i] = EditWriteStatus::SkippedDependency;
            } else {
                const bool ok = taxes->setTaxEnabled(i, draft_.enabled[i] ? 1 : 0) == 1;
                r.enabled[i] = ok ? EditWriteStatus::Accepted : EditWriteStatus::Rejected;
                if (ok) ++r.accepted; else ++r.rejected;
            }
        }
    }
    r.observed = BuildFinanceSnapshot(currency, taxes);
    r.observedValid = ValidPolicy(r.observed) && r.observed.activeCurrency.Available();
    if (!r.observedValid) { r.status = EditApplyStatus::ReadbackFailed; return r; }
    bool verified = r.observed.activeCurrency.value == baseline_.activeCurrency.value;
    if (valueChanged[2] || enabledChanged[2])
        verified = verified && r.observed.property.rule.value == baseline_.property.rule.value;
    for (int i = 0; i < 3; ++i) {
        if (r.value[i] == EditWriteStatus::Accepted)
            verified = verified && Configured(r.observed, i) == double(target[i]);
        if (r.enabled[i] == EditWriteStatus::Accepted)
            verified = verified && Enabled(r.observed, i) == draft_.enabled[i];
    }
    if (!verified) r.status = EditApplyStatus::VerificationMismatch;
    else if (r.rejected) r.status = r.accepted ? EditApplyStatus::PartiallyApplied : EditApplyStatus::Rejected;
    else r.status = EditApplyStatus::Applied;
    return r;
}
} // namespace almanac
