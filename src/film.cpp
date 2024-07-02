#include"film.h"
#include"E:\Coding\github_repo\xeno\ext\lodepng.h"
#include"fileutil.h"

namespace xeno {

Film::Film(int xReso, int yReso, const std::string &name)
    :xResolution(xReso), yResolution(yReso),
    filename(name) {
    pixels = std::unique_ptr<Pixel[]>(new Pixel[xReso * yReso]);
}

// Export an 8bit-RGB .png file
void Film::writePng() const {
    // Check filename
    if (!hasExtension(filename, ".png")) {
        std::cerr << "The filename doesn't have a PNG extension!" << std::endl;
    }

    // Storing R, G and B separately
    auto image8bitValues = std::unique_ptr<uint8_t[]>(new uint8_t[3 * xResolution * yResolution]);

    // Some Corrections
    Pixel *pixel = pixels.get();
    auto ptr = image8bitValues.get();
    for (int j = 0; j < yResolution; ++j)
        for (int i = 0; i < xResolution; ++i) {

            
            ptr[0] = static_cast<uint8_t>(clamp(255.f * gammaCorrect(pixel->rgb[0]) + 0.5f, 0.f, 255.f));
            ptr[1] = static_cast<uint8_t>(clamp(255.f * gammaCorrect(pixel->rgb[1]) + 0.5f, 0.f, 255.f));
            ptr[2] = static_cast<uint8_t>(clamp(255.f * gammaCorrect(pixel->rgb[2]) + 0.5f, 0.f, 255.f));



            ptr += 3;
            pixel += 1;
        }

    unsigned int error = lodepng_encode24_file(filename.c_str(), image8bitValues.get(), xResolution, yResolution);
    if (error != 0) {
        std::cerr << "Error writing PNG: " << filename << std::endl;
    }

}



} // namespace xeno