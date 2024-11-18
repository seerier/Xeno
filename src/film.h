#pragma once

#include"xeno.h"
#include"vecmath.h"

namespace xeno {

struct Pixel {
    Vector3f rgb;
};

class Film {
public:
    Film(int xReso, int yReso, const std::string &name);

    void getRadiance(int x, int y, const Spectrum &val) {
        int index = y * xResolution + x;
        //std::memcpy(pixels[index].rgb, val, 3 * sizeof(float));
        pixels[index].rgb = val;
    }

    void addRadiance(int x, int y, const Spectrum &val) {
        int index = y * xResolution + x;
        pixels[index].rgb += val;
    }

    // Export an 8bit-RGB .png file
    void writePng() const;

    const int xResolution, yResolution;
    const std::string filename;

private:
    std::unique_ptr<Pixel[]> pixels;
};

} // namespace xeno