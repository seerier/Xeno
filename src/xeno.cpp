#include"xeno.h"
#include"film.h"
#include"vecmath.h"
#include"ray.h"
#include"transform.h"
#include"integrator.h"
#include"primitive.h"
#include"parallel.h"
#include"renderParams.h"
#include"shapes/sphere.h"
#include"shapes/triangle.h"
#include"shapes/quad.h"
#include"accelerators/objectList.h"
#include"sensors/pinhole.h"
#include"integrators/simplePathTracer.h"
#include"integrators/PathTracer.h"
#include"io/jsonutil.h"


using namespace xeno;

int main(int argc, char *argv[]) {

    google::InitGoogleLogging(argv[0]);

    int xReso = 1080;
    int yReso = 1080;

    // scene parsing
    std::string inFilename("E:\\Coding\\github_repo\\xeno\\scenes\\firstTry.json");
    json data = loadJson(inFilename);
    xReso = data["imageWidth"];
    yReso = data["imageHeight"];

    RenderParams params = data.template get<RenderParams>();

    //int spp = std::dynamic_pointer_cast<PathTracer>(params.integrator)->spp;
    int spp = data.at("integrator").at("spp").get<int>();

    std::string outFilename = "Cornell-" + data.at("integrator").at("type").get<std::string>() + "-" + std::to_string(xReso) + "_" + std::to_string(yReso) + "-" + std::to_string(spp) + "spp.png";

    // Scene Definition for Cornell Box

    // Sensor
    std::shared_ptr<Film> film = std::make_shared<Film>(xReso, yReso, outFilename);
    Pinhole camera(film, Transform::cameraToWorld(Point3f(278, 278, -800), Point3f(278, 278, 0), Vector3f(0, 1, 0)), 40);

    /*
    // Materials
    std::shared_ptr<Material> whiteMat = std::make_shared<Diffuse>(.73, .73, .73);
    std::shared_ptr<Material> redMat = std::make_shared<Diffuse>(.65, .05, .05);
    std::shared_ptr<Material> greenMat = std::make_shared<Diffuse>(.12, .45, .15);
    std::shared_ptr<Material> blackMat = std::make_shared<Diffuse>(.0, .0, .0);

    // Lights
    //std::shared_ptr<Light> light = std::make_shared<Light>(15.f, 15.f, 15.f);

    // Shapes
    // basic box
    
    //std::shared_ptr<Shape> quad1 = std::make_shared<Quad>(Point3f(555, 0, 0), Vector3f(0, 0, 555), Vector3f(0, 555, 0));
    std::shared_ptr<Shape> quad2 = std::make_shared<Quad>(Point3f(0, 0, 555), Vector3f(0, 0, -555), Vector3f(0, 555, 0));
    std::shared_ptr<Shape> quad3 = std::make_shared<Quad>(Point3f(0, 555, 0), Vector3f(555, 0, 0), Vector3f(0, 0, 555));
    std::shared_ptr<Shape> quad4 = std::make_shared<Quad>(Point3f(0, 0, 555), Vector3f(555, 0, 0), Vector3f(0, 0, -555));
    std::shared_ptr<Shape> quad5 = std::make_shared<Quad>(Point3f(555, 0, 555), Vector3f(-555, 0, 0), Vector3f(0, 555, 0));
    std::shared_ptr<Shape> lightQuad = std::make_shared<Quad>(Point3f(213, 554, 227), Vector3f(130, 0, 0), Vector3f(0, 0, 105));
    std::shared_ptr<AreaLight> light = std::make_shared<AreaLight>(lightQuad, 15.f);


    //std::shared_ptr<Shape> lightQuad2 = std::make_shared<Quad>(Point3f(1, 250, 250), Vector3f(0, 50, 0), Vector3f(0, 0, 50));
    //std::shared_ptr<Shape> lightQuad3 = std::make_shared<Quad>(Point3f(554, 250, 250), Vector3f(0, 0, 50), Vector3f(0, 50, 0));
    //std::shared_ptr<AreaLight> light2 = std::make_shared<AreaLight>(lightQuad2, 10.f);
    //std::shared_ptr<AreaLight> light3 = std::make_shared<AreaLight>(lightQuad3, 10.f);

    ObjectList objectList(std::make_shared<GeometricPrimitive>(params.shapes.at("quad1"), params.materials.at("greenMat")));
    //ObjectList objectList(std::make_shared<GeometricPrimitive>(quad1, greenMat));
    objectList.add(std::make_shared<GeometricPrimitive>(quad2, redMat));
    objectList.add(std::make_shared<GeometricPrimitive>(quad3, whiteMat));
    objectList.add(std::make_shared<GeometricPrimitive>(quad4, whiteMat));
    objectList.add(std::make_shared<GeometricPrimitive>(quad5, whiteMat));
    objectList.add(std::make_shared<GeometricPrimitive>(lightQuad, blackMat, light));
    //objectList.add(std::make_shared<GeometricPrimitive>(lightQuad2, blackMat, light2));
    //objectList.add(std::make_shared<GeometricPrimitive>(lightQuad3, blackMat, light3));

    
    // 2 additional boxes
    objectList.add(std::make_shared<GeometricPrimitive>(std::make_shared<Quad>(Point3f(130, 165, 65), Vector3f(-48, 0, 160), Vector3f(160, 0, 49)), whiteMat));
    objectList.add(std::make_shared<GeometricPrimitive>(std::make_shared<Quad>(Point3f(290, 0, 114), Vector3f(0, 165, 0), Vector3f(-50, 0, 158)), whiteMat));
    objectList.add(std::make_shared<GeometricPrimitive>(std::make_shared<Quad>(Point3f(130, 0, 65), Vector3f(0, 165, 0), Vector3f(160, 0, 49)), whiteMat));
    objectList.add(std::make_shared<GeometricPrimitive>(std::make_shared<Quad>(Point3f(82, 0, 225), Vector3f(0, 165, 0), Vector3f(48, 0, -160)), whiteMat));
    objectList.add(std::make_shared<GeometricPrimitive>(std::make_shared<Quad>(Point3f(240, 0, 272), Vector3f(0, 165, 0), Vector3f(-158, 0, -47)), whiteMat));

    objectList.add(std::make_shared<GeometricPrimitive>(std::make_shared<Quad>(Point3f(265, 330, 296), Vector3f(49, 0, 160), Vector3f(158, 0, -49)), whiteMat));
    objectList.add(std::make_shared<GeometricPrimitive>(std::make_shared<Quad>(Point3f(423, 0, 247), Vector3f(0, 330, 0), Vector3f(49, 0, 159)), whiteMat));
    objectList.add(std::make_shared<GeometricPrimitive>(std::make_shared<Quad>(Point3f(472, 0, 406), Vector3f(0, 330, 0), Vector3f(-158, 0, 50)), whiteMat));
    objectList.add(std::make_shared<GeometricPrimitive>(std::make_shared<Quad>(Point3f(314, 0, 456), Vector3f(0, 330, 0), Vector3f(-49, 0, -160)), whiteMat));
    objectList.add(std::make_shared<GeometricPrimitive>(std::make_shared<Quad>(Point3f(265, 0, 296), Vector3f(0, 330, 0), Vector3f(158, 0, -49)), whiteMat));

    std::shared_ptr<Primitive> objects = std::make_shared<ObjectList>(std::move(objectList));

    std::vector<std::shared_ptr<Light>> lights;
    lights.push_back(light);
    //lights.push_back(light2);
    //lights.push_back(light3);
    */

    //Scene scene(objects, lights);
    std::shared_ptr<ObjectList> list = std::make_shared<ObjectList>();
    for (const auto &primitive : params.primitives) {
        list->add(primitive);
    }
    Scene scene(list, params.lights);

    ParallelInit();
    params.integrator->Render(camera, scene);
    ParallelCleanup();

    std::cout << "spp = " << spp << std::endl;

    return 0;
}

