#pragma once

#include "xeno.h"

namespace xeno {

inline float random_float() {
    //Returns a random real in [0, 1).
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

}