#pragma once

#include "xeno.h"
#include "vecmath.h"

namespace xeno {

inline float random_float() {
    //Returns a random real in [0, 1).
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    static std::mt19937 generator;
    //static std::minstd_rand generator;
    //gpt edited
    //static std::random_device rd;  // 用于生成随机种子
    //static std::mt19937 generator(rd());  // 使用种子初始化生成器
    //std::minstd_rand generator(rd());
    return distribution(generator);
}

inline Point2f random2D() {
    return Point2f(random_float(), random_float());
}

}