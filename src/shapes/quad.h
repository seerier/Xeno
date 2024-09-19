#pragma once

#include "shape.h"

namespace xeno{

class Quad : public Shape {
public:
    Quad() {}
    Quad(const Point3f &p, const Vector3f &e0, const Vector3f &e1) :p(p), e0(e0), e1(e1) {}

    Bounds3f aabb() const override {
        return aabbUnion(Bounds3f(p, p + e0 + e1), Bounds3f(p + e0, p + e1));
    }

    bool intersect(Ray &ray, float &t, Interaction &intr) const override;

private:
    Point3f p;
    Vector3f e0, e1;
};

}