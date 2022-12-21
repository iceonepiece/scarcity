#include "Scene.h"
#include "System.h"
#include "GameState.h"

Scene::Scene(Game* game)
	: m_game(game)
	, m_camera(glm::vec3(0.0f, 0.0f, -16.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1280, 720))
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

void Scene::Init()
{
}

void Scene::ProcessInput(Input& input)
{
}

void Scene::Update(float deltaTime)
{
}

void Scene::Render()
{
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