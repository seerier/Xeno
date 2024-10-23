#pragma once

#include "texture.h"

#include "rtw_stb_image.h"

namespace xeno {

class ImageTexture : public Texture {
public:
    ImageTexture(const std::string &filename) : image(filename) {}

    Vector3f lookup(const Point2f &uv) const override {
        if (image.height() <= 0) return Vector3f(0, 1, 1);
        float u = clamp(uv.x, 0, 1);
        float v = clamp(uv.y, 0, 1);

        auto i = static_cast<int>(u * image.width());
        auto j = static_cast<int>(v * image.height());
        auto pixel = image.pixel_data(i, j);

        auto color_scale = 1.0 / 255.0;
        return Vector3f(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
    }

private:
    rtw_image image;
};

} // namespace xeno