#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <cstdio>

class FileUtils
{
public:
	static bool CopyFile_(std::filesystem::path sourcePath, std::filesystem::path destPath);
	static bool RemoveFile(std::filesystem::path path);
	static bool FileExists(std::filesystem::path filePath);
	static bool CreateFolder(std::filesystem::path directoryPath);

};