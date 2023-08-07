#pragma once

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>
#include "Components.h"

using json = nlohmann::json;

class ComponentSerializer
{
public:
	ComponentSerializer(entt::registry& registry)
		: m_registry(registry)
	{
	}

	template <typename T>
	void Serialize(json& entityJson, entt::entity entity)
	{
		if (m_registry.all_of<T>(entity))
		{
			T& component = m_registry.get<T>(entity);
			DoSerialize(component, entityJson);
		}
	}

	template <typename T>
	void Deserialize(json& entityJson, Entity& entity)
	{
		if (entityJson[T::Name()].is_object())
		{
			T component;
			DoDeserialize(component, entityJson[T::Name()]);
			entity.AddComponent<T>(component);
		}
	}

private:
	entt::registry& m_registry;
};
