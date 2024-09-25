#pragma once

#include "interaction.h"

namespace xeno {

class Light {
public:
    Light() :color(Vector3f(1, 1, 1)) {}
    Light(const Vector3f &c) :color(c) {}

    Vector3f L(const Interaction &intr, const Vector3f &w) const {
        if (dot(w, intr.n) > 0) return color;
        else return Vector3f(0, 0, 0);
    }

    Vector3f color;
};

}