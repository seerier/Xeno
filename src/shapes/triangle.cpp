#include "triangle.h"

namespace xeno {

bool Triangle::intersect(const Ray &ray, float &ray_t, Interaction &intr) const {
    // Moller-Trumbore algorithm
    Vector3f e1 = v1 - v0, e2 = v2 - v0;
    Vector3f pvec = cross(ray.d, e2);

    float det = dot(pvec, e1);
    if (std::fabs(det) < 1e-7) return false;
    float inv_det = 1.0f / det;

    Vector3f tvec = ray.o - v0;
    float u = dot(tvec, pvec) * inv_det;
    if (u < 0 || u > 1) return false;

    Vector3f qvec = cross(tvec, e1);
    float v = dot(ray.d, qvec) * inv_det;
    if (v < 0 || u + v > 1) return false;

    intr.uv.x = u, intr.uv.y = v;
    float t = dot(e2, qvec) * inv_det;
    if (t<ray.tMin || t>ray.tMax) return false;
    ray_t = t;
    //ray_t = dot(e2, qvec) * inv_det;
    ray.tMax = ray_t;

    intr.wo = -ray.d;
    Vector3f n = normalize(cross(e1, e2));
    //n = dot(n, ray.d) < 0 ? n : -n;
    intr.n = Normal3f(n);
    //intr.p = ray(ray_t);
    intr.p = v0 + u * e1 + v * e2;

    intr.shape = this;

    return true;
}

Interaction Triangle::sample(const Point2f &uv, float *pdf) const {
    *pdf = 1.f / surfaceArea;
    Interaction intr;
    intr.p = (uv.x + uv.y > 1.f) ? (v0 + (1 - uv.x) * (v1 - v0) + (1 - uv.y) * (v2 - v0)) : (v0 + uv.x * (v1 - v0) + uv.y * (v2 - v0));
    intr.n = normalize(Normal3f(cross(v1 - v0, v2 - v0)));
    return intr;
}

}