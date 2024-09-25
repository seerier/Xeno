#pragma once

#include "mathutil.h"
#include "ray.h"
#include "light.h"

namespace xeno {

class Shape {
public:
    Shape() {}
    Shape(std::shared_ptr<Material> mat) :material(mat) {}

    virtual Bounds3f aabb() const { return worldBound; }

    virtual bool intersect(Ray &r, float &ray_t, Interaction &intr) const = 0;

    virtual ~Shape() {}

    const Material *getMaterial() const { return material.get(); }
    const Light *getLight() const { return light.get(); }

    void bindLight(std::shared_ptr<Light> l) { light = l; }
    bool isEmitter() const { return light != nullptr; }

    Vector3f L(const Interaction &intr, const Vector3f &w) const {
        if (light) return light->L(intr, w);
        return Vector3f(0, 0, 0);
    }

private:
    std::shared_ptr<Material> material;
    std::shared_ptr<Light> light;
protected:
    Bounds3f worldBound;
};

} // namespace xeno