#pragma once

#include <vector>
#include "Core/Application.h"
#include "ImGuiLayer.h"
#include "SceneLayer.h"
#include "Project/Project.h"

class EditorApplication : public Application
{
public:
	EditorApplication();
	virtual ~EditorApplication();

	virtual void Initialize(std::string title, int width, int height) override;

	void NewProject(std::unique_ptr<Project> project);

private:
	std::unique_ptr<Project> m_activeProject;
};