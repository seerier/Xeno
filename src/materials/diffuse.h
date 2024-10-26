#pragma once

#include "material.h"
#include "interaction.h"
#include "textures/constantTexture.h"

namespace xeno {

class DiffuseBxDF : public BxDF {
public:
    DiffuseBxDF(float k = 0.5f) :kd(k, k, k) {}
    DiffuseBxDF(float r, float g, float b) :kd(r, g, b) {}
    DiffuseBxDF(const Spectrum &s) :kd(s) {}

    Spectrum f(const Vector3f &wo, const Vector3f &wi, TransportMode mode = TransportMode::Camera) const override {
        //if (dot(wo, n) < 0 || dot(wi, n) < 0) return Spectrum(0, 0, 0);
        if (wo.z < 0 || wi.z < 0) return Spectrum(0);
        return kd * InvPi;
    }

    Spectrum sample_f(const Vector3f &wo, const Point2f &sample, Vector3f *wi, float *pdf,
        BxDFType *sampledType = nullptr, BxDFType type = BxDFType::ALL, TransportMode mode = TransportMode::Camera) const override;

    float pdf(const Vector3f &wo, const Vector3f &wi, TransportMode mode = TransportMode::Camera) const override {
        return InvPi;
    }

    BxDFType flags() const override {
        return BxDFType::DIFFUSE_REFLECTION;
    }

private:
    Spectrum kd;
};

class Diffuse : public Material {
public:

    Diffuse(float k) :kd(std::make_shared<ConstantTexture>(k)) {}
    Diffuse(const Spectrum &s) :kd(std::make_shared<ConstantTexture>(s)) {}
    Diffuse(const std::shared_ptr<Texture> &k) : kd(k) {}

    BSDF getBSDF(const Interaction &intr) const {
        //BxDF *bxdf = new DiffuseBxDF(kd);
        std::shared_ptr<BxDF> bxdf = std::make_shared<DiffuseBxDF>(kd->lookup(intr.uv));
        Frame frame = Frame::fromZ(intr.n);
        return BSDF(bxdf, frame);
    }

private:
    //Spectrum kd;
    std::shared_ptr<Texture> kd;
};

} // namespace xeno