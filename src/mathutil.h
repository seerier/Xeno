#pragma once

#include "xeno.h"

namespace xeno {

inline bool quadratic(float a, float b, float c, float *t0, float *t1) {
    double discrim = b * b - 4 * a * c;
    if (discrim < 0) return false;
    double root = std::sqrt(discrim);
    float q;
    if (b < 0) q = -.5 * (b - root);
    else q = -.5 * (b + root);
    *t0 = q / a;
    *t1 = c / q;
    if (*t0 > *t1) std::swap(*t0, *t1);
    return true;
}

}