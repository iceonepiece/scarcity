#pragma once

#include "System.h"
#include "Scene/Scene.h"

class PhysicsSystem : public System
{
public:
    PhysicsSystem(Scene& scene)
        : System(scene)
    {}

    virtual void OnStart() override
    {
        auto view = m_registry.view<TransformComponent, Rigidbody2DComponent>();
        for (auto [entity, transform, rb2d] : view.each())
        {
            Entity _entity{ &m_scene.GetEntityManager(), entity };
            m_scene.GetPhysics().InitializePhysicsEntity(_entity, transform, rb2d);
        }
    }

    virtual void OnStop() override
    {
        auto view = m_registry.view<Rigidbody2DComponent>();
        for (auto [entity, rb2d] : view.each())
            m_scene.GetPhysics().DestroyPhysicsEntity(rb2d);
    }

    virtual void Update(float deltaTime) override
    {
		m_scene.GetPhysics().Update(deltaTime);

        auto view = m_registry.view<TransformComponent, Rigidbody2DComponent>();
        for (auto [entity, transform, rb2d] : view.each())
        {
            b2Body* body = (b2Body*)rb2d.body;

            const auto& position = body->GetPosition();
            transform.position.x = position.x;
            transform.position.y = position.y;
            transform.rotation.z = body->GetAngle();
        }
    }
};