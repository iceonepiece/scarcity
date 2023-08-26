#include "FileUtils.h"

bool FileUtils::CopyFile_(std::filesystem::path sourcePath, std::filesystem::path destPath)
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


bool FileUtils::RemoveFile(std::filesystem::path path)
{
	return std::remove(path.string().c_str());
}

bool FileUtils::FileExists(std::filesystem::path filePath)
{
	std::ifstream file(filePath);
	return file.good();
}

bool FileUtils::CreateFolder(std::filesystem::path directoryPath)
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