#pragma once

#pragma once

#include "System.h"
#include <iostream>
#include "Components/TransformComponent.h"
#include "Components/Relationship.h"

class HierarchySystem : public System
{
public:
    HierarchySystem(Scene& scene)
        : System(scene)
    {}

    virtual void Update(float deltaTime) override
    {
        auto view = m_registry.view<TransformComponent, ChildrenComponent>();
        for (auto [entity, transform, parent] : view.each())
        {
            std::cout << (int)entity << std::endl;
        }
    }
};