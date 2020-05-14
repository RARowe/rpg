#ifndef SCENE_FILE_READER_H
#define SCENE_FILE_READER_H
#include <string>
#include <vector>
#include "scenes.h"

SceneData readSceneFile(const std::string& path);
#endif