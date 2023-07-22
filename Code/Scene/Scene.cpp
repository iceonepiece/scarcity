#include "Scene.h"
#include "Core/System.h"
#include "Core/GameState.h"
#include "Systems/ScriptableSystem.h"
#include "Core/Camera2D.h"

Scene::Scene()
	: m_camera(
        new Camera2D({ 0.0f, 0.0f, -14.0f }, { 0.5f, 0.25f }, { 1280, 720 })
    )
    , m_ui(this)
{
    m_systems.emplace_back(new ScriptableSystem(this));
}

Scene::~Scene()
{
	for (auto system : m_systems)
	{
		delete system;
	}
	m_systems.clear();

    for (auto gs : m_gameStates)
    {
        delete gs.second;
    }
    m_gameStates.clear();
}

void Scene::OnEvent(Event* e)
{

}

void Scene::Initialize()
{
}

void Scene::Update(float deltaTime)
{
    for (auto system : m_systems)
        system->Update(deltaTime);

    if (physicsActive)
        m_physics.Update(deltaTime);

    m_camera->Update();
}

void Scene::Enter()
{
    auto view = m_manager.m_registry.view<ScriptableComponent>();

    for (auto [entity, scriptable] : view.each())
        scriptable.scriptable->Start();

    m_app->GetRenderer().SetCamera(m_camera.get());
}

void Scene::Exit()
{

}

void Scene::Render()
{
    WindowData windowData = m_app->GetWindow().GetWindowData();
    m_camera->SetScreenSize({ windowData.width, windowData.height });

    for (auto system : m_systems)
        system->Render();
}

void Scene::RenderUI()
{
    m_ui.Render();
}

EntityManager& Scene::GetEntityManager()
{
    return m_manager;
}

void Scene::ChangeGameState(std::string gameStateName)
{
    std::cout << "Change from " << m_currentGameStateName << " to " << gameStateName << std::endl;


    if (m_gameStates[m_currentGameStateName] != nullptr)
        m_gameStates[m_currentGameStateName]->OnExit();

    m_currentGameStateName = gameStateName;

    if (m_gameStates[m_currentGameStateName] != nullptr)
        m_gameStates[m_currentGameStateName]->OnEnter();

}