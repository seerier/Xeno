#pragma once

#include "primitive.h"

namespace xeno {

struct BVHNode;
struct PrimitiveInfo;

class BVH : public Primitive {
public:
    BVH(const std::vector<std::shared_ptr<Primitive>> &primitives);

    bool intersect(const Ray &r, float &ray_t, Interaction &intr) const override;

    Bounds3f aabb() const override;

private:
    BVHNode *buildBVH(std::vector<PrimitiveInfo> &primitiveInfos, std::vector<std::shared_ptr<Primitive>> &orderedPrims, int start, int end) const;
    
    std::vector<std::shared_ptr<Primitive>> primitives;
    BVHNode *bvhNode;
};

} // namespace xeno