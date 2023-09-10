#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include "Asset/Asset.h"

class FileSystem
{
public:
	static void HandleMetaFile(const std::filesystem::path& path);
	static AssetType GetAssetType(const std::filesystem::path& path);
	static bool IsImageFile(const std::filesystem::path& path);
	static bool IsAnimatorFile(const std::filesystem::path& path);

	static void GenerateImageMetaFile(const std::filesystem::path& path);
};

static const std::vector<std::string> supportedImageFormats {
	".jpg",
	".png"
};