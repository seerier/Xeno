#pragma once

#include "bxdf.h"

namespace xeno {

class BSDF {
public:
    BSDF() = default;
    BSDF(const std::shared_ptr<BxDF> &bxdf, const Frame &frame) : bxdf(bxdf), frame(frame) {}
    //BSDF(BxDF &&bxdf, const Frame &frame) :bxdf(std::make_unique<BxDF>(std::move(bxdf))), frame(frame) {}

    Vector3f toLocal(const Vector3f &v) const {
        return frame.toLocal(v);
    }

    Vector3f fromLocal(const Vector3f &v) const {
        return frame.fromLocal(v);
    }

    Spectrum f(const Vector3f &wo, const Vector3f &wi, TransportMode mode = TransportMode::Camera) const {
        return bxdf->f(toLocal(wo), toLocal(wi), mode);
    }

    Spectrum sample_f(const Vector3f &wo, const Point2f &sample, Vector3f *wi, float *pdf,
        BxDFType *sampledType = nullptr, BxDFType type = BxDFType::ALL, TransportMode mode = TransportMode::Camera) const {
        
        Vector3f wiLocal;
        Spectrum f = bxdf->sample_f(toLocal(wo), sample, &wiLocal, pdf, sampledType, type, mode);
        *wi = fromLocal(wiLocal);
        return f;
    }

    float pdf(const Vector3f &wo, const Vector3f &wi, TransportMode mode = TransportMode::Camera) const {
        return bxdf->pdf(toLocal(wo), toLocal(wi), mode);
    }

    BxDFType flags() const{
        return bxdf->flags();
    }

    std::shared_ptr<BxDF> bxdf;
    //std::unique_ptr<BxDF> bxdf;
    Frame frame;
};

}