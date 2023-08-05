#include "Project.h"
#include "ProjectSerializer.h"

std::shared_ptr<Project> Project::New()
{
	s_activeProject = std::make_shared<Project>();
	return s_activeProject;
}

std::shared_ptr<Project> Project::Load(const std::filesystem::path& path)
{
	std::shared_ptr<Project> project = std::make_shared<Project>();

	ProjectSerializer serializer(*project.get());
	if (serializer.Deserialize(path))
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