#include"xeno.h"
#include"film.h"
#include"vecmath.h"
#include"ray.h"
#include"transform.h"
#include"integrator.h"
#include"shapes/sphere.h"
#include"shapes/triangle.h"
#include"shapes/quad.h"
#include"accelerators/objectList.h"
#include"sensors/pinhole.h"


using namespace xeno;

Vector3f returnColor(int i, int j, int xReso, int yReso) {
    Point3f camPos(0, 0, -1);
    Point3f screenCenterPos(0, 0, 0);
    float xRatio = static_cast<float>(2 * i) / xReso - 1;
    float yRatio = static_cast<float>(2 * j) / yReso - 1;

    Point3f pixelPos = screenCenterPos + Vector3f(xRatio, yRatio, 0);

    Vector3f dir = pixelPos - camPos;

    dir /= distance(pixelPos, camPos);

    Vector3f color = cross(Normal3f(0, 0, 1), dir);

    color = (color + Vector3f(1, 1, 1)) / 2;

    return color;
}


int main(int argc, char *argv[]) {

    google::InitGoogleLogging(argv[0]);

    int xReso = 1080;
    int yReso = 1080;

    // Scene Definition for Cornell Box

    // Sensor
    std::shared_ptr<Film> film = std::make_shared<Film>(xReso, yReso, "Cornell-Box-50depth-100spp.png");
    Pinhole camera(film, Transform::cameraToWorld(Point3f(278, 278, -800), Point3f(278, 278, 0), Vector3f(0, 1, 0)), 40);

    // Materials
    std::shared_ptr<Material> whiteMat = std::make_shared<Diffuse>(.73, .73, .73);
    std::shared_ptr<Material> redMat = std::make_shared<Diffuse>(.65, .05, .05);
    std::shared_ptr<Material> greenMat = std::make_shared<Diffuse>(.12, .45, .15);
    std::shared_ptr<Material> blackMat = std::make_shared<Diffuse>(.0, .0, .0);

    // Lights
    std::shared_ptr<Light> light = std::make_shared<Light>(15.f, 15.f, 15.f);

    // Shapes
    std::shared_ptr<Shape> quad1 = std::make_shared<Quad>(Point3f(555, 0, 0), Vector3f(0, 0, 555), Vector3f(0, 555, 0), greenMat);
    std::shared_ptr<Shape> quad2 = std::make_shared<Quad>(Point3f(0, 0, 555), Vector3f(0, 0, -555), Vector3f(0, 555, 0), redMat);
    std::shared_ptr<Shape> quad3 = std::make_shared<Quad>(Point3f(0, 555, 0), Vector3f(555, 0, 0), Vector3f(0, 0, 555), whiteMat);
    std::shared_ptr<Shape> quad4 = std::make_shared<Quad>(Point3f(0, 0, 555), Vector3f(555, 0, 0), Vector3f(0, 0, -555), whiteMat);
    std::shared_ptr<Shape> quad5 = std::make_shared<Quad>(Point3f(555, 0, 555), Vector3f(-555, 0, 0), Vector3f(0, 555, 0), whiteMat);
    std::shared_ptr<Shape> lightQuad = std::make_shared<Quad>(Point3f(213, 554, 227), Vector3f(130, 0, 0), Vector3f(0, 0, 105), blackMat);
    lightQuad->bindLight(light);
    ObjectList objectList(quad1);
    objectList.add(quad2);
    objectList.add(quad3);
    objectList.add(quad4);
    objectList.add(quad5);
    objectList.add(lightQuad);

    std::shared_ptr<Shape> objects = std::make_shared<ObjectList>(std::move(objectList));

    Scene scene(objects);

    Integrator integrator;
    integrator.Render(camera, scene);

    /*
    std::shared_ptr<Shape> quad1 = std::make_shared<Quad>(Point3f(-1, -1, 0), Vector3f(0, 2, 0), Vector3f(0, 0, 2), redMat);
    std::shared_ptr<Shape> quad2 = std::make_shared<Quad>(Point3f(-1, -1, 2), Vector3f(0, 2, 0), Vector3f(2, 0, 0), greenMat);
    std::shared_ptr<Shape> quad3 = std::make_shared<Quad>(Point3f(-1, -1, 0), Vector3f(0, 0, 2), Vector3f(2, 0, 0), mat);
    std::shared_ptr<Shape> quad4 = std::make_shared<Quad>(Point3f(1, 1, 2), Vector3f(0, 0, -2), Vector3f(0, -2, 0), blueMat);
    std::shared_ptr<Shape> quad5 = std::make_shared<Quad>(Point3f(1, 1, 2), Vector3f(0, 0, -2), Vector3f(-2, 0, 0), mat);
    std::shared_ptr<Shape> light = std::make_shared<Quad>(Point3f(-0.5, 0.95, 0.5), Vector3f(0, 0, 1), Vector3f(1, 0, 0), blackMat);
    light->bindLight(std::make_shared<Light>());
    ObjectList objectList(quad1);
    objectList.add(quad2);
    objectList.add(quad3);
    objectList.add(quad4);
    objectList.add(quad5);
    objectList.add(light);
    std::shared_ptr<Shape> objects = std::make_shared<ObjectList>(std::move(objectList));
    Scene scene(objects);

    Integrator integrator;
    integrator.Render(camera, scene);
    */

    return 0;
}

