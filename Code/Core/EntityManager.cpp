#include "EntityManager.h"
#include "Entity.h"
#include "Components/Components.h"
#include "Components/Relationship.h"

bool EntityManager::IsEntityValid(entt::entity entity)
{
    return m_registry.valid(entity);
}

bool EntityManager::DeleteEntity(entt::entity entity)
{
    if (!IsEntityValid(entity))
        return false;

    std::cout << "Delete entity: " << (int)entity << std::endl;

    if (ChildrenComponent* children = m_registry.try_get<ChildrenComponent>(entity))
    {
        for (int i = 0; i < children->size; i++)
        {
            DeleteEntity(children->entities[i]);
        }
    }

    if (ParentComponent* parent = m_registry.try_get<ParentComponent>(entity))
        RemoveChild(parent->entity, entity);

    m_registry.destroy(entity);

    return true;
}

Entity EntityManager::CreateEntity()
{
  Entity entity(this, m_registry.create());
  return entity;
}

void EntityManager::RemoveChild(entt::entity parent, entt::entity child)
{
    if (ChildrenComponent* children = m_registry.try_get<ChildrenComponent>(parent))
    {
        int indexToRemove = -1;
        for (int i = 0; i < children->size; i++)
        {
            if (children->entities[i] == child)
            {
                indexToRemove = i;
                break;
            }
        }

        if (indexToRemove >= 0)
        {
            for (size_t i = indexToRemove; i < children->size - 1; i++)
                children->entities[i] = children->entities[i + 1];

            children->entities[children->size - 1] = entt::null;
            children->size--;
        }

        if (children->size == 0)
            m_registry.remove<ChildrenComponent>(parent);
    }
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
    entity.AddComponent<IDComponent>();
    entity.AddComponent<BaseComponent>("Empty");
    entity.AddComponent<TransformComponent>();

    if (parent != entt::null)
        AddChild(parent, entity);

    return entity;
}

Entity EntityManager::CreateCanvasEntity(entt::entity parent)
{
    auto entity = CreateEntity();
    entity.AddComponent<IDComponent>();
    entity.AddComponent<BaseComponent>("Canvas");
    entity.AddComponent<TransformComponent>();
    entity.AddComponent<CanvasComponent>();

    if (parent != entt::null)
        AddChild(parent, entity);

    return entity;
}
