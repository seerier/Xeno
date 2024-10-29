#include "light.h"
#include "scene.h"

namespace xeno {

bool VisibilityTester::unoccluded(const Scene &scene) const {
    float ray_t;
    Interaction intr;
    return (!scene.intersect(p0.spawnRayTo(p1), ray_t, intr));
}

Spectrum AreaLight::sample_Li(const Interaction &it, const Point2f &uv, Vector3f *wi, float *pdf, VisibilityTester *vis) const {
    Interaction pLight = shape->sample(it, uv, pdf);
    Vector3f dir = pLight.p - it.p;
    if (dir.lengthSquared() == 0) {
        *pdf = 0;
        return Spectrum(.0f, .0f, .0f);
    }
    *wi = normalize(dir);
    *pdf *= dir.lengthSquared() / absDot(pLight.n, -*wi);
    if (std::isinf(*pdf)) {
        *pdf = 0.f;
        return Spectrum(.0f, .0f, .0f);
    }
    *vis = VisibilityTester(it, pLight);
    return L(pLight, -*wi);
}

float AreaLight::pdf_Li(const Interaction &it, const Vector3f &w) const {
    return shape->pdf_Li(it, w);
}

Spectrum AreaLight::sample_Le(const Point2f &uPoint, const Point2f &uDir, Ray &ray, Normal3f *nLight, float *pPoint, float *pDir) const {

    Interaction pLight = shape->sample(uPoint, pPoint);
    *nLight = pLight.n;

    Vector3f localWi = cosineSampleHemisphere(uDir);
    *pDir = localWi.z * InvPi;

    const Normal3f &n = *nLight;
    Vector3f s;
    if (fabs(n.z) < 0.9f) s = normalize(cross(Vector3f(0, 0, 1), n));
    else s = normalize(cross(Vector3f(0, 1, 0), n));
    Vector3f t = cross(n, s);
    Vector3f wi = s * localWi.x + t * localWi.y + n * localWi.z;

    ray = pLight.spawnRay(wi);
    return L(pLight, wi);
}

} // namespace xeno