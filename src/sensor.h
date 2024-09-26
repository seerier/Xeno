#pragma once

#include "film.h"
#include "transform.h"

namespace xeno {

class Sensor {
public:
    Sensor(std::shared_ptr<Film> film) :film(film) {}

    virtual ~Sensor() {}

    virtual Ray generateRay(int i, int j, const Point2f &sample) const = 0;

    std::shared_ptr<Film> film;
};

} //namespace xeno