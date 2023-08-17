#pragma once

#include <string>
#include <memory>
#include "Core/Application.h"
#include "Core/Entity.h"

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

	virtual void Start() {}
	virtual void Destroy() {}
	virtual void Update(float dt) {}

protected:
	Application* m_app;
	Entity m_entity;

	friend class Scene;
};