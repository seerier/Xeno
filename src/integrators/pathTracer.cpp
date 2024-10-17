#include "pathTracer.h"
#include "parallel.h"

namespace xeno {

Spectrum PathTracer::Li(Ray &ray, const Scene &scene) const {

    float ray_t;
    Interaction intr;

    int bounces = 0;
    Spectrum Li(0, 0, 0);
    // throughput
    Spectrum beta(1, 1, 1);

    float beta_maxComponent = 1;

    while (1) {
        if (!scene.intersect(ray, ray_t, intr)) {
            return Li;
        }

        if (bounces == 0) {
            if (intr.primitive->isEmitter()) {
                Li += beta * intr.Le(-ray.d);
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
        const Material *mat = intr.primitive->getMaterial();

        // estimate direct light to this intersect point
        float lightSelectPdf;
        int lightIndex = scene.uniformSampleOneLight(random_float(), &lightSelectPdf);
        Li += beta * estimateDirect(scene, intr, *scene.lights[lightIndex]) / lightSelectPdf;

        // generate next ray direction
        Spectrum nextF = mat->sample_f(normalize(-ray.d), &wi, intr.n, Point2f(random_float(), random_float()), &pdf);
        if (pdf == 0 || nextF == Spectrum(0, 0, 0)) return Li;
        beta *= nextF * absDot(intr.n, wi) / pdf;
        beta_maxComponent = maxComponent(beta);
        if (beta_maxComponent <= .0f) return Li;

        ray = intr.spawnRay(wi);

        ++bounces;
    }
    return Li;
    

}

void PathTracer::Render(Sensor &sensor, const Scene &scene) const {

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