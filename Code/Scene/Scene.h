#pragma once

#include <memory>
#include "Core/Application.h"
#include "Core/EntityManager.h"
#include "Core/Camera.h"
#include "Physics/Physics.h"
#include "Input/Input.h"
#include "UIs/UI.h"
#include "Events/Event.h"

class System;
class GameState;

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void Initialize();
	virtual void OnEvent(Event* e);
	virtual void Update(float deltaTime);
	virtual void Render();

	virtual void Start();
	virtual void Stop();

	void StartPhysics();
	void StopPhysics();

	virtual void Enter();
	virtual void Exit();

	void SetApplication(Application* app) { m_app = app; }
	Application* GetApplication() { return m_app; }

	void RenderUI();

	template<typename T, typename... Args>
	void Instantiate(Args&&... args)
	{
		Entity entity = m_manager.CreateEntity();

		T(entity, this, std::forward<Args>(args)...);
	}

	void ChangeGameState(std::string gameStateName);

	EntityManager& GetEntityManager();

	std::unordered_map<std::string, GameState*> m_gameStates;
	std::string m_currentGameStateName;

	bool physicsActive = true;
	bool particleActive = true;

	std::vector<System*> m_systems;
	Application *m_app;
	
	std::unique_ptr<Camera> m_camera;
	
	//std::unique_ptr<Physics> m_physics = nullptr;
	b2World* m_physics = nullptr;

	EntityManager m_manager;
	UI m_ui;

	friend class SceneSerializer;
};