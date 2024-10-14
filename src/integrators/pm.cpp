#include "pm.h"
#include "parallel.h"

namespace xeno {

struct PMPixel {
    Spectrum Ld;
    Vector3<double> LPhoton; // maybe float is not enough and suffer from overflow
    struct VisiblePoint {
        VisiblePoint() {}
        VisiblePoint(const Interaction &intr, const Spectrum &beta) :intr(intr), beta(beta) {}
        Interaction intr;
        Spectrum beta;
    } vp;
};

void PM::Render(Sensor &sensor, const Scene &scene) const{

    int xReso = sensor.film->xResolution;
    int yReso = sensor.film->yResolution;
    float inv_yReso = 1.f / (yReso - 1);
    int nPixels = xReso * yReso;

    //std::unique_ptr<SPPMPixel[]> pixels(new SPPMPixel[nPixels])
    // initialize PMPixels
    std::unique_ptr<PMPixel[]> pixels(new PMPixel[nPixels]);

    for (int iter = 0; iter < nIterations; ++iter) {
        // generate camera rays and setup visible points
        ParallelFor([&](int64_t j) {
            for (int i = 0; i < xReso; ++i) {
                Spectrum beta(1.f);
                Point2f sample(random_float(), random_float());
                Ray ray = sensor.generateRay(i, j, sample);
                float ray_t;
                Interaction intr;
                if (!scene.intersect(ray, ray_t, intr)) continue;
                int index = j * yReso + i;
                if (intr.primitive->isEmitter()) pixels[index].Ld += intr.primitive->getAreaLight()->L(intr, -ray.d);
                //pixels[index].vp.intr = std::move(intr);
                pixels[index].vp.intr = intr;
                pixels[index].vp.beta = beta;
            }
            }, yReso);

        // generate photons
        for (int photonIndex = 0; photonIndex < photonsPerIteration; ++photonIndex) {
            // throughput
            Spectrum beta(1.f);

            // sample a light source
            float lightSelectPdf;
            int lightIndex = scene.uniformSampleOneLight(random_float(), &lightSelectPdf);

            Ray photonRay;
            Normal3f nLight;
            float pPoint, pDir;
            Spectrum Le = scene.lights[lightIndex]->sample_Le(random2D(), random2D(), photonRay, &nLight, &pPoint, &pDir);

            if (Le == Spectrum(0) || pPoint == 0 || pDir == 0) continue;
            beta *= Le * absDot(normalize(photonRay.d), nLight) / (lightSelectPdf * pPoint * pDir);
            if (beta == Spectrum(0)) continue;

            float ray_t;
            Interaction intr;
            if (scene.intersect(photonRay, ray_t, intr)) {
                ParallelFor([&](int j) {
                    for (int i = 0; i < xReso; ++i) {
                        int index = j * yReso + i;
                        PMPixel &pixel = pixels[index];
                        if (pixel.vp.beta == Spectrum(0.f)) continue;
                        if (distanceSquared(intr.p, pixel.vp.intr.p) < radius * radius) {
                            //pixel.LPhoton += beta * pixel.vp.beta * pixel.vp.intr.primitive->getMaterial()->f(pixel.vp.intr.wo, -photonRay.d, pixel.vp.intr.n);
                            Spectrum phi = beta * pixel.vp.beta * pixel.vp.intr.primitive->getMaterial()->f(pixel.vp.intr.wo, -photonRay.d, pixel.vp.intr.n);
                            for (int k = 0; k < 3; ++k)
                                pixel.LPhoton[k] += phi[k];
                        }
                    }

                    }, yReso);
                
            }
        }

        // clear Visible Points 
        ParallelFor([&](int j) {
                    for (int i = 0; i < xReso; ++i) {
                        int index = j * yReso + i;
                        PMPixel &pixel = pixels[index];
                        pixel.vp.beta = Spectrum(0.f);
                    }

                    }, yReso);
    }


    // write image
    ParallelFor([&](int j) {
                    for (int i = 0; i < xReso; ++i) {
                        int index = j * yReso + i;
                        PMPixel &pixel = pixels[index];
                        uint64_t totalN = nIterations * photonsPerIteration;
                        pixel.LPhoton /= (totalN * Pi * radius * radius);
                        //pixel.Ld /= nIterations;
                        Spectrum rgbVal;
                        for (int k = 0; k < 3; ++k)
                            rgbVal[k] += pixel.LPhoton[k] + pixel.Ld[k];

                        sensor.film->getRadiance(i, j, rgbVal);
                    }

                    }, yReso);

    sensor.film->writePng();
}

}