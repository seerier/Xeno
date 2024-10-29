#include "quad.h"

namespace xeno {

bool Quad::intersect(const Ray &ray, float &ray_t, Interaction &intr) const {
    // Moller-Trumbore algorithm

    Vector3f pvec = cross(ray.d, e1);

    float det = dot(pvec, e0);
    if (std::fabs(det) < 1e-7) return false;
    float inv_det = 1.0f / det;

    Vector3f tvec = ray.o - p;
    float u = dot(tvec, pvec) * inv_det;
    if (u < 0 || u > 1) return false;

    Vector3f qvec = cross(tvec, e0);
    float v = dot(ray.d, qvec) * inv_det;
    if (v < 0 || v > 1) return false;

    float t = dot(e1, qvec) * inv_det;
    if (t<ray.tMin || t>ray.tMax) return false;
    ray_t = t;
    //ray_t = dot(e1, qvec) * inv_det;
    ray.tMax = ray_t;

    intr.wo = -ray.d;
    Vector3f n = normalize(cross(e0, e1));
    //n = dot(n, ray.d) > 0 ? -n : n;
    intr.n = Normal3f(n);
    //intr.p = ray(ray_t);
    intr.p = p + u * e0 + v * e1;
    intr.uv.x = u, intr.uv.y = v;

    intr.shape = this;

    return true;
}

Interaction Quad::sample(const Interaction &i, const Point2f &uv, float *pdf) const {
    *pdf = 1.f / surfaceArea;
    Interaction intr;
    intr.p = p + uv.x * e0 + uv.y * e1;
    intr.n = normalize(Normal3f(cross(e0, e1)));
    return intr;
}

Interaction Quad::sample(const Point2f &uv, float *pdf) const {
    *pdf = 1.f / surfaceArea;
    Interaction intr;
    intr.p = p + uv.x * e0 + uv.y * e1;
    intr.n = normalize(Normal3f(cross(e0, e1)));
    return intr;
}

}