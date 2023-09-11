#include "GameLayer.h"
#include "Scene/SceneSerializer.h"
#include "Core/ResourceAPI.h"

GameLayer::GameLayer()
{
	ResourceManager* resourceManager = ResourceAPI::GetResourceManager();
	resourceManager->InitializeAssets("C:\\BossFight Projects\\bison\\Assets");

	std::unique_ptr<Scene> startScene = std::make_unique<Scene>();
	SceneSerializer::Deserialize(*startScene, "C:\\BossFight Projects\\bison\\Scenes\\Untitled.bfs");
	m_sceneMap.insert({ "start", std::move(startScene) });
}

void GameLayer::Initialize()
{
	std::cout << "GameLayer::Initialize()\n";
	m_sceneMap["start"]->Start();
}

void GameLayer::Shutdown()
{
	std::cout << "GameLayer::Shutdown()\n";
	m_sceneMap["start"]->Stop();
}

void GameLayer::Update(float dt)
{
	if (m_sceneMap.find("start") != m_sceneMap.end())
	{
		m_sceneMap["start"]->Update(dt);
		m_sceneMap["start"]->SetCamera(nullptr);
		m_sceneMap["start"]->Render();
	}
}

