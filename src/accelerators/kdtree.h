#pragma once

#include "primitive.h"

namespace xeno {

struct KdTreeNode;
struct BoundEdge;

class KdTree : public Primitive {
public:
    
    KdTree(const std::vector<std::shared_ptr<Primitive>> &primitives);

    bool intersect(const Ray &r, float &ray_t, Interaction &intr) const override;

    Bounds3f aabb() const override {
        return wholeBound;
    }

private:

    void build(int nodeIndex, const Bounds3f &nodeBound, const std::vector<Bounds3f> &allPrimBounds, int *primIndices,
        int nPrimitives, int depth, const std::unique_ptr<BoundEdge[]> edges[3], int *prims0, int *prims1, int badRefines);

    std::vector<std::shared_ptr<Primitive>> primitives;
    std::vector<int> primitiveIndices;
    KdTreeNode *nodes;
    const int isectCost = 80, traversalCost = 1, maxPrims = 1;
    const float emptyBonus = 0.5;
    Bounds3f wholeBound;
    int nextFreeNode, nAllocatedNodes;
    int maxDepth;
};

} //namespace xeno