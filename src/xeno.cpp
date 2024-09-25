#include"xeno.h"
#include"film.h"
#include"vecmath.h"
#include"ray.h"
#include"transform.h"
#include"integrator.h"
#include"shapes/sphere.h"
#include"shapes/triangle.h"
#include"shapes/quad.h"
#include"accelerators/objectList.h"


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

    Point3f pixelPos = screenCenterPos + Vector3f(xRatio, yRatio, 0);



    Vector3f dir = pixelPos - camPos;

    dir /= distance(pixelPos, camPos);

    Vector3f color = cross(Normal3f(0, 0, 1), dir);
    //Vector3f color = cross(dir, Normal3f(0, 0, 1))*0.8;
    //color = faceForward(color, Normal3f(1, 0, 0));

    //if (color != Vector3f(0, 0, 0)) color = normalize(color);
    color = (color + Vector3f(1, 1, 1)) / 2;
    

    /*
    float dtt = dot(dir, Vector3<float>(1, 0, 0));
    dtt = (dtt + 1) / 2;

    Vector3<float> color(dtt, dtt, dtt);
    */

    /*
    float dtt = dir.lengthSquared();
    dtt = dtt / 2;

    Vector3<float> color(dtt, dtt, dtt);
    */


    return color;
}

Ray generateRay(int i, int j, int xReso, int yReso) {
    Point3f camPos(0, 0, -1);
    Point3f screenCenterPos(0, 0, 0);
    float xRatio = static_cast<float>(2 * i) / xReso - 1;
    float yRatio = static_cast<float>(2 * j) / yReso - 1;

    Point3f pixelPos = screenCenterPos + Vector3f(xRatio, -yRatio, 0);



    Vector3f dir = pixelPos - camPos;
    return Ray(camPos, dir);
}


int main(int argc, char *argv[]) {

    google::InitGoogleLogging(argv[0]);

    int xReso = 1920;
    int yReso = 1080;
    //int yReso=1920;

    /*
    //Film film(xReso, yReso, "DirectionalRGB-dirCrossFaceforwardNormalizeMapped-Distance.png");
    //Film film(xReso, yReso, "myDirectionalRGB-1-1-dirInverseCrossMapped-upsideDown-Distance.png");
    Film film(xReso, yReso, "Quad.png");
    for (int j = 0; j < yReso; ++j)
        for (int i = 0; i < xReso; ++i) {


            
            //rgbVal[0] = static_cast<float>(std::abs(i - j) / 200 % 2);
            //rgbVal[1] = 0.2f;
            //rgbVal[2] = (i + j) / 100 % 3 * 0.5f;
            */

            /*
            // Compute color
            Vector3<float> vec = returnColor(i, j, xReso, yReso);
            rgbVal[0] = vec.x;
            rgbVal[1] = vec.y;
            rgbVal[2] = vec.z;
            */
/*
            Vector3f rgbVal;

            // Sphere Intersection Test
            Ray r = generateRay(i, j, xReso, yReso);
            Sphere sph(Point3f(0, 0, 0), 0.5);
            //std::shared_ptr<Shape> t1 = std::make_shared<Triangle>(Point3f(-0.5, -0.5, 0), Point3f(0.5, -0.5, 0), Point3f(0.5, 0.5, 0));
            //Shape *t1 = new Triangle(Point3f(-0.5, -0.5, 0), Point3f(0.5, -0.5, 0), Point3f(0.5, 0.5, 0));
            std::shared_ptr<Shape> q1 = std::make_shared<Quad>(Point3f(-0.5f, -0.5f, 0), Vector3f(1.f, 0, 0), Vector3f(0, 1.f, 0));
            Interaction intr;
            float t = 0;
            if (q1->intersect(r, t, intr)) {
                //rgbVal = Vector3f(intr.p + Vector3f(1, 1, 1)) / 2.f;
                //rgbVal = Vector3f(1, 1, 1);
                //rgbVal = (Vector3f(intr.uv[0],intr.uv[1],0) + Vector3f(1, 1, 1)) / 2.f;
                //rgbVal = Vector3f(intr.uv[0], intr.uv[1], 0);
                rgbVal = Vector3f(intr.uv.x, intr.uv.y, 0.9f);
            }


            // Pass value to film
            film.getRadiance(i, j, rgbVal);
        }

    film.writePng();
    */
    

    /*
    Bounds3f a(Point3f(0, 0, 0), Point3f(1, 1, 1));
    Bounds3f b(Point3f(-1, -1, -1), Point3f(0, 0, 0));
    Point3f p(3, 4, 5);

    std::cout << a.contains(b) << a.overlaps(b) << a.overlapsExclusive(b) << std::endl;
    std::cout << a.intersection(b) << std::endl;
    std::cout << a.containsExclusive(p) << std::endl;
    std::cout << a.distanceSquaredTo(p) << ' ' << a.distanceTo(p) << ' ' << b.distanceSquaredTo(p) << ' ' << b.distanceTo(p) << std::endl;

    std::cout << a.lerp(p) << std::endl;

    std::cout << a.distanceSquaredTo(b) << ' ' << a.distanceSquaredTo(Bounds3f(Point3f(-1, -10, -1), Point3f(-3, -5, 9))) << std::endl;
    */

    /*
    Ray r;
    Ray rb(Point3f(0, 0, 0), Vector3f(3, -2, 1));
    std::cout << r << std::endl;
    std::cout << rb << std::endl;
    std::cout << r(1) << "   " << rb(4.2) << std::endl;
    std::cout << RayDifferential(rb) << std::endl;

    Matrix4x4 m;
    std::cout << m << std::endl;
    float theta = Pi / 3;
    std::cout << std::sin(theta) << std::endl;
    m.m[1][1] = std::cos(theta); m.m[1][2] = -std::sin(theta);
    m.m[2][1] = std::sin(theta); m.m[2][2] = std::cos(theta);
    std::cout << m << std::endl;
    std::cout << transpose(m) << std::endl;
    std::cout << inverse(m) << std::endl;

    Matrix4x4 singular(1, 2, 3, 4, 2, 4, 6, 8, 3, 6, 9, 12, 4, 8, 12, 16);
    std::cout << "singular: " << singular << std::endl;
    std::cout << transpose(singular) << std::endl;
    std::cout << inverse(singular) << std::endl;
    

    
    
    Point3f p(0, 0, 1);

    std::cout << p << std::endl;

    //vec = Transform::translate(Vector3f(0, 2, 2))(vec);
    //vec = Transform::scale(1, 2, 3)(vec);
    Transform translating = Transform::translate(Vector3f(0, 2, 2));
    std::cout << translating << std::endl;

    p = translating(p);

    std::cout << p << std::endl;

    std::cout << Transform::translate(Vector3f(0, 2, 0))(p);

    p = Transform::rotateZ(45)(p);
    std::cout << p << std::endl;

    std::cout << Transform::perspective(45, 0.1, 1) << std::endl;
    std::cout << Transform::orthographic(0.1, 1) << std::endl;
*/

    std::unique_ptr<Film> film = std::make_unique<Film>(xReso, yReso, "5Quads.png");
    Camera camera(std::move(film));

    Integrator integrator;
    integrator.Render(camera);

    return 0;
}

