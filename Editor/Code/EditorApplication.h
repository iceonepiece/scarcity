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

	inline Project* GetActiveProject() { return m_activeProject.get(); }
	inline Scene* GetActiveScene() { return m_activeScene.get(); }

private:
	std::unique_ptr<Project> m_activeProject;
	std::unique_ptr<Scene> m_activeScene;
};