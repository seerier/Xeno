#pragma once

#include "primitive.h"

namespace xeno{

class ObjectList : public Primitive {
public:
    ObjectList() {}
    ObjectList(const std::shared_ptr<Primitive> &obj) { add(obj); }

    void add(const std::shared_ptr<Primitive> &obj) {
        primitives.push_back(obj);
        //wholeAabb = wholeAabb.expand(obj->aabb());
        wholeAabb.expandby(obj->aabb());
    }

    Bounds3f aabb() const override {
        return wholeAabb;
    }

    bool intersect(const Ray &r, float &ray_t, Interaction &intr) const override;

private:
    std::vector<std::shared_ptr<Primitive>> primitives;
    Bounds3f wholeAabb;
};

} // namespace xeno