#pragma once

#include"shape.h"

namespace xeno {

class Sphere {
public:
    Sphere(const Point3f &p, float r) :p(p), radius(r) {}

    Bounds3f aabb() const {
        return Bounds3f(Point3f(p.x - radius, p.y - radius, p.z - radius), Point3f(p.x + radius, p.y + radius, p.z + radius));
    }

    bool intersect(Ray &r, Interaction &intr) const {
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
        intr.p = r(t);
        intr.wo = normalize(intr.p - p);
        intr.n = Normal3f(intr.wo);

        return true;

    }

private:
    Point3f p;
    float radius;
};

}