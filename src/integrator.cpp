#include "integrator.h"
#include "shapes/quad.h"
#include "materials/diffuse.h"
#include "accelerators/objectList.h"

namespace xeno {

Vector3f Integrator::Li(Ray &ray) const {
    //return Vector3f(0.2f, 0.3f, 0.5f);
    std::shared_ptr<Shape> quad1 = std::make_shared<Quad>(Point3f(-1, -1, 0), Vector3f(0, 2, 0), Vector3f(0, 0, 2));
    std::shared_ptr<Shape> quad2 = std::make_shared<Quad>(Point3f(-1, -1, 2), Vector3f(0, 2, 0), Vector3f(2, 0, 0));
    std::shared_ptr<Shape> quad3 = std::make_shared<Quad>(Point3f(-1, -1, 0), Vector3f(0, 0, 2), Vector3f(2, 0, 0));
    std::shared_ptr<Shape> quad4 = std::make_shared<Quad>(Point3f(1, 1, 2), Vector3f(0, 0, -2), Vector3f(0, -2, 0));
    std::shared_ptr<Shape> quad5 = std::make_shared<Quad>(Point3f(1, 1, 2), Vector3f(0, 0, -2), Vector3f(-2, 0, 0));
    ObjectList objectList(quad1);
    objectList.add(quad2);
    objectList.add(quad3);
    objectList.add(quad4);
    objectList.add(quad5);
    float ray_t;
    Interaction intr;

    if (!objectList.intersect(ray, ray_t, intr)) return Vector3f(0.1, 0.1, 0.1);
    Diffuse mat(0.5);
    //float f = mat.f(normalize(-ray.d), Vector3f(1, 0, 0), Normal3f(1, 0, 0));
    //float pdf = mat.pdf(normalize(-ray.d), Vector3f(1, 0, 0), Normal3f(1, 0, 0));
    float pdf;
    Vector3f wi;
    float f = mat.sample_f(normalize(-ray.d), &wi, intr.n, Point2f(random_float(), random_float()), &pdf);
    //return Vector3f(f, f, f) / pdf * dot(wi, intr.n) * 0.8f;
    return (Vector3f(1, 1, 1) + wi) / 2.0f;

}

void Integrator::Render(Camera &camera) const {

    int xReso = camera.film->xResolution;
    int yReso = camera.film->yResolution;

    for (int j = 0; j < yReso; ++j)
        for (int i = 0; i < xReso; ++i) {
            Vector3f rgbVal;
            Ray ray = camera.generateRay(i, j);
            rgbVal = Li(ray);
            camera.film->getRadiance(i, j, rgbVal);
        }

    camera.film->writePng();
}

} //namespace xeno