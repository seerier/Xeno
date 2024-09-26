#include "integrator.h"

namespace xeno {

Spectrum Integrator::Li(Ray &ray, const Scene &scene) const {
    //return Vector3f(0.2f, 0.3f, 0.5f);
    
    float ray_t;
    Interaction intr;

    int bounces = 0;
    Spectrum Li(0, 0, 0);
    // throughput
    Spectrum beta(1, 1, 1);

    while (1) {
        if (!scene.intersect(ray, ray_t, intr)) {
            //Li += beta * Vector3f(0.1, 0.1, 0.1);
            return Li;
        }
        if (bounces > 5) break;
        //Diffuse mat(0.5);
        //float f = mat.f(normalize(-ray.d), Vector3f(1, 0, 0), Normal3f(1, 0, 0));
        //float pdf = mat.pdf(normalize(-ray.d), Vector3f(1, 0, 0), Normal3f(1, 0, 0));
        Li += beta * intr.Le(-ray.d);
        float pdf;
        Vector3f wi;
        const Material *mat = intr.shape->getMaterial();
        Spectrum f = mat->sample_f(normalize(-ray.d), &wi, intr.n, Point2f(random_float(), random_float()), &pdf);
        ray = Ray(intr.p, wi);
        beta *= f * dot(intr.n, wi) / pdf;
        ++bounces;
    }
    return Li;
}

void Integrator::Render(Camera &camera, const Scene &scene) const {

    int xReso = camera.film->xResolution;
    int yReso = camera.film->yResolution;
    float inv_yReso = 1.f / (yReso - 1);

    for (int j = 0; j < yReso; ++j) {
        for (int i = 0; i < xReso; ++i) {
            Spectrum rgbVal;
            Ray ray = camera.generateRay(i, j);
            rgbVal = Li(ray, scene);
            camera.film->getRadiance(i, j, rgbVal);
            //int percentage = 100 * (j * yReso + i) / (xReso * yReso);
            //std::cout << "\rRendering Progress: " << percentage << "%" << std::flush;
        }
        int percentage = 100 * j * inv_yReso;
        std::cout << "\rRendering Progress: " << percentage << "%" << std::flush;
    }

    camera.film->writePng();
}

} //namespace xeno