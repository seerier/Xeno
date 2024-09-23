#include "ray.h"

namespace xeno {

class Material {
public:
    virtual float f(const Point3f &wo, const Point3f &wi) const;

    virtual float sample_f(const Point3f &wo, Point3f *wi, const Point3f &n, const Point2f &sample, float *pdf) const;

};

} // namespace xeno