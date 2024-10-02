#pragma once

#include "light.h"

namespace xeno {

class Primitive {
public:
    virtual Bounds3f aabb() const = 0;

    virtual bool intersect(const Ray &r, float &ray_t, Interaction &intr) const = 0;

    virtual ~Primitive() {}

    virtual const Material *getMaterial() const { return nullptr; }
    virtual const AreaLight *getAreaLight() const { return nullptr; }

    bool isEmitter() const { return getAreaLight() != nullptr; }
};

class GeometricPrimitive : public Primitive {
public:
    GeometricPrimitive(const std::shared_ptr<Shape> &shape, const std::shared_ptr<Material> &mat,
        std::shared_ptr<AreaLight> l = nullptr) :shape(shape), material(mat), light(l) {}

    Bounds3f aabb() const override { return shape->aabb(); }

    bool intersect(const Ray &r, float &ray_t, Interaction &intr) const override {
        if (!shape->intersect(r, ray_t, intr)) return false;
        intr.primitive = this;
        return true;
    }

    const Material *getMaterial() const override { return material.get(); }
    const AreaLight *getAreaLight() const override { return light.get(); }

    void bindAreaLight(const std::shared_ptr<AreaLight> &l) { light = l; }

private:
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Material> material;
    std::shared_ptr<AreaLight> light;

};

}