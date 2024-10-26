#include"dielectric.h"

namespace xeno {

Spectrum DielectricBxDF::sample_f(const Vector3f &wo, const Point2f &sample, Vector3f *wi, 
                                    float *pdf, BxDFType *sampledType, BxDFType type, 
                                    TransportMode mode) const {
    if (!isSpecular(type)) {
        *pdf = 0;
        return Spectrum(0);
    }

    float R = rDielectric(wo.z, eta);
    float T = 1.f - R;
    float ratioR = R, ratioT = T;

    if (!(type & BxDFType::REFLECTION)) ratioR = 0;
    if (!(type & BxDFType::TRANSMISSION)) ratioT = 0;

    if (ratioR == 0 && ratioT == 0) {
        *pdf = 0;
        return Spectrum(0);
    }

    if (random_float() < ratioR / (ratioR + ratioT)) {
        *wi = Vector3f(-wo.x, -wo.y, wo.z);
        *pdf = ratioR / (ratioR + ratioT);
        if (sampledType) *sampledType = BxDFType::SPECULAR_REFLECTION;
        return R / std::abs(wo.z);
    } else {
        float etap;
        bool valid = refract(wo, eta, &etap, wi);
        if (!valid) {
            *pdf = 0;
            return Spectrum(0.f);
        }

        float f = T / std::abs(wo.z);
        if (mode == TransportMode::Camera) {
            f /= square(etap);
        }

        *pdf = ratioT / (ratioR + ratioT);
        if (sampledType) *sampledType = BxDFType::SPECULAR_TRANSMISSION;
        return f;
    }
}



} // namespace xeno