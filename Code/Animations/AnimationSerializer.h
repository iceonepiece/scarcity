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
	static void Deserialize(AnimatorController& controller, const std::filesystem::path& filepath);
};