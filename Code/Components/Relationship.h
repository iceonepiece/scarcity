#pragma once

#include <array>
#include <entt/entt.hpp>

const uint8_t MAX_ENTITY_CHILDREN = 8;

struct ParentComponent
{
    entt::entity entity = entt::null;
};

struct ChildrenComponent
{
    size_t size = 0;
    std::array<entt::entity, MAX_ENTITY_CHILDREN> entities;
};
