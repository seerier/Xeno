#include "quad.h"

namespace xeno {

bool Quad::intersect(Ray &ray, float &ray_t, Interaction &intr) const {
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

    intr.uv.x = u, intr.uv.y = v;
    ray_t = dot(e1, qvec) * inv_det;
    return true;
}

}