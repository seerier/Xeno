#pragma once

#include"xeno.h"
#include"jsonutil.h"

namespace xeno {

struct RenderParams {

    std::shared_ptr<Integrator> createIntegrator(const json &j) const;
    void createMaterial(const json &j);
    void createShape(const json &j);
    void createPrimitive(const json &j);
    void createLight(const json &j);
    std::shared_ptr<Sensor> createSensor(const json &j) const;
    std::shared_ptr<Primitive> createAccelerator(const json &j) const;


    std::shared_ptr<Integrator> integrator;
    std::shared_ptr<Sensor> sensor;
    std::shared_ptr<Primitive> accelerator;
    std::map<std::string, std::shared_ptr<Material>> materials;
    //std::map<std::string, std::shared_ptr<Shape>> shapes;
    std::map<std::string, std::vector<std::shared_ptr<Shape>>> shapes;
    std::map<std::string, std::vector<std::shared_ptr<AreaLight>>> areaLightsMap;
    //std::vector<std::shared_ptr<AreaLight>> areaLights;
    std::vector<std::shared_ptr<Primitive>> primitives;
    std::vector<std::shared_ptr<Light>> lights;
};

void from_json(const json &j, RenderParams &params);

}