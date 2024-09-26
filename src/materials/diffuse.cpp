#include "diffuse.h"

namespace xeno {

Spectrum Diffuse::sample_f(const Vector3f &wo, Vector3f *wi, const Normal3f &n, const Point2f &sample, float *pdf) const {
    Vector3f localWi = cosineSampleHemisphere(sample);
    *pdf = localWi.z * InvPi;
    Vector3f s;
    if (fabs(n.z) < 0.9f) s = cross(Vector3f(0, 0, 1), n);
    else s = cross(Vector3f(0, 1, 0), n);
    Vector3f t = cross(n, s);
    *wi = s * localWi.x + t * localWi.y + n * localWi.z;
    return kd * InvPi;
}

} // namespace xeno