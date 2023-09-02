#pragma once

#include <vector>
#include <memory>
#include <mutex>
#include <filesystem>
#include <filewatch/FileWatch.h>

struct FileEvent
{
	std::filesystem::path path;
	filewatch::Event type;
};


class FileHandler
{
public:
	void OnFileEvent(FileEvent& event);

};