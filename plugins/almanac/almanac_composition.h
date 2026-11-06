#ifndef TSM_ALMANAC_COMPOSITION_H
#define TSM_ALMANAC_COMPOSITION_H

#include "almanac_shell.h"
#include "causation_ledger.h"
#include "statistics_history.h"
#include "tesmio_api.h"

#include <cstddef>

namespace almanac {

// Owns the single mod-owned shell and all provider registrations. Construct
// from the plugin Start phase, after dependencies have published their services.
// This class does not install hooks, register a plugin entry point, or enable a
// native host. The caller still supplies and validates AlmanacHostSurface data.
class AlmanacComposition {
public:
    explicit AlmanacComposition(const TsmHost* host);
    ~AlmanacComposition();

    AlmanacComposition(const AlmanacComposition&) = delete;
    AlmanacComposition& operator=(const AlmanacComposition&) = delete;

    bool ready() const;
    AlmanacShell* shell() const;

    // Simulation/update-thread operations. Service calls happen here, not in
    // statistics/causation page population. Invalid clock data safely clears a
    // previously valid derived snapshot. Backward time resets derived history.
    bool updateStatistics();
    bool recordStatisticsHistory();
    std::size_t statisticsHistoryCount() const;

    // Explicit save/world lifecycle boundary. Clears derived statistics,
    // history, and causation data; never mutates authoritative economy state.
    void reset(ResetReason reason);

    // Explicit causation ingestion for existing public observers. No observer
    // is registered by this composition yet. Calls are serialized with page
    // population. observeCausationTime returns true only when time rewound.
    bool addCausationNode(const causation_ledger::EventNode& node);
    bool addCausationEdge(const causation_ledger::CausalEdge& edge);
    bool observeCausationTime(double simTime);

    // Idempotent. Unregisters every provider before destroying the shell.
    void shutdown();

private:
    struct Impl;
    Impl* impl_ = nullptr;
};

} // namespace almanac

#endif // TSM_ALMANAC_COMPOSITION_H
