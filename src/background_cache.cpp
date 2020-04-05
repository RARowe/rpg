#include <iostream>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <vector>
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

// TODO: There may be a better way to do this in the future
static std::vector<int> readBackgroundCSVFile(const std::string& path)
{
    std::vector<int> ret;
   	std::string line;
   	std::ifstream infile(path);
    const char* format = "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d";
    int o[19];
   	if (infile)
    {
   	   	while (std::getline(infile, line))
        {
            const char* chars = line.c_str();
            for (int i = 0; i < line.size(); i++)
            {
                std::sscanf
                (
                    line.c_str(),
                    format,
                    &o[0], &o[1], &o[2],
                    &o[3], &o[4], &o[5],
                    &o[6], &o[7], &o[8],
                    &o[9], &o[10], &o[11],
                    &o[12], &o[13], &o[14],
                    &o[15], &o[16], &o[17], &o[18]);
            }

            for (auto i : o)
            {
                ret.push_back(i);
            }
   	   	}
   	}
   	infile.close();

    return ret;
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
