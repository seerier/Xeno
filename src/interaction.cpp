#include "interaction.h"
#include "shape.h"
#include "light.h"

namespace xeno {

Vector3f Interaction::Le(const Vector3f &w) const {
    return shape->isEmitter() ? shape->L(*this, w) : Vector3f(0, 0, 0);
}

} //namespace xeno