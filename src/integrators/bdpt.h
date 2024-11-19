#pragma once

#include"integrator.h"

namespace xeno {

struct Vertex {
    Vertex() {}
    Vertex(const Interaction &i, const Spectrum &throughput) :intr(i), beta(throughput) {}

    Point3f p() const{
        return intr.p;
    }

    Normal3f ng() const {
        return intr.n;
    }

    float convertDensity(float pdf, const Vertex &next) const {
        // Return solid angle density if _next_ is an infinite area light
        Vector3f w = next.p() - p();
        if (w.lengthSquared() == 0) return 0;
        float invDist2 = 1 / w.lengthSquared();
        pdf *= absDot(next.ng(), w * std::sqrt(invDist2));
        return pdf * invDist2;
    }

    void setSurface(const Interaction &i, const Spectrum &throughput, float pdf, const Vertex &prev) {
        intr = i;
        beta = throughput;
        pdfFwd = prev.convertDensity(pdf, *this);
    }

    Interaction intr;
    Spectrum beta;
    float pdfFwd = 0, pdfRev = 0;
    bool delta = false;
};

class BDPT : public Integrator {
public:
    BDPT(int i = 16, int maxDepth = 10) :spp(i), maxDepth(maxDepth) {}
    virtual void Render(Sensor &sensor, const Scene &scene) const;
    //Spectrum Li(Ray &ray, const Scene &scene) const;
    Spectrum connectBDPT(const Scene &scene) const;

private:
    int spp;
    int maxDepth;
};

} // namespace xeno