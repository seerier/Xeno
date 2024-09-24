#include "camera.h"

namespace xeno {

Ray Camera::generateRay(int i, int j) const {
    Point3f camPos(0, 0, -1);
        Point3f screenCenterPos(0, 0, 0);
        float xRatio = static_cast<float>(2 * i) / film->xResolution - 1;
        float yRatio = static_cast<float>(2 * j) / film->yResolution - 1;

        Point3f pixelPos = screenCenterPos + Vector3f(xRatio, -yRatio, 0);

        Vector3f dir = pixelPos - camPos;
        return Ray(camPos, dir);
}

}