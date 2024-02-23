#pragma once

#include "System.h"
#include "Components/SpriteAnimatorComponent.h"

class AnimatorSystem : public System
{
public:
    AnimatorSystem(Scene& scene)
        : System(scene)
    {}

    virtual void Update(float deltaTime) override
    {
        auto animatorView = m_registry.view<SpriteAnimatorComponent>();
        for (auto [entity, animator] : animatorView.each())
        {
            if (animator.controller != nullptr)
                animator.controller->Process();
        }
    }
};