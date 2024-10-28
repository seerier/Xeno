#pragma once

#include "light.h"
#include "transform.h"

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
        intr.material = material.get();
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

// TransformedPrimitive Declarations
class TransformedPrimitive : public Primitive {
  public:
    // TransformedPrimitive Public Methods
      TransformedPrimitive(std::shared_ptr<Primitive> primitive, const Transform &PrimitiveToWorld)
          :primitive(primitive), PrimitiveToWorld(PrimitiveToWorld) {
      }
      

    bool intersect(const Ray &r, float &ray_t, Interaction &intr) const override {
        Ray ray = inverse(PrimitiveToWorld)(r);
        if (!primitive->intersect(ray, ray_t, intr)) {
            return false;
        }
        intr = PrimitiveToWorld(intr);
        return true;
    }

    const Material *getMaterial() const override { return primitive->getMaterial(); }
    const AreaLight *getAreaLight() const override { return primitive->getAreaLight(); }

    Bounds3f aabb() const override {
        Bounds3f bound = primitive->aabb();
        Bounds3f newBound;
        for (int i = 0; i < 8; ++i) {
            newBound.expandby(PrimitiveToWorld(bound.corner(i)));
        }
        return newBound;
    }

  private:
    // TransformedPrimitive Private Data
    std::shared_ptr<Primitive> primitive;
    const Transform PrimitiveToWorld;
};

}