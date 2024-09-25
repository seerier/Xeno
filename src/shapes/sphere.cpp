#include "sphere.h"

namespace xeno {

bool Sphere::intersect(Ray &r, float &ray_t, Interaction &intr) const {
        Vector3f half_b = r.o - p;
        float a = r.d.lengthSquared();
        float b = 2 * dot(half_b, r.d);
        float c = half_b.lengthSquared() - radius * radius;

        float t0, t1;
        if (!quadratic(a, b, c, &t0, &t1)) return false;

        if (r.tMax <= t0 || r.tMin > t1) return false;

        float t;
        if (r.tMin < t0) t = t0;
        else if (r.tMax > t1) t = t1;
        else return false;

        r.tMax = t;
        ray_t = t;
        intr.p = r(t);
        Vector3f n = normalize(intr.p - p);
        intr.wo = n;
        intr.n = Normal3f(n);

        intr.uv.y = std::acosf(intr.wo.z) * InvPi;
        intr.uv.x = 0.5f + std::atan2f(intr.wo.y, intr.wo.x) * InvPi;

        intr.shape = this;

        return true;
    }

}