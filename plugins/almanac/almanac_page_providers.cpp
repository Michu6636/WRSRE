#include "almanac_page_providers.h"

#include <cmath>
#include <cstdio>
#include <cstring>

namespace {

void copyText(char* dst, std::size_t capacity, const char* src) {
    if (!dst || capacity == 0) return;
    if (!src) {
        dst[0] = '\0';
        return;
    }
    std::size_t i = 0;
    for (; i + 1 < capacity && src[i]; ++i) dst[i] = src[i];
    dst[i] = '\0';
}

bool addRow(AlmanacPageSnapshot* out, const char* label, const char* value,
            AlmanacValueClass valueClass, int available, int stale,
            int currency, const char* unit, std::uint64_t updatedAt) {
    if (!out || out->rowCount >= ALMANAC_MAX_ROWS) return false;
    AlmanacDisplayRow& row = out->rows[out->rowCount++];
    row = AlmanacDisplayRow{};
    copyText(row.label, sizeof(row.label), label);
    copyText(row.value, sizeof(row.value), value);
    copyText(row.unit, sizeof(row.unit), unit);
    row.currency = currency;
    row.valueClass = valueClass;
    row.available = available;
    row.stale = stale;
    row.updatedAt = updatedAt;
    return true;
}

const char* valueClassName(int valueClass) {
    switch (valueClass) {
        case almanac::VC_ACTUAL: return "ACTUAL";
        case almanac::VC_DRY_RUN: return "DRY_RUN";
        case almanac::VC_ESTIMATED: return "ESTIMATED";
        default: return "UNKNOWN";
    }
}

AlmanacValueClass toUiValueClass(int valueClass) {
    switch (valueClass) {
        case almanac::VC_ACTUAL: return ALMANAC_VALUE_ACTUAL;
        case almanac::VC_DRY_RUN: return ALMANAC_VALUE_DRY_RUN;
        case almanac::VC_ESTIMATED: return ALMANAC_VALUE_ESTIMATED;
        default: return ALMANAC_VALUE_INVALID;
    }
}

bool metricOnPage(almanac::MetricId id, AlmanacPageId page) {
    using namespace almanac;
    switch (page) {
        case ALMANAC_PAGE_OVERVIEW:
            return id >= MID_OVERVIEW_TREASURY && id <= MID_OVERVIEW_ENTERPRISE_COUNT;
        case ALMANAC_PAGE_MONEY_FLOW:
            return id >= MID_FLOW_WAGES && id <= MID_FLOW_UNEXPLAINED;
        case ALMANAC_PAGE_PRIVATE_ECONOMY:
            return id >= MID_PRIVATE_SALES && id <= MID_PRIVATE_RENT_MONTHLY;
        case ALMANAC_PAGE_PRICES_CITIZENS:
            return id >= MID_PRICE_FOOD && id <= MID_CITIZEN_COUNT;
        case ALMANAC_PAGE_LOGISTICS_PRODUCTION:
            return id >= MID_LOGISTICS_FUEL_CONSUMED && id <= MID_PRODUCTION_OUTPUT;
        default:
            return false;
    }
}

void formatMetricValue(const almanac::Metric& metric, char* out, std::size_t capacity) {
    if (!out || capacity == 0) return;
    const bool integral = std::strcmp(metric.unit, "count") == 0 ||
                          std::strcmp(metric.unit, "flag") == 0;
    const bool money = std::strcmp(metric.unit, "RUB") == 0 ||
                       std::strcmp(metric.unit, "USD") == 0;
    if (integral) {
        std::snprintf(out, capacity, "%.0f", metric.value);
    } else if (money) {
        std::snprintf(out, capacity, "%.2f", metric.value);
    } else {
        std::snprintf(out, capacity, "%.6g", metric.value);
    }
}

} // namespace

