#include"objectList.h"

namespace xeno {

bool ObjectList::intersect(Ray &r, float &ray_t, Interaction &intr) const {
    int n = shapes.size();
    bool flag = false;
    for (int i = 0; i < n; ++i) {
        if (shapes[i]->intersect(r, ray_t, intr)) flag = true;
    }
    return flag;
}

}