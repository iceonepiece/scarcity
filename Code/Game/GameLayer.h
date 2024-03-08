#pragma once

#include <map>
#include <memory>
#include <filesystem>
#include "Core/Layer.h"
#include "Core/Application.h"
#include "Scene/Scene.h"

class GameLayer : public Layer
{
public:
	GameLayer(Application& app);
	GameLayer(Application& app, std::unique_ptr<Project> project);

	void Start();
	virtual void Update(float deltaTime) override;
	virtual void OnEvent(Event& event) override;
	virtual void Initialize() override;
	virtual void Shutdown() override;

	void ReloadNativeScripts();
	bool OpenScene(const std::filesystem::path& absolutePath);
	void AddScene(const std::string& name, const std::filesystem::path& filePath);
	void AddScene(const std::string& name, Scene* scene);
	void ChangeScene(const std::string& name);
	Scene* GetCurrentScene();
	void DestroyAllScenes();

	Scene* LoadScene(const std::filesystem::path& filePath);

	static GameLayer* Get() { return s_instance; }

private:
	bool m_firstFrame = true;
	bool m_onExit = false;
	std::string m_currentSceneName;
	std::map<std::string, Scene*> m_sceneMap;

	Application& m_app;

	std::unique_ptr<Project> m_activeProject;
	Scene* m_activeScene = nullptr;

	std::vector<std::string> m_nativeClassNames;

	inline static GameLayer* s_instance = nullptr;
};