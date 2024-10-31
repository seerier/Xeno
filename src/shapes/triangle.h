#pragma once

#include "shape.h"

namespace xeno {

class Triangle : public Shape {
public:
    Triangle(const Point3f &v0, const Point3f &v1, const Point3f &v2, const Normal3f &n0, const Normal3f &n1, const Normal3f &n2, bool interpolate = false)
        :v0(v0), v1(v1), v2(v2), n0(n0), n1(n1), n2(n2), interpolate(interpolate) {
        worldBound = std::move(aabbUnion(Bounds3f(v0, v1), v2));
        surfaceArea = .5f * cross(v1 - v0, v2 - v0).length();
    }

    bool intersect(const Ray &ray, float &ray_t, Interaction &intr) const override;

    Interaction sample(const Interaction &i, const Point2f &uv, float *pdf) const override;
    Interaction sample(const Point2f &uv, float *pdf) const override;

private:
    Point3f v0, v1, v2;
    Normal3f n0, n1, n2;
    bool interpolate;
};

} // namespace xeno