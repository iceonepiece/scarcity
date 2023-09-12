#pragma once

#include <memory>
#include <functional>
#include <filesystem>
#include "Core/Application.h"
#include "Core/EntityManager.h"
#include "Core/Camera.h"
#include "Physics/Physics.h"
#include "Input/Input.h"
#include "UIs/UI.h"
#include "Events/Event.h"
#include "Core/System.h"
#include "Core/GameState.h"

#include "Events/Event.h"

using InitializeFunction = std::function<void(Scene&)>;
class NativeScriptEngine;

class Scene
{
public:

	Scene(const std::string& name = "Untitled");
	virtual ~Scene() = default;

	virtual void Initialize();
	virtual void OnEvent(Event* e);
	virtual void Update(float deltaTime);
	virtual void Render();
	virtual void RenderEditor();

	void SetCamera(Camera* camera);
	
	bool HasSaved();

	entt::entity DuplicateEntity(entt::entity entity);

	virtual void Start();
	virtual void Stop();

	void StartPhysics();
	void StopPhysics();
	void StartNativeScripts(NativeScriptEngine& scriptEngine);

	virtual void Enter();
	virtual void Exit();

	static std::unique_ptr<Scene> CreateDefaultScene(std::filesystem::path directory);
	static std::unique_ptr<Scene> Copy(Scene& sourceScene);

	void SetApplication(Application* app) { m_app = app; }
	Application* GetApplication() { return m_app; }

	template<typename Component>
	static void CopyComponent(entt::registry& srcRegistry, entt::registry& destRegistry, entt::entity srcEntity, entt::entity destEntity)
	{
		if (srcRegistry.all_of<Component>(srcEntity)) {
			auto& component = srcRegistry.get<Component>(srcEntity);
			destRegistry.emplace_or_replace<Component>(destEntity, component);
		}
	}

	template<typename Component>
	static void DuplicateComponent(entt::registry& registry, entt::entity srcEntity, entt::entity destEntity)
	{
		if (registry.all_of<Component>(srcEntity)) {
			auto& component = registry.get<Component>(srcEntity);
			registry.emplace_or_replace<Component>(destEntity, component);
		}
	}

	void SetInitializeFunction(InitializeFunction fn)
	{
		m_initializeFunction = fn;
	}

	InitializeFunction GetInitializeFunction()
	{
		return m_initializeFunction;
	}

	void RenderUI();

	template<typename T, typename... Args>
	void Instantiate(Args&&... args)
	{
		Entity entity = m_manager.CreateEntity();

		T(entity, this, std::forward<Args>(args)...);
	}

	void ChangeGameState(std::string gameStateName);

	EntityManager& GetEntityManager();

	std::string m_name;
	std::filesystem::path m_location;
	std::filesystem::path m_path;
	std::unordered_map<std::string, std::unique_ptr<GameState>> m_gameStates;
	std::string m_currentGameStateName;

	bool physicsActive = true;
	bool particleActive = true;

	std::vector<std::unique_ptr<System>> m_systems;
	Application *m_app;
	
	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<b2World> m_physics = nullptr;

	EntityManager m_manager;
	UI m_ui;

	friend class SceneSerializer;

	InitializeFunction m_initializeFunction;
};