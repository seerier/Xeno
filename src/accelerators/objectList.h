#pragma once

#include "shape.h"

namespace xeno{

class ObjectList : public Shape {
public:
    ObjectList() {}
    ObjectList(std::shared_ptr<Shape> obj) { add(obj); }

    void add(std::shared_ptr<Shape> obj) {
        shapes.push_back(obj);
        wholeAabb = wholeAabb.expand(obj->aabb());
    }

    Bounds3f aabb() const override {
        return wholeAabb;
    }

    bool intersect(Ray &r, float &ray_t, Interaction &intr) const override;

private:
    std::vector<std::shared_ptr<Shape>> shapes;
    Bounds3f wholeAabb;
};

} // namespace xeno