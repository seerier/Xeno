#pragma once

#include "material.h"
#include "interaction.h"

namespace xeno {

inline float rDielectric(float cos_i, float eta) {
    cos_i = clamp(cos_i, -1, 1);
    if (cos_i < 0) {
        eta = 1.f / eta;
        cos_i = -cos_i;
    }

    float sin2_i = std::max(0.f, 1 - square(cos_i));
    float sin2_t = sin2_i / (eta * eta);
    if (sin2_t >= 1) return 1.f;
    float cos_t = sqrtf(std::max(0.f, 1.f - sin2_t));

    float r_parallel = (eta * cos_i - cos_t) / (eta * cos_i + cos_t);
    float r_perpendicular = (cos_i - eta * cos_t) / (cos_i + eta * cos_t);
    return (square(r_parallel) + square(r_perpendicular)) / 2;
}

class DielectricBxDF : public BxDF {
public:
    DielectricBxDF(float eta) :eta(eta) {}

    Spectrum f(const Vector3f &wo, const Vector3f &wi, TransportMode mode = TransportMode::Camera) const override {
        return Spectrum(0);
    }

    Spectrum sample_f(const Vector3f &wo, const Point2f &sample, Vector3f *wi, float *pdf,
        BxDFType *sampledType = nullptr, BxDFType type = BxDFType::ALL, TransportMode mode = TransportMode::Camera) const override;

    float pdf(const Vector3f &wo, const Vector3f &wi, TransportMode mode = TransportMode::Camera) const override {
        return 0.f;
    }

    BxDFType flags() const override {
        BxDFType type = static_cast<BxDFType>((eta == 1) ? BxDFType::TRANSMISSION : (BxDFType::TRANSMISSION | BxDFType::REFLECTION));
        return static_cast<BxDFType>(type | BxDFType::SPECULAR);
    }

private:
    float eta;
};

class Dielectric : public Material {
public:

    Dielectric(float ior = 1.f) : ior(ior) {}

    BSDF getBSDF(const Interaction &intr) const {
        Frame frame = Frame::fromZ(intr.n);
        return BSDF(std::make_shared<DielectricBxDF>(ior), frame);
    }

private:
    float ior;
};

} // namespace xeno