#pragma once

#include <map>
#include <memory>
#include "Core/Layer.h"
#include "EditorApplication.h"
#include "Scene/Scene.h"

class GameLayer : public Layer
{
public:
	GameLayer(EditorApplication& app);
	virtual void Update(float deltaTime) override;
	virtual void OnEvent(Event& event) override;
	virtual void Initialize() override;
	virtual void Shutdown() override;

	void AddScene(const std::string& name, const std::filesystem::path& filePath);
	void AddScene(const std::string& name, Scene* scene);
	void ChangeScene(const std::string& name);
	Scene* GetCurrentScene();
	void DestroyAllScenes();

	Scene* LoadScene(const std::filesystem::path& filePath);

private:
	std::string m_currentSceneName;
	std::map<std::string, Scene*> m_sceneMap;

	EditorApplication& m_app;
};