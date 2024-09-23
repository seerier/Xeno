#include "material.h"

namespace xeno {

class Diffuse : public Material {
public:
    Diffuse(float k = 0.5f) :kd(k) {}

    float f(const Point3f &wo, const Point3f &wi) const override {
        // if (dot(wo, n)<0||dot(wi, n)<0) return 0;

        return kd * InvPi;
    }

    float sample_f(const Point3f &wo, Point3f *wi, const Point3f &n, const Point2f &sample, float *pdf) const override {
        Vector3f localWi = cosineSampleHemisphere(sample);
        *pdf = localWi.z * InvPi;

    }

private:
    float kd;
};

} // namespace xeno