#include "FileSystem.h"
#include "Utils/FileUtils.h"
#include "MetaSerializer.h"

ResourceType FileSystem::GetResourceType(const std::filesystem::path& path)
{
	if (IsImageFile(path))
		return ResourceType::Image;

	return ResourceType::None;
}

Resource FileSystem::GetResource(const std::filesystem::path& path)
{
	return Resource {
		GetResourceType(path),
		path.filename().generic_string(),
		path
	};
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
		MetaSerializer::SerializeImage(path);
	}
	else
	{
		std::cout << "Meta file was already exist " << path << std::endl;
	}

}