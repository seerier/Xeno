#pragma once

#include "shape.h"

namespace xeno {

class Sphere : public Shape {
public:
    Sphere(const Point3f &p, float r) :p(p), radius(r) {}

    Bounds3f aabb() const override {
        return Bounds3f(Point3f(p.x - radius, p.y - radius, p.z - radius), Point3f(p.x + radius, p.y + radius, p.z + radius));
    }

    bool intersect(Ray &r, float &ray_t, Interaction &intr) const override;

private:
    Point3f p;
    float radius;
};

}