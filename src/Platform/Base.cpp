#include "Platform/Base.h"

#include <filesystem>

std::string Platform::Base::DataPath(const std::string &path) {
#if defined(_WIN32)
    auto fullpath = std::filesystem::current_path() / path;
    return fullpath.string();
#else
    auto fullpath = std::filesystem::path("/tmp") / path;
    return fullpath.string();
#endif
}
