#pragma once

#include <memory>
#include <filesystem>

class Scene;

class Project
{
public:
	Project() = default;

	Project(const std::string& name, const std::filesystem::path& path)
		: m_name(name)
		, m_absolutePath(path)
		, m_directory(path)
	{}

	std::unique_ptr<Scene> LoadScene(const std::filesystem::path& relativePath);

	inline void SetStartScene(std::filesystem::path path) { m_startScene = path; }
	inline std::filesystem::path GetStartScene() { return m_startScene; }
	inline std::filesystem::path GetStartSceneAbsolutePath() { return m_absolutePath / m_startScene;  }

	inline std::string GetName() { return m_name; }
	inline std::filesystem::path GetDirectory() { return m_directory; }
	inline std::filesystem::path GetAbsolutePath() { return m_directory; }

	static std::shared_ptr<Project> New();
	static std::shared_ptr<Project> Load(const std::filesystem::path& path);

	static bool SaveActive();
	static std::shared_ptr<Project> GetActive() { return s_activeProject; }

private:
	std::string m_name = "Untitled";
	std::filesystem::path m_directory;
	std::filesystem::path m_absolutePath;
	std::filesystem::path m_startScene;

	inline static std::shared_ptr<Project> s_activeProject;

	friend class ProjectSerializer;
};