#ifndef SCENE_FILE_READER_H
#define SCENE_FILE_READER_H
#include <string>
#include <vector>
#include "types.h"

SceneData readSceneFile(const std::string& path, const std::string& fileName);
#endif
