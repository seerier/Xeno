#include "conductor.h"

namespace xeno {

BxDFType ConductorBxDF::flags() const {
    return mfDistrib.EffectivelySmooth() ? BxDFType::SPECULAR_REFLECTION : BxDFType::GLOSSY_REFLECTION;
}

Spectrum ConductorBxDF::sample_f(const Vector3f &wo, const Point2f &sample, Vector3f *wi, float *pdf,
    BxDFType *sampledType, BxDFType type, TransportMode mode) const {
    if (!(type & BxDFType::REFLECTION) || wo.z == 0) {
        *pdf = 0;
        return Spectrum(0);
    }

    if (mfDistrib.EffectivelySmooth()) {
        *wi = Vector3f(-wo.x, -wo.y, wo.z);
        Spectrum f = FrComplex(std::abs(wo.z), eta, k) / std::abs(wo.z);
        *sampledType = BxDFType::SPECULAR_REFLECTION;
        *pdf = 1.f;
        return f;
    }

    Vector3f wm = mfDistrib.Sample_wm(wo, random2D());
    Vector3f w = reflect(wo, wm);
    if (dot(wo, w) < 0) {
        *pdf = 0;
        return Spectrum(0);
    }
    float p = mfDistrib.PDF(wo, wm) / (4 * absDot(wo, wm));
    float cos_i = std::abs(wo.z), cos_o = std::abs(w.z);
    if (cos_i == 0 || cos_o == 0) {
        *pdf = 0;
        return Spectrum(0);
    }
    Spectrum F = FrComplex(absDot(wo, wm), eta, k);
    Spectrum f = mfDistrib.D(wm) * F * mfDistrib.G(wo, w) / (4 * cos_i * cos_o);
    *wi = w;
    *pdf = p;
    return f;
}

Spectrum ConductorBxDF::f(const Vector3f &wo, const Vector3f &wi, TransportMode mode) const {
    if (mfDistrib.EffectivelySmooth() || dot(wo, wi) < 0) {
        return Spectrum(0);
    }
    float cos_i = std::abs(wo.z), cos_o = std::abs(wi.z);
    if (cos_i == 0 || cos_o == 0) {
        return Spectrum(0);
    }
    Vector3f wm = wi + wo;
    if (wm.lengthSquared() == 0) {
        return Spectrum(0);
    }
    wm = normalize(wm);

    Spectrum F = FrComplex(absDot(wo, wm), eta, k);
    Spectrum f = mfDistrib.D(wm) * F * mfDistrib.G(wo, wi) / (4 * cos_i * cos_o);
    return f;
}

} // namespace xeno