namespace almanac {

int populateStatisticsPage(const AlmanacHostSurface* host,
                           AlmanacPageSnapshot* out, void* userData) {
    (void)host;
    if (!out) return 0;
    StatisticsPageContext* ctx = static_cast<StatisticsPageContext*>(userData);
    if (!ctx || !ctx->alive || !ctx->statistics) return 0;

    out->populated = 1;
    out->coherent = 1;
    out->rowCount = 0;
    out->available = 0;
    out->stale = 0;

    const Snapshot snapshot = ctx->statistics->getSnapshot();
    for (std::size_t i = 0; i < snapshot.metricCount; ++i) {
        const Metric& metric = snapshot.metrics[i];
        if (!metricOnPage(metric.id, out->page)) continue;

        char label[ALMANAC_TEXT_LEN];
        char value[ALMANAC_TEXT_LEN];
        std::snprintf(label, sizeof(label), "[%s] %s",
                      valueClassName(metric.valueClass), metric.idString);
        const int available = metric.available && metric.hasValue &&
                              metric.valueFinite && std::isfinite(metric.value);
        if (available) {
            formatMetricValue(metric, value, sizeof(value));
            if (metric.stale) {
                const std::size_t used = std::strlen(value);
                if (used < sizeof(value)) {
                    std::snprintf(value + used, sizeof(value) - used, " (stale)");
                }
            }
        } else {
            copyText(value, sizeof(value), "unavailable");
        }

        addRow(out, label, value, toUiValueClass(metric.valueClass), available,
               metric.stale, metric.currency, metric.unit, snapshot.sequence);
        out->available = out->available || available;
        out->stale = out->stale || metric.stale;
    }

    if (out->rowCount == 0) {
        copyText(out->emptyState, sizeof(out->emptyState),
                 snapshot.metricCount == 0
                     ? "Statistics have not been updated"
                     : "No statistics are assigned to this page");
    }
    return 1;
}

namespace {

const char* evidenceName(causation_ledger::EvidenceStatus evidence) {
    using causation_ledger::EvidenceStatus;
    switch (evidence) {
        case EvidenceStatus::Supplied: return "supplied";
        case EvidenceStatus::Verified: return "verified";
        case EvidenceStatus::Unresolved: return "unresolved";
        case EvidenceStatus::Truncated: return "truncated";
        case EvidenceStatus::Unknown:
        default: return "unknown";
    }
}

const char* edgeKindName(causation_ledger::EdgeKind kind) {
    using causation_ledger::EdgeKind;
    switch (kind) {
        case EdgeKind::Accrues: return "accrues";
        case EdgeKind::Settles: return "settles";
        case EdgeKind::MovesTreasury: return "moves-treasury";
        case EdgeKind::Estimates: return "estimates";
        case EdgeKind::Pairs: return "pairs";
        case EdgeKind::DependsOn: return "depends-on";
        case EdgeKind::Causes: return "causes";
        default: return "unknown";
    }
}

void appendText(char* dst, std::size_t capacity, const char* text) {
    if (!dst || capacity == 0 || !text) return;
    const std::size_t used = std::strlen(dst);
    if (used < capacity) std::snprintf(dst + used, capacity - used, "%s", text);
}

} // namespace

int populateCausationPage(const AlmanacHostSurface* host,
                          AlmanacPageSnapshot* out, void* userData) {
    (void)host;
    if (!out) return 0;
    CausationPageContext* ctx = static_cast<CausationPageContext*>(userData);
    if (!ctx || !ctx->alive || !ctx->ledger || !ctx->mutex ||
        out->page != ALMANAC_PAGE_CAUSATION) return 0;

    out->populated = 1;
    out->coherent = 1;
    out->rowCount = 0;
    out->available = 0;
    out->stale = 0;

    std::lock_guard<std::mutex> lock(*ctx->mutex);
    causation_ledger::Ledger& ledger = *ctx->ledger;
    const std::size_t totalNodes = ledger.nodeCount();
    const std::size_t totalEdges = ledger.edgeCount();
    const std::uint64_t updatedAt = ledger.lastSequence();
    if (totalNodes == 0) {
        copyText(out->emptyState, sizeof(out->emptyState), "No causation events");
        return 1;
    }

    causation_ledger::Snapshot snapshot;
    if (!ledger.buildSnapshot(snapshot)) return 0;

    // Reserve diagnostics before event/edge rows so a full display budget can
    // never hide truncation or missing-parent information.
    addRow(out, "Causation snapshot", "bounded snapshot", ALMANAC_VALUE_ACTUAL,
           1, 0, -1, "", updatedAt);
    addRow(out, "Events shown", "", ALMANAC_VALUE_ACTUAL,
           1, 0, -1, "count", updatedAt);
    addRow(out, "Edges shown", "", ALMANAC_VALUE_ACTUAL,
           1, 0, -1, "count", updatedAt);

    std::size_t missingParents = 0;
    for (std::size_t i = 0; i < snapshot.nodeCount; ++i) {
        if (snapshot.nodes[i].parentMissing) ++missingParents;
    }
    char diagnostic[ALMANAC_TEXT_LEN];
    std::snprintf(diagnostic, sizeof(diagnostic), "%llu",
                  static_cast<unsigned long long>(missingParents));
    addRow(out, "Missing parents", diagnostic, ALMANAC_VALUE_ACTUAL,
           1, 0, -1, "count", updatedAt);

    std::size_t shownNodes = 0;
    for (std::size_t i = 0; i < snapshot.nodeCount &&
                            out->rowCount < ALMANAC_MAX_ROWS; ++i) {
        const causation_ledger::SnapshotNode& node = snapshot.nodes[i];
        const causation_ledger::EventNode* detail = ledger.getNode(node.eventId);
        if (!detail) continue;

        char label[ALMANAC_TEXT_LEN];
        char value[ALMANAC_TEXT_LEN];
        const char* plugin = detail->sourcePlugin[0] ? detail->sourcePlugin : "unknown";
        const char* kind = detail->sourceKind[0] ? detail->sourceKind : "event";
        std::snprintf(label, sizeof(label), "[%s] %s.%s",
                      valueClassName(static_cast<int>(detail->valueClass)), plugin, kind);
        std::snprintf(value, sizeof(value), "#%llu @ %.6g",
                      static_cast<unsigned long long>(node.eventId), node.simTime);
        if (detail->unit[0]) {
            char amount[64];
            std::snprintf(amount, sizeof(amount), " %.6g %s", detail->amount, detail->unit);
            appendText(value, sizeof(value), amount);
        }
        appendText(value, sizeof(value), " [");
        appendText(value, sizeof(value), evidenceName(detail->evidence));
        appendText(value, sizeof(value), "]");
        if (node.parentMissing) appendText(value, sizeof(value), " missing-parent");

        const int stale = (node.truncated || detail->truncated) ? 1 : 0;
        if (!addRow(out, label, value, toUiValueClass(static_cast<int>(detail->valueClass)),
                    1, stale, detail->currency, detail->unit, updatedAt)) break;
        ++shownNodes;
        out->stale = out->stale || stale;
    }

    std::size_t shownEdges = 0;
    for (std::size_t i = 0; i < snapshot.edgeCount &&
                            out->rowCount < ALMANAC_MAX_ROWS; ++i) {
        const causation_ledger::SnapshotEdge& edge = snapshot.edges[i];
        char label[ALMANAC_TEXT_LEN];
        char value[ALMANAC_TEXT_LEN];
        std::snprintf(label, sizeof(label), "edge %llu %s [%s]",
                      static_cast<unsigned long long>(edge.edgeId),
                      edgeKindName(edge.kind), evidenceName(edge.evidence));
        std::snprintf(value, sizeof(value), "%llu -> %llu",
                      static_cast<unsigned long long>(edge.fromEventId),
                      static_cast<unsigned long long>(edge.toEventId));
        const int stale = edge.truncated ? 1 : 0;
        if (!addRow(out, label, value, ALMANAC_VALUE_ACTUAL, 1, stale,
                    -1, "", updatedAt)) break;
        ++shownEdges;
        out->stale = out->stale || stale;
    }

    const bool sourceIncomplete = snapshot.truncated || snapshot.overflow ||
                                  ledger.truncated() || ledger.overflow();
    const bool displayIncomplete = shownNodes < totalNodes || shownEdges < totalEdges;
    const bool incomplete = sourceIncomplete || displayIncomplete;
    const char* status = "bounded snapshot";
    if (sourceIncomplete && displayIncomplete) status = "incomplete: truncated/display-limited";
    else if (sourceIncomplete) status = "incomplete: truncated evidence";
    else if (displayIncomplete) status = "incomplete: display budget";
    copyText(out->rows[0].value, sizeof(out->rows[0].value), status);
    out->rows[0].stale = incomplete ? 1 : 0;

    std::snprintf(diagnostic, sizeof(diagnostic), "%llu / %llu",
                  static_cast<unsigned long long>(shownNodes),
                  static_cast<unsigned long long>(totalNodes));
    copyText(out->rows[1].value, sizeof(out->rows[1].value), diagnostic);
    std::snprintf(diagnostic, sizeof(diagnostic), "%llu / %llu",
                  static_cast<unsigned long long>(shownEdges),
                  static_cast<unsigned long long>(totalEdges));
    copyText(out->rows[2].value, sizeof(out->rows[2].value), diagnostic);

    out->available = 1;
    out->stale = out->stale || incomplete;
    return 1;
}

} // namespace almanac
