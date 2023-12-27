#pragma once

#include <filesystem>

class Project;

class ProjectSerializer
{
public:
	ProjectSerializer(Project& project);

	static bool Serialize(Project& project, const std::filesystem::path& filepath);
	static bool Deserialize(Project& project, const std::filesystem::path& filepath);

private:
	Project& m_project;
};