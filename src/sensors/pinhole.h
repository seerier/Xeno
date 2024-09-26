#pragma once

#include "sensor.h"

namespace xeno {

class Pinhole : public Sensor {
public:
    Pinhole(std::shared_ptr<Film> film, const Transform &trans, float fov = 40.f) :cameraToWorld(trans), fov(fov), Sensor(film) {}

    Ray generateRay(int i, int j, const Point2f &sample) const override;

private:
    Transform cameraToWorld;
    float fov;
};

}