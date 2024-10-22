#include "integrator.h"

namespace xeno {

Spectrum estimateDirect(const Scene &scene, const Interaction &intr, const BSDF &bsdf, const Light &light) {
    Vector3f wi;
    float pdf;
    VisibilityTester vis;
    Spectrum Li = light.sample_Li(intr, random2D(), &wi, &pdf, &vis);
    Spectrum L(.0f);

    if (pdf > 0 && Li != Spectrum(.0f)) {
        //Spectrum f = intr.material->f(intr.wo, wi, intr.n) * absDot(intr.n, wi);
        Spectrum f = bsdf.f(intr.wo, wi) * absDot(intr.n, wi);
        //float brdfPdf = intr.material->pdf(intr.wo, wi, intr.n);
        float brdfPdf = bsdf.pdf(intr.wo, wi);
        if (brdfPdf > 0 && f != Spectrum(.0f)) {
            if (vis.unoccluded(scene)) {
                L += f * Li * balanceHeuristic(pdf, brdfPdf) / pdf;
            }
        }
    }

    //Spectrum f = intr.material->sample_f(intr.wo, &wi, intr.n, random2D(), &pdf);
    Spectrum f = bsdf.sample_f(intr.wo, random2D(), &wi, &pdf);
    f *= absDot(intr.n, wi);
    if (pdf > 0 && f != Spectrum(.0f)) {
        float ray_t;
        Interaction possibleLightInteraction;
        if (scene.intersect(intr.spawnRay(wi), ray_t, possibleLightInteraction)) {
            if (possibleLightInteraction.primitive->getAreaLight() == &light) {
                Spectrum Le = possibleLightInteraction.Le(-wi);
                if (Le != Spectrum(.0f)) {
                    float lightPdf = light.pdf_Li(intr, wi);
                    L += f * Le * balanceHeuristic(pdf, lightPdf) / pdf;
                }
            }
        }
    }
    return L;
}

} //namespace xeno