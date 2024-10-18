#include "sppm.h"
#include "parallel.h"

namespace xeno {

struct SPPMPixel {
    Spectrum Ld;
    Spectrum LPhoton; // maybe float is not enough and suffer from overflow
    struct VisiblePoint {
        VisiblePoint() {}
        VisiblePoint(const Interaction &intr, const Spectrum &beta) :intr(intr), beta(beta) {}
        Interaction intr;
        Spectrum beta;
    } vp;
    float N = 0;
    int M = 0;
    float radius;
    Spectrum tau;
};

void SPPM::Render(Sensor &sensor, const Scene &scene) const{

    int xReso = sensor.film->xResolution;
    int yReso = sensor.film->yResolution;
    float inv_yReso = 1.f / (yReso - 1);
    int nPixels = xReso * yReso;

    //std::unique_ptr<SPPMPixel[]> pixels(new SPPMPixel[nPixels])
    // initialize SPPMPixels
    std::unique_ptr<SPPMPixel[]> pixels(new SPPMPixel[nPixels]);

    for (int iter = 0; iter < nIterations; ++iter) {
        // generate camera rays and setup visible points
        ParallelFor([&](int64_t j) {
            for (int i = 0; i < xReso; ++i) {
                //cameraRay intersect
                Spectrum beta(1.f);
                Point2f sample(random_float(), random_float());
                Ray ray = sensor.generateRay(i, j, sample);
                float ray_t;
                Interaction intr;
                if (!scene.intersect(ray, ray_t, intr)) continue;

                int index = j * yReso + i;
                pixels[index].radius = radius;

                if (intr.primitive->isEmitter()) pixels[index].Ld += intr.primitive->getAreaLight()->L(intr, -ray.d);
                float lightSelectPdf;
                int lightIndex = scene.uniformSampleOneLight(random_float(), &lightSelectPdf);
                pixels[index].Ld += beta * estimateDirect(scene, intr, *scene.lights[lightIndex]) / lightSelectPdf;

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
            beta = Le * absDot(normalize(photonRay.d), nLight) / (lightSelectPdf * pPoint * pDir);
            if (beta == Spectrum(0)) continue;

            // trace a photon
            for (int photonbounces = 0; photonbounces < maxDepth; ++photonbounces) {
                float ray_t;
                Interaction intr;
                if (scene.intersect(photonRay, ray_t, intr)) {
                    if (photonbounces > 0) {
                        // search visible points and add contribution
                        ParallelFor([&](int j) {
                            for (int i = 0; i < xReso; ++i) {
                                int index = j * yReso + i;
                                SPPMPixel &pixel = pixels[index];
                                if (pixel.vp.beta == Spectrum(0.f)) continue;
                                if (distanceSquared(intr.p, pixel.vp.intr.p) < pixel.radius * pixel.radius) {
                                    //pixel.LPhoton += beta * pixel.vp.beta * pixel.vp.intr.primitive->getMaterial()->f(pixel.vp.intr.wo, -photonRay.d, pixel.vp.intr.n);
                                    //Spectrum phi = beta * pixel.vp.beta * pixel.vp.intr.primitive->getMaterial()->f(pixel.vp.intr.wo, -photonRay.d, pixel.vp.intr.n);
                                    //pixel.LPhoton += beta * pixel.vp.intr.primitive->getMaterial()->f(pixel.vp.intr.wo, -photonRay.d, pixel.vp.intr.n);
                                    pixel.LPhoton += beta * pixel.vp.intr.material->f(pixel.vp.intr.wo, -photonRay.d, pixel.vp.intr.n);
                                    
                                    pixel.M++;
                                }
                            }
                            }, yReso);
                    }

                    // possibly bounce and generate next ray direction
                    if (photonbounces >= maxDepth - 1) break;
                    //if (photonbounces >= maxDepth - 1) break;
                    Vector3f wi;
                    float pdf;
                    //Spectrum f = intr.primitive->getMaterial()->sample_f(-photonRay.d, &wi, intr.n, random2D(), &pdf);
                    Spectrum f = intr.material->sample_f(-photonRay.d, &wi, intr.n, random2D(), &pdf);
                    if (f == Spectrum(0) || pdf == 0) break;
                    Spectrum newBeta = beta * f * absDot(intr.n, wi) / pdf;
                    float continuePdf = std::min(1.f, newBeta.sum() / beta.sum());
                    continuePdf = .8f;
                    if (random_float() > continuePdf) break;
                    beta = newBeta / continuePdf;
                    
                    //beta *= f * absDot(intr.n, wi) / pdf;
                    photonRay = intr.spawnRay(wi);
                }
            }
        }

        // handle some mathematical operations in sppm
        ParallelFor([&](int j) {
                    for (int i = 0; i < xReso; ++i) {
                        int index = j * yReso + i;
                        SPPMPixel &p = pixels[index];
                        if (p.M > 0) {
                            float gamma = 2.f / 3.f;
                            float Nnew = p.N + gamma * p.M;
                            float Rnew = p.radius * std::sqrt(Nnew / (p.N + p.M));
                            p.tau = (p.tau + p.vp.beta * p.LPhoton) * (Rnew * Rnew) / (p.radius * p.radius);
                            p.radius = Rnew;
                            p.N = Nnew;
                            p.M = 0;
                            p.LPhoton = Spectrum(0.f);
                        }

                        // reset and prepare for next iteration
                        p.vp.beta = Spectrum(0.f);
                    }

                    }, yReso);
    }


    // write image
    ParallelFor([&](int j) {
                    for (int i = 0; i < xReso; ++i) {
                        int index = j * yReso + i;
                        SPPMPixel &pixel = pixels[index];
                        uint64_t totalN = nIterations * photonsPerIteration;
                        pixel.tau /= (totalN * Pi * pixel.radius * pixel.radius);
                        pixel.Ld /= nIterations;
                        Spectrum rgbVal;
                        rgbVal += pixel.tau + pixel.Ld;
                        //rgbVal += pixel.Ld;
                        /*
                        for (int i = 0; i < 2; ++i) {
                            pixel.tau[i] += 1e30;
                        }
                        rgbVal += pixel.tau;
                        */

                        /*
                        for (int i = 0; i < 2; ++i) {
                            pixel.Ld[i] += 1e30;
                        }
                        rgbVal += pixel.Ld;
                        */

                        sensor.film->getRadiance(i, j, rgbVal);
                    }

                    }, yReso);

    sensor.film->writePng();
}

}