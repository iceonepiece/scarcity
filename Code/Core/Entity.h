#pragma once

#include <functional>
#include <entt/entt.hpp>
#include "EntityManager.h"
#include "Components/Relationship.h"

class Entity;

using UpdateEntityCallback = std::function<void(Entity&)>;

class Entity
{
public:
	Entity() = default;

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

	void UpdateEntityAndChildren(UpdateEntityCallback callback)
	{
		callback(*this);

		if (ChildrenComponent* children = GetComponent<ChildrenComponent>())
		{
			for (int i = 0; i < children->size; i++)
			{
				Entity child = Entity(m_manager, children->entities[i]);
				callback(child);
			}
		}
	}

	entt::entity GetEntity() { return m_id; }
	entt::registry& GetRegistry() { return m_manager->m_registry; }

protected:
	entt::entity m_id = entt::null;
	EntityManager* m_manager = nullptr;
};
