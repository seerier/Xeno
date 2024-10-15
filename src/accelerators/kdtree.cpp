#include "kdtree.h"

namespace xeno {

struct KdTreeNode {

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

KdTree::KdTree(const std::vector<std::shared_ptr<Primitive>> &primitives) :primitives(primitives) {
    int n = primitives.size();
    std::vector<Bounds3f> primBounds;
    primBounds.reserve(n);
    for (const auto &primPtr : primitives) {
        Bounds3f b = primPtr->aabb();
        primBounds.push_back(b);
        wholeBound.expandby(b);
    }

    std::unique_ptr<BoundEdge[]> edges[3];
    for (int i = 0; i < 3; ++i) {
        edges[i].reset(new BoundEdge[2 * n]);
    }
    std::unique_ptr<int[]> prims0(new int[n]);
    std::unique_ptr<int[]> prims1(new int[n]);
    std::unique_ptr<int[]> primIndices(new int[n]);
    for (int i = 0; i < n; ++i) primIndices[i] = i;
}

void KdTree::build(int nodeIndex, const Bounds3f &nodeBound, const std::vector<Bounds3f> &allPrimBounds, int *primIndices,
    int nPrimitives, int depth, const std::unique_ptr<BoundEdge[]> edges[3], int badRefines) {
    
}

} //namespace xeno