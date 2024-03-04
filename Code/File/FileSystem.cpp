#include "FileSystem.h"
#include "MetaSerializer.h"
#include "Graphics/Image.h"
#include "Audio/AudioClip.h"

std::filesystem::path FileSystem::GetRelativePath(const std::filesystem::path& basePath, const std::filesystem::path& filePath)
{
	if (basePath.root_name() != filePath.root_name())
		return filePath;

	auto baseIter = basePath.begin();
	auto fileIter = filePath.begin();

	while (baseIter != basePath.end() && fileIter != filePath.end() && *baseIter == *fileIter)
	{
		++baseIter;
		++fileIter;
	}

	std::filesystem::path relativePath;

	for (; baseIter != basePath.end(); ++baseIter)
		relativePath /= "..";

	for (; fileIter != filePath.end(); ++fileIter)
		relativePath /= *fileIter;

	return relativePath;
}


bool FileSystem::CopyFile_(std::filesystem::path sourcePath, std::filesystem::path destPath)
{
	std::ifstream source(sourcePath, std::ios::binary);
	if (!source.is_open()) {
		std::cerr << "Error opening source file: " << sourcePath << std::endl;
		return false;
	}

	std::ofstream dest(destPath, std::ios::binary);
	if (!dest.is_open()) {
		std::cerr << "Error opening destination file: " << destPath << std::endl;
		return false;
	}

	dest << source.rdbuf();

	if (!source.good() || !dest.good()) {
		std::cerr << "Error occurred during file copy." << std::endl;
		return false;
	}

	source.close();
	dest.close();

	return true;
}


bool FileSystem::RemoveFile(std::filesystem::path path)
{
	return std::remove(path.string().c_str());
}

bool FileSystem::FileExists(std::filesystem::path filePath)
{
	std::ifstream file(filePath);
	return file.good();
}

bool FileSystem::CreateFolder(std::filesystem::path directoryPath)
{
	if (!std::filesystem::exists(directoryPath))
	{
		if (std::filesystem::create_directory(directoryPath))
		{
			std::cout << "Directory created: " << directoryPath << std::endl;
			return true;
		}
		else
		{
			std::cout << "Failed to create directory: " << directoryPath << std::endl;
			return false;
		}
	}

	std::cout << "Directory already exists: " << directoryPath << std::endl;
	return false;
}

bool FileSystem::ReadFile(const std::filesystem::path& path, HandleFileCallback callback)
{
	std::fstream file;
	file.open(path, std::ios_base::in);

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

bool FileSystem::WriteFile(const std::filesystem::path& path, HandleFileCallback callback)
{
	std::fstream file;
	file.open(path, std::ios_base::out);

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
	if (IsImageFile(path) && !FileSystem::FileExists(path.string() + ".meta"))
	{
		std::cout << "No meta file exists for an image: " << path << std::endl;
		GenerateImageMetaFile(path);
	}
	/*
	else if (IsAudioFile(path) && !FileSystem::FileExists(path.string() + ".meta"))
	{
		std::cout << "No meta file exists for an audio: " << path << std::endl;
		GenerateAudioMetaFile(path);
	}
	*/
}

AssetType FileSystem::GetAssetType(const std::filesystem::path& path)
{
	if (IsImageFile(path))
		return AssetType::Image;

	return AssetType::None;
}

bool FileSystem::IsNativeScriptFile(const std::filesystem::path& path)
{
	const std::string extension = path.extension().generic_string();
	for (const auto& format : supportedNativeScriptFormats)
	{
		if (extension == format)
			return true;
	}

	return false;
}

bool FileSystem::IsLuaScriptFile(const std::filesystem::path& path)
{
	return path.extension().generic_string() == ".lua";
}

bool FileSystem::IsIgnoreDirectory(const std::filesystem::path& path)
{
	const std::string pathName = path.filename().generic_string();
	for (const auto& directory : ignoreDirectories)
	{
		if (pathName == directory)
			return true;
	}

	return false;
}

bool FileSystem::IsSceneFile(const std::filesystem::path& path)
{
	return path.extension().generic_string() == ".bfs";
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

bool FileSystem::IsAudioFile(const std::filesystem::path& path)
{
	const std::string extension = path.extension().generic_string();
	for (const auto& format : supportedAudioFormats)
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

bool FileSystem::IsAnimationFile(const std::filesystem::path& path)
{
	return path.extension().generic_string() == ".anim";
}

bool FileSystem::IsPrefabFile(const std::filesystem::path& path)
{
	return path.extension().generic_string() == ".prefab";
}

bool FileSystem::IsFontFile(const std::filesystem::path& path)
{
	return path.extension().generic_string() == ".ttf";
}

void FileSystem::GenerateImageMetaFile(const std::filesystem::path& path)
{
	if (!FileSystem::FileExists(path.string() + ".meta"))
	{
		std::cout << "Create meta file for " << path << std::endl;
		Image image(path, nullptr);
		MetaSerializer::SerializeImage(image, path);
	}
	else
	{
		std::cout << "Meta file was already exist " << path << std::endl;
	}

}

void FileSystem::GenerateAudioMetaFile(const std::filesystem::path& path)
{
	if (!FileSystem::FileExists(path.string() + ".meta"))
	{
		std::cout << "Create meta file for " << path << std::endl;
	}
	else
	{
		std::cout << "Meta file was already exist " << path << std::endl;
	}

}