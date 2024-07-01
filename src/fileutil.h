#pragma once

#include"xeno.h"

namespace xeno {

inline bool hasExtension(const std::string &name, const std::string &extension) {
    if (extension.size() > name.size()) return false;
    return std::equal(extension.rbegin(), extension.rend(), name.rbegin(),
        [](char a, char b) {return std::tolower(a) == std::tolower(b); });
}

} // namespace xeno