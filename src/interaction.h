#pragma once

#include "material.h"

namespace xeno {

class Interaction {
public:
    Interaction() {}
    Interaction(const Point3f &p, const Normal3f &n, const Vector3f &wo, const Point2f uv = Point2f(0, 0))
        :p(p), n(n), wo(normalize(wo)), uv(uv) {}

    // Geometry related members
    Point3f p;
    Normal3f n;
    Vector3f wo;

    // Material related members
    //const Material *material = nullptr;
    Point2f uv;

    // Ptr to Shape
    const Shape *shape = nullptr;
};

}