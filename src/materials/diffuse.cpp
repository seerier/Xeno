#include "diffuse.h"

namespace xeno {

Spectrum DiffuseBxDF::sample_f(const Vector3f &wo, const Point2f &sample, Vector3f *wi, float *pdf,
    BxDFType *sampledType, BxDFType type, TransportMode mode) const {
    if (wo.z < 0 || !(type & BxDFType::REFLECTION)) {
        *pdf = 0;
        return Spectrum(0);
    }

    *wi = cosineSampleHemisphere(sample);
    *pdf = (*wi).z * InvPi;
    if (sampledType) *sampledType = BxDFType::DIFFUSE_REFLECTION;
    return kd * InvPi;
}

} // namespace xeno