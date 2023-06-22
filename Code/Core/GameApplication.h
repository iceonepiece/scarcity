#pragma once

#include <memory>
#include <map>
#include <string>
#include "Application.h"

class Scene;

class GameApplication : public Application
{
public:
	GameApplication();
	virtual ~GameApplication();

	virtual void Initialize(std::string title, int width, int height) override;
	virtual void Run() override;

	void LoadScenes();
	void AddScene(std::string name, Scene *scene);
	void ChangeScene(std::string sceneName);

protected:
	virtual void ProcessInput() override;
	virtual void Update() override;
	virtual void Render() override;

	std::string m_currentSceneName;
	std::map<std::string, std::unique_ptr<Scene>> m_scenes;
};