#pragma once

#include "Project/Project.h"
#include "EditorAssetManager.h"

class EditorProject : public Project
{
public:
	EditorProject(const std::filesystem::path filepath)
		: Project(filepath)
	{}

	EditorProject(const std::string& name, const std::filesystem::path& path)
		: Project(name, path)
	{}

	virtual void Initialize() override
	{
		m_assetManager = std::make_unique<EditorAssetManager>();
		m_assetManager->InitializeAssets(m_directory);
	}
};