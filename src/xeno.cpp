#include"xeno.h"
#include"film.h"
#include"vecmath.h"
#include"ray.h"
#include"transform.h"
#include"integrator.h"
#include"primitive.h"
#include"parallel.h"
#include"io/renderParams.h"
#include"io/cmdOptions.h"
#include"shapes/sphere.h"
#include"shapes/triangle.h"
#include"shapes/quad.h"
#include"accelerators/objectList.h"
#include"accelerators/bvh.h"
#include"sensors/pinhole.h"
#include"io/jsonutil.h"
#include"io/fileutil.h"


using namespace xeno;
using std::cout;
using std::endl;

CmdOptions xeno::cmdOption;

void warn(const std::string &s) {
    std::cerr << "no argument after: " << s << std::endl;
    exit(1);
}

void help() {
    cout << "Options: " << endl;
    cout << "-f         input filename" << endl;
    cout << "-o         output filename" << endl;
    cout << "-help      help()" << endl;
    cout << "-w -width  image width" << endl;
    cout << "-h -height image height" << endl;
    cout << "-spp       sample per pixel" << endl;
    cout << "-nthreads  parallel threads" << endl;
}

int main(int argc, char *argv[]) {

    //google::InitGoogleLogging(argv[0]);
    //FLAGS_log_dir = "../../logs";

    // cmd parsing
    CmdOptions cmdRenderOption;
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-f")) {
            if (i + 1 == argc) warn("-f");
            cmdRenderOption.inFilename = argv[++i];
        }
        else if (!strcmp(argv[i], "-o")) {
            if (i + 1 == argc) warn("-o");
            cmdRenderOption.outFilename = argv[++i];
        }
        else if (!strcmp(argv[i], "-nthreads")) {
            if (i + 1 == argc) warn("-nthreads");
            cmdRenderOption.nthreads = atoi(argv[++i]);
        }
        else if (!strcmp(argv[i], "-spp")) {
            if (i + 1 == argc) warn("-spp");
            cmdRenderOption.spp = atoi(argv[++i]);
        }
        else if (!strcmp(argv[i], "-w") || !strcmp(argv[i], "-width")) {
            if (i + 1 == argc) warn("-width");
            cmdRenderOption.width = atoi(argv[++i]);
        }
        else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "-width")) {
            if (i + 1 == argc) warn("-height");
            cmdRenderOption.height = atoi(argv[++i]);
        }
        else if (!strcmp(argv[i], "-help")) {
            help();
            return 0;
        }
    }
    cmdOption = cmdRenderOption;


    // scene parsing
    //std::string inFilename("E:\\Coding\\github_repo\\xeno\\scenes\\firstTry.json"); //default input filename
    //std::string inFilename("E:\\Coding\\github_repo\\xeno\\scenes\\cbox\\2triangleLight.json");
    auto buildStart = std::chrono::high_resolution_clock::now();
    std::string inFilename("E:\\Coding\\github_Repo\\xeno\\scenes\\cbox\\cbox.json");
    if (cmdOption.inFilename != "") inFilename = cmdOption.inFilename;
    json data = loadJson(inFilename);
    setSearchDirectory(directoryContaining(inFilename));

    RenderParams params = data.template get<RenderParams>();

    /*
    //std::shared_ptr<ObjectList> list = std::make_shared<ObjectList>();
    std::vector<std::shared_ptr<Primitive>> prims;
    for (const auto &primitive : params.primitives) {
        //list->add(primitive);
        prims.push_back(primitive);
    }
    std::shared_ptr<BVH> bvh = std::make_shared<BVH>(prims);
    */

    //Scene scene(list, params.lights);
    //Scene scene(bvh, params.lights);
    Scene scene(params.accelerator, params.lights);


    ParallelInit();
    auto start = std::chrono::high_resolution_clock::now();
    params.integrator->Render(*params.sensor, scene);
    auto end = std::chrono::high_resolution_clock::now();
    ParallelCleanup();

    int spp = data.at("integrator").value("spp", 0);
    if (cmdOption.spp != 0) spp = cmdOption.spp;
    std::cout << "spp = " << spp << std::endl;
    std::chrono::duration<double> buildDuration = start - buildStart;
    std::chrono::duration<double> duration = end - start;
    std::ostringstream rendertimeStr, initializationTimeStr;
    rendertimeStr << std::fixed << std::setprecision(2) << duration.count();
    initializationTimeStr << std::fixed << std::setprecision(2) << buildDuration.count();
    std::cout << "Initialization time = " << initializationTimeStr.str() << " seconds" << std::endl;
    std::cout << "Render time = " << rendertimeStr.str() << " seconds" << std::endl;

    return 0;
}

