#pragma once

#include <vector>
#include <filesystem>
#include <nlohmann/json.hpp>
#include "Graphics/Sprite.h"
#include "Graphics/Image.h"

using json = nlohmann::json;

class MetaSerializer
{
public:
	static bool SerializeImage(Image& image, const std::filesystem::path& path);
	static bool DeserializeImage(Image& image, const std::filesystem::path& path);
};