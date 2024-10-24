#pragma once

#include "ray.h"

namespace xeno {

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

class BxDF {
public:
    virtual Spectrum f(const Vector3f &wo, const Vector3f &wi) const = 0;

    virtual Spectrum sample_f(const Vector3f &wo, const Point2f &sample, Vector3f *wi, float *pdf,
        BxDFType *sampledType = nullptr, BxDFType type = BxDFType::ALL) const = 0;

    virtual float pdf(const Vector3f &wo, const Vector3f &wi) const = 0;

    virtual ~BxDF() {}
};

} //namespace xeno