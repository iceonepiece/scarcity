#include "FileSystem.h"
#include "Utils/FileUtils.h"
#include "MetaSerializer.h"
#include "Asset/TextureAsset.h"

void FileSystem::HandleMetaFile(const std::filesystem::path& path)
{
	if (!FileUtils::FileExists(path.string() + ".meta"))
	{
		std::cout << "No meta file exists for " << path << std::endl;

		if (IsImageFile(path))
			GenerateImageMetaFile(path);
	}
}

AssetType FileSystem::GetAssetType(const std::filesystem::path& path)
{
	if (IsImageFile(path))
		return AssetType::Texture;

	return AssetType::None;
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

bool FileSystem::IsAnimatorFile(const std::filesystem::path& path)
{
	return path.extension().generic_string() == ".controller";
}

void FileSystem::GenerateImageMetaFile(const std::filesystem::path& path)
{
	if (!FileUtils::FileExists(path.string() + ".meta"))
	{
		std::cout << "Create meta file for " << path << std::endl;
		TextureAsset textureAsset(path, nullptr);
		MetaSerializer::SerializeImage(textureAsset, path);
	}
	else
	{
		std::cout << "Meta file was already exist " << path << std::endl;
	}

}