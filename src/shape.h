#pragma once

#include "interaction.h"
#include "mathutil.h"

namespace xeno {

class Shape {
public:
    Shape() {}
    Shape(std::shared_ptr<Material> mat) :material(mat) {}

    virtual Bounds3f aabb() const { return worldBound; }

    virtual bool intersect(Ray &r, float &ray_t, Interaction &intr) const = 0;

    virtual ~Shape() {}

    const Material *getMaterial() const { return material.get(); }

private:
    std::shared_ptr<Material> material;
protected:
    Bounds3f worldBound;
};

} // namespace xeno