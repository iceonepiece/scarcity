#include "EntityManager.h"
#include "Entity.h"
#include "Components/Components.h"
#include "Components/Relationship.h"

Entity EntityManager::CreateEntity()
{
  Entity entity(this, m_registry.create());
  return entity;
}

void EntityManager::AddChild(entt::entity parent, Entity& child)
{
    child.AddComponent<ParentComponent>(parent);

    ChildrenComponent* children = m_registry.try_get<ChildrenComponent>(parent);

    if (children == nullptr)
    {
        children = &m_registry.emplace<ChildrenComponent>(parent);
    }

    children->entities[children->size++] = child.GetEntity();
}

Entity EntityManager::CreateEmptyEntity(entt::entity parent)
{
    auto entity = CreateEntity();
    entity.AddComponent<BaseComponent>("Empty");
    entity.AddComponent<TransformComponent>();

    if (parent != entt::null)
        AddChild(parent, entity);

    return entity;
}

Entity EntityManager::CreateCanvasEntity(entt::entity parent)
{
    auto entity = CreateEntity();
    entity.AddComponent<BaseComponent>("Canvas");
    entity.AddComponent<TransformComponent>();
    entity.AddComponent<CanvasComponent>();

    if (parent != entt::null)
        AddChild(parent, entity);

    return entity;
}
