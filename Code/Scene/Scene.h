#pragma once

#include <memory>
#include <functional>
#include <filesystem>
#include "Entity/EntityManager.h"
#include "Graphics/Camera.h"
#include "Physics/Physics.h"
#include "System/System.h"
#include "Asset/Asset.h"

class LuaEngine;
class Application;
class Project;
class Event;
class ScriptableEntity;

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

enum class SystemType
{
	NativeScript,
	LuaScript,
	Physics,
	Animator
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

	void SetProject(Project* project);
	void SetCamera(Camera& camera);

	void SetSystemActive(SystemType type, bool value)
	{
		size_t index = (size_t)type;

		if (index < m_systems.size())
			m_systems[index]->SetActive(value);
	}
	
	bool HasSaved();

	void OnDestroyEntity(entt::entity entity);
	entt::entity DuplicateEntity(entt::entity entity);
	void DestroyEntity(entt::entity entity);
	void SpawnCollision2D(Collision2D* collision);

	virtual void Start();
	virtual void Stop();

	void StartPhysics();
	void StopPhysics();
	void StartNativeScripts(NativeScriptEngine& scriptEngine);
	void RenderCollisionComponents();
	void RenderTexts();

	virtual void Enter();
	virtual void Exit();

	void SetApplication(Application* app) { m_app = app; }
	Application* GetApplication() { return m_app; }

	Physics& GetPhysics() { return m_physics; }

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

	Entity InstantiateEntity(Entity entity, const glm::vec3& position, bool playing = true);

	template<typename T, typename... Args>
	void Instantiate(Args&&... args)
	{
		Entity entity = m_manager.CreateEntity();

		T(entity, this, std::forward<Args>(args)...);
	}

	LuaEngine& GetLuaEngine()
	{
		return *m_luaEngine;
	}

	EntityManager& GetEntityManager();

	std::vector<SpawnCommand> m_spawnCommands;

	unsigned int m_viewportWidth;
	unsigned int m_viewportHeight;

	std::string m_name;
	std::filesystem::path m_location;

	bool physicsActive = true;
	bool particleActive = true;

	ScriptableEntity* GetScriptableEntity(const std::string& name)
	{
		if (m_scriptableEntities.find(name) != m_scriptableEntities.end())
			return m_scriptableEntities[name];

		return nullptr;
	}

	std::unordered_map<std::string, ScriptableEntity*> m_scriptableEntities;

	std::vector<std::unique_ptr<System>> m_systems;
	Application *m_app;
	
	std::vector<entt::entity> m_toDestroyEntities;
	std::unique_ptr<Camera> m_camera;

	static Scene* GetActive() { return s_activeScene; }
	inline static Scene* s_activeScene = nullptr;

	Physics m_physics;

	EntityManager m_manager;

	friend class SceneSerializer;
	friend class SceneManager;

	InitializeFunction m_initializeFunction;

	Project* m_project;

	std::unique_ptr<LuaEngine> m_luaEngine;
};