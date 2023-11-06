#include "TagManager.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void TagManager::AddTag(const std::string& name)
{
	for (int i = 0; i < m_tags.size(); i++)
	{
		if (m_tags[i] == name)
			return;
	}

	m_tags.push_back(name);
}

void TagManager::RemoveTag(size_t index)
{
	m_tags.erase(m_tags.begin() + index);
}

bool TagManager::Serialize(const std::filesystem::path& path)
{
	std::ofstream serialized;
	serialized.open(path);

	if (serialized.is_open())
	{
		json tagManagerJson;
		tagManagerJson["tags"] = json::array();;

		for (auto& tag : m_tags)
			tagManagerJson["tags"].push_back(tag);

		serialized << tagManagerJson.dump(4);
	}
	else
	{
		std::cout << "Error opening the file!" << std::endl;
		return false;
	}

	serialized.close();

	return true;
}

bool TagManager::Deserialize(const std::filesystem::path& path)
{
	std::ifstream deserialzed(path);

	if (deserialzed.is_open())
	{
		json tagManagerJson = json::parse(deserialzed);

		if (tagManagerJson["tags"].is_array())
		{
			m_tags.clear();

			for (auto& tagJson : tagManagerJson["tags"])
				AddTag(tagJson.get<std::string>());
		}
	}
	else
	{
		std::cerr << "Error opening the file!" << std::endl;
		return false;
	}

	deserialzed.close();

	return true;
}