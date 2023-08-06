#pragma once

#include "Project.h"

class ProjectSerializer
{
public:
	ProjectSerializer(Project& project);

	static bool Serialize(Project& project, const std::filesystem::path& filepath);
	static bool Deserialize(Project& project, const std::filesystem::path& filepath);

	bool Serialize(const std::filesystem::path& filepath);
	bool Deserialize(const std::filesystem::path& filepath);

private:
	Project& m_project;
};