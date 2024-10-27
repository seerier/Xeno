#include "normal.h"
#include "bsdf.h"

namespace xeno {

Spectrum NormalIntegrator::Li(Ray &ray, const Scene &scene) const {
    
    float ray_t;
    Interaction intr;

    if (!scene.intersect(ray, ray_t, intr))
        return Spectrum(.0f, .0f, .0f);

    //return (Vector3f(intr.n) + Vector3f(1, 1, 1)) / 2.f;
    //return Vector3f(intr.p) / 1000.f;

    BSDF bsdf = intr.getBSDF();
    Vector3f wo = bsdf.frame.toLocal(-ray.d);
    return (wo + Vector3f(1)) / 2.f;
}

void NormalIntegrator::Render(Sensor &sensor, const Scene &scene) const {

    int xReso = sensor.film->xResolution;
    int yReso = sensor.film->yResolution;
    float inv_yReso = 1.f / (yReso - 1);

    for (int j = 0; j < yReso; ++j) {
        for (int i = 0; i < xReso; ++i) {
            Spectrum rgbVal;

            //Point2f sample(random_float(), random_float());
            Ray ray = sensor.generateRay(i, j, Point2f(.5f, .5f));
            rgbVal = Li(ray, scene);

            sensor.film->getRadiance(i, j, rgbVal);
            //int percentage = 100 * (j * yReso + i) / (xReso * yReso);
            //std::cout << "\rRendering Progress: " << percentage << "%" << std::flush;
        }
        int percentage = 100 * j * inv_yReso;
        std::cout << "\rRendering Progress: " << percentage << "%" << std::flush;
    }

    sensor.film->writePng();
}
} // namespace xeno