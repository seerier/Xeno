#include"xeno.h"
#include"film.h"


using namespace xeno;


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

/*
int main(int argc, char *argv[]) {
    int xReso = 1920;
    int yReso = 1080;
    
    Film film(xReso, yReso, "singleThreadRGB-1000.png");
    for (int j = 0; j < yReso; ++j)
        for (int i = 0; i < xReso; ++i) {
            float rgbVal[3];
            rgbVal[0] = static_cast<float>(std::abs(i - j) / 200 % 2);
            rgbVal[1] = 0.5f;
            //rgbVal[2] = (i + j) / 100 % 3 * 0.5f;

            // Assign some task to consume time.
            std::vector<int> vec;
            int count = (argc == 2) ? atoi(argv[1]) : 1000;
            for (int k = 0; k < count; ++k) {
                vec.emplace_back(k);
            }
            std::sort(vec.begin(), vec.end());
            rgbVal[2] = (i + j) / 100 % 3 * 0.5f;

            // Pass value to film
            film.getRadiance(i, j, rgbVal);
        }

    film.writePng();
    return 0;
}

*/