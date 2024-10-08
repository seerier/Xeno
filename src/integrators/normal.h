#pragma once

#include"integrator.h"

namespace xeno {

class NormalIntegrator : public Integrator {
public:
    NormalIntegrator() {}
    virtual void Render(Sensor &sensor, const Scene &scene) const;
    Spectrum Li(Ray &ray, const Scene &scene) const;


};

} // namespace xeno