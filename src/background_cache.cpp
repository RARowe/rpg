#include <iostream>
#include <filesystem>
#include <fstream>
#include "background_cache.h"
namespace fs = std::filesystem;

static std::string readBackgroundFile(const std::string& path)
{
	std::string returnVal;
   	std::string line;
   	std::ifstream infile(path);
   	if (infile) {
   	   	while (std::getline(infile, line)) {
			line += " ";
			returnVal += line;
   	   	}
   	}
   	infile.close();

    return returnVal;
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
