#pragma once

#include"xeno.h"
#include"vecmath.h"
#include"mathutil.h"
#include"rng.h"

namespace xeno {

class Ray {
public:
    // Ray Public Methods
    Ray() :tMax(Infinity), tMin(Epsilon), time(0.f), medium(nullptr) {}
    Ray(const Point3f &p, const Vector3f &dir, float tmax = Infinity,
        float tmin = Epsilon, float t = 0.f, const Medium *m = nullptr) :
        o(p), d(dir),
        tMax(tmax), tMin(tmin), time(t), medium(m) {}

    Point3f operator()(float t) const {
        return o + d * t;
    }

    bool hasNaN() const {
        return o.hasNaN() || d.hasNaN() || isNaN(tMax) || isNaN(tMin) || isNaN(time);
    }

    friend std::ostream &operator<<(std::ostream &os, const Ray &r) {
        os << "Ray: [ o=" << r.o << ", d=" << r.d << ", tMax=" << r.tMax << ", tMin=" << r.tMin << ", time=" << r.time << " ]";
        return os;
    }


    // Ray Public Data
    Point3f o;
    Vector3f d;
    mutable float tMax;
    float tMin;
    float time;
    const Medium *medium;
};


class RayDifferential : public Ray {
public:
    // RayDifferential Public Methods
    RayDifferential() :hasDifferential(false) {}

    explicit RayDifferential(const Ray &r) : Ray(r), hasDifferential(false) {}

    RayDifferential(const Point3f &p, const Vector3f &dir, float tmax = Infinity,
        float tmin = Epsilon, float t = 0.f, const Medium *m = nullptr)
        : Ray(p, dir, tmax, tmin, t, m), hasDifferential(false) {}

    bool hasNaN() const {
        return Ray::hasNaN() ||
            (hasDifferential &&
                (rxDirection.hasNaN() || ryDirection.hasNaN()
                    || rxOrigin.hasNaN() || ryOrigin.hasNaN()));
    }

    void scaleDifferential(float t) {
        rxOrigin = o + (rxOrigin - o) * t;
        ryOrigin = o + (ryOrigin - o) * t;
        rxDirection = d + (rxDirection - d) * t;
        ryDirection = d + (ryDirection - d) * t;
    }

    friend std::ostream &operator<<(std::ostream &os, const RayDifferential &r) {
        os << "[ " << (Ray &)r << " has differentials? " << (r.hasDifferential ? "true" : "false")
            << ". xo = " << r.rxOrigin << ", xd = " << r.rxDirection << ", yo = " << r.ryOrigin
            << ", yd = " << r.ryDirection << " ]";
        return os;

    }


    // RayDifferential Public Data
    bool hasDifferential;
    Point3f rxOrigin, ryOrigin;
    Vector3f rxDirection, ryDirection;
};

} // namespace xeno