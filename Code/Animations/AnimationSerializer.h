#pragma once

#include <filesystem>
#include <string>
#include <fstream>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "AnimatorController.h"

using json = nlohmann::json;

class AnimationSerializer
{
public:
	static AnimatorController* Deserialize(const std::filesystem::path& filepath);
};