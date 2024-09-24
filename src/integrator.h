#pragma once

#include "transform.h"
#include "camera.h"

namespace xeno {

class Integrator {
public:
    Vector3f Li(Ray &ray) const;
    void Render(Camera &camera) const;
};

} //namespace xeno