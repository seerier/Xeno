#include "bdpt.h"
#include "parallel.h"

namespace xeno {

Spectrum BDPT::connectBDPT(const Scene &scene) const {
    return Spectrum(.7f);
}

void BDPT::Render(Sensor &sensor, const Scene &scene) const {
    // fetch some parameters
    int xReso = sensor.film->xResolution;
    int yReso = sensor.film->yResolution;
    float inv_yReso = 1.f / (yReso - 1);
    int nPixels = xReso * yReso;

    ParallelFor([&](int64_t j) {
        for (int i = 0; i < xReso; ++i) {
            Spectrum rgbVal;
            for (int k = 0; k < spp; ++k) {
                Point2f sample(random_float(), random_float());
                Ray ray = sensor.generateRay(i, j, sample);;
                //rgbVal += Li(ray, scene);
                rgbVal += connectBDPT(scene);
            }
            rgbVal /= spp;
            sensor.film->getRadiance(i, j, rgbVal);
        }
        }, yReso);
       
    sensor.film->writePng();
}

} // namespace xeno