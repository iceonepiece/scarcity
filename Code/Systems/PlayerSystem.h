#pragma once

#include <iostream>

const float PLAYER_MOVE_SPEED = 5.5f;
const float PLAYER_MOVE_ACCELERATION = 14.0f;
const float PLAYER_MOVE_DECELERATION = 22.0f;
const float PLAYER_JUMP_FORCE = 8.5f;

class PlayerSystem
{
public:
  static void ProcessInput(Input& input, entt::registry& registry)
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

  static void Update(float deltaTime, entt::registry& m_registry)
  {
    auto view = m_registry.view<PlayerComponent, Collider2DComponent>();

    for (auto [entity, player, collider]: view.each())
    {
      b2Body* body = collider.body;
      if (body)
      {
        b2Vec2 velocity = body->GetLinearVelocity();

        float desiredVelocity = 0;

        switch (player.movementState)
        {
          case MS_LEFT:
            desiredVelocity = velocity.x < 0 ? velocity.x - (PLAYER_MOVE_ACCELERATION * deltaTime) : -PLAYER_MOVE_ACCELERATION * deltaTime; break;
          case MS_IDLE:
            desiredVelocity = 0; break;
          case MS_RIGHT:
            desiredVelocity = velocity.x > 0 ? velocity.x + (PLAYER_MOVE_ACCELERATION * deltaTime) : PLAYER_MOVE_ACCELERATION * deltaTime; break;
        }

        if (desiredVelocity == 0)
        {
          if (velocity.x < 0)
          {
            desiredVelocity = velocity.x + (PLAYER_MOVE_DECELERATION * deltaTime);

            if (desiredVelocity > 0)
              desiredVelocity = 0;

          }
          else if (velocity.x > 0)
          {
            desiredVelocity = velocity.x - (PLAYER_MOVE_DECELERATION * deltaTime);

            if (desiredVelocity < 0)
              desiredVelocity = 0;
          }
        }

        if (desiredVelocity > PLAYER_MOVE_SPEED)
          desiredVelocity = PLAYER_MOVE_SPEED;
        else if (desiredVelocity < -PLAYER_MOVE_SPEED)
          desiredVelocity = -PLAYER_MOVE_SPEED;


        body->SetLinearVelocity(b2Vec2(desiredVelocity, velocity.y));

        if (player.jump && player.numGrounds > 0)
          body->ApplyLinearImpulse(b2Vec2(0, PLAYER_JUMP_FORCE), body->GetWorldCenter(), true);
      }
    }
  }
};
