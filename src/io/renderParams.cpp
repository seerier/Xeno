#include"renderParams.h"
#include"cmdOptions.h"
#include"transform.h"
#include"integrators/simplePathTracer.h"
#include"integrators/pathTracer.h"
#include"materials/diffuse.h"
#include"shapes/quad.h"
#include"shapes/sphere.h"
#include"shapes/triangleMesh.h"
#include"shapes/triangle.h"
#include"sensors/pinhole.h"

namespace xeno {

std::shared_ptr<Integrator> RenderParams::createIntegrator(const json &j) const {
    std::string type = j.at("type").get<std::string>();
    if (type == "SimplePathTracer") {
        int spp = j.at("spp").get<int>();
        if (cmdOption.spp != 0) spp = cmdOption.spp;
        return std::make_shared<SimplePathTracer>(spp);
    }
    else if (type == "PathTracer") {
        int spp = j.at("spp").get<int>();
        if (cmdOption.spp != 0) spp = cmdOption.spp;
        return std::make_shared<PathTracer>(spp);
    }

    std::cerr << "Failed to create integrator for type: " << type << std::endl;
    throw std::runtime_error("Unknown integrator type: " + type);
}

void RenderParams::createMaterial(const json &j) {
    std::string name = j.at("name").get<std::string>();
    std::string type = j.at("type").get<std::string>();
    if (type == "Diffuse") {
        std::vector<float> albedo;
        auto albedoValue = j.at("albedo");
        if (albedoValue.is_array()) {
            if (albedoValue.size() == 3) {
                albedo = albedoValue.get<std::vector<float>>();
            }
            else {
                std::cerr << "albedo in " << name << " doesn't have three values" << std::endl;
                throw std::runtime_error("albedo in " + name + " doesn't have three values");
            }
        }
        else {
            if (albedoValue.is_number()) {
                albedo = std::vector<float>(3, albedoValue.get<float>());
            }
            else {
                throw std::runtime_error("albedo in " + name + " is not number or array");
            }
        }
        materials.emplace(name, std::make_shared<Diffuse>(albedo[0], albedo[1], albedo[2]));
        return;
    }

    std::cerr << "Failed to create material for name: " << name << std::endl;
    throw std::runtime_error("Unknown material name: " + name);
}

inline std::vector<float> threeValueCheck(const json &j, const std::string &name, const std::string attribute) {
    auto threeValue = j.at(attribute);
    if (threeValue.is_array()) {
        if (threeValue.size() == 3) {
            return threeValue.get<std::vector<float>>();
        }
        else {
            LOG(ERROR) << attribute + " in " + name + " doesn't have three values";
            throw std::runtime_error(attribute + " in " + name + " doesn't have three values");
        }
    }
    else {
        LOG(ERROR) << attribute + " in " + name + " is not array";
        throw std::runtime_error(attribute + " in " + name + " is not array");
    }
}

inline Point3f threeValue2Point3f(const std::vector<float> &values) {
    return Point3f(values[0], values[1], values[2]);
}

inline Vector3f threeValue2Vector3f(const std::vector<float> &values) {
    return Vector3f(values[0], values[1], values[2]);
}

void RenderParams::createShape(const json &j) {
    std::string name = j.at("name").get<std::string>();
    std::string type = j.at("type").get<std::string>();
    if (type == "Sphere") {
        std::vector<float> point = threeValueCheck(j, name, "origin");
        float radius = j.at("radius").get<float>();

        //shapes.emplace(name, std::make_shared<Sphere>(Point3f(point[0], point[1], point[2]), radius));
        //shapes.emplace(name, std::vector<std::shared_ptr<Sphere>>(1, std::make_shared<Sphere>(Point3f(point[0], point[1], point[2]), radius)));
        shapes[name].emplace_back(std::make_shared<Sphere>(Point3f(point[0], point[1], point[2]), radius));
        return;
    }
    else if (type == "Quad") {
        std::vector<float> p = threeValueCheck(j, name, "p");
        std::vector<float> e0 = threeValueCheck(j, name, "e0");
        std::vector<float> e1 = threeValueCheck(j, name, "e1");
        //shapes.emplace(name, std::make_shared<Quad>(Point3f(p[0], p[1], p[2]), Vector3f(e0[0], e0[1], e0[2]), Vector3f(e1[0], e1[1], e1[2])));
        shapes[name].emplace_back(std::make_shared<Quad>(Point3f(p[0], p[1], p[2]), Vector3f(e0[0], e0[1], e0[2]), Vector3f(e1[0], e1[1], e1[2])));
        return;
    }
    else if (type == "Triangle") {
        std::vector<float> v0 = threeValueCheck(j, name, "v0");
        std::vector<float> v1 = threeValueCheck(j, name, "v1");
        std::vector<float> v2 = threeValueCheck(j, name, "v2");
        //shapes.emplace(name, std::make_shared<Triangle>(Point3f(v0[0], v0[1], v0[2]), Point3f(v1[0], v1[1], v1[2]), Point3f(v2[0], v2[1], v2[2])));
        shapes[name].emplace_back(std::make_shared<Triangle>(Point3f(v0[0], v0[1], v0[2]), Point3f(v1[0], v1[1], v1[2]), Point3f(v2[0], v2[1], v2[2])));
        return;
    }
    else if (type == "ObjTriangleMesh") {
        std::string path = j.at("path").get<std::string>();
        ObjTriangleMesh mesh;
        if (mesh.createFromObjFile(path)) {
            //std::vector<Triangle> triList = mesh.toTriangleList();
            std::vector<std::shared_ptr<Triangle>> triList = mesh.toSharedTriangleList();
            for (const auto &triPtr : triList) {
                shapes[name].emplace_back(triPtr);
            }
        }
        return;
    }

    std::cerr << "Failed to create shape for name: " << name << std::endl;
    throw std::runtime_error("Unknown shape name: " + name);
}

void RenderParams::createPrimitive(const json &j) {
    std::string material = j.at("material").get<std::string>();
    std::string shape = j.at("shape").get<std::string>();
    if (j.contains("areaLight")) {
        std::string areaLight = j.at("areaLight").get<std::string>();
        //primitives.push_back(std::make_shared<GeometricPrimitive>(shapes.at(shape), materials.at(material), areaLights.at(areaLight)));
        for (const auto &shapePtr : shapes.at(shape)) {
            primitives.push_back(std::make_shared<GeometricPrimitive>(shapePtr, materials.at(material), areaLights.at(areaLight)));
        }
    }
    else {
        //primitives.push_back(std::make_shared<GeometricPrimitive>(shapes.at(shape), materials.at(material)));
        for (const auto &shapePtr : shapes.at(shape)) {
            primitives.push_back(std::make_shared<GeometricPrimitive>(shapePtr, materials.at(material)));
        }
    }
    return;
}

void RenderParams::createLight(const json &j) {
    std::string name = j.at("name").get<std::string>();
    std::string type = j.at("type").get<std::string>();
    if (type == "AreaLight") {
        std::string shape = j.at("shape").get<std::string>();

        std::vector<float> emission;
        auto emissionValue = j.at("emission");
        if (emissionValue.is_array()) {
            if (emissionValue.size() == 3) {
                emission = emissionValue.get<std::vector<float>>();
            }
            else {
                std::cerr << "emission in " << name << " doesn't have three values" << std::endl;
                throw std::runtime_error("emission in " + name + " doesn't have three values");
            }
        }
        else {
            if (emissionValue.is_number()) {
                emission = std::vector<float>(3, emissionValue.get<float>());
            }
            else {
                LOG(ERROR) << "emission in " + name + " is not number or array";
                throw std::runtime_error("emission in " + name + " is not number or array");
            }
        }
        for (const auto &shapePtr : shapes.at(shape)) {
            std::shared_ptr<AreaLight> l = std::make_shared<AreaLight>(shapePtr, emission[0], emission[1], emission[2]);
            lights.push_back(l);
            areaLights.emplace(name, l);
        }
        //std::shared_ptr<AreaLight> l = std::make_shared<AreaLight>(shapes.at(shape), emission[0], emission[1], emission[2]);
        //lights.push_back(l);
        //areaLights.emplace(name, l);
        return;
    }

    std::cerr << "Failed to create light for name: " << name << std::endl;
    throw std::runtime_error("Unknown light name: " + name);
}

std::shared_ptr<Sensor> RenderParams::createSensor(const json &scenejson) const {
    const json j = scenejson.at("sensor");
    std::string sceneName = scenejson.at("name").get<std::string>();
    std::string type = j.at("type").get<std::string>();
    const json transform = j.at("transform");
    std::vector<float> pos = threeValueCheck(transform, type, "pos");
    std::vector<float> lookat = threeValueCheck(transform, type, "lookat");
    std::vector<float> up = threeValueCheck(transform, type, "up");

    Transform trans = Transform::cameraToWorld(threeValue2Point3f(pos), threeValue2Point3f(lookat), threeValue2Vector3f(up));

    int xReso = scenejson.at("imageWidth").get<int>();
    if (cmdOption.width != 0) xReso = cmdOption.width;
    int yReso = scenejson.at("imageHeight").get<int>();
    if (cmdOption.height != 0) yReso = cmdOption.height;
    int spp = scenejson.at("integrator").at("spp").get<int>();
    if (cmdOption.spp != 0) spp = cmdOption.spp;

    std::string outfilename = sceneName + "-" + scenejson.at("integrator").at("type").get<std::string>() + std::to_string(xReso) +
        "_" + std::to_string(yReso) + "-" + std::to_string(spp) + "spp.png";
    if (cmdOption.outFilename != "") outfilename = cmdOption.outFilename;

    if (type == "pinhole") {
        std::shared_ptr<Film> film = std::make_shared<Film>(xReso, yReso, outfilename);
        if (j.contains("fov")) {
            float fov = j.at("fov").get<float>();
            return std::make_shared<Pinhole>(film, trans, fov);
        }
        else {
            return std::make_shared<Pinhole>(film, trans);
        }
    }

    return nullptr;
    LOG(ERROR) << "Fail to create sensor for type: " << type;
}

void from_json(const json &j, RenderParams &params) {
    params.integrator = params.createIntegrator(j.at("integrator"));
    json mats = j.at("materials");
    for (const auto &mat : mats) {
        params.createMaterial(mat);
    }
    json jsonShapes = j.at("shapes");
    for (const auto &jsonShape : jsonShapes) {
        params.createShape(jsonShape);
    }
    json jsonLights = j.at("lights");
    for (const auto &jsonLight : jsonLights) {
        params.createLight(jsonLight);
    }
    json jsonPrimitives = j.at("primitives");
    for (const auto &jsonPrimitive : jsonPrimitives) {
        params.createPrimitive(jsonPrimitive);
    }
    params.sensor = params.createSensor(j);
}

}