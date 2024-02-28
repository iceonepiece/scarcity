#pragma once

#include <vector>
#include "Core/Application.h"
#include "ImGuiLayer.h"
#include "Project/Project.h"
#include "Constants/GameEngine.h"

class EditorApplication : public Application
{
public:
	EditorApplication(const ApplicationConfigs& configs);
	virtual ~EditorApplication();

	virtual void Initialize() override;

	virtual void ChangeScene(std::string name) override;

	bool NewProject(const std::string& name, std::filesystem::path location);
	void OpenProject(std::filesystem::path path);
	void SaveProject();

	virtual void StopGame() override;

private:

};