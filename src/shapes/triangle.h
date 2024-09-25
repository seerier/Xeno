#pragma once

#include "shape.h"

namespace xeno {

class Triangle : public Shape {
public:
    Triangle(const Point3f &v0, const Point3f &v1, const Point3f &v2, std::shared_ptr<Material> mat) :v0(v0), v1(v1), v2(v2), Shape(mat) {
        worldBound = std::move(aabbUnion(Bounds3f(v0, v1), v2));
    }

    bool intersect(Ray &ray, float &ray_t, Interaction &intr) const override;

private:
    Point3f v0, v1, v2;
};

}