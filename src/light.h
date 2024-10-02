#pragma once

#include "shape.h"

namespace xeno {

class Light {
public:
    virtual Spectrum sample_Li(const Interaction &it, const Point2f &uv, Vector3f *wi, float *pdf, VisibilityTester *vis) const = 0;

    virtual float pdf_Li(const Interaction &it, const Vector3f &w) const = 0;

};

class VisibilityTester {
public:
    VisibilityTester() {}
    VisibilityTester(const Interaction &p0, const Interaction &p1) :p0(p0), p1(p1) {}

    bool unoccluded(const Scene &scene) const;

private:
    Interaction p0, p1;
};

class AreaLight : public Light {
public:
    AreaLight(const std::shared_ptr<Shape> &shape, float l = 1.f) :shape(shape), color(l, l, l) {}
    AreaLight(const std::shared_ptr<Shape> &shape, float r, float g, float b) :shape(shape), color(r, g, b) {}
    AreaLight(const std::shared_ptr<Shape> &shape, const Spectrum &c) :shape(shape), color(c) {}

    Spectrum L(const Interaction &intr, const Vector3f &w) const {
        if (dot(w, intr.n) > 0) return color;
        else return Vector3f(0, 0, 0);
    }

    Spectrum sample_Li(const Interaction &it, const Point2f &uv, Vector3f *wi, float *pdf, VisibilityTester *vis) const override;
    float pdf_Li(const Interaction &it, const Vector3f &w) const override;

private:
    std::shared_ptr<Shape> shape;
    Spectrum color;

};

}