#pragma once

#include <functional>
#include <entt/entt.hpp>
#include "EntityManager.h"
#include "Components/Relationship.h"
#include "Components/IDComponent.h"
#include "Components/BaseComponent.h"
#include "Core/UniqueID.h"

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

	bool IsValid()
	{
		return m_manager != nullptr && m_id != entt::null;
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

	std::string GetName()
	{
		return GetComponent<BaseComponent>()->name;
	}

	UniqueID GetID()
	{
		return GetComponent<IDComponent>()->ID;
	}

	inline EntityManager* GetManager() { return m_manager; }

	entt::entity GetEntity() { return m_id; }
	entt::registry& GetRegistry() { return m_manager->m_registry; }

protected:
	entt::entity m_id = entt::null;
	EntityManager* m_manager = nullptr;
};
