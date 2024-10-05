#include"jsonutil.h"

namespace xeno {

json loadJson(const std::string &path) {
    std::ifstream f(path);
    if (!f.is_open()) {
        std::cerr << "Unable to load json: " << path << std::endl;
        return json();
    }
    f.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    json data;
    try {
        data = json::parse(f);
    }
    catch (const json::parse_error &e) {
        std::cerr << "json parsing error: " << e.what() << std::endl;
        return json();
    }
    return data;
}

}