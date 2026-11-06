#ifndef TSM_ALMANAC_PAGE_PROVIDERS_H
#define TSM_ALMANAC_PAGE_PROVIDERS_H

#include "almanac_ui_api.h"
#include "causation_ledger.h"
#include "statistics_history.h"

#include <mutex>

namespace almanac {

// The composition owns these contexts for at least as long as their shell
// registrations. Population copies bounded snapshots; it does not retain
// service pointers or source locks after return.
struct StatisticsPageContext {
    StatisticsHistory* statistics = nullptr;
    bool alive = true;
};

struct CausationPageContext {
    causation_ledger::Ledger* ledger = nullptr;
    std::mutex* mutex = nullptr;
    bool alive = true;
};

int populateStatisticsPage(const AlmanacHostSurface* host,
                           AlmanacPageSnapshot* out, void* userData);
int populateCausationPage(const AlmanacHostSurface* host,
                          AlmanacPageSnapshot* out, void* userData);

} // namespace almanac

#endif // TSM_ALMANAC_PAGE_PROVIDERS_H
