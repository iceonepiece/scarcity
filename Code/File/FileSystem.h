#pragma once

#include <vector>
#include <string>
#include <filesystem>

enum class ResourceType
{
	None,
	Directory,
	Image,
	Audio
};

struct Resource
{
	ResourceType type = ResourceType::None;
	std::string name;
	std::filesystem::path path;
};

class FileSystem
{
public:
	static Resource GetResource(const std::filesystem::path& path);
	static ResourceType GetResourceType(const std::filesystem::path& path);
	static bool IsImageFile(const std::filesystem::path& path);

	static void GenerateImageMetaFile(const std::filesystem::path& path);
};

static const std::vector<std::string> supportedImageFormats {
	".jpg",
	".png"
};