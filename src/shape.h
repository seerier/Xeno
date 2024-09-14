#pragma once

#include "interaction.h"
#include "mathutil.h"

namespace xeno {

class Shape {
public:
    virtual Bounds3f aabb() const = 0;

    virtual bool intersect(Ray &r, float &ray_t, Interaction &intr) const = 0;

    virtual ~Shape() {}
};

} // namespace xeno