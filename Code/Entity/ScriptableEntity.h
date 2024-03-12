#pragma once

#include <string>
#include <memory>
#include "Core/Application.h"
#include "Entity/Entity.h"
#include "Scene/Scene.h"
#include "Project/Project.h"

#define SERIALIZE_FIELD(type, name) type name;

class ScriptableEntity
{
public:
	virtual ~ScriptableEntity() {}

	virtual ScriptableEntity* Copy()
	{
		return new ScriptableEntity;
	}

	virtual void CallFunction(const std::string& name) {}
	virtual void ExportFunctions() {}

	template <typename T>
	std::unique_ptr<T> Clone() const
	{
		return std::make_unique<T>(static_cast<const T&>(*this));
	}

	template<typename T>
	T* GetComponent()
	{
		return m_entity.GetComponent<T>();
	}

	template<typename T>
	T& Get()
	{
		return m_entity.Get<T>();
	}

	virtual void OnCollisionEnter(Collision2D collision) {}
	virtual void OnCollisionExit(Collision2D collision) {}

	virtual void OnTriggerEnter(Collision2D collision) {}
	virtual void OnTriggerExit(Collision2D collision) {}

	void SpawnCollision2D(Collision2D* collision)
	{
		if (collision)
			m_scene->SpawnCollision2D(collision);
	}

	virtual void Start() {}
	virtual void Update(float dt) {}
	virtual void LateUpdate(float dt) {}

	void Destroy();

	virtual std::vector<std::string> GetFunctionNames()
	{
		return std::vector<std::string>();
	}

	void RunLuaScript(const std::string& fileName, const std::string& functionName, sol::object data);

	Input& GetInput()
	{
		return m_app->GetInput();
	}

	Entity GetPrefab(const std::string& name)
	{
		return m_project->GetPrefabByName(name);
	}

	std::filesystem::path GetPath(const std::filesystem::path& relativePath)
	{
		return m_project->GetDirectory() / relativePath;
	}

protected:
	Project* m_project = nullptr;
	Application* m_app = nullptr;
	Entity m_entity;
	Scene* m_scene = nullptr;

	friend class Scene;
};