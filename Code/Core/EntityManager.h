#pragma once

#include <entt/entt.hpp>

class Entity;

class EntityManager
{
public:
	bool IsEntityValid(entt::entity entity);
	bool DeleteEntity(entt::entity entity);

	Entity CreateEntity();
	Entity CreateEmptyEntity(entt::entity parent = entt::null);
	Entity CreateCanvasEntity(entt::entity parent = entt::null);

	void AddChild(entt::entity parent, Entity& child);
	void RemoveChild(entt::entity parent, entt::entity child);

	entt::registry m_registry;
};
