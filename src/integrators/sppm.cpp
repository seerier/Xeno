#include "sppm.h"
#include "parallel.h"

namespace xeno {

struct SPPMPixel {
    Spectrum Ld;
    //Spectrum LPhoton;
    AtomicFloat LPhoton[3];
    struct VisiblePoint {
        VisiblePoint() {}
        VisiblePoint(const Interaction &intr, const Spectrum &beta) :intr(intr), beta(beta) {}
        Interaction intr;
        Spectrum beta;
        BSDF bsdf;
    } vp;
    float N = 0;
    std::atomic<int> M = 0;
    float radius;
    Spectrum tau;
};

struct SPPMPixelListNode {
    SPPMPixel *pixel;
    SPPMPixelListNode *next;
};

void deleteList(SPPMPixelListNode* head) {
    while (head != nullptr) {
        SPPMPixelListNode* nextNode = head->next;
        delete head;
        head = nextNode;
    }
}

static bool toGrid(const Point3f &p, const Bounds3f &bounds,
                   const int gridRes[3], Point3i *pi) {
    bool inBounds = true;
    Vector3f pg = bounds.normalizedOffset(p);
    for (int i = 0; i < 3; ++i) {
        (*pi)[i] = (int)(gridRes[i] * pg[i]);
        inBounds &= ((*pi)[i] >= 0 && (*pi)[i] < gridRes[i]);
        (*pi)[i] = clamp((*pi)[i], 0, gridRes[i] - 1);
    }
    return inBounds;
}

inline unsigned int hash(const Point3i &p, int hashSize) {
    return (unsigned int)((p.x * 73856093) ^ (p.y * 19349663) ^ (p.z * 83492791)) % hashSize;
}

void SPPM::Render(Sensor &sensor, const Scene &scene) const{

    int xReso = sensor.film->xResolution;
    int yReso = sensor.film->yResolution;
    float inv_yReso = 1.f / (yReso - 1);
    int nPixels = xReso * yReso;

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

                int index = j * yReso + i;
                if (iter == 0) pixels[index].radius = radius;
                if (!scene.intersect(ray, ray_t, intr)) continue;

                if (intr.primitive->isEmitter()) pixels[index].Ld += intr.primitive->getAreaLight()->L(intr, -ray.d);
                float lightSelectPdf;
                int lightIndex = scene.uniformSampleOneLight(random_float(), &lightSelectPdf);

                BSDF bsdf = intr.getBSDF();
                pixels[index].Ld += beta * estimateDirect(scene, intr, bsdf, *scene.lights[lightIndex]) / lightSelectPdf;

                //pixels[index].vp.intr = std::move(intr);
                pixels[index].vp.intr = intr;
                pixels[index].vp.beta = beta;
                pixels[index].vp.bsdf = bsdf;
            }
            }, yReso);

        // create a hash grid for visible points
        // local variables needed
            int gridRes[3];
            Bounds3f gridBound;
            const int hashSize = nPixels;
            std::vector<std::atomic<SPPMPixelListNode *>> grid(hashSize);
            //std::vector<std::atomic<std::shared_ptr<SPPMPixelListNode>>> grid(hashSize);
            float maxRadius = .0f;
        {
            // compute grid extent and resolution
            for (int i = 0; i < nPixels; ++i) {
                const SPPMPixel &pixel = pixels[i];
                if (pixel.vp.beta == Spectrum(0)) continue;
                Bounds3f bound = Bounds3f(pixel.vp.intr.p).expand(pixel.radius);
                gridBound.expandby(bound);
                maxRadius = std::max(maxRadius, pixel.radius);
            }
            Vector3f diag = gridBound.diagonal();
            float maxDiag = maxComponent(diag);
            int baseGridRes = std::max(1, int(maxDiag / maxRadius));
            for (int i = 0; i < 3; ++i) {
                gridRes[i] = std::max(int(baseGridRes * diag[i] / maxDiag), 1);
            }

            // map visible points to grid
            ParallelFor([&](int pixelIndex) {
                SPPMPixel &pixel = pixels[pixelIndex];
                if (!(pixel.vp.beta == Spectrum(0))) {
                    float r = pixel.radius;
                    Point3i pMin, pMax;
                    toGrid(pixel.vp.intr.p - Vector3f(r, r, r), gridBound, gridRes, &pMin);
                    toGrid(pixel.vp.intr.p + Vector3f(r, r, r), gridBound, gridRes, &pMax);
                    for (int k = pMin.z; k <= pMax.z; ++k)
                        for (int j = pMin.y; j <= pMax.y; ++j)
                            for (int i = pMin.x; i <= pMax.x; ++i) {
                                int hashIndex = hash(Point3i(i, j, k), hashSize);
                                //std::unique_ptr<SPPMPixelListNode> node = std::make_unique<SPPMPixelListNode>();
                                // there is a memory leak!!!
                                SPPMPixelListNode *node = new SPPMPixelListNode();
                                //std::shared_ptr<SPPMPixelListNode> node = std::make_shared<SPPMPixelListNode>();
                                node->pixel = &pixel;
                                node->next = grid[hashIndex];
                                while (grid[hashIndex].compare_exchange_weak(node->next, node) == false);
                            }
                }
                }, nPixels, 4096);
        }

        // photon pass
        ParallelFor([&](int photonIndex) {
            // throughput
            Spectrum beta(1.f);

            // sample a light source
            float lightSelectPdf;
            int lightIndex = scene.uniformSampleOneLight(random_float(), &lightSelectPdf);

            Ray photonRay;
            Normal3f nLight;
            float pPoint, pDir;
            Spectrum Le = scene.lights[lightIndex]->sample_Le(random2D(), random2D(), photonRay, &nLight, &pPoint, &pDir);

            if (Le == Spectrum(0) || pPoint == 0 || pDir == 0) return;
            beta = Le * absDot(normalize(photonRay.d), nLight) / (lightSelectPdf * pPoint * pDir);
            if (beta == Spectrum(0)) return;

            // trace a photon
            float ray_t;
            Interaction intr;
            for (int photonbounces = 0; photonbounces < maxDepth; ++photonbounces) {
                if (scene.intersect(photonRay, ray_t, intr)) {
                    BSDF bsdf = intr.getBSDF();
                    if (photonbounces > 0) {
                        // search visible points and add contribution
                        Point3i pi;
                        if (toGrid(intr.p, gridBound, gridRes, &pi)) {
                            int hashIndex = hash(pi, hashSize);
                            
                            for (SPPMPixelListNode *node = grid[hashIndex].load(std::memory_order_relaxed); node != nullptr; node = node->next) {
                                SPPMPixel &pixel = *node->pixel;
                                if (pixel.vp.beta == Spectrum(0)) continue;
                                if (distanceSquared(pixel.vp.intr.p, intr.p) < pixel.radius * pixel.radius) {
                                    //Spectrum LPhoton = beta * pixel.vp.intr.material->f(pixel.vp.intr.wo, -photonRay.d, pixel.vp.intr.n);
                                    Spectrum LPhoton = beta * pixel.vp.bsdf.f(pixel.vp.intr.wo, -photonRay.d);
                                    for (int i = 0; i < 3; ++i) {
                                        pixel.LPhoton[i].add(LPhoton[i]);
                                    }
                                    pixel.M++;
                                }
                            }                            
                        }
                    }

                    // possibly bounce and generate next ray direction
                    if (photonbounces >= maxDepth - 1) break;
                    //if (photonbounces >= maxDepth - 1) break;
                    Vector3f wi;
                    float pdf;

                    //Spectrum f = intr.material->sample_f(-photonRay.d, &wi, intr.n, random2D(), &pdf);
                    Spectrum f = bsdf.sample_f(-photonRay.d, random2D(), &wi, &pdf);

                    if (f == Spectrum(0) || pdf == 0) break;
                    Spectrum newBeta = beta * f * absDot(intr.n, wi) / pdf;
                    float continuePdf = std::min(1.f, newBeta.sum() / beta.sum());
                    //continuePdf = .8f;
                    if (random_float() > continuePdf) break;
                    beta = newBeta / continuePdf;

                    photonRay = intr.spawnRay(wi);
                }
                else break;
            }
            }, photonsPerIteration, 8192);
        

        // handle some mathematical operations in sppm
        ParallelFor([&](int j) {
                    for (int i = 0; i < xReso; ++i) {
                        int index = j * yReso + i;
                        SPPMPixel &p = pixels[index];
                        if (p.M > 0) {
                            float gamma = 2.f / 3.f;
                            float Nnew = p.N + gamma * p.M;
                            float Rnew = p.radius * std::sqrt(Nnew / (p.N + p.M));
                            Spectrum LPhoton;
                            for (int i = 0; i < 3; ++i) LPhoton[i] = p.LPhoton[i];
                            p.tau = (p.tau + p.vp.beta * LPhoton) * (Rnew * Rnew) / (p.radius * p.radius);
                            p.radius = Rnew;
                            p.N = Nnew;
                            p.M = 0;
                            for (int i = 0; i < 3; ++i) p.LPhoton[i] = 0;
                        }

                        // reset and prepare for next iteration
                        p.vp.beta = Spectrum(0.f);
                        deleteList(grid[index]);
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

                        sensor.film->getRadiance(i, j, rgbVal);
                    }

                    }, yReso);

    sensor.film->writePng();
}

}