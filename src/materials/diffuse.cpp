#include "diffuse.h"

namespace xeno {

Spectrum DiffuseBxDF::sample_f(const Vector3f &wo, const Point2f &sample, Vector3f *wi, float *pdf) const {
    if (wo.z < 0) {
        *pdf = 0;
        return Spectrum(0);
    }

    *wi = cosineSampleHemisphere(sample);
    *pdf = (*wi).z * InvPi;
    return kd * InvPi;
}

} // namespace xeno