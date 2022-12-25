#pragma once

#include "EntityManager.h"
#include "Camera.h"
#include "../Physics/Physics.h"
#include "../Input/Input.h"
#include "../UIs/UI.h"

class System;
class GameState;

class Scene
{
public:
	Scene(Game* game);
	~Scene();

	virtual void Init();
	virtual void ProcessInput();
	virtual void Update(float deltaTime);
	virtual void Render();
	void RenderUI();

	template<typename T, typename... Args>
	void Instantiate(Args&&... args)
	{
		Entity entity = m_manager.CreateEntity();

		T(entity, this, std::forward<Args>(args)...);
	}

	void ChangeGameState(std::string gameStateName);

	std::unordered_map<std::string, GameState*> m_gameStates;
	std::string m_currentGameStateName;

	bool physicsActive = true;
	bool particleActive = true;

	std::vector<System*> m_systems;
	Game* m_game;
	Camera m_camera;
	Physics m_physics;
	EntityManager m_manager;
	UI m_ui;
};