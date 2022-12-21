#pragma once

#include <vector>

enum MovementState
{
    MS_IDLE,
    MS_LEFT,
    MS_RIGHT
};

struct PlayerComponent
{
    float PLAYER_MOVE_SPEED = 6.0f;
    float PLAYER_MOVE_ACCELERATION = 16.0f;
    float PLAYER_MOVE_DECELERATION = 23.0f;
    float PLAYER_JUMP_FORCE = 7.5f;

    MovementState movementState;
    bool jump;
    bool attack;
    int numGrounds;
    std::vector<FixtureData*> groundFixtures;

    PlayerComponent()
        : movementState(MS_IDLE)
        , jump(false)
        , attack(false)
        , numGrounds(0)
    {
    }
};
