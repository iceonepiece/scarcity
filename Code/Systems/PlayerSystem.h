#pragma once

#include <iostream>
#include "../Core/System.h"
#include "../Components/PlayerComponent.h"
#include "../Components/Collider2DComponent.h"
#include "../Components/TransformComponent.h"
#include "../Prefabs/Projectile.h"
#include "../Core/ResourceAPI.h"

class PlayerSystem : public System
{
public:
    PlayerSystem(Scene* scene)
        : System(scene)
    {

    }

    void Move(PlayerComponent& player, b2Body *body, float deltaTime)
    {
        b2Vec2 velocity = body->GetLinearVelocity();

        float desiredVelocity = 0;

        switch (player.movementState)
        {
        case MS_LEFT:
            desiredVelocity = velocity.x < 0 ? velocity.x - (player.PLAYER_MOVE_ACCELERATION * deltaTime) : -player.PLAYER_MOVE_ACCELERATION * deltaTime; break;
        case MS_IDLE:
            desiredVelocity = 0; break;
        case MS_RIGHT:
            desiredVelocity = velocity.x > 0 ? velocity.x + (player.PLAYER_MOVE_ACCELERATION * deltaTime) : player.PLAYER_MOVE_ACCELERATION * deltaTime; break;
        }

        if (player.movementState == MS_LEFT || player.movementState == MS_RIGHT && !(desiredVelocity < 2 && desiredVelocity > -2))
        {
            ParticleProps props = ResourceAPI::GetParticle("running");
            b2Vec2 position = body->GetPosition();
            props.position = { position.x, position.y - 0.8 };
            for (int i = 0; i < props.amount; i++)
            {
                ParticleSystem::Emit(props);
            }
        }

        if (desiredVelocity == 0)
        {
            if (velocity.x < 0)
            {
                desiredVelocity = velocity.x + (player.PLAYER_MOVE_DECELERATION * deltaTime);

                if (desiredVelocity > 0)
                    desiredVelocity = 0;

            }
            else if (velocity.x > 0)
            {
                desiredVelocity = velocity.x - (player.PLAYER_MOVE_DECELERATION * deltaTime);

                if (desiredVelocity < 0)
                    desiredVelocity = 0;
            }
        }

        if (desiredVelocity > player.PLAYER_MOVE_SPEED)
            desiredVelocity = player.PLAYER_MOVE_SPEED;
        else if (desiredVelocity < -player.PLAYER_MOVE_SPEED)
            desiredVelocity = -player.PLAYER_MOVE_SPEED;

        body->SetLinearVelocity(b2Vec2(desiredVelocity, velocity.y));

        if (player.jump && player.numGrounds > 0)
            body->ApplyLinearImpulse(b2Vec2(0, player.PLAYER_JUMP_FORCE), body->GetWorldCenter(), true);
    }

    virtual void Update(float deltaTime) override
    {
        /*
        auto view = m_registry.view<TransformComponent, PlayerComponent>();

        for (auto [entity, transform, player] : view.each())
        {
            player.movementState = MS_IDLE;
            player.jump = false;

            if (Input::IsKeyHeld(Key::Left))
                player.movementState = MS_LEFT;

            if (Input::IsKeyHeld(Key::Right))
                player.movementState = MS_RIGHT;

            if (Input::IsKeyPressed(Key::Space))
                player.jump = true;

            if ((player.direction == 1 && player.movementState == MS_LEFT) || (player.direction == -1 && player.movementState == MS_RIGHT))
            {
                player.direction *= -1;
                transform.scale.x = player.direction;
            }
        }
        */

        auto view2 = m_registry.view<PlayerComponent, Collider2DComponent>();

        for (auto [entity, player, collider] : view2.each())
        {
            b2Body* body = collider.body;

            if (body)
                Move(player, body, deltaTime);

            if (player.attack && body)
            {
                player.attack = false;

                b2Vec2 position = body->GetPosition();
                
                auto projectile = m_scene->m_manager.CreateEntity();
                Projectile(projectile, m_scene, position, 0.25, 0.25, player.direction * 10);
            }

            if (Input::IsKeyPressed(Key::Z))
            {
                std::cout << "Pressed Z" << std::endl;
                player.attack = true;
            }
            
            if (Input::IsKeyPressed(Key::X))
            {
                std::cout << "Pressed X" << std::endl;
            }

        }
    }

    virtual void Render() override
    {
        Renderer& renderer = m_scene->m_game->GetRenderer();

        renderer.DrawCircle(glm::vec3(0.0f), 1.0f, false);
        renderer.DrawCircle(glm::vec3(0.0f), 2.0f, false);
        renderer.DrawCircle(glm::vec3(0.0f), 4.0f, false);
    }
};
