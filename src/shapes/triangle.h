#pragma once

#include "shape.h"

namespace xeno {

class Triangle : public Shape {
public:
    Triangle(const Point3f &v0, const Point3f &v1, const Point3f &v2) :v0(v0), v1(v1), v2(v2) {
        worldBound = std::move(aabbUnion(Bounds3f(v0, v1), v2));
        surfaceArea = .5f * cross(v1 - v0, v2 - v0).length();
    }

    bool intersect(const Ray &ray, float &ray_t, Interaction &intr) const override;

    Interaction sample(const Point2f &uv, float *pdf) const override;

private:
    Point3f v0, v1, v2;
};

}