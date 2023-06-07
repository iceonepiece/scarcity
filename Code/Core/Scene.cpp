#include "Scene.h"
#include "Scene.h"
#include "System.h"
#include "GameState.h"

Scene::Scene(Application* game)
	: m_game(game)
	, m_camera(glm::vec3(0.0f, 0.0f, -14.0f), glm::vec2(0.5f, 0.25f), glm::vec2(1280, 720))
    , m_ui(this)
{
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

void Scene::Init()
{
}

void Scene::ProcessInput()
{
}

void Scene::Update(float deltaTime)
{
    for (auto system : m_systems)
        system->Update(deltaTime);
}

void Scene::Render()
{
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