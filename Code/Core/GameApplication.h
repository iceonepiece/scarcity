#pragma once

#include <map>
#include <string>
#include "Application.h"

class Scene;

class GameApplication : public Application
{
public:
	GameApplication();
	virtual ~GameApplication();

	virtual void Initialize() override;
	virtual void Run() override;

	void ChangeScene(std::string sceneName);

protected:
	virtual void ProcessInput() override;
	virtual void Update() override;
	virtual void Render() override;

	void LoadScenes();

	std::string m_currentSceneName;
	std::map<std::string, Scene*> m_scenes;
};