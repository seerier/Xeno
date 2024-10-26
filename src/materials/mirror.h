#pragma once

#include "material.h"
#include "interaction.h"

namespace xeno {

class MirrorBxDF : public BxDF {
public:
    MirrorBxDF() {}

    Spectrum f(const Vector3f &wo, const Vector3f &wi, TransportMode mode = TransportMode::Camera) const override {
        return Spectrum(0);
    }

    Spectrum sample_f(const Vector3f &wo, const Point2f &sample, Vector3f *wi, float *pdf,
        BxDFType *sampledType = nullptr, BxDFType type = BxDFType::ALL, TransportMode mode = TransportMode::Camera) const override {
        if (!isSpecular(type)) {
            *pdf = 0;
            return Spectrum(0);
        }
        *pdf = 1;
        *wi = Vector3f(-wo.x, -wo.y, wo.z);
        if (sampledType) *sampledType = BxDFType::SPECULAR_REFLECTION;
        return .9f / wo.z;
    }

    float pdf(const Vector3f &wo, const Vector3f &wi, TransportMode mode = TransportMode::Camera) const override {
        return 0.f;
    }

    BxDFType flags() const override {
        return BxDFType::SPECULAR_REFLECTION;
    }

};

class Mirror : public Material {
public:

    Mirror() {}

    BSDF getBSDF(const Interaction &intr) const {
        //BxDF *bxdf = new MirrorBxDF(kd);
        //std::shared_ptr<BxDF> bxdf = std::make_shared<MirrorBxDF>(kd->lookup(intr.uv));
        Frame frame = Frame::fromZ(intr.n);
        //return BSDF(bxdf, frame);
        return BSDF(std::make_shared<MirrorBxDF>(), frame);
    }
};

} // namespace xeno