#pragma once

#include "ray.h"
#include "bsdf.h"

namespace xeno {

class Material {
public:

    virtual BSDF getBSDF(const Interaction &intr) const = 0;

    virtual ~Material() {}
};

} // namespace xeno