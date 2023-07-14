#pragma once

#include "Entity.h"

class Scriptable
{
public:
	Scriptable(Entity entity)
		: m_entity(entity)
	{
	}

	template <typename T>
	T* GetComponent()
	{
		return m_entity.GetComponent<T>();
	}

	virtual void Start() = 0;
	virtual void Update(float dt) = 0;

protected:
	Entity m_entity;
};