#include "ProjectSerializer.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "Physics/Physics.h"

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
		projectJson["startScene"] = project.m_startScene.string();

		serialized << projectJson.dump(4);

		project.m_tagManager.Serialize(project.m_directory / "ProjectSettings" / "TagManager.asset");
		Physics::Serialize(project.m_directory / "ProjectSettings" / "Physics.asset");
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
		project.m_absolutePath = filepath.parent_path();
		project.m_directory = filepath.parent_path();
		project.m_startScene = projectJson["startScene"].get<std::string>();

		project.m_tagManager.Deserialize(project.m_directory / "ProjectSettings" / "TagManager.asset");
		Physics::Deserialize(project.m_directory / "ProjectSettings" / "Physics.asset");
	}
	else
	{
		std::cerr << "Error opening the file!" << std::endl;
		return false;
	}

	deserialzed.close();

	return true;
}