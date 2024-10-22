#include "simplePathTracer.h"
#include "parallel.h"

namespace xeno {

Spectrum SimplePathTracer::Li(Ray &ray, const Scene &scene) const {
    //return Vector3f(0.2f, 0.3f, 0.5f);
    
    float ray_t;
    Interaction intr;

    int bounces = 0;
    Spectrum Li(0, 0, 0);
    // throughput
    Spectrum beta(1, 1, 1);

    while (1) {
        if (!scene.intersect(ray, ray_t, intr)) {
            return Li;
        }
        if (intr.primitive->isEmitter()) {
            //if(bounces==1)
            Li += beta * intr.Le(-ray.d);
            break;
        }
        if (bounces >= 50) break;

        float pdf;
        Vector3f wi;
        BSDF bsdf = intr.getBSDF();
        Spectrum f = bsdf.sample_f(-ray.d, random2D(), &wi, &pdf);

        if (pdf == 0 || f == Spectrum(0, 0, 0)) return Li;
        ray = intr.spawnRay(wi);
        beta *= f * absDot(intr.n, wi) / pdf;

        ++bounces;
    }
    return Li;
    

}

void SimplePathTracer::Render(Sensor &sensor, const Scene &scene) const {

    int xReso = sensor.film->xResolution;
    int yReso = sensor.film->yResolution;
    float inv_yReso = 1.f / (yReso - 1);

    /*
    for (int j = 0; j < yReso; ++j) {
        for (int i = 0; i < xReso; ++i) {
            Spectrum rgbVal;
            for (int k = 0; k < spp; ++k) {
                Point2f sample(random_float(), random_float());
                Ray ray = sensor.generateRay(i, j, sample);;
                rgbVal += Li(ray, scene);
            }
            rgbVal /= spp;
            sensor.film->getRadiance(i, j, rgbVal);
            //int percentage = 100 * (j * yReso + i) / (xReso * yReso);
            //std::cout << "\rRendering Progress: " << percentage << "%" << std::flush;
        }
        int percentage = 100 * j * inv_yReso;
        std::cout << "\rRendering Progress: " << percentage << "%" << std::flush;
    }
    */

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
            //int finished = ++lines;
            //int percentage = 100 * finished * inv_yReso;
            //std::cout << "\rRendering Progress: " << percentage << "%" << std::flush;
        }
        }, yReso);

    sensor.film->writePng();
}
} // namespace xeno