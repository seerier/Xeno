#pragma once

#include"integrator.h"

namespace xeno {

class SimplePathTracer : public Integrator {
public:
    SimplePathTracer(int i = 16, int maxDepth = 10) :spp(i), maxDepth(maxDepth) {}
    virtual void Render(Sensor &sensor, const Scene &scene) const;
    Spectrum Li(Ray &ray, const Scene &scene) const;

private:
    int spp;
    int maxDepth;
};

} // namespace xeno