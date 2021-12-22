#pragma once

#include "../External/entt/entt.hpp"
#include "EntityManager.h"

class Entity
{
public:
	Entity(EntityManager* manager, entt::entity id)
		:m_manager(manager)
		,m_id(id)
	{

	}

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		T& component = m_manager->m_registry.emplace<T>(m_id, std::forward<Args>(args)...);
		return component;
	}

	template<typename T>
	T* GetComponent()
	{
		return m_manager->m_registry.try_get<T>(m_id);
	}

protected:
	entt::entity m_id;
	EntityManager* m_manager;
};
