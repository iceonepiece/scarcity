#pragma once

#include <memory>
#include <string>
#include <filesystem>
#include <filewatch/FileWatch.h>

class FileWatcher
{
public:
	FileWatcher(const std::filesystem::path& targetPath);

	void OnAdded(const std::string& path);
	void OnRemoved(const std::string& path);
	void OnModified(const std::string& path);
	void OnRenamedOld(const std::string& path);
	void OnRenamedNew(const std::string& path);

private:
	std::unique_ptr<filewatch::FileWatch<std::string>> m_fileWatcher;
	std::filesystem::path m_targetPath;
};