#pragma once

#include "ray.h"

namespace xeno {

class BxDF {
public:
    virtual Spectrum f(const Vector3f &wo, const Vector3f &wi) const = 0;

    virtual Spectrum sample_f(const Vector3f &wo, const Point2f &sample, Vector3f *wi, float *pdf) const = 0;

    virtual float pdf(const Vector3f &wo, const Vector3f &wi) const = 0;

    virtual ~BxDF() {}
};

} //namespace xeno