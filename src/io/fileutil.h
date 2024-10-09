#pragma once

#include"xeno.h"
#include <cctype>
#include <string.h>

namespace xeno {

// the following functions are from pbrt-v3
bool isAbsolutePath(const std::string &filename);
std::string absolutePath(const std::string &filename);
std::string resolveFilename(const std::string &filename);
std::string directoryContaining(const std::string &filename);
void setSearchDirectory(const std::string &dirname);

inline bool hasExtension(const std::string &value, const std::string &ending) {
    if (ending.size() > value.size()) return false;
    return std::equal(
        ending.rbegin(), ending.rend(), value.rbegin(),
        [](char a, char b) { return std::tolower(a) == std::tolower(b); });
}

} // namespace xeno