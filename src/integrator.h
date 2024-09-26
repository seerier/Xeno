#pragma once

#include "transform.h"
#include "sensor.h"
#include "scene.h"

namespace xeno {

class Integrator {
public:
    Spectrum Li(Ray &ray, const Scene &scene) const;
    void Render(Sensor &sensor, const Scene &scene) const;
};

} //namespace xeno