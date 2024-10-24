#include "misPathTracer.h"
#include "parallel.h"

namespace xeno {

Spectrum MISPathTracer::Li(Ray &ray, const Scene &scene) const {
    
    float ray_t;
    Interaction intr;
    Interaction preIntr;

    float matPdf = 1.f;

    int bounces = 0;
    Spectrum Li(0, 0, 0);
    // throughput
    Spectrum beta(1, 1, 1);

    float beta_maxComponent = 1;
    bool specularBounce = false;

    while (1) {
        if (!scene.intersect(ray, ray_t, intr)) {
            return Li;
        }

        if (intr.primitive->isEmitter()) {
            if (bounces == 0 || specularBounce) {
                Li += beta * intr.Le(-ray.d);
            }
            else {
                float lightPdf = intr.primitive->getAreaLight()->pdf_Li(preIntr, ray.d);
                //float lightPdf = intr.primitive->getAreaLight()->pdf_Li(preIntr, ray.d) / scene.lights.size();
                float weight = balanceHeuristic(matPdf, lightPdf);
                Li += weight * beta * intr.Le(-ray.d);
            }
        }
        if (bounces >= maxDepth) break;

        // rr
        if (bounces >= 5) {
            float continue_p = beta_maxComponent;
            if (continue_p > .9f) continue_p = .9f;
            if (random_float() > continue_p) return Li;
            beta /= continue_p;
        }

        float pdf;
        Vector3f wi;
        VisibilityTester vis;
        BSDF bsdf = intr.getBSDF();

        float lightSelectPdf;
        int lightIndex = scene.uniformSampleOneLight(random_float(), &lightSelectPdf);

        Spectrum Ld = scene.lights[lightIndex]->sample_Li(intr, Point2f(random_float(), random_float()), &wi, &pdf, &vis);

        if (pdf != 0 && Ld != Spectrum(0, 0, 0)) {
            Spectrum f = bsdf.f(-ray.d, wi);
            if (f != Spectrum(0, 0, 0)) {
                if (vis.unoccluded(scene)) {
                    float weight = balanceHeuristic(pdf, bsdf.pdf(-ray.d, wi));
                    Li += weight * beta * Ld * f * absDot(intr.n, wi) / (lightSelectPdf * pdf);
                }
            }
        }

        // generate next ray direction
        BxDFType type;
        Spectrum nextF = bsdf.sample_f(-ray.d, random2D(), &wi, &pdf, &type);
        if (pdf == 0 || nextF == Spectrum(0, 0, 0)) return Li;
        beta *= nextF * absDot(intr.n, wi) / pdf;
        beta_maxComponent = maxComponent(beta);
        if (beta_maxComponent <= .0f) return Li;

        ray = intr.spawnRay(wi);
        preIntr = intr;
        matPdf = pdf;
        specularBounce = isSpecular(type);

        ++bounces;
    }
    return Li;
    

}

void MISPathTracer::Render(Sensor &sensor, const Scene &scene) const {

    int xReso = sensor.film->xResolution;
    int yReso = sensor.film->yResolution;
    float inv_yReso = 1.f / (yReso - 1);



    
    //std::atomic<int> lines(0);
    ParallelFor([&](int64_t j) {
        for (int i = 0; i < xReso; ++i) {
            Spectrum rgbVal;
            for (int k = 0; k < spp; ++k) {
                Point2f sample(random_float(), random_float());
                Ray ray = sensor.generateRay(i, j, sample);;
                rgbVal += Li(ray, scene);
            }
            rgbVal /= spp;
            sensor.film->getRadiance(i, j, rgbVal);
        }
        }, yReso);
       
    sensor.film->writePng();

}
} // namespace xeno