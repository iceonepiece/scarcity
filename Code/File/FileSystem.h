#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <functional>
#include "Asset/Asset.h"

using HandleFileCallback = std::function<void(std::fstream&)>;

class FileSystem
{
public:
	static bool CopyFile_(std::filesystem::path sourcePath, std::filesystem::path destPath);
	static bool RemoveFile(std::filesystem::path path);
	static bool FileExists(std::filesystem::path filePath);
	static bool CreateFolder(std::filesystem::path directoryPath);

	static void HandleMetaFile(const std::filesystem::path& path);
	static AssetType GetAssetType(const std::filesystem::path& path);
	static bool IsImageFile(const std::filesystem::path& path);
	static bool IsAnimatorFile(const std::filesystem::path& path);
	static bool IsPrefabFile(const std::filesystem::path& path);

	static bool ReadOrWriteFile(const std::filesystem::path& path, HandleFileCallback callback);

	static void GenerateImageMetaFile(const std::filesystem::path& path);
};

static const std::vector<std::string> supportedImageFormats {
	".jpg",
	".png"
};