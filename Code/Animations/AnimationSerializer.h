#pragma once

#include <filesystem>
#include <string>
#include <fstream>
#include <unordered_map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class AnimatorController;

class AnimationSerializer
{
public:
	static void Serialize(AnimatorController& controller, const std::filesystem::path& filePath);
	static AnimatorController* Deserialize(const std::filesystem::path& filepath);
};