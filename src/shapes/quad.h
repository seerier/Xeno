#pragma once

#include "shape.h"

namespace xeno{

class Quad : public Shape {
public:
    Quad(const Point3f &p, const Vector3f &e0, const Vector3f &e1) :p(p), e0(e0), e1(e1) {
        worldBound = std::move(aabbUnion(Bounds3f(p, p + e0 + e1), Bounds3f(p + e0, p + e1)));
        surfaceArea = cross(e0, e1).length();
    }

    bool intersect(const Ray &ray, float &t, Interaction &intr) const override;

    Interaction sample(const Interaction &i, const Point2f &uv, float *pdf) const override;
    Interaction sample(const Point2f &uv, float *pdf) const override;

private:
    Point3f p;
    Vector3f e0, e1;
};

}