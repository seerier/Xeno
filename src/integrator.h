#pragma once

#include "transform.h"
#include "sensor.h"
#include "scene.h"

namespace xeno {

class Integrator {
public:
    virtual void Render(Sensor &sensor, const Scene &scene) const = 0;
    virtual ~Integrator() {}
};

Spectrum estimateDirect(const Scene &scene, const Interaction &intr, const Light &light);

} //namespace xeno