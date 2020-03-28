#pragma once
#ifndef BACKGROUND_CACHE_H
#define BACKGROUND_CACHE_H
#include <map>
#include <string>

class BackgroundCache
{
    public:
        BackgroundCache(const std::string& backgroundPath);
        const std::string& getBackground(const std::string& backgroundKey);
    private:
        std::map<std::string, std::string> _backgrounds;
};
#endif
