#pragma once

#include <string>
#include <sol/sol.hpp>
#include "Scene/Scene.h"

class LevelManager
{
public:
	static void LoadLevel(Scene* scene, std::string filePath);
};