#pragma once

#include "Project/Project.h"
#include "EditorAssetManager.h"

class EditorProject : public Project
{
public:
	EditorProject() = default;

	EditorProject(const std::string& name, const std::filesystem::path& path)
		: Project(name, path)
	{}

	virtual void Initialize() override
	{
		m_assetManager = std::make_unique<EditorAssetManager>();
	}
};