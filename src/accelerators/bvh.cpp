#include"bvh.h"

namespace xeno {

struct BVHNode {
    BVHNode(const Bounds3f &aabb, int offset, int nPrimitives, BVHNode *l = nullptr, BVHNode *r = nullptr)
        :aabb(aabb), offset(offset), nPrimitives(nPrimitives), left(l), right(r) {}
    Bounds3f aabb;
    BVHNode *left, *right;
    int offset, nPrimitives;
};

struct PrimitiveInfo {
    PrimitiveInfo() {}
    PrimitiveInfo(int n, const Bounds3f &bounds) : aabb(bounds), index(n) {
        center = .5f * aabb.pMin + .5f * aabb.pMax;
    }
    Bounds3f aabb;
    Point3f center;
    int index;
};

Bounds3f BVH::aabb() const {
    return bvhNode ? bvhNode->aabb : Bounds3f();
}

BVH::BVH(const std::vector<std::shared_ptr<Primitive>> &prims) : primitives(std::move(prims)) {
    if (primitives.empty()) return;
    int n = primitives.size();

    std::vector<PrimitiveInfo> primitiveInfos;
    primitiveInfos.reserve(n);
    for (int i = 0; i < n; ++i) {
        primitiveInfos.emplace_back(i, primitives[i]->aabb());
    }

    std::vector<std::shared_ptr<Primitive>> orderedPrims;
    orderedPrims.reserve(n);

    bvhNode = buildBVH(primitiveInfos, orderedPrims, 0, n);

    primitives.swap(orderedPrims);

    primitiveInfos.resize(0);

}

BVHNode *BVH::buildBVH(std::vector<PrimitiveInfo> &primitiveInfos, std::vector<std::shared_ptr<Primitive>> &orderedPrims, int start, int end) const {
    int n = end - start;
    if (n == 1) {
        int prim_index = primitiveInfos[start].index;
        orderedPrims.push_back(primitives[prim_index]);
        //return new BVHNode(primitives[prim_index]->aabb(), orderedPrims.size() - 1, 1);
        return new BVHNode(primitiveInfos[start].aabb, orderedPrims.size() - 1, 1);
    }
    else {
        Bounds3f wholeBound;
        Bounds3f centerBound;
        for (int i = start; i < end; ++i) {
            wholeBound.expand(primitiveInfos[i].aabb);
            centerBound.expand(primitiveInfos[i].center);
        }
        int splitAxis = centerBound.maxExtent();
        int offset = orderedPrims.size();

        // in case that all the centers locate in the same point, create a leaf node directly
        if (centerBound.pMax[splitAxis] == centerBound.pMin[splitAxis]) {
            for (int i = start; i < end; ++i) {
                int prim_index = primitiveInfos[i].index;
                orderedPrims.push_back(primitives[prim_index]);
            }
            return new BVHNode(wholeBound, offset, n);
        }
        else {
            float pMid = .5f * centerBound.pMax[splitAxis] + .5f * centerBound.pMin[splitAxis];
            PrimitiveInfo *midPtr = std::partition(&primitiveInfos[start], &primitiveInfos[end - 1] + 1,
                [splitAxis, pMid](const PrimitiveInfo &primInfo) {
                    return primInfo.center[splitAxis] < pMid;
                });
            int midIndex = midPtr - &primitiveInfos[0];

            // the case that partition didn't function
            if (midIndex == start || midIndex == end) {
                midIndex = (start + end) / 2;
            }

            BVHNode *left = buildBVH(primitiveInfos, orderedPrims, start, midIndex);
            BVHNode *right = buildBVH(primitiveInfos, orderedPrims, midIndex, end);
            // use the offset variable to store splitAxis
            return new BVHNode(wholeBound, splitAxis, 0, left, right);
        }
    }
}


bool BVH::intersect(const Ray &r, float &ray_t, Interaction &intr) const {
    if (!bvhNode) return false;

    bool hit = false;
    Vector3f invDir(1.f / r.d.x, 1.f / r.d.y, 1.f / r.d.z);
    int dirIsNeg[3] = { invDir.x < 0,invDir.y < 0,invDir.z < 0 };

    const BVHNode *nodesToVisit[1024];
    int indexOffset = 0;
    const BVHNode *currentNode = bvhNode;

    while (currentNode != nullptr) {
        if (currentNode->aabb.intersectP(r, invDir, dirIsNeg)) {
            if (currentNode->nPrimitives > 0) {
                for (int i = 0; i < currentNode->nPrimitives; ++i) {
                    if (primitives[currentNode->offset + i]->intersect(r, ray_t, intr)) hit = true;
                }
                if (indexOffset == 0) break;
                currentNode = nodesToVisit[--indexOffset];
            }
            else {
                if (dirIsNeg[currentNode->offset]) {
                    if (currentNode->left) {
                        nodesToVisit[indexOffset++] = currentNode->left;
                    }
                    currentNode = currentNode->right;
                }
                else {
                    if (currentNode->right) {
                        nodesToVisit[indexOffset++] = currentNode->right;
                    }
                    currentNode = currentNode->left;
                }
            }
        }
        else {
            if (indexOffset == 0) break;
            currentNode = nodesToVisit[--indexOffset];
        }
    }
    return hit;
}

/*
bool BVH::intersect(const Ray &ray, float &ray_t, Interaction &intr) const {
    if (!bvhNode) return false;
    
    bool hit = false;
    Vector3f invDir(1 / ray.d.x, 1 / ray.d.y, 1 / ray.d.z);
    int dirIsNeg[3] = {invDir.x < 0, invDir.y < 0, invDir.z < 0};
    
    // Stack to store nodes to visit
    int toVisitOffset = 0, currentNodeIndex = 0;
    BVHNode *nodesToVisit[1024];
    
    while (true) {
        const BVHNode *node = &bvhNode[currentNodeIndex];
        
        // Check if the ray intersects the current node's bounding box
        if (node->aabb.intersectP(ray, invDir, dirIsNeg)) {
            if (node->nPrimitives > 0) {
                // Leaf node: Check intersection with each primitive in this node
                for (int i = 0; i < node->nPrimitives; ++i) {
                    if (primitives[node->offset + i]->intersect(ray, ray_t, intr)) {
                        hit = true;
                    }
                }
                // If no more nodes to visit, break
                if (toVisitOffset == 0) break;
                // Go back to the last node in the stack
                currentNodeIndex = nodesToVisit[--toVisitOffset] - bvhNode;
            } else {
                // Internal node: Traverse down either left or right child
                if (dirIsNeg[node->aabb.maxExtent()]) {
                    nodesToVisit[toVisitOffset++] = &bvhNode[currentNodeIndex + 1]; // Push far child
                    currentNodeIndex = node->right - bvhNode;  // Traverse near child
                } else {
                    nodesToVisit[toVisitOffset++] = node->right;  // Push far child
                    currentNodeIndex = currentNodeIndex + 1;      // Traverse near child
                }
            }
        } else {
            // If no intersection with current node, go back to last visited node
            if (toVisitOffset == 0) break;
            currentNodeIndex = nodesToVisit[--toVisitOffset] - bvhNode;
        }
    }
    
    return hit;
}
*/

}