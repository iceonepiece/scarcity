#include "FileSystem.h"
#include "Utils/FileUtils.h"
#include "MetaSerializer.h"

void FileSystem::HandleMetaFile(const std::filesystem::path& path)
{
	if (!FileUtils::FileExists(path.string() + ".meta"))
	{
		std::cout << "No meta file exists for " << path << std::endl;

		if (IsImageFile(path))
			GenerateImageMetaFile(path);
	}
}

ResourceType FileSystem::GetResourceType(const std::filesystem::path& path)
{
	if (IsImageFile(path))
		return ResourceType::Image;

	return ResourceType::None;
}

bool FileSystem::IsImageFile(const std::filesystem::path& path)
{
	const std::string extension = path.extension().generic_string();
	for (const auto& format : supportedImageFormats)
	{
		if (extension == format)
			return true;
	}

	return false;
}

void FileSystem::GenerateImageMetaFile(const std::filesystem::path& path)
{
	if (!FileUtils::FileExists(path.string() + ".meta"))
	{
		std::cout << "Create meta file for " << path << std::endl;
		SpriteResource sprite;
		MetaSerializer::SerializeImage(sprite, path);
	}
	else
	{
		std::cout << "Meta file was already exist " << path << std::endl;
	}

}