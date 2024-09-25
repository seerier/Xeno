#pragma once

#include "shapes/quad.h"
#include "shapes/sphere.h"
#include "shapes/triangle.h"
#include "materials/diffuse.h"
#include "accelerators/objectList.h"

namespace xeno {

class Scene {
public:
    Scene(std::shared_ptr<Shape> obj) :objects(obj) { worldBound = obj->aabb(); }

    bool intersect(Ray &r, float &ray_t, Interaction &intr) const {
        return objects->intersect(r, ray_t, intr);
    }

    Bounds3f aabb() const {
        return worldBound;
    }

    std::shared_ptr<Shape> objects;
    Bounds3f worldBound;
};

} // namespace xeno