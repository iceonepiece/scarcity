#pragma once

#include <vector>
#include "Core/Application.h"
#include "ImGuiLayer.h"
#include "SceneLayer.h"
#include "Project/Project.h"
#include "Constants/GameEngine.h"

class EditorApplication : public Application
{
public:
	EditorApplication(const ApplicationConfigs& configs);
	virtual ~EditorApplication();

	virtual void Initialize(std::string title, int width, int height) override;

	bool NewProject(const std::string& name, std::filesystem::path location);
	void OpenProject(std::filesystem::path path);
	void SaveProject();

	void NewScene(const std::string& name, std::filesystem::path directory);
	void OpenScene(std::filesystem::path path);
	void SaveScene();

private:

};