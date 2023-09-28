#pragma once

#include <entt/entt.hpp>

class Entity;

class EntityManager
{
public:
	Entity CreateEntity();
	Entity CreateEmptyEntity(entt::entity parent = entt::null);
	Entity CreateCanvasEntity(entt::entity parent = entt::null);

	void AddChild(entt::entity parent, Entity& child);

	entt::registry m_registry;
};
