#pragma once

#include <iostream>
#include "../Core/System.h"
#include "../Components/PlayerComponent.h"
#include "../Components/Collider2DComponent.h"
#include "../Prefabs/Projectile.h"

class PlayerSystem : public System
{
public:
    PlayerSystem(Scene* scene)
        : System(scene)
    {

    }

    virtual void ProcessInput(Input& input, entt::registry& registry) override
    {
        auto view = registry.view<PlayerComponent>();

        for (auto [entity, player]: view.each())
        {
            player.movementState = MS_IDLE;
            player.jump = false;

            if (input.IsKeyHeld(GLFW_KEY_LEFT))
                player.movementState = MS_LEFT;

            if (input.IsKeyHeld(GLFW_KEY_RIGHT))
                player.movementState = MS_RIGHT;

            if (input.IsKeyPressed(GLFW_KEY_SPACE))
                player.jump = true;
        }
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
            ParticleProps props = ResourceManager::s_particles["running"];
            b2Vec2 position = body->GetPosition();
            props.position = { position.x, position.y - 0.8 };
            for (int i = 0; i < props.amount; i++)
                ParticleSystem::Emit(props);
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

    virtual void Update(float deltaTime, entt::registry& registry) override
    {
        auto view = registry.view<PlayerComponent, Collider2DComponent>();

        for (auto [entity, player, collider] : view.each())
        {
            b2Body* body = collider.body;

            if (body)
                Move(player, body, deltaTime);

            if (player.attack && body)
            {
                player.attack = false;

                b2Vec2 position = body->GetPosition();
                
                auto projectile = scene->m_manager.CreateEntity();
                Projectile(projectile, scene, position, 0.25, 0.25, 10);
            }

            Input& input = scene->m_game->GetInput();

            if (input.IsKeyPressed(GLFW_KEY_Z))
            {
                std::cout << "Pressed Z" << std::endl;
                player.attack = true;
            }
            
            if (input.IsKeyPressed(GLFW_KEY_X))
            {
                std::cout << "Pressed X" << std::endl;
            }

        }
    }
};
