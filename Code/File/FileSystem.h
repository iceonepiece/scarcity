#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <functional>
#include "Asset/Asset.h"

using HandleFileCallback = std::function<void(std::ofstream&)>;

class FileSystem
{
public:
	static void HandleMetaFile(const std::filesystem::path& path);
	static AssetType GetAssetType(const std::filesystem::path& path);
	static bool IsImageFile(const std::filesystem::path& path);
	static bool IsAnimatorFile(const std::filesystem::path& path);
	static bool IsPrefabFile(const std::filesystem::path& path);

	static bool OpenAndWriteFile(const std::filesystem::path& path, HandleFileCallback callback);

	static void GenerateImageMetaFile(const std::filesystem::path& path);
};

static const std::vector<std::string> supportedImageFormats {
	".jpg",
	".png"
};