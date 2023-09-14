#include "FileSystem.h"
#include "Utils/FileUtils.h"
#include "MetaSerializer.h"
#include "Asset/TextureAsset.h"

bool FileSystem::OpenAndWriteFile(const std::filesystem::path& path, HandleFileCallback callback)
{
	std::ofstream file(path);

	if (file.is_open())
	{
		callback(file);
	}
	else
	{
		std::cerr << "Error opening the file!" << std::endl;
		return false;
	}

	file.close();

	return true;
}

void FileSystem::HandleMetaFile(const std::filesystem::path& path)
{
	if (IsImageFile(path) && !FileUtils::FileExists(path.string() + ".meta"))
	{
		std::cout << "No meta file exists for an image: " << path << std::endl;
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

bool FileSystem::IsPrefabFile(const std::filesystem::path& path)
{
	return path.extension().generic_string() == ".prefab";
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