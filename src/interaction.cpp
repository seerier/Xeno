#include "interaction.h"
#include "primitive.h"

namespace xeno {

Spectrum Interaction::Le(const Vector3f &w) const {
    const AreaLight *light = primitive->getAreaLight();
    return light ? light->L(*this, w) : Spectrum(.0f, .0f, .0f);
}

Ray Interaction::spawnRayTo(const Interaction &i) const {
    Vector3f dir = i.p - p;
    Point3f o = offsetRayOrigin(p, n, normalize(dir));
    return Ray(o, dir, 1 - ShadowRayEpsilon);
}

} //namespace xeno