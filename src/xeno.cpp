#include"xeno.h"
#include"film.h"
#include"vecmath.h"


using namespace xeno;

/*
int main(int argc, char *argv[]) {

    FLAGS_logtostdout = true;
    FLAGS_minloglevel = 0;

    google::InitGoogleLogging(argv[0]);

    //FLAGS_logtostderr = true;

    LOG(INFO) << "Some information";

    VLOG(1) << "VLOG 1 !!!";

    LOG(WARNING) << "I'm warnning u!";

    DCHECK(1 == 0) << "Error occured!";

    LOG(WARNING) << "I'm warnning u!";

    //google::ShutdownGoogleLogging();
    return 0;
}
*/

Vector3f returnColor(int i, int j, int xReso, int yReso) {
    Point3f camPos(0, 0, -1);
    Point3f screenCenterPos(0, 0, 0);
    float xRatio = static_cast<float>(2 * i) / xReso - 1;
    float yRatio = static_cast<float>(2 * j) / yReso - 1;

    Point3f pixelPos = screenCenterPos + Vector3f(xRatio, -yRatio, 0);



    Vector3f dir = pixelPos - camPos;

    /*
    Vector3f color = cross(dir, Normal3f(0, 0, 1));

    if (color != Vector3f(0, 0, 0)) normalize(color);
    color = (color + Vector3f(1, 1, 1)) / 2;
    */

    /*
    float dtt = dot(dir, Vector3<float>(1, 0, 0));
    dtt = (dtt + 1) / 2;

    Vector3<float> color(dtt, dtt, dtt);
    */

    float dtt = dir.lengthSquared();
    dtt = dtt / 2;

    Vector3<float> color(dtt, dtt, dtt);

    return color;
}


int main(int argc, char *argv[]) {

    google::InitGoogleLogging(argv[0]);

    int xReso = 1920;
    int yReso = 1080;
    
    Film film(xReso, yReso, "DirectionalRGB-VectorPointNormal-LengthSquared.png");
    for (int j = 0; j < yReso; ++j)
        for (int i = 0; i < xReso; ++i) {
            float rgbVal[3];

            /*
            rgbVal[0] = static_cast<float>(std::abs(i - j) / 200 % 2);
            rgbVal[1] = 0.2f;
            //rgbVal[2] = (i + j) / 100 % 3 * 0.5f;
            rgbVal[2] = (i + j) / 100 % 3 * 0.5f;
            */

            // Compute color
            Vector3<float> vec = returnColor(i, j, xReso, yReso);
            rgbVal[0] = vec.x;
            rgbVal[1] = vec.y;
            rgbVal[2] = vec.z;

            // Pass value to film
            film.getRadiance(i, j, rgbVal);
        }

    film.writePng();
    return 0;
}

