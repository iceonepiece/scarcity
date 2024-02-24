#pragma once

#include "Core/Application.h"

class GameApplication : public Application
{
public:
	GameApplication(const ApplicationConfigs& configs);
	void OpenProject(std::filesystem::path path);
	virtual void Initialize() override;

};