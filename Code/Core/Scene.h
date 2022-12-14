#pragma once

#include "EntityManager.h"
#include "Camera.h"
#include "Physics.h"
#include "Input.h"

class Scene
{
public:
	Scene(Game* game);
	~Scene();

	virtual void Init();
	virtual void ProcessInput(Input& input);
	virtual void Update(float deltaTime);
	virtual void Render();

protected:
	Game* m_game;
	Camera m_camera;
	Physics m_physics;
	EntityManager m_manager;
};