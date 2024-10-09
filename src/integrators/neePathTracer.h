#pragma once

#include"integrator.h"

namespace xeno {

class NEEPathTracer : public Integrator {
public:
    NEEPathTracer(int i = 16) :spp(i) {}
    virtual void Render(Sensor &sensor, const Scene &scene) const;
    Spectrum Li(Ray &ray, const Scene &scene) const;

private:
    int spp;
};

} // namespace xeno