#pragma once

#include "material.h"
#include "interaction.h"
#include "scattering.h"

namespace xeno {

class ConductorBxDF : public BxDF {
public:
    ConductorBxDF(const TrowbridgeReitzDistribution &distrib, Spectrum eta, Spectrum k) :
        mfDistrib(distrib), eta(eta), k(k) {
    }

    Spectrum f(const Vector3f &wo, const Vector3f &wi, TransportMode mode = TransportMode::Camera) const override;

    Spectrum sample_f(const Vector3f &wo, const Point2f &sample, Vector3f *wi, float *pdf,
        BxDFType *sampledType = nullptr, BxDFType type = BxDFType::ALL, TransportMode mode = TransportMode::Camera) const override;

    float pdf(const Vector3f &wo, const Vector3f &wi, TransportMode mode = TransportMode::Camera) const override;

    BxDFType flags() const override;

private:
    Spectrum eta, k;
    TrowbridgeReitzDistribution mfDistrib;
};

class Conductor : public Material {
public:

    Conductor(Spectrum eta, Spectrum k, float uRoughness, float vRoughness) :
        eta(eta), k(k), uRoughness(uRoughness), vRoughness(vRoughness) {
    }

    BSDF getBSDF(const Interaction &intr) const {
        Frame frame = Frame::fromZ(intr.n);

        TrowbridgeReitzDistribution distrib(uRoughness, vRoughness);
        return BSDF(std::make_shared<ConductorBxDF>(distrib, eta, k), frame);
    }

private:
    Spectrum eta, k;
    float uRoughness, vRoughness;
};

} // namespace xeno