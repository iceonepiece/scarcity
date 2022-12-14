#include "Scene.h"

Scene::Scene(Game* game)
	: m_game(game)
	, m_camera(glm::vec3(0.0f, 0.0f, -16.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1280, 720))
{
}

Scene::~Scene()
{

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