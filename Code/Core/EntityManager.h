#pragma once

#include "../External/entt/entt.hpp"

class Entity;

class EntityManager
{
public:
  entt::registry m_registry;

  Entity CreateEntity();
};
