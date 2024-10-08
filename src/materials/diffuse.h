#pragma once

#include "material.h"

namespace xeno {

class Diffuse : public Material {
public:
    Diffuse(float k = 0.5f) :kd(k, k, k) {}
    Diffuse(float r, float g, float b) :kd(r, g, b) {}
    Diffuse(const Spectrum &s) :kd(s) {}

    Spectrum f(const Vector3f &wo, const Vector3f &wi, const Normal3f &n) const override {
        if (dot(wo, n) < 0 || dot(wi, n) < 0) return Spectrum(0, 0, 0);
        return kd * InvPi;
    }

    Spectrum sample_f(const Vector3f &wo, Vector3f *wi, const Normal3f &n, const Point2f &sample, float *pdf) const override;

    float pdf(const Vector3f &wo, const Vector3f &wi, const Normal3f &n) const override {
        return InvPi;
    }

private:
    Spectrum kd;
};

} // namespace xeno