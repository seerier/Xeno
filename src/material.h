#pragma once

#include "ray.h"
#include "bsdf.h"

namespace xeno {

class Material {
public:
    virtual Spectrum f(const Vector3f &wo, const Vector3f &wi, const Normal3f &n) const = 0;

    virtual Spectrum sample_f(const Vector3f &wo, Vector3f *wi, const Normal3f &n, const Point2f &sample, float *pdf) const = 0;

    virtual float pdf(const Vector3f &wo, const Vector3f &wi, const Normal3f &n) const = 0;

    virtual BSDF getBSDF(const Interaction &intr) const = 0;

    virtual ~Material() {}
};

} // namespace xeno