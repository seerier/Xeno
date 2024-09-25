#include "integrator.h"

namespace xeno {

Vector3f Integrator::Li(Ray &ray, const Scene &scene) const {
    //return Vector3f(0.2f, 0.3f, 0.5f);
    
    float ray_t;
    Interaction intr;

    int bounces = 0;
    Vector3f Li(0, 0, 0);
    // throughput
    Vector3f beta(1, 1, 1);

    while (1) {
        if (!scene.intersect(ray, ray_t, intr)) {
            Li += beta * Vector3f(0.5, 0.5, 0.5);
            return Li;
        }
        if (bounces > 5) break;
        Diffuse mat(0.5);
        //float f = mat.f(normalize(-ray.d), Vector3f(1, 0, 0), Normal3f(1, 0, 0));
        //float pdf = mat.pdf(normalize(-ray.d), Vector3f(1, 0, 0), Normal3f(1, 0, 0));
        float pdf;
        Vector3f wi;
        float f = mat.sample_f(normalize(-ray.d), &wi, intr.n, Point2f(random_float(), random_float()), &pdf);
        ray = Ray(intr.p, wi);
        beta *= f * dot(intr.n, wi) / pdf;
        ++bounces;
    }
    return Li;
}

void Integrator::Render(Camera &camera, const Scene &scene) const {

    int xReso = camera.film->xResolution;
    int yReso = camera.film->yResolution;

    for (int j = 0; j < yReso; ++j)
        for (int i = 0; i < xReso; ++i) {
            Vector3f rgbVal;
            Ray ray = camera.generateRay(i, j);
            rgbVal = Li(ray, scene);
            camera.film->getRadiance(i, j, rgbVal);
        }

    camera.film->writePng();
}

} //namespace xeno