#pragma once

#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
#include <filesystem>

using json = nlohmann::json;

struct BuildConfigs
{
	std::string name = "Untitled";
	uint32_t width = 1280;
	uint32_t height = 720;
	std::string path;
};

BuildConfigs Load(const std::filesystem::path& filepath)
{
	std::ifstream file(filepath);

	BuildConfigs configs;

	if (file.is_open())
	{
		json buildJson = json::parse(file);

		configs.path = buildJson["path"].get<std::string>();

		configs.name = buildJson["name"].get<std::string>();
		configs.width = buildJson["width"].get<uint32_t>();
		configs.height = buildJson["height"].get<uint32_t>();
	}
	else
	{
		std::cerr << "Error opening the file!" << std::endl;
	}

	file.close();

	return configs;
}