#include "bdpt.h"
#include "parallel.h"

namespace xeno {

int randomWalk(const Scene &scene, Ray &ray, Spectrum beta, float pdf, int maxDepth, TransportMode mode, Vertex *path) {
    if (maxDepth <= 0) return 0;
    int bounces = 0;
    float pdfFwd = pdf, pdfRev = 0;
    while (true) {
        Interaction intr;
        float ray_t;
        bool intersected = scene.intersect(ray, ray_t, intr);
        Vertex &vertex = path[bounces], prev = path[bounces - 1];
        if (!intersected) break;
        vertex.setSurface(intr, beta, pdfFwd, prev);

        if (++bounces >= maxDepth) break;

        Vector3f wi, wo = intr.wo;
        BSDF bsdf = intr.getBSDF();
        //float samplePdf;
        BxDFType sampledType;
        //Spectrum f = bsdf.sample_f(wo, random2D(), &wi, &samplePdf, &sampledType, BxDFType::ALL, mode);
        Spectrum f = bsdf.sample_f(wo, random2D(), &wi, &pdfFwd, &sampledType, BxDFType::ALL, mode);

        if (f == Spectrum(0.f) || pdfFwd == 0.f) break;
        beta *= f * absDot(wi, intr.n) / pdfFwd;
        pdfRev = bsdf.pdf(wi, wo, mode);

        if (sampledType | BxDFType::SPECULAR) {
            vertex.delta = true;
            pdfRev = pdfFwd = 0;
        }

        ray = intr.spawnRay(wi);
        prev.pdfRev = vertex.convertDensity(pdfRev, prev);
    }
    return bounces;
}

int generateCameraPath(const Scene &scene, Sensor &sensor, int i, int j, int maxDepth, Vertex *path) {
    Ray ray = sensor.generateRay(i, j, random2D());
    path[0] = Vertex(Interaction(), Spectrum(1.f));
    path[0].intr.p = ray.o;
    return randomWalk(scene, ray, Spectrum(1.f), 1.f, maxDepth - 1, TransportMode::Camera, path + 1);
}

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