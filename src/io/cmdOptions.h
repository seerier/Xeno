#pragma once

#include"xeno.h"

namespace xeno {

struct CmdOptions {

    std::string inFilename;
    std::string outFilename;
    int spp = 0;
    int nthreads = 0;
    int width = 0;
    int height = 0;
};

extern CmdOptions cmdOption;

}