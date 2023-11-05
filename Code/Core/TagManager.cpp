#include "TagManager.h"

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

void TagManager::Serialize(const std::filesystem::path& path)
{

}

void TagManager::Deserialize(const std::filesystem::path& path)
{

}