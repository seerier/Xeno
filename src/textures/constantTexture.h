#pragma once

#include "texture.h"

namespace xeno {

class ConstantTexture : public Texture {
public:
    ConstantTexture() = default;
    ConstantTexture(float v) : value(v) {}
    ConstantTexture(const Spectrum &s) : value(s) {}

    Vector3f lookup(const Point2f &uv) const override {
        return value;
    }

private:
    Spectrum value;
};

} // namespace xeno