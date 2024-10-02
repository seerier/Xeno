#pragma once

#include"integrator.h"

namespace xeno {

class PathTracer : public Integrator {
public:
    PathTracer(int i = 16) :spp(i) {}
    virtual void Render(Sensor &sensor, const Scene &scene) const;
    Spectrum Li(Ray &ray, const Scene &scene) const;

private:
    int spp;
};

} // namespace xeno