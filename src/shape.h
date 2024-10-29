#pragma once

#include "mathutil.h"
#include "ray.h"
#include "interaction.h"

namespace xeno {

class Shape {
public:
    Shape() {}

    virtual Bounds3f aabb() const { return worldBound; }

    float area() const { return surfaceArea; }

    //virtual float pdf(const Interaction &intr, const Vector3f &w) const { return 1.f / surfaceArea; }

    virtual float pdf_Li(const Interaction &intr, const Vector3f &w) const;

    virtual bool intersect(const Ray &r, float &ray_t, Interaction &intr) const = 0;

    virtual Interaction sample(const Interaction &i, const Point2f &uv, float *pdf) const = 0;

    virtual Interaction sample(const Point2f &uv, float *pdf) const = 0;

    virtual ~Shape() {}

protected:
    Bounds3f worldBound;
    float surfaceArea;
};

} // namespace xeno