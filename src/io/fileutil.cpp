#include"fileutil.h"

namespace xeno {

static std::string searchDirectory;

// the following functions are from pbrt-v3
bool isAbsolutePath(const std::string &filename) {
    if (filename.empty()) return false;
    return (filename[0] == '\\' || filename[0] == '/' ||
            filename.find(':') != std::string::npos);
}

std::string absolutePath(const std::string &filename) {
    char full[_MAX_PATH];
    if (_fullpath(full, filename.c_str(), _MAX_PATH))
        return std::string(full);
    else
        return filename;
}

std::string resolveFilename(const std::string &filename) {
    if (searchDirectory.empty() || filename.empty())
        return filename;
    else if (isAbsolutePath(filename))
        return filename;

    char searchDirectoryEnd = searchDirectory[searchDirectory.size() - 1];
    if (searchDirectoryEnd == '\\' || searchDirectoryEnd == '/')
        return searchDirectory + filename;
    else
        return searchDirectory + "\\" + filename;
}

std::string directoryContaining(const std::string &filename) {
    // This code isn't tested but I believe it should work. Might need to add
    // some const_casts to make it compile though.
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char ext[_MAX_EXT];

    errno_t err = _splitpath_s(filename.c_str(), drive, _MAX_DRIVE, dir,
                               _MAX_DIR, nullptr, 0, ext, _MAX_EXT);
    if (err == 0) {
        char fullDir[_MAX_PATH];
        err = _makepath_s(fullDir, _MAX_PATH, drive, dir, nullptr, nullptr);
        if (err == 0) return std::string(fullDir);
    }
    return filename;
}

void setSearchDirectory(const std::string &dirname) {
    searchDirectory = dirname;
}

}