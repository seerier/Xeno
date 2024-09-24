#pragma once

#include "film.h"
#include "ray.h"

namespace xeno {

class Camera {
public:
    Camera(std::unique_ptr<Film> film) :film(std::move(film)) {}

    Ray generateRay(int i, int j) const;

    std::unique_ptr<Film> film;
};

} //namespace xeno