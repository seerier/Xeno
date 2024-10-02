#include "shape.h"
#include "light.h"

namespace xeno {

float Shape::pdf_Li(const Interaction &intr, const Vector3f &w) const {
    Ray ray = intr.spawnRay(w);
    float ray_t;
    Interaction iShape;
    if (!intersect(ray, ray_t, iShape)) return 0.f;

    float pdf = (iShape.p - intr.p).lengthSquared() / (surfaceArea * absDot(w, iShape.n));

    if (std::isinf(pdf)) return .0f;
    return pdf;
}

}