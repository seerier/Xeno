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

    /*
    // abnormal depth==1 specific test
    if (!scene.intersect(ray, ray_t, intr)) {
        //Li += beta * Vector3f(0.1, 0.1, 0.1);
        return Li;
    }

    if (intr.shape->isEmitter()) {
        Li += beta * intr.Le(-ray.d);
        return Li;
    }

    float pdf;
    Vector3f wi;
    const Material *mat = intr.shape->getMaterial();
    Spectrum f = mat->sample_f(normalize(-ray.d), &wi, intr.n, Point2f(random_float(), random_float()), &pdf);
    //ray = Ray(intr.p, normalize(wi));
    ray = intr.spawnRay(wi);
    //beta *= f * dot(intr.n, wi) / pdf;
    beta *= f * Pi;

    Interaction intr2;
    if (!scene.intersect(ray, ray_t, intr2)) {
        return Li;
    }

    if (ray.tMax < 10e-2) {
        return Spectrum(.5f, .5f, .5f);
        //return Spectrum(ray.tMax + .1f, ray.tMax + .1f, ray.tMax + .1f);
        //return Spectrum(ray.o.x * 1e-3f, ray.o.y * 1e-3f, ray.o.z * 1e-3f);
    }
    return Spectrum(0, 0, 0);
    */


   
    // normal pt
    
    while (1) {
        if (!scene.intersect(ray, ray_t, intr)) {
            //Li += beta * Vector3f(0.1, 0.1, 0.1);
            return Li;
        }
        //Diffuse mat(0.5);
        //float f = mat.f(normalize(-ray.d), Vector3f(1, 0, 0), Normal3f(1, 0, 0));
        //float pdf = mat.pdf(normalize(-ray.d), Vector3f(1, 0, 0), Normal3f(1, 0, 0));
        if (intr.shape->isEmitter()) {
            //if(bounces==1)
            Li += beta * intr.Le(-ray.d);
            break;
        }
        if (bounces >= 50) break;

        float pdf;
        Vector3f wi;
        const Material *mat = intr.shape->getMaterial();
        Spectrum f = mat->sample_f(normalize(-ray.d), &wi, intr.n, Point2f(random_float(), random_float()), &pdf);
        //ray = Ray(intr.p, normalize(wi));
        ray = intr.spawnRay(wi);
        beta *= f * dot(intr.n, wi) / pdf;
        //beta *= f * Pi;
        ++bounces;
    }
    return Li;
    

}

void Integrator::Render(Sensor &sensor, const Scene &scene) const {

    int xReso = sensor.film->xResolution;
    int yReso = sensor.film->yResolution;
    float inv_yReso = 1.f / (yReso - 1);
    int spp = 1024;

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

    sensor.film->writePng();
}

} //namespace xeno