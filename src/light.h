#pragma once

#include "interaction.h"

namespace xeno {

class Light {
public:
    Light(float l = 1.f) :color(l, l, l) {}
    Light(float r, float g, float b) :color(r, g, b) {}
    Light(const Spectrum &c) :color(c) {}

    Spectrum L(const Interaction &intr, const Vector3f &w) const {
        if (dot(w, intr.n) > 0) return color;
        else return Vector3f(0, 0, 0);
    }

    Spectrum color;
};

}