#pragma once

#include "vecmath.h"

namespace xeno {

class Texture {
public:
    ~Texture() {}
    virtual Vector3f lookup(const Point2f &uv) const = 0;
};

} //namespace xeno