#pragma once

#include <memory>
#include <filesystem>

class Project
{
public:
	Project() = default;

	Project(const std::string& name, const std::string& path)
		: m_name(name)
		, m_projectDirectory(path)
	{}

	static std::shared_ptr<Project> New();
	static std::shared_ptr<Project> Load(const std::filesystem::path& path);

	static bool SaveActive();
	static std::shared_ptr<Project> GetActive() { return s_activeProject; }

private:
	std::string m_name = "Untitled";
	std::filesystem::path m_projectDirectory;
	std::filesystem::path m_startScene;

	inline static std::shared_ptr<Project> s_activeProject;
};