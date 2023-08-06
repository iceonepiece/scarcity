#include "ProjectSerializer.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

ProjectSerializer::ProjectSerializer(Project& project)
	: m_project(project)
{
}

bool ProjectSerializer::Serialize(Project& project, const std::filesystem::path& filepath)
{
	std::ofstream serialized;
	serialized.open(filepath);

	if (serialized.is_open())
	{
		json projectJson;
		projectJson["name"] = project.m_name;
		projectJson["directory"] = project.m_directory.string();
		projectJson["startScene"] = project.m_startScene.string();

		serialized << projectJson.dump(4);
	}
	else
	{
		std::cout << "Error opening the file!" << std::endl;
		return false;
	}

	serialized.close();

	return true;
}

bool ProjectSerializer::Serialize(const std::filesystem::path& filepath)
{
	std::ofstream serialized;
	serialized.open(filepath);

	if (serialized.is_open())
	{
		json projectJson;
		projectJson["name"] = m_project.m_name;
		projectJson["directory"] = m_project.m_directory.string();
		projectJson["startScene"] = m_project.m_startScene.string();

		serialized << projectJson.dump(4);
	}
	else
	{
		std::cout << "Error opening the file!" << std::endl;
		return false;
	}

	serialized.close();

	return true;
}

bool ProjectSerializer::Deserialize(Project& project, const std::filesystem::path& filepath)
{
	std::ifstream deserialzed(filepath);

	if (deserialzed.is_open())
	{
		json projectJson = json::parse(deserialzed);

		project.m_name = projectJson["name"];
		project.m_directory = std::string(projectJson["directory"]);
		project.m_startScene = std::string(projectJson["startScene"]);
	}
	else
	{
		std::cerr << "Error opening the file!" << std::endl;
		return false;
	}

	deserialzed.close();

	return true;
}

bool ProjectSerializer::Deserialize(const std::filesystem::path& filepath)
{
	std::ifstream deserialzed(filepath);

	if (deserialzed.is_open())
	{
		json projectJson = json::parse(deserialzed);

		m_project.m_name = projectJson["name"];
		m_project.m_directory = std::string(projectJson["directory"]);
		m_project.m_startScene = std::string(projectJson["startScene"]);
	}
	else
	{
		std::cerr << "Error opening the file!" << std::endl;
		return false;
	}

	deserialzed.close();

	return true;
}