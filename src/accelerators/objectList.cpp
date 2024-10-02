#include"objectList.h"

namespace xeno {

bool ObjectList::intersect(const Ray &r, float &ray_t, Interaction &intr) const {
    int n = primitives.size();
    bool flag = false;
    for (int i = 0; i < n; ++i) {
        if (primitives[i]->intersect(r, ray_t, intr)) flag = true;
    }
    return flag;
}

}