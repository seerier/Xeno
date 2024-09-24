#pragma once

#include "ray.h"

namespace xeno {

class Material {
public:
    virtual float f(const Vector3f &wo, const Vector3f &wi, const Normal3f &n) const = 0;

    virtual float sample_f(const Vector3f &wo, Vector3f *wi, const Normal3f &n, const Point2f &sample, float *pdf) const = 0;

    virtual float pdf(const Vector3f &wo, const Vector3f &wi, const Normal3f &n) const = 0;

};

} // namespace xeno