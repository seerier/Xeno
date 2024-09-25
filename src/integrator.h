#pragma once

#include "transform.h"
#include "camera.h"
#include "scene.h"

namespace xeno {

class Integrator {
public:
    Vector3f Li(Ray &ray, const Scene &scene) const;
    void Render(Camera &camera, const Scene &scene) const;
};

} //namespace xeno