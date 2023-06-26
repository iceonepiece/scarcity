#pragma once

#include <vector>
#include "Physics/FixtureData.h"

enum MovementState
{
    MS_IDLE,
    MS_LEFT,
    MS_RIGHT
};

struct PlayerComponent
{
    float PLAYER_MOVE_SPEED = 3.25f;
    float PLAYER_MOVE_ACCELERATION = 15.0f;
    float PLAYER_MOVE_DECELERATION = 25.0f;
    float PLAYER_JUMP_FORCE = 6.25f;

    MovementState movementState;
    bool jump;
    bool attack;
    int numGrounds;
    std::vector<FixtureData*> groundFixtures;
    int direction;
    int health;

    PlayerComponent()
        : movementState(MS_IDLE)
        , jump(false)
        , attack(false)
        , numGrounds(0)
        , direction(1)
        , health(5)
    {
    }
};
