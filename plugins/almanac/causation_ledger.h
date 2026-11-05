#ifndef ALMANAC_CAUSATION_LEDGER_H
#define ALMANAC_CAUSATION_LEDGER_H

#include <cstddef>
#include <cstdint>
#include <cmath>
#include <cstring>

namespace causation_ledger {

constexpr std::size_t kMaxNodes = 256;
constexpr std::size_t kMaxEdges = 1024;
constexpr std::size_t kMaxSnapshotNodes = 256;
constexpr std::size_t kMaxSnapshotEdges = 1024;
constexpr std::size_t kMaxAggregates = 128;
constexpr std::size_t kSourceSize = 32;
constexpr std::size_t kKindSize = 32;
constexpr std::size_t kSubjectSize = 64;
constexpr std::size_t kUnitSize = 16;

enum class ValueClass : std::uint8_t {
    Actual = 0,
    DryRun = 1,
    Estimated = 2
};

enum class EvidenceStatus : std::uint8_t {
    Unknown = 0,
    Supplied = 1,
    Verified = 2,
    Unresolved = 3,
    Truncated = 4
};

enum class EdgeKind : std::uint8_t {
    Causes = 0,
    Accrues = 1,
    Settles = 2,
    MovesTreasury = 3,
    Estimates = 4,
    Pairs = 5,
    DependsOn = 6
};

struct EventNode {
    std::uint64_t eventId = 0;
    std::uint64_t correlationId = 0;
    std::uint64_t parentEventId = 0;
    std::uint64_t causalEdgeId = 0;
    double simTime = 0.0;
    std::uint64_t sequence = 0;
    char sourcePlugin[kSourceSize] = {};
    char sourceKind[kKindSize] = {};
    char subjectKey[kSubjectSize] = {};
    double amount = 0.0;
    char unit[kUnitSize] = {};
    int currency = 0;
    ValueClass valueClass = ValueClass::Actual;
    EvidenceStatus evidence = EvidenceStatus::Unknown;
    bool truncated = false;
    bool valid = false;
};

struct CausalEdge {
    std::uint64_t edgeId = 0;
    std::uint64_t fromEventId = 0;
    std::uint64_t toEventId = 0;
    EdgeKind kind = EdgeKind::Causes;
    EvidenceStatus evidence = EvidenceStatus::Unknown;
    bool truncated = false;
    bool valid = false;
};

struct Aggregate {
    ValueClass valueClass = ValueClass::Actual;
    int currency = 0;
    char unit[kUnitSize] = {};
    double totalAmount = 0.0;
    std::uint32_t count = 0;
    bool truncated = false;
};

struct SnapshotNode {
    std::uint64_t eventId = 0;
    std::uint64_t parentEventId = 0;
    std::uint64_t correlationId = 0;
    double simTime = 0.0;
    std::uint64_t sequence = 0;
    ValueClass valueClass = ValueClass::Actual;
    EvidenceStatus evidence = EvidenceStatus::Unknown;
    bool truncated = false;
    bool parentMissing = false;
    int depth = 0;
};

struct SnapshotEdge {
    std::uint64_t edgeId = 0;
    std::uint64_t fromEventId = 0;
    std::uint64_t toEventId = 0;
    EdgeKind kind = EdgeKind::Causes;
    EvidenceStatus evidence = EvidenceStatus::Unknown;
    bool truncated = false;
};

struct Snapshot {
    SnapshotNode nodes[kMaxSnapshotNodes] = {};
    std::size_t nodeCount = 0;
    SnapshotEdge edges[kMaxSnapshotEdges] = {};
    std::size_t edgeCount = 0;
    bool truncated = false;
    bool overflow = false;
};

class Ledger {
public:
    Ledger();

    // Lifecycle / reset
    void resetAll();
    bool observeTime(double simTime); // returns true if time rewound -> reset
    bool resetOnIdReuse(std::uint64_t newMaxId);

    // Insertion
    bool addNode(const EventNode& node);
    bool addEdge(const CausalEdge& edge);

    // Lookup
    bool hasNode(std::uint64_t eventId) const;
    const EventNode* getNode(std::uint64_t eventId) const;
    bool hasEdge(std::uint64_t edgeId) const;

    // Removal / eviction
    bool removeSubtree(std::uint64_t rootEventId);
    bool evictOldest(std::size_t count);

    // Traversal / snapshot
    // rootEventId == 0 means all roots (parent missing/0). maxDepth <= 0 means budget default.
    bool buildSnapshot(Snapshot& out, std::uint64_t rootEventId = 0,
                      int maxDepth = 64, std::size_t nodeBudget = kMaxSnapshotNodes,
                      std::size_t edgeBudget = kMaxSnapshotEdges) const;

    // Aggregation
    std::size_t aggregate(Aggregate* out, std::size_t capacity) const;
    std::size_t aggregateForRoot(std::uint64_t rootEventId, Aggregate* out,
                                 std::size_t capacity) const;

    // Status
    std::size_t nodeCount() const { return nodeCount_; }
    std::size_t edgeCount() const { return edgeCount_; }
    bool truncated() const { return truncated_; }
    bool overflow() const { return overflow_; }
    std::uint64_t lastSequence() const { return lastSequence_; }
    double lastSimTime() const { return lastSimTime_; }

private:
    EventNode nodes_[kMaxNodes];
    CausalEdge edges_[kMaxEdges];
    std::size_t nodeCount_ = 0;
    std::size_t edgeCount_ = 0;
    std::uint64_t lastSequence_ = 0;
    double lastSimTime_ = 0.0;
    bool haveTime_ = false;
    bool truncated_ = false;
    bool overflow_ = false;
    std::uint64_t maxEventId_ = 0;

    int findNodeIndex(std::uint64_t eventId) const;
    int findEdgeIndex(std::uint64_t edgeId) const;
    bool wouldCycleNode(std::uint64_t nodeId, std::uint64_t parentId) const;
    bool wouldCycleEdge(std::uint64_t fromId, std::uint64_t toId) const;
    bool finiteNode(const EventNode& n) const;
    bool finiteEdge(const CausalEdge& e) const;
    void resolveParentMissing();
};

// Helpers
inline bool Finite(double x) { return std::isfinite(x); }

inline void CopyStr(char* dst, std::size_t cap, const char* src) {
    if (!dst || cap == 0) return;
    if (!src) { dst[0] = '\0'; return; }
    std::size_t i = 0;
    for (; i + 1 < cap && src[i]; ++i) dst[i] = src[i];
    dst[i] = '\0';
}

} // namespace causation_ledger

#endif
