#include "EntityManager.h"
#include "Entity.h"

Entity EntityManager::CreateEntity()
{
  Entity entity(this, m_registry.create());
  return entity;
}
