#pragma once

#include <string>
#include <memory>
#include "Core/Application.h"
#include "Core/Entity.h"
#include "Components/TransformComponent.h"
#include "Scene.h"

#define SERIALIZE_FIELD(type, name) type name;

class ScriptableEntity
{
public:
	virtual ~ScriptableEntity() {}

	virtual ScriptableEntity* Copy()
	{
		return new ScriptableEntity;
	}

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

	void CreateCollision(TransformComponent tranform, float lifeTime = -0.1f)
	{
		if (m_scene != nullptr)
			m_scene->InstantiateEntity(1, tranform.position, tranform.scale, lifeTime);
	}

	void Test() {}

	virtual void Start() {}
	virtual void Destroy() {}
	virtual void Update(float dt) {}

protected:
	Application* m_app;
	Entity m_entity;
	Scene* m_scene = nullptr;

	friend class Scene;
};