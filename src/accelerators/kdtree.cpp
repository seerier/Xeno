#include "kdtree.h"

namespace xeno {

struct alignas(8) KdTreeNode {
    void initLeaf(const int *primIndices, int primNums, std::vector<int> *primitiveIndices);

    void initInterior(int axis, int aboveChild, float t) {
        flags = axis | (aboveChild << 2);
        splitT = t;
    }

    float splitPos() const {
        return splitT;
    }

    int nPrimitives() const {
        return flags >> 2;
    }

    int splitAxis() const {
        return flags & 3;
    }

    bool isLeaf() const {
        return (flags & 3) == 3;
    }

    int aboveChild() const {
        return flags >> 2;
    }

    union {
        float splitT;
        int onePrimitiveIndex;
        int primitiveIndicesOffset;
    };

private:
    uint32_t flags;
};

enum class EdgeType { Start, End };

struct BoundEdge {
    BoundEdge() {}
    BoundEdge(float t, int primIndex, bool isStart) :t(t), primIndex(primIndex) {
        type = isStart ? EdgeType::Start : EdgeType::End;
    }

    float t;
    int primIndex;
    EdgeType type;
};

struct KDToVisit {
    const KdTreeNode *node;
    float tMin, tMax;
};

void KdTreeNode::initLeaf(const int *primIndices, int primNums, std::vector<int> *primitiveIndices) {
    flags = 3 | (primNums << 2);
    if (primNums == 0) onePrimitiveIndex = -1;
    else if (primNums == 1) onePrimitiveIndex = primIndices[0];
    else {
        primitiveIndicesOffset = primitiveIndices->size();
        for (int i = 0; i < primNums; ++i)
            primitiveIndices->push_back(primIndices[i]);
    }
}

KdTree::KdTree(const std::vector<std::shared_ptr<Primitive>> &primitives) :primitives(primitives) {
    nextFreeNode = nAllocatedNodes = 0;
    int n = primitives.size();
    std::vector<Bounds3f> primBounds;
    primBounds.reserve(n);
    for (const auto &primPtr : primitives) {
        Bounds3f b = primPtr->aabb();
        primBounds.push_back(b);
        wholeBound.expandby(b);
    }
    maxDepth = 8 + std::_Ceiling_of_log_2(n) * 1.3;

    std::unique_ptr<BoundEdge[]> edges[3];
    for (int i = 0; i < 3; ++i) {
        edges[i].reset(new BoundEdge[2 * n]);
    }
    std::unique_ptr<int[]> prims0(new int[n]);
    std::unique_ptr<int[]> prims1(new int[n * (maxDepth + 1)]);
    std::unique_ptr<int[]> primIndices(new int[n]);
    for (int i = 0; i < n; ++i) primIndices[i] = i;

    build(0, wholeBound, primBounds, primIndices.get(), n, 0, edges, prims0.get(), prims1.get(), 0);
}

void KdTree::build(int nodeIndex, const Bounds3f &nodeBound, const std::vector<Bounds3f> &allPrimBounds, int *primIndices,
    int nPrimitives, int depth, const std::unique_ptr<BoundEdge[]> edges[3], int *prims0, int *prims1, int badRefines) {
    CHECK_EQ(nextFreeNode, nodeIndex);
    if (nextFreeNode == nAllocatedNodes) {
        int nNewAllocatedNodes = std::max(2 * nAllocatedNodes, 512);
        KdTreeNode *n = new KdTreeNode[nNewAllocatedNodes];
        if (nAllocatedNodes > 0) {
            std::memcpy(n, nodes, nAllocatedNodes * sizeof(KdTreeNode));
            delete[] nodes;
        }
        nodes = n;
        nAllocatedNodes = nNewAllocatedNodes;
    }
    ++nextFreeNode;

    if (nPrimitives <= maxPrims || depth == maxDepth) {
        nodes[nodeIndex].initLeaf(primIndices, nPrimitives, &primitiveIndices);
        return;
    }

    // variables for space division
    int bestAxis = -1, bestOffset = -1;
    float bestCost = Infinity, leafCost = nPrimitives * isectCost;
    float invTotalSA = 1.f / nodeBound.surfaceArea();
    Vector3f d = nodeBound.diagonal();
    int axis = nodeBound.maxExtent();

    int retries = 0;
    // ready to perform split
retrySplit:
    // prepare the edges
    for (int i = 0; i < nPrimitives; ++i) {
        int index = primIndices[i];
        edges[axis][2 * i] = BoundEdge(allPrimBounds[index].pMin[axis], index, true);
        edges[axis][2 * i + 1] = BoundEdge(allPrimBounds[index].pMax[axis], index, false);
    }
    std::sort(&edges[axis][0], &edges[axis][0] + 2 * nPrimitives,
        [](const BoundEdge &e1, const BoundEdge &e2)-> bool {
            return std::tie(e1.t, e1.type) < std::tie(e2.t, e2.type);
        });

    // find the best position, then split
    int nBelow = 0, nAbove = nPrimitives;
    for (int i = 0; i < 2 * nPrimitives; ++i) {
        if (edges[axis][i].type == EdgeType::End) --nAbove;
        float edgeT = edges[axis][i].t;
        if (edgeT > nodeBound.pMin[axis] && edgeT < nodeBound.pMax[axis]) {
            float otherAxis1Extent = d[(axis + 1) % 3], otherAxis2Extent = d[(axis + 2) % 3];
            float belowSA = 2 * (otherAxis1Extent * otherAxis2Extent + (edgeT - nodeBound.pMin[axis]) * (otherAxis1Extent + otherAxis2Extent));
            float aboveSA = 2 * (otherAxis1Extent * otherAxis2Extent + (nodeBound.pMax[axis] - edgeT) * (otherAxis1Extent + otherAxis2Extent));

            float pBelow = belowSA * invTotalSA, pAbove = aboveSA * invTotalSA;
            float eb = (nBelow == 0 || nAbove == 0) ? emptyBonus : 0.f;
            float cost = traversalCost + (1 - eb) * (pBelow * nBelow + pAbove * nAbove) * isectCost;

            // update if current split is better
            if (cost < bestCost) {
                bestAxis = axis;
                bestCost = cost;
                bestOffset = i;
            }
        }
        if (edges[axis][i].type == EdgeType::Start) ++nBelow;
    }
    CHECK(nBelow == nPrimitives && nAbove == 0);

    // try another axis if no good split is found
    if (bestAxis == -1 && retries < 2) {
        axis = (axis + 1) % 3;
        retries++;
        goto retrySplit;
    }

    // create a leaf if no good split is found
    if (bestCost > leafCost)
        ++badRefines;
    if ((bestCost > 4.f * leafCost && nPrimitives < 16) || bestAxis == -1 || badRefines == 3) {
        nodes[nodeIndex].initLeaf(primIndices, nPrimitives, &primitiveIndices);
        return;
    }

    int n0 = 0, n1 = 0;
    for (int i = 0; i < bestOffset; ++i) {
        if (edges[bestAxis][i].type == EdgeType::Start)
            prims0[n0++] = edges[bestAxis][i].primIndex;
    }
    for (int i = bestOffset + 1; i < 2 * nPrimitives; ++i) {
        if (edges[bestAxis][i].type == EdgeType::End)
            prims1[n1++] = edges[bestAxis][i].primIndex;
    }

    // build left and right childs
    float tSplit = edges[bestAxis][bestOffset].t;
    Bounds3f bound0 = nodeBound, bound1 = nodeBound;
    bound0.pMax[bestAxis] = tSplit;
    bound1.pMin[bestAxis] = tSplit;
    build(nodeIndex + 1, bound0, allPrimBounds, prims0, n0, depth + 1, edges, prims0, prims1 + nPrimitives, badRefines);
    int aboveNode = nextFreeNode;
    nodes[nodeIndex].initInterior(bestAxis, aboveNode, tSplit);
    build(aboveNode, bound1, allPrimBounds, prims1, n1, depth + 1, edges, prims0, prims1 + nPrimitives, badRefines);
}

bool KdTree::intersect(const Ray &r, float &ray_t, Interaction &intr) const {
    float tmin, tmax;
    if (!wholeBound.intersectP(r, &tmin, &tmax)) return false;

    bool hit = false;

    Vector3f invDir(1.f / r.d.x, 1.f / r.d.y, 1.f / r.d.z);
    constexpr int maxToVisit = 64;
    KDToVisit toVisit[maxToVisit];
    int toVisitIndex = 0;

    const KdTreeNode *node = &nodes[0];
    while (node) {
        if (r.tMax < tmin) break;

        if (!node->isLeaf()) {
            int axis = node->splitAxis();
            float tSplit = (node->splitPos() - r.o[axis]) * invDir[axis];

            const KdTreeNode *firstChild, *secondChild;
            bool belowFirst = (r.o[axis] < node->splitPos()) || (r.o[axis] == node->splitPos() && r.d[axis] <= 0);
            if (belowFirst) {
                firstChild = node + 1;
                secondChild = &nodes[node->aboveChild()];
            }
            else {
                firstChild = &nodes[node->aboveChild()];
                secondChild = node + 1;
            }

            // process two childs
            if (tSplit > tmax || tSplit <= 0)
                node = firstChild;
            else if (tSplit < tmin)
                node = secondChild;
            else {
                toVisit[toVisitIndex].node = secondChild;
                toVisit[toVisitIndex].tMax = tmax;
                toVisit[toVisitIndex].tMin = tSplit;
                ++toVisitIndex;

                node = firstChild;
                tmax = tSplit;
            }
        }
        else {
            // handle leaf node
            int nPrimitives = node->nPrimitives();
            if (nPrimitives == 1) {
                const auto &p = primitives[node->onePrimitiveIndex];
                if (p->intersect(r, ray_t, intr)) hit = true;
            }
            else {
                for (int i = 0; i < nPrimitives; ++i) {
                    int index = primitiveIndices[node->primitiveIndicesOffset + i];
                    const auto &p = primitives[index];
                    if (p->intersect(r, ray_t, intr)) hit = true;
                }
            }

            if (toVisitIndex > 0) {
                --toVisitIndex;
                node = toVisit[toVisitIndex].node;
                tmin = toVisit[toVisitIndex].tMin;
                tmax = toVisit[toVisitIndex].tMax;
            }
            else break;
        }
    }
    return hit;
}

} //namespace xeno