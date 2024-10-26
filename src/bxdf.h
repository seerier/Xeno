#pragma once

#include "ray.h"

namespace xeno {

enum TransportMode {
    Camera,
    LightSource
};

enum BxDFType {
    REFLECTION = 1 << 0,
    TRANSMISSION = 1 << 1,
    DIFFUSE = 1 << 2,
    GLOSSY = 1 << 3,
    SPECULAR = 1 << 4,

    DIFFUSE_REFLECTION = DIFFUSE | REFLECTION,
    DIFFUSE_TRANSMISSION = DIFFUSE | TRANSMISSION,
    GLOSSY_REFLECTION = GLOSSY | REFLECTION,
    GLOSSY_TRANSMISSION = GLOSSY | TRANSMISSION,
    SPECULAR_REFLECTION = SPECULAR | REFLECTION,
    SPECULAR_TRANSMISSION = SPECULAR | TRANSMISSION,
    ALL = REFLECTION | TRANSMISSION | DIFFUSE | GLOSSY | SPECULAR,
};

inline bool isReflection(BxDFType type) {
    return type & BxDFType::REFLECTION;
}

inline bool isTransmission(BxDFType type) {
    return type & BxDFType::TRANSMISSION;
}

inline bool isDiffuse(BxDFType type) {
    return type & BxDFType::DIFFUSE;
}

inline bool isGlossy(BxDFType type) {
    return type & BxDFType::GLOSSY;
}

inline bool isSpecular(BxDFType type) {
    return type & BxDFType::SPECULAR;
}

inline bool refract(Vector3f wi, float eta, float *etap, Vector3f *wt) {
    float cosTheta_i = wi.z;
    Normal3f n(0, 0, 1);
    // Potentially flip interface orientation for Snell's law
    if (cosTheta_i < 0) {
        eta = 1 / eta;
        cosTheta_i = -cosTheta_i;
        n = -n;
    }

    // Compute $\cos\,\theta_\roman{t}$ using Snell's law
    float sin2Theta_i = std::max<float>(0, 1 - square(cosTheta_i));
    float sin2Theta_t = sin2Theta_i / square(eta);
    // Handle total internal reflection case
    if (sin2Theta_t >= 1)
        return false;

    float cosTheta_t = std::sqrt(1 - sin2Theta_t);

    *wt = -wi / eta + (cosTheta_i / eta - cosTheta_t) * Vector3f(n);
    // Provide relative IOR along ray to caller
    if (etap)
        *etap = eta;

    return true;
}

class BxDF {
public:
    virtual Spectrum f(const Vector3f &wo, const Vector3f &wi, TransportMode mode = TransportMode::Camera) const = 0;

    virtual Spectrum sample_f(const Vector3f &wo, const Point2f &sample, Vector3f *wi, float *pdf,
        BxDFType *sampledType = nullptr, BxDFType type = BxDFType::ALL, TransportMode mode = TransportMode::Camera) const = 0;

    virtual float pdf(const Vector3f &wo, const Vector3f &wi, TransportMode mode = TransportMode::Camera) const = 0;

    virtual BxDFType flags() const = 0;

    virtual ~BxDF() {}
};

} //namespace xeno