#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

class TagManager
{
public:
	void AddTag(const std::string& name);
	void RemoveTag(size_t index);

	void Serialize(const std::filesystem::path& path);
	void Deserialize(const std::filesystem::path& path);

	inline const std::vector<std::string>& GetTags() { return m_tags; }
	inline const std::vector<std::string>& GetDefaultTags() { return DEFAULT_TAGS; }

private:
	inline static const std::vector<std::string> DEFAULT_TAGS { "Untagged", "Player", "Enemy", "Platform" };
	
	std::vector<std::string> m_tags;
};