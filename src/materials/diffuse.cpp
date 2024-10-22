#include "diffuse.h"

namespace xeno {

Spectrum Diffuse::sample_f(const Vector3f &wo, Vector3f *wi, const Normal3f &n, const Point2f &sample, float *pdf) const {

    if (dot(wo, n) < 0) {
        *pdf = .0;
        return Spectrum(.0f, .0f, .0f);
    }

    Vector3f localWi = cosineSampleHemisphere(sample);
    *pdf = localWi.z * InvPi;
    Vector3f s;
    if (fabs(n.z) < 0.9f) s = normalize(cross(Vector3f(0, 0, 1), n));
    else s = normalize(cross(Vector3f(0, 1, 0), n));
    Vector3f t = cross(n, s);
    *wi = s * localWi.x + t * localWi.y + n * localWi.z;
    return kd * InvPi;
}

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