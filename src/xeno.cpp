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

    int xReso = 1920;
    int yReso = 1080;


    std::unique_ptr<Film> film = std::make_unique<Film>(xReso, yReso, "5Quads-new.png");
    Camera camera(std::move(film));

    // Scene Definition
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
    std::shared_ptr<Shape> objects = std::make_shared<ObjectList>(std::move(objectList));
    Scene scene(objects);

    Integrator integrator;
    integrator.Render(camera, scene);

    return 0;
}

