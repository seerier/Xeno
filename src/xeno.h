#pragma once 

#include<iostream>
#include<fstream>
#include<cmath>
#include<algorithm>
#include<vector>
#include<string>
#include<memory>
#include<limits>
#include<iomanip>
#include<random>
#include<chrono>
#include<iomanip>

#include"glog/logging.h"
#include"json.hpp"

//#define float double


namespace xeno {

// json support
using json = nlohmann::json;

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

// scene.h
class Scene;

// ray.h
class Ray;

// medium.h
class Medium;

// interaction
class Interaction;

// shapes
class Shape;
class Quad;
class Triangle;
class Sphere;

//materials
class Material;
class Diffuse;

//accelerators
class ObjectList;

//sensors
class Sensor;
class Film;

//integrators
class Integrator;

//lights
class Light;
class VisibilityTester;
class AreaLight;

//primitive
class Primitive;
class GeometricPrimitive;


// Global Constants
static constexpr float Pi = 3.1415926535f;
static constexpr float PiOver2 = 1.5707963268f;
static constexpr float PiOver4 = 0.7853981634f;
static constexpr float InvPi = 0.3183098862f;
static constexpr float Inv2Pi = 0.1591549431f;
static constexpr float Inv4Pi = 0.0795774715f;

static constexpr float MaxFloat = std::numeric_limits<float>::max();
static constexpr float Infinity = std::numeric_limits<float>::infinity();

// Epsilon to prevent self-occlusion
static constexpr float Epsilon = 1e-4f;
static constexpr float OffsetScale = 1e-4f;
static constexpr float ShadowRayEpsilon = 1e-4f;


// Type aliased
typedef Vector3<float> Spectrum;

//using double = float;

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

inline float radians(float d) {
    return (Pi / 180) * d;
}

inline float degrees(float d) {
    return (180 / Pi) * d;
}

inline uint32_t floatToBits(float f) {
    uint32_t ui;
    memcpy(&ui, &f, sizeof(float));
    return ui;
}

inline float bitsToFloat(uint32_t ui) {
    float f;
    memcpy(&f, &ui, sizeof(uint32_t));
    return f;
}

inline uint64_t floatToBits(double f) {
    uint64_t ui;
    memcpy(&ui, &f, sizeof(double));
    return ui;
}

inline double bitsToFloat(uint64_t ui) {
    double f;
    memcpy(&f, &ui, sizeof(uint64_t));
    return f;
}

}  //namespace xeno
