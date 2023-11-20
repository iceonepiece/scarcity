#pragma once

#include <memory>
#include <functional>
#include <filesystem>
#include "Core/EntityManager.h"
#include "Graphics/Camera.h"
#include "Physics/Physics.h"
#include "Input/Input.h"
#include "Core/System.h"
#include "Components/TransformComponent.h"
#include "Components/Rigidbody2DComponent.h"
#include "Project/Project.h"
#include "Asset/Asset.h"

class Application;
class Event;

using InitializeFunction = std::function<void(Scene&)>;
class NativeScriptEngine;

struct SpawnCommand
{
	Collision2D* collision = nullptr;
};

struct RenderOptions
{
	bool collisionVisible = true;
};

class Scene : public Asset
{
public:

	Scene(const std::string& name = "Untitled", const std::filesystem::path& path = "");
	virtual ~Scene() = default;

	virtual void Initialize();
	virtual void OnEvent(Event* e);
	virtual void Update(float deltaTime);
	virtual void Render(RenderOptions renderOptions = {});
	virtual void RenderEditor(RenderOptions renderOptions = {});

	void UpdateUI(float deltaTime);

	std::filesystem::path GetAbsolutePath();

	inline void SetProject(Project* project) { m_project = project; }
	void SetViewportSize(unsigned int width, unsigned int height);
	void SetCamera(Camera& camera);
	void OnViewportResize();
	
	bool HasSaved();

	void OnDestroyEntity(entt::entity entity);
	entt::entity DuplicateEntity(entt::entity entity);
	void DestroyEntity(entt::entity entity);
	void SpawnCollision2D(Collision2D* collision);

	virtual void Start();
	virtual void Stop();

	//void InitializePhysicsEntity(entt::entity entity, TransformComponent& transform, Rigidbody2DComponent& rb2d);
	//void DestroyPhysicsEntity(Rigidbody2DComponent& rb2d);
	void StartPhysics();
	void StopPhysics();
	void StartNativeScripts(NativeScriptEngine& scriptEngine);
	void RenderCollisionComponents();
	void RenderTexts();

	virtual void Enter();
	virtual void Exit();

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

	Entity InstantiateEntity(Entity entity, const glm::vec3& position, bool playing = true);

	template<typename T, typename... Args>
	void Instantiate(Args&&... args)
	{
		Entity entity = m_manager.CreateEntity();

		T(entity, this, std::forward<Args>(args)...);
	}

	EntityManager& GetEntityManager();

	std::vector<SpawnCommand> m_spawnCommands;

	unsigned int m_viewportWidth;
	unsigned int m_viewportHeight;

	std::string m_name;
	std::filesystem::path m_location;

	bool physicsActive = true;
	bool particleActive = true;

	std::vector<std::unique_ptr<System>> m_systems;
	Application *m_app;
	
	std::vector<entt::entity> m_toDestroyEntities;
	std::unique_ptr<Camera> m_camera;

	//std::unique_ptr<b2World> m_physics = nullptr;
	//std::unique_ptr<b2ContactListener> m_contactListener;

	Physics m_physics;

	EntityManager m_manager;

	friend class SceneSerializer;
	friend class SceneManager;

	InitializeFunction m_initializeFunction;

	Project* m_project;
};