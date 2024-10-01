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

} //namespace xeno