#include "pinhole.h"

namespace xeno {

Ray Pinhole::generateRay(int i, int j, const Point2f &sample) const {
    /*
    Point3f camPos(0, 0, -1);
        Point3f screenCenterPos(0, 0, 0);
        float xRatio = static_cast<float>(2 * i) / film->xResolution - 1;
        float yRatio = static_cast<float>(2 * j) / film->yResolution - 1;

        Point3f pixelPos = screenCenterPos + Vector3f(xRatio, -yRatio, 0);

        Vector3f dir = pixelPos - camPos;
        return Ray(camPos, dir);
    */
    Point3f camPos(0, 0, 0);
    Point3f screenCenterPos(0, 0, 1);

    float aspectRatio = static_cast<float>(film->xResolution) / film->yResolution;

    float xRatio = (2 * (i + sample.x)) / film->xResolution - 1;
    float yRatio = (2 * (j + sample.y)) / film->yResolution - 1;
    float ratio = std::tan(radians(fov) / 2.f);
    //Point3f pixelPos = screenCenterPos + Vector3f(xRatio * ratio, -yRatio * ratio, 0);
    Point3f pixelPos = screenCenterPos + Vector3f(-xRatio * ratio * aspectRatio, -yRatio * ratio, 0);

    Ray ray(camPos, normalize(pixelPos - camPos));
    return cameraToWorld(ray);
}

}