#pragma once 

#include<iostream>
#include<cmath>
#include<algorithm>
#include<vector>
#include<string>
#include<memory>
#include<limits>
#include<iomanip>

#include"glog/logging.h"

namespace xeno {

// Global Declarations

// vector.h
template <typename T>
class Vector2;
template <typename T>
class Vector3;
template <typename T>
class Point2;
template <typename T>
class Point3;
template <typename T>
class Point2;
template <typename T>
class Point3;
template <typename T>
class Bounds2;
template <typename T>
class Bounds3;
template <typename T>
class Normal3;

// ray.h
class Ray;


// Global Constants
static constexpr float Pi = 3.1415926535f;
static constexpr float PiOver2 = 1.5707963268f;
static constexpr float PiOver4 = 0.7853981634f;
static constexpr float InvPi = 0.3183098862f;
static constexpr float Inv2Pi = 0.1591549431f;
static constexpr float Inv4Pi = 0.0795774715f;

// Global Utility Functions
inline float gammaCorrect(float v) {
    if (v <= 0.0031308f)return 12.92f * v;
    return 1.055f * pow(v, float(1.f / 2.4f)) - 0.055f;
}

template <typename T, typename U, typename V>
inline T clamp(T val, U low, V high) {
    if (val < low)
        return low;
    else if (val > high)
        return high;
    else
        return val;
}

template <typename T>
inline bool isNaN(const T v) {
    return std::isnan(v);
}

template <>
inline bool isNaN(const int v) {
    return false;
}

inline float lerp(float t, float a, float b){
    return (1 - t) * a + t * b;
}

}  //namespace xeno
