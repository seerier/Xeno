#pragma once

#include"xeno.h"

namespace xeno {

struct Pixel {
    float rgb[3];
};

class Film {
public:
    Film(int xReso, int yReso, const std::string &name);

    void getRadiance(int x, int y, const float *val) {
        int index = y * xResolution + x;
        std::memcpy(pixels[index].rgb, val, 3 * sizeof(float));
    }

    // Export an 8bit-RGB .png file
    void writePng() const;

    const int xResolution, yResolution;
    const std::string filename;

private:
    std::unique_ptr<Pixel[]> pixels;
};

} // namespace xeno