#pragma once

#include <filesystem>
#include <string>
#include <fstream>
#include <unordered_map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class AnimatorController;
class AnimationClip;

class AnimationSerializer
{
public:
	static void Serialize(AnimatorController& controller, const std::filesystem::path& filePath);
	static void Deserialize(AnimatorController& controller, const std::filesystem::path& filepath, bool instant = false);

	static void Serialize(AnimationClip& clip, const std::filesystem::path& filePath);
	static void Deserialize(AnimationClip& clip, const std::filesystem::path& filepath);
};