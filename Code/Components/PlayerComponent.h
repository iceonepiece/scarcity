#pragma once

#include <vector>

enum MovementState {
  MS_IDLE,
  MS_LEFT,
  MS_RIGHT
};

struct PlayerComponent
{
  MovementState movementState;
  bool jump;
  int numGrounds;
  std::vector<FixtureData*> groundFixtures;

  PlayerComponent()
    : movementState(MS_IDLE)
    , jump(false)
    , numGrounds(0)
  {
  }
};
