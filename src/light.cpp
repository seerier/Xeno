#include "light.h"
#include "scene.h"

namespace xeno {

bool VisibilityTester::unoccluded(const Scene &scene) const {
    float ray_t;
    Interaction intr;
    return (!scene.intersect(p0.spawnRayTo(p1), ray_t, intr));
}

Spectrum AreaLight::sample_Li(const Interaction &it, const Point2f &uv, Vector3f *wi, float *pdf, VisibilityTester *vis) const {
    Interaction pLight = shape->sample(uv, pdf);
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

} // namespace xeno