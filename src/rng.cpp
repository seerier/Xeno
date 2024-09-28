#pragma once

#include "rng.h"

namespace xeno {

float random_float3() {
    //Returns a random real in [0, 1).
    static int counter = 1;
    counter *= 3;
    if (counter > 100) counter = counter % 101;
    return static_cast<float>(counter % 101) / 100;
}

float random_float7() {
    //Returns a random real in [0, 1).
    static int counter = 1;
    counter *= 7;
    if (counter > 100) counter = counter % 101;
    return static_cast<float>(counter % 101) / 100;
}

}