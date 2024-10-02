#pragma once

#include "materials/diffuse.h"
#include "accelerators/objectList.h"
#include "primitive.h"

namespace xeno {

class Scene {
public:
    Scene(const std::shared_ptr<Primitive> &obj, const std::vector<std::shared_ptr<Light>> &lights)
        :objects(obj), lights(lights) {
        worldBound = obj->aabb();
    }

    bool intersect(Ray &r, float &ray_t, Interaction &intr) const {
        return objects->intersect(r, ray_t, intr);
    }

    Bounds3f aabb() const {
        return worldBound;
    }

    int uniformSampleOneLight(float sample, float *pdf) const {
        *pdf = 1.f / lights.size();
        return std::min(static_cast<unsigned long long>(lights.size() * sample), lights.size() - 1);
    }

    std::vector<std::shared_ptr<Light>> lights;

private:
    std::shared_ptr<Primitive> objects;
    Bounds3f worldBound;
};

} // namespace xeno