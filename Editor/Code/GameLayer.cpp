#include "GameLayer.h"
#include <iostream>
#include "Scene/SceneManager.h"

GameLayer::GameLayer(EditorApplication& app)
	: m_app(app)
{
	std::cout << "GameLayer Constructor()\n\n";
}

void GameLayer::Initialize()
{
	std::cout << "GameLayer Initialize()\n\n";
}

void GameLayer::Shutdown()
{
	std::cout << "GameLayer Shutdown()\n\n";
}

void GameLayer::Update(float deltaTime)
{
	std::cout << "GameLayer::Update()\n\n";

	if (m_sceneMap.find(m_currentSceneName) != m_sceneMap.end())
		m_sceneMap[m_currentSceneName]->Update(deltaTime);
}

void GameLayer::OnEvent(Event& event)
{
	if (event.GetType() == EventType::MouseButtonPressed)
	{
		std::cout << "GameLayer MouseButtonPressed\n";
	}
}

void GameLayer::ChangeScene(const std::string& name)
{
	std::cout << "GameLayer::ChangeScene() - " << name << std::endl;

	if (m_sceneMap.find(name) == m_sceneMap.end())
	{
		if (Scene* sceneBlueprint = Application::Get().GetAssetManager().GetScene(name))
		{
			std::unique_ptr<Scene> activeScene = SceneManager::LoadScene(sceneBlueprint->GetPath());

			if (activeScene != nullptr)
			{
				activeScene->SetApplication(&m_app);
				activeScene->Initialize();
				m_sceneMap.insert({ name, activeScene.release() });
				m_currentSceneName = name;

				return;
			}
		}

		std::cout << "Cannot find the scene " << name << "\t[ GameLayer::ChangeScene() ]\n";
		return;
	}

	m_currentSceneName = name;
}

void GameLayer::AddScene(const std::string& name, const std::filesystem::path& filePath)
{
	if (m_sceneMap.find(name) == m_sceneMap.end())
	{
		Scene* scene = LoadScene(filePath);

		if (scene != nullptr)
		{
			m_sceneMap.insert({ name, scene });
		}
	}
}

void GameLayer::AddScene(const std::string& name, Scene* scene)
{
	if (scene != nullptr)
	{
		m_sceneMap.insert({ name, scene });
	}
}

Scene* GameLayer::GetCurrentScene()
{
	if (m_sceneMap.find(m_currentSceneName) != m_sceneMap.end())
		return m_sceneMap[m_currentSceneName];

	return nullptr;
}

void GameLayer::DestroyAllScenes()
{
	for (auto scene : m_sceneMap) {
		scene.second->Stop();
		delete scene.second;
	}

	m_sceneMap.clear();
}

Scene* GameLayer::LoadScene(const std::filesystem::path& filePath)
{
	return nullptr;
}