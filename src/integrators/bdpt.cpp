#include "bdpt.h"
#include "parallel.h"

namespace xeno {

Spectrum G(const Scene &scene, const Vertex &v0,
           const Vertex &v1) {
    Vector3f d = v0.p() - v1.p();
    float g = 1 / d.lengthSquared();
    d *= std::sqrt(g);
    //if (v0.IsOnSurface()) g *= AbsDot(v0.ns(), d);
    //if (v1.IsOnSurface()) g *= AbsDot(v1.ns(), d);
    g *= absDot(v0.ng(), d);
    g *= absDot(v1.ng(), d);
    VisibilityTester vis(v0.intr, v1.intr);
    if (vis.unoccluded(scene)) return Spectrum(g);
    return Spectrum(0);
}

int randomWalk(const Scene &scene, Ray &ray, Spectrum beta, float pdf, int maxDepth, TransportMode mode, Vertex *path) {
    if (maxDepth <= 0) return 0;
    int bounces = 0;
    float pdfFwd = pdf, pdfRev = 0;
    while (true) {
        Interaction intr;
        float ray_t;
        bool intersected = scene.intersect(ray, ray_t, intr);
        //Vertex &vertex = path[bounces];
        //Vertex &prev = path[bounces - 1];
        Vertex &vertex = path[bounces+1];
        Vertex &prev = path[bounces];
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
    if (maxDepth <= 0) return 0;
    Ray ray = sensor.generateRay(i, j, random2D());
    path[0] = Vertex(Interaction(), Spectrum(1.f));
    path[0].intr.p = ray.o;
    //return randomWalk(scene, ray, Spectrum(1.f), 1.f, maxDepth - 1, TransportMode::Camera, path + 1) + 1;
    return randomWalk(scene, ray, Spectrum(1.f), 1.f, maxDepth - 1, TransportMode::Camera, path) + 1;
}

int generateLightPath(const Scene &scene, int maxDepth, Vertex *path) {
    if (maxDepth <= 0) return 0;

    // sample a light source
    float lightSelectPdf;
    int lightIndex = scene.uniformSampleOneLight(random_float(), &lightSelectPdf);

    Ray photonRay;
    Normal3f nLight;
    float pPoint, pDir;
    Spectrum Le = scene.lights[lightIndex]->sample_Le(random2D(), random2D(), photonRay, &nLight, &pPoint, &pDir);
    if (Le == Spectrum(0) || pPoint == 0 || pDir == 0) return 0;
    Spectrum beta = Le * absDot(normalize(photonRay.d), nLight) / (lightSelectPdf * pPoint * pDir);
    if (beta == Spectrum(0)) return 0;

    path[0] = Vertex(Interaction(), beta);
    path[0].intr.p = photonRay.o;
    //return randomWalk(scene, photonRay, beta, pDir, maxDepth - 1, TransportMode::LightSource, path + 1) + 1;
    return randomWalk(scene, photonRay, beta, pDir, maxDepth - 1, TransportMode::LightSource, path) + 1;
}

Spectrum BDPT::connectBDPT(const Scene &scene, Vertex *cameraVertices, Vertex *lightVertices, int s, int t) const {
    if (s == 1) {
        const Vertex &cameraVertex = cameraVertices[t - 1];

        float pdf;
        Vector3f wi;
        VisibilityTester vis;
        float lightSelectPdf;
        int lightIndex = scene.uniformSampleOneLight(random_float(), &lightSelectPdf);
        Spectrum Ld = scene.lights[lightIndex]->sample_Li(cameraVertex.intr, Point2f(random_float(), random_float()), &wi, &pdf, &vis);

        if (Ld == Spectrum(0) || pdf == 0) return Spectrum(0);
        BSDF bsdf = cameraVertex.intr.getBSDF();
        Spectrum L = cameraVertex.beta * bsdf.f(cameraVertex.intr.wo, wi) * absDot(cameraVertex.ng(), wi) * Ld / (lightSelectPdf * pdf);
        if (!vis.unoccluded(scene)) L = Spectrum(0);
        return L;
    }
    //return Spectrum(.7f);
    const Vertex &lightVertex = lightVertices[s - 1];
    const Vertex &cameraVertex = cameraVertices[t - 1];
    Spectrum radiance = lightVertex.beta * cameraVertex.beta;
    radiance *= cameraVertex.f(lightVertex) * lightVertex.f(cameraVertex);
    if (radiance != Spectrum(0)) radiance *= G(scene, cameraVertex, lightVertex);
    if (radiance == Spectrum(0)) return Spectrum(0);
    return radiance;
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
                //Point2f sample(random_float(), random_float());
                //Ray ray = sensor.generateRay(i, j, sample);;
                //rgbVal += Li(ray, scene);

                /*
                Vertex *cameraPath = new Vertex[maxDepth + 2];
                Vertex *lightPath = new Vertex[maxDepth];
                int nCameraVertices = generateCameraPath(scene, sensor, i, j, maxDepth + 2, cameraPath);
                int nLightVertices = generateLightPath(scene, maxDepth, lightPath);
                */
                std::unique_ptr<Vertex[]> cameraPath(new Vertex[maxDepth + 2]);
                std::unique_ptr<Vertex[]> lightPath(new Vertex[maxDepth]);
                //Vertex *lightPath = new Vertex[maxDepth];
                int nCameraVertices = generateCameraPath(scene, sensor, i, j, maxDepth + 2, cameraPath.get());
                int nLightVertices = generateLightPath(scene, maxDepth, lightPath.get());

                /*
                std::vector<int> inum(nCameraVertices + nLightVertices + 1, 0);
                for (int s = 2; s <= nLightVertices; ++s) {
                    for (int t = 2; t <= nCameraVertices; ++t) {
                        inum[s + t]++;
                    }
                }

                if (nLightVertices >= 2 && nCameraVertices >= 2) {
                    for (int s = 2; s <= nLightVertices; ++s) {
                        for (int t = 2; t <= nCameraVertices; ++t) {
                            rgbVal += connectBDPT(scene, cameraPath.get(), lightPath.get(), s, t) / inum[s + t];
                        }
                    }
                    //rgbVal += connectBDPT(scene, cameraPath.get(), lightPath.get(), nLightVertices, nCameraVertices);
                }
                */
                if (nLightVertices >= 1 && nCameraVertices >= 4) {
                    /*
                    for (int s = 2; s <= nLightVertices; ++s) {
                        rgbVal += connectBDPT(scene, cameraPath.get(), lightPath.get(), s, 2);
                    }
                    */
                    
                    for (int t = 2; t <= nCameraVertices; ++t) {
                        //rgbVal+=connectBDPT(scene, cameraPath.get(), lightPath.get(), 3, t);
                    }
                    rgbVal += connectBDPT(scene, cameraPath.get(), lightPath.get(), 1, 4);
                    //rgbVal += connectBDPT(scene, cameraPath.get(), lightPath.get(), 1, 2);
                }
            }
            rgbVal /= spp;
            sensor.film->getRadiance(i, j, rgbVal);
        }
        }, yReso);
       
    sensor.film->writePng();
}

} // namespace xeno