#include "bxdf.h"

namespace xeno {

class BSDF {
public:
    BSDF() = default;
    BSDF(BxDF &&bxdf, const Frame &frame) :bxdf(std::make_unique<BxDF>(std::move(bxdf))), frame(frame) {}


    //BxDF bxdf;
    std::unique_ptr<BxDF> bxdf;
    Frame frame;
};

}