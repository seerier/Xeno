#pragma once

#include "../shape.h"

namespace xeno {

class Triangle : public Shape {
public:
    Triangle(const Point3f &v0, const Point3f &v1, const Point3f &v2) :v0(v0), v1(v1), v2(v2) {}

    Bounds3f aabb() const {
        return aabbUnion(Bounds3f(v0, v1), v2);
    }

    bool intersect(Ray &ray, float &ray_t, Interaction &intr) const;

private:
    Point3f v0, v1, v2;
};

}