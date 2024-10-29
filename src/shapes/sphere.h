#pragma once

#include "shape.h"

namespace xeno {

class Sphere : public Shape {
public:
    Sphere(const Point3f &p, float r) :p(p), radius(r) {
        worldBound = std::move(Bounds3f(Point3f(p.x - radius, p.y - radius, p.z - radius), Point3f(p.x + radius, p.y + radius, p.z + radius)));
        surfaceArea = 4 * Pi * radius * radius;
    }

    bool intersect(const Ray &r, float &ray_t, Interaction &intr) const override;

    Interaction sample(const Interaction &i, const Point2f &uv, float *pdf) const override;

    Interaction sample(const Point2f &uv, float *pdf) const override;

private:
    Point3f p;
    float radius;
};

}