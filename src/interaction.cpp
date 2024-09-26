#include "interaction.h"
#include "shape.h"
#include "light.h"

namespace xeno {

Spectrum Interaction::Le(const Vector3f &w) const {
    return shape->isEmitter() ? shape->L(*this, w) : Spectrum(0, 0, 0);
}

} //namespace xeno