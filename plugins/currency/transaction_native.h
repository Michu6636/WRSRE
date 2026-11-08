// Native memory adapter. No STL/unwind objects in these SEH scopes (MSVC C2712).
#ifndef WRSRE_CURRENCY_TRANSACTION_NATIVE_H
#define WRSRE_CURRENCY_TRANSACTION_NATIVE_H

static uint64_t ReadPersonId(void* person)
{
    __try {
        if (!ValidPerson(person)) return 0;
        return TsmCitizenID(person);
    }
    __except (FaultFilter("currency person ID", GetExceptionInformation())) {
        return 0;
    }
}

// Called under currency's lock, after wallet allocation and validation. All
// native values are checked before writing. If a treasury store faults after
// the spent store, restore spent before returning failure. No observer here.
static bool CommitNative(void* person, float spentDelta,
                         double* treasury, double treasuryDelta)
{
    float* spent = person && spentDelta > 0.0f
        ? (float*)((BYTE*)person + PERSON_SPENT_OFFSET) : nullptr;
    float oldSpent = 0.0f;
    bool wroteSpent = false;
    __try {
        double nextTreasury = 0.0;
        if (treasury) {
            if (!ReadablePtr(treasury, sizeof(*treasury)) ||
                !std::isfinite(*treasury)) return false;
            nextTreasury = *treasury + treasuryDelta;
            if (!std::isfinite(nextTreasury) ||
                (treasuryDelta != 0.0 && nextTreasury == *treasury)) return false;
        }
        float nextSpent = 0.0f;
        if (spent) {
            if (!ReadablePtr(spent, sizeof(*spent))) return false;
            oldSpent = *spent;
            nextSpent = oldSpent + spentDelta;
            if (!std::isfinite(oldSpent) || !std::isfinite(nextSpent)) return false;
        }
        if (spent) { *spent = nextSpent; wroteSpent = true; }
        if (treasury) *treasury = nextTreasury;
        return true;
    }
    __except (FaultFilter("currency native transaction", GetExceptionInformation())) {}

    // A stale/unwritable game field must not escape with the currency lock held.
    if (wroteSpent) {
        __try { *spent = oldSpent; }
        __except (FaultFilter("currency spent rollback", GetExceptionInformation())) {
            Logf("currency  ERROR native spent rollback failed; game memory became unavailable");
        }
    }
    return false;
}

static double ReadTreasuryNative(const double* treasury)
{
    if (!treasury) return 0.0;
    __try {
        if (!ReadablePtr(treasury, sizeof(*treasury))) return 0.0;
        return std::isfinite(*treasury) ? *treasury : 0.0;
    }
    __except (FaultFilter("currency treasury read", GetExceptionInformation())) {
        return 0.0;
    }
}
#endif
