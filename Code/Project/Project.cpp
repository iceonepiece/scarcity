#include "Project.h"
#include "ProjectSerializer.h"
#include "Scene/Scene.h"
#include "Scene/SceneSerializer.h"
#include "File/FileSystem.h"
#include "Constants/GameEngine.h"

std::shared_ptr<Project> Project::New()
{
	s_activeProject = std::make_shared<Project>();
	return s_activeProject;
}

bool Project::Save()
{
	if (!std::filesystem::is_directory(m_directory / "ProjectSettings"))
		FileSystem::CreateFolder(m_directory / "ProjectSettings");

	std::filesystem::path projFilePath = m_directory / (m_name + PROJECT_FILE_EXTENSION);

	return ProjectSerializer::Serialize(*this, projFilePath);
}

std::unique_ptr<Scene> Project::LoadScene(const std::filesystem::path& relativePath)
{
	std::unique_ptr<Scene> scene = std::make_unique<Scene>();
	scene->SetProject(this);

	if (SceneSerializer::Deserialize(*scene, relativePath))
		return scene;

	return nullptr;
}

std::shared_ptr<Project> Project::Load(const std::filesystem::path& path)
{
	std::shared_ptr<Project> project = std::make_shared<Project>();

	if (ProjectSerializer::Deserialize(*project, path))
	{
		project->m_directory = path.parent_path();
		s_activeProject = project;
		return s_activeProject;
	}

	return nullptr;
}

bool Project::SaveActive()
{
	return true;
}