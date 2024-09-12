#pragma once

#include "interaction.h"
#include "mathutil.h"

namespace xeno {

class Shape {
public:
    virtual Bounds3f aabb() const = 0;

    virtual bool intersect(Ray &r, const Interaction &intr) const = 0;

    virtual ~Shape() = 0;
};

} // namespace xeno