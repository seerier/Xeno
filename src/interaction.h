#pragma once

#include"ray.h"

namespace xeno {

class Interaction {
public:
    Interaction() {}
    Interaction(const Point3f &p, const Normal3f &n, const Vector3f &wo) :p(p), n(n), wo(normalize(wo)) {}
    Point3f p;
    Normal3f n;
    Vector3f wo;
};

}