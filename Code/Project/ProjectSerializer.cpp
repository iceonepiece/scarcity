#include "ProjectSerializer.h"
#include "Project.h"
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

		json luaScriptJson = json::array();

		for (const auto& luaScript : project.m_luaScripts)
		{
			luaScriptJson.push_back(luaScript->GetPath().string());
		}

		projectJson["luaScripts"] = luaScriptJson;

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
		project.m_startScene = projectJson["startScene"].get<std::string>();

		if (projectJson["luaScripts"].is_array())
		{
			for (auto& luaScriptJson : projectJson["luaScripts"])
			{
				std::filesystem::path absoluteScriptPath = project.m_absolutePath / luaScriptJson.get<std::string>();
				if (Asset* asset = project.m_assetManager->GetAsset(absoluteScriptPath))
				{
					project.m_luaScripts.push_back(dynamic_cast<LuaScript*>(asset));
				}
			}
		}

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