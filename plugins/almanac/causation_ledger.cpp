#include "causation_ledger.h"

#include <algorithm>

namespace causation_ledger {

Ledger::Ledger() {
    resetAll();
}

void Ledger::resetAll() {
    for (std::size_t i = 0; i < kMaxNodes; ++i) nodes_[i] = EventNode{};
    for (std::size_t i = 0; i < kMaxEdges; ++i) edges_[i] = CausalEdge{};
    nodeCount_ = 0;
    edgeCount_ = 0;
    lastSequence_ = 0;
    lastSimTime_ = 0.0;
    haveTime_ = false;
    truncated_ = false;
    overflow_ = false;
    maxEventId_ = 0;
}

bool Ledger::observeTime(double simTime) {
    if (!std::isfinite(simTime)) return false;
    if (!haveTime_) {
        haveTime_ = true;
        lastSimTime_ = simTime;
        return false;
    }
    if (simTime < lastSimTime_ - 1e-9) {
        resetAll();
        haveTime_ = true;
        lastSimTime_ = simTime;
        return true;
    }
    lastSimTime_ = simTime;
    return false;
}

bool Ledger::resetOnIdReuse(std::uint64_t newMaxId) {
    if (maxEventId_ > 0 && newMaxId <= maxEventId_) {
        resetAll();
        return true;
    }
    if (newMaxId > maxEventId_) maxEventId_ = newMaxId;
    return false;
}

int Ledger::findNodeIndex(std::uint64_t eventId) const {
    if (eventId == 0) return -1;
    for (std::size_t i = 0; i < nodeCount_; ++i) {
        if (nodes_[i].valid && nodes_[i].eventId == eventId) return static_cast<int>(i);
    }
    return -1;
}

int Ledger::findEdgeIndex(std::uint64_t edgeId) const {
    if (edgeId == 0) return -1;
    for (std::size_t i = 0; i < edgeCount_; ++i) {
        if (edges_[i].valid && edges_[i].edgeId == edgeId) return static_cast<int>(i);
    }
    return -1;
}

bool Ledger::wouldCycleNode(std::uint64_t nodeId, std::uint64_t parentId) const {
    if (nodeId == 0 || parentId == 0) return false;
    std::uint64_t cur = parentId;
    for (std::size_t guard = 0; guard < kMaxNodes; ++guard) {
        if (cur == nodeId) return true;
        int idx = findNodeIndex(cur);
        if (idx < 0) break;
        cur = nodes_[static_cast<std::size_t>(idx)].parentEventId;
        if (cur == 0) break;
    }
    return false;
}

bool Ledger::wouldCycleEdge(std::uint64_t fromId, std::uint64_t toId) const {
    if (fromId == 0 || toId == 0) return false;
    if (fromId == toId) return true;
    // DFS from toId over outgoing edges; if reaches fromId => cycle.
    std::uint64_t visited[kMaxNodes];
    std::size_t visitedCount = 0;
    std::uint64_t stack[kMaxNodes];
    std::size_t sp = 0;
    stack[sp++] = toId;
    visited[visitedCount++] = toId;
    while (sp > 0) {
        std::uint64_t cur = stack[--sp];
        // find outgoing edges
        for (std::size_t e = 0; e < edgeCount_; ++e) {
            if (!edges_[e].valid) continue;
            if (edges_[e].fromEventId != cur) continue;
            std::uint64_t nxt = edges_[e].toEventId;
            if (nxt == fromId) return true;
            bool seen = false;
            for (std::size_t v = 0; v < visitedCount; ++v) {
                if (visited[v] == nxt) { seen = true; break; }
            }
            if (!seen && visitedCount < kMaxNodes) {
                visited[visitedCount++] = nxt;
                if (sp < kMaxNodes) stack[sp++] = nxt;
            }
        }
    }
    return false;
}

bool Ledger::finiteNode(const EventNode& n) const {
    if (n.eventId == 0) return false;
    if (!std::isfinite(n.simTime)) return false;
    if (!std::isfinite(n.amount)) return false;
    return true;
}

bool Ledger::finiteEdge(const CausalEdge& e) const {
    if (e.edgeId == 0) return false;
    if (e.fromEventId == 0 || e.toEventId == 0) return false;
    return true;
}

bool Ledger::addNode(const EventNode& node) {
    if (!finiteNode(node)) return false;
    if (findNodeIndex(node.eventId) >= 0) return false; // duplicate
    if (node.parentEventId != 0 && wouldCycleNode(node.eventId, node.parentEventId)) return false;
    if (nodeCount_ >= kMaxNodes) {
        overflow_ = true;
        truncated_ = true;
        return false;
    }
    EventNode copy = node;
    copy.valid = true;
    if (copy.sequence == 0) {
        copy.sequence = ++lastSequence_;
    } else {
        if (copy.sequence > lastSequence_) lastSequence_ = copy.sequence;
    }
    if (copy.eventId > maxEventId_) maxEventId_ = copy.eventId;
    nodes_[nodeCount_++] = copy;
    return true;
}

bool Ledger::addEdge(const CausalEdge& edge) {
    if (!finiteEdge(edge)) return false;
    if (findEdgeIndex(edge.edgeId) >= 0) return false; // duplicate
    if (findNodeIndex(edge.fromEventId) < 0) return false; // missing node
    if (findNodeIndex(edge.toEventId) < 0) return false; // missing node
    if (wouldCycleEdge(edge.fromEventId, edge.toEventId)) return false;
    if (edgeCount_ >= kMaxEdges) {
        overflow_ = true;
        truncated_ = true;
        return false;
    }
    CausalEdge copy = edge;
    copy.valid = true;
    edges_[edgeCount_++] = copy;
    return true;
}

bool Ledger::hasNode(std::uint64_t eventId) const {
    return findNodeIndex(eventId) >= 0;
}

const EventNode* Ledger::getNode(std::uint64_t eventId) const {
    int idx = findNodeIndex(eventId);
    if (idx < 0) return nullptr;
    return &nodes_[static_cast<std::size_t>(idx)];
}

bool Ledger::hasEdge(std::uint64_t edgeId) const {
    return findEdgeIndex(edgeId) >= 0;
}

bool Ledger::removeSubtree(std::uint64_t rootEventId) {
    int rootIdx = findNodeIndex(rootEventId);
    if (rootIdx < 0) return false;

    // Collect removed node ids.
    bool removedNode[kMaxNodes];
    for (std::size_t i = 0; i < kMaxNodes; ++i) removedNode[i] = false;
    // Mark root.
    // Map node index -> removed. We'll use a list of ids.
    std::uint64_t removedIds[kMaxNodes];
    std::size_t removedCount = 0;
    removedIds[removedCount++] = rootEventId;

    // Expand: any node whose parent chain leads to a removed id.
    bool changed = true;
    while (changed) {
        changed = false;
        for (std::size_t i = 0; i < nodeCount_; ++i) {
            if (!nodes_[i].valid) continue;
            std::uint64_t pid = nodes_[i].parentEventId;
            // if pid in removed set
            bool inRemoved = false;
            for (std::size_t r = 0; r < removedCount; ++r) {
                if (removedIds[r] == pid) { inRemoved = true; break; }
            }
            // also node itself already removed?
            bool already = false;
            for (std::size_t r = 0; r < removedCount; ++r) {
                if (removedIds[r] == nodes_[i].eventId) { already = true; break; }
            }
            if (inRemoved && !already && removedCount < kMaxNodes) {
                removedIds[removedCount++] = nodes_[i].eventId;
                changed = true;
            }
        }
    }

    // Mark removedNode indices.
    for (std::size_t i = 0; i < nodeCount_; ++i) {
        for (std::size_t r = 0; r < removedCount; ++r) {
            if (nodes_[i].eventId == removedIds[r]) { removedNode[i] = true; break; }
        }
    }

    // Remove edges referencing removed nodes.
    std::size_t newEdgeCount = 0;
    for (std::size_t i = 0; i < edgeCount_; ++i) {
        if (!edges_[i].valid) continue;
        bool refRemoved = false;
        for (std::size_t r = 0; r < removedCount; ++r) {
            if (edges_[i].fromEventId == removedIds[r] || edges_[i].toEventId == removedIds[r]) {
                refRemoved = true; break;
            }
        }
        if (!refRemoved) {
            edges_[newEdgeCount++] = edges_[i];
        }
    }
    edgeCount_ = newEdgeCount;
    // Clear tail.
    for (std::size_t i = edgeCount_; i < kMaxEdges; ++i) edges_[i] = CausalEdge{};

    // Compact nodes.
    std::size_t newCount = 0;
    for (std::size_t i = 0; i < nodeCount_; ++i) {
        if (removedNode[i]) continue;
        nodes_[newCount++] = nodes_[i];
    }
    for (std::size_t i = newCount; i < kMaxNodes; ++i) nodes_[i] = EventNode{};
    nodeCount_ = newCount;

    truncated_ = true;
    return true;
}

bool Ledger::evictOldest(std::size_t count) {
    if (count == 0 || nodeCount_ == 0) return true;
    std::size_t removed = 0;
    while (removed < count && nodeCount_ > 0) {
        // find oldest by sequence
        std::size_t oldestIdx = 0;
        for (std::size_t i = 1; i < nodeCount_; ++i) {
            if (nodes_[i].sequence < nodes_[oldestIdx].sequence) oldestIdx = i;
        }
        std::uint64_t id = nodes_[oldestIdx].eventId;
        std::size_t before = nodeCount_;
        removeSubtree(id);
        if (nodeCount_ < before) removed += (before - nodeCount_);
        else break;
    }
    return true;
}

bool Ledger::buildSnapshot(Snapshot& out, std::uint64_t rootEventId,
                          int maxDepth, std::size_t nodeBudget,
                          std::size_t edgeBudget) const {
    out = Snapshot{};
    if (maxDepth <= 0) maxDepth = 64;
    if (nodeBudget == 0 || nodeBudget > kMaxSnapshotNodes) nodeBudget = kMaxSnapshotNodes;
    if (edgeBudget == 0 || edgeBudget > kMaxSnapshotEdges) edgeBudget = kMaxSnapshotEdges;

    // Collect root indices.
    int roots[kMaxNodes];
    int nRoots = 0;
    if (rootEventId != 0) {
        int ri = findNodeIndex(rootEventId);
        if (ri < 0) return false;
        roots[nRoots++] = ri;
    } else {
        for (std::size_t i = 0; i < nodeCount_; ++i) {
            if (!nodes_[i].valid) continue;
            std::uint64_t pid = nodes_[i].parentEventId;
            if (pid == 0 || findNodeIndex(pid) < 0) {
                roots[nRoots++] = static_cast<int>(i);
            }
        }
        // sort roots by sequence then eventId
        std::sort(roots, roots + nRoots, [this](int a, int b) {
            if (nodes_[static_cast<std::size_t>(a)].sequence != nodes_[static_cast<std::size_t>(b)].sequence)
                return nodes_[static_cast<std::size_t>(a)].sequence < nodes_[static_cast<std::size_t>(b)].sequence;
            return nodes_[static_cast<std::size_t>(a)].eventId < nodes_[static_cast<std::size_t>(b)].eventId;
        });
    }

    bool visited[kMaxNodes];
    for (std::size_t i = 0; i < kMaxNodes; ++i) visited[i] = false;

    int order[kMaxNodes];
    int depths[kMaxNodes];
    int nOrder = 0;
    bool depthExceeded = false;
    bool nodeOverflow = false;

    // Iterative DFS with explicit bounded frames.
    for (std::size_t i = 0; i < kMaxNodes; ++i) visited[i] = false;
    nOrder = 0;
    depthExceeded = false;
    nodeOverflow = false;

    struct CFrame { int idx; int depth; int cursor; int kids[kMaxNodes]; int nkids; };
    CFrame cframes[kMaxNodes];
    int csp = 0;

    for (std::size_t i = 0; i < kMaxNodes; ++i) visited[i] = false;

    auto pushRoot2 = [&](int ri, int d) {
        if (visited[ri]) return;
        visited[ri] = true;
        CFrame& cf = cframes[csp];
        cf.idx = ri;
        cf.depth = d;
        cf.cursor = 0;
        cf.nkids = 0;
        for (std::size_t i = 0; i < nodeCount_; ++i) {
            if (!nodes_[i].valid) continue;
            if (nodes_[i].parentEventId != nodes_[static_cast<std::size_t>(ri)].eventId) continue;
            cf.kids[cf.nkids++] = static_cast<int>(i);
        }
        std::sort(cf.kids, cf.kids + cf.nkids, [this](int a, int b) {
            if (nodes_[static_cast<std::size_t>(a)].sequence != nodes_[static_cast<std::size_t>(b)].sequence)
                return nodes_[static_cast<std::size_t>(a)].sequence < nodes_[static_cast<std::size_t>(b)].sequence;
            return nodes_[static_cast<std::size_t>(a)].eventId < nodes_[static_cast<std::size_t>(b)].eventId;
        });
        ++csp;
    };

    for (int r = nRoots - 1; r >= 0; --r) pushRoot2(roots[r], 0);

    // Iterative DFS: process top frame.
    while (csp > 0) {
        CFrame& f = cframes[csp - 1];
        if (f.cursor == 0) {
            // enter node
            if (f.depth > maxDepth) {
                depthExceeded = true;
                --csp;
                continue;
            }
            if (nOrder >= static_cast<int>(nodeBudget)) {
                nodeOverflow = true;
                depthExceeded = true;
                break;
            }
            order[nOrder] = f.idx;
            depths[nOrder] = f.depth;
            ++nOrder;
        }
        if (f.cursor < f.nkids) {
            int childIdx = f.kids[f.cursor++];
            if (visited[childIdx]) continue;
            visited[childIdx] = true;
            // push child frame
            CFrame& cf = cframes[csp];
            cf.idx = childIdx;
            cf.depth = f.depth + 1;
            cf.cursor = 0;
            cf.nkids = 0;
            for (std::size_t i = 0; i < nodeCount_; ++i) {
                if (!nodes_[i].valid) continue;
                if (nodes_[i].parentEventId != nodes_[static_cast<std::size_t>(childIdx)].eventId) continue;
                cf.kids[cf.nkids++] = static_cast<int>(i);
            }
            std::sort(cf.kids, cf.kids + cf.nkids, [this](int a, int b) {
                if (nodes_[static_cast<std::size_t>(a)].sequence != nodes_[static_cast<std::size_t>(b)].sequence)
                    return nodes_[static_cast<std::size_t>(a)].sequence < nodes_[static_cast<std::size_t>(b)].sequence;
                return nodes_[static_cast<std::size_t>(a)].eventId < nodes_[static_cast<std::size_t>(b)].eventId;
            });
            ++csp;
        } else {
            --csp;
        }
    }

    // Fill snapshot nodes.
    for (int i = 0; i < nOrder; ++i) {
        int idx = order[i];
        out.nodes[i].eventId = nodes_[static_cast<std::size_t>(idx)].eventId;
        out.nodes[i].parentEventId = nodes_[static_cast<std::size_t>(idx)].parentEventId;
        out.nodes[i].correlationId = nodes_[static_cast<std::size_t>(idx)].correlationId;
        out.nodes[i].simTime = nodes_[static_cast<std::size_t>(idx)].simTime;
        out.nodes[i].sequence = nodes_[static_cast<std::size_t>(idx)].sequence;
        out.nodes[i].valueClass = nodes_[static_cast<std::size_t>(idx)].valueClass;
        out.nodes[i].evidence = nodes_[static_cast<std::size_t>(idx)].evidence;
        out.nodes[i].truncated = nodes_[static_cast<std::size_t>(idx)].truncated;
        std::uint64_t pid = nodes_[static_cast<std::size_t>(idx)].parentEventId;
        out.nodes[i].parentMissing = (pid != 0 && findNodeIndex(pid) < 0);
        out.nodes[i].depth = depths[i];
    }
    out.nodeCount = static_cast<std::size_t>(nOrder);

    // Included set for edges.
    bool included[kMaxNodes];
    for (std::size_t i = 0; i < kMaxNodes; ++i) included[i] = false;
    for (int i = 0; i < nOrder; ++i) {
        int idx = order[i];
        // map node index -> included
        included[static_cast<std::size_t>(idx)] = true;
    }
    // But included by node index; check edge endpoints by find index.
    int edgeIdxs[kMaxEdges];
    int nEdges = 0;
    for (std::size_t e = 0; e < edgeCount_; ++e) {
        if (!edges_[e].valid) continue;
        int fi = findNodeIndex(edges_[e].fromEventId);
        int ti = findNodeIndex(edges_[e].toEventId);
        if (fi < 0 || ti < 0) continue;
        if (!included[static_cast<std::size_t>(fi)] || !included[static_cast<std::size_t>(ti)]) continue;
        edgeIdxs[nEdges++] = static_cast<int>(e);
    }
    std::sort(edgeIdxs, edgeIdxs + nEdges, [this](int a, int b) {
        return edges_[static_cast<std::size_t>(a)].edgeId < edges_[static_cast<std::size_t>(b)].edgeId;
    });

    bool edgeOverflow = false;
    int eIncluded = 0;
    for (int i = 0; i < nEdges; ++i) {
        if (static_cast<std::size_t>(eIncluded) >= edgeBudget) { edgeOverflow = true; break; }
        int e = edgeIdxs[i];
        out.edges[eIncluded].edgeId = edges_[static_cast<std::size_t>(e)].edgeId;
        out.edges[eIncluded].fromEventId = edges_[static_cast<std::size_t>(e)].fromEventId;
        out.edges[eIncluded].toEventId = edges_[static_cast<std::size_t>(e)].toEventId;
        out.edges[eIncluded].kind = edges_[static_cast<std::size_t>(e)].kind;
        out.edges[eIncluded].evidence = edges_[static_cast<std::size_t>(e)].evidence;
        out.edges[eIncluded].truncated = edges_[static_cast<std::size_t>(e)].truncated;
        ++eIncluded;
    }
    out.edgeCount = static_cast<std::size_t>(eIncluded);

    bool moreNodes = false;
    // If there are valid nodes not included.
    for (std::size_t i = 0; i < nodeCount_; ++i) {
        if (!nodes_[i].valid) continue;
        // A root-specific snapshot is not truncated merely because unrelated
        // trees exist. Nodes reached within the requested tree still count.
        if (rootEventId != 0 && !visited[i]) continue;
        // check if included in order
        bool isIn = false;
        for (int j = 0; j < nOrder; ++j) {
            if (order[j] == static_cast<int>(i)) { isIn = true; break; }
        }
        if (!isIn) { moreNodes = true; break; }
    }

    out.truncated = truncated_ || overflow_ || depthExceeded || nodeOverflow || edgeOverflow || moreNodes;
    for (int i = 0; i < nOrder; ++i) {
        if (nodes_[static_cast<std::size_t>(order[i])].truncated) out.truncated = true;
    }
    out.overflow = nodeOverflow || edgeOverflow || (static_cast<std::size_t>(nOrder) >= nodeBudget && moreNodes);
    return true;
}

std::size_t Ledger::aggregate(Aggregate* out, std::size_t capacity) const {
    if (!out || capacity == 0) return 0;
    // Use a temporary array of aggregates (on stack? capacity up to 128; use fixed max).
    Aggregate groups[kMaxAggregates];
    std::size_t groupCount = 0;
    bool resultTruncated = truncated_ || overflow_;

    for (std::size_t i = 0; i < nodeCount_; ++i) {
        if (!nodes_[i].valid) continue;
        // find group
        std::size_t gi = groupCount;
        for (std::size_t g = 0; g < groupCount; ++g) {
            if (groups[g].valueClass == nodes_[i].valueClass &&
                groups[g].currency == nodes_[i].currency &&
                std::strncmp(groups[g].unit, nodes_[i].unit, kUnitSize) == 0) {
                gi = g; break;
            }
        }
        if (gi == groupCount) {
            if (groupCount >= kMaxAggregates) {
                resultTruncated = true;
                continue;
            }
            groups[groupCount].valueClass = nodes_[i].valueClass;
            groups[groupCount].currency = nodes_[i].currency;
            CopyStr(groups[groupCount].unit, kUnitSize, nodes_[i].unit);
            groups[groupCount].totalAmount = 0.0;
            groups[groupCount].count = 0;
            groups[groupCount].truncated = false;
            ++groupCount;
        }
        groups[gi].totalAmount += nodes_[i].amount;
        groups[gi].count += 1;
        if (nodes_[i].truncated) groups[gi].truncated = true;
        if (!std::isfinite(groups[gi].totalAmount)) {
            groups[gi].totalAmount = 0.0;
            groups[gi].truncated = true;
            resultTruncated = true;
        }
    }

    std::size_t toCopy = groupCount < capacity ? groupCount : capacity;
    if (toCopy < groupCount) resultTruncated = true;
    for (std::size_t i = 0; i < toCopy; ++i) {
        out[i] = groups[i];
        if (resultTruncated) out[i].truncated = true;
    }
    return toCopy;
}

std::size_t Ledger::aggregateForRoot(std::uint64_t rootEventId, Aggregate* out,
                                     std::size_t capacity) const {
    if (!out || capacity == 0) return 0;
    // Collect subtree node indices.
    // Find root.
    int ri = findNodeIndex(rootEventId);
    if (ri < 0) return 0;

    // Collect all descendants.
    std::uint64_t subtreeIds[kMaxNodes];
    std::size_t sc = 0;
    subtreeIds[sc++] = rootEventId;
    bool changed = true;
    while (changed) {
        changed = false;
        for (std::size_t i = 0; i < nodeCount_; ++i) {
            if (!nodes_[i].valid) continue;
            std::uint64_t pid = nodes_[i].parentEventId;
            bool inSub = false;
            for (std::size_t s = 0; s < sc; ++s) {
                if (subtreeIds[s] == pid) { inSub = true; break; }
            }
            bool already = false;
            for (std::size_t s = 0; s < sc; ++s) {
                if (subtreeIds[s] == nodes_[i].eventId) { already = true; break; }
            }
            if (inSub && !already && sc < kMaxNodes) {
                subtreeIds[sc++] = nodes_[i].eventId;
                changed = true;
            }
        }
    }

    Aggregate groups[kMaxAggregates];
    std::size_t groupCount = 0;
    bool resultTruncated = truncated_ || overflow_;
    for (std::size_t s = 0; s < sc; ++s) {
        int idx = findNodeIndex(subtreeIds[s]);
        if (idx < 0) continue;
        const EventNode& node = nodes_[static_cast<std::size_t>(idx)];
        std::size_t gi = groupCount;
        for (std::size_t g = 0; g < groupCount; ++g) {
            if (groups[g].valueClass == node.valueClass &&
                groups[g].currency == node.currency &&
                std::strncmp(groups[g].unit, node.unit, kUnitSize) == 0) {
                gi = g; break;
            }
        }
        if (gi == groupCount) {
            if (groupCount >= kMaxAggregates) {
                resultTruncated = true;
                continue;
            }
            groups[groupCount].valueClass = node.valueClass;
            groups[groupCount].currency = node.currency;
            CopyStr(groups[groupCount].unit, kUnitSize, node.unit);
            groups[groupCount].totalAmount = 0.0;
            groups[groupCount].count = 0;
            groups[groupCount].truncated = false;
            ++groupCount;
        }
        groups[gi].totalAmount += node.amount;
        groups[gi].count += 1;
        if (node.truncated) groups[gi].truncated = true;
        if (!std::isfinite(groups[gi].totalAmount)) {
            groups[gi].totalAmount = 0.0;
            groups[gi].truncated = true;
            resultTruncated = true;
        }
    }
    std::size_t toCopy = groupCount < capacity ? groupCount : capacity;
    if (toCopy < groupCount) resultTruncated = true;
    for (std::size_t i = 0; i < toCopy; ++i) {
        out[i] = groups[i];
        if (resultTruncated) out[i].truncated = true;
    }
    return toCopy;
}

} // namespace causation_ledger
