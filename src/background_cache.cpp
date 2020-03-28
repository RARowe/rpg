#include <iostream>
#include <filesystem>
#include "background_cache.h"
namespace fs = std::filesystem;

static std::string readBackgroundFile(const std::string& path)
{
    return "hello";
}
BackgroundCache::BackgroundCache(const std::string& backgroundPath)
{
    for (const auto& entry : fs::directory_iterator(backgroundPath))
    {
        _backgrounds[entry.path().filename()] = readBackgroundFile(entry.path());
    }
}

const std::string& BackgroundCache::getBackground(const std::string& backgroundKey)
{
    return _backgrounds[backgroundKey];
}
