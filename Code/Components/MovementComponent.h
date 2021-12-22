#pragma once

enum MovementState {
  MS_IDLE,
  MS_LEFT,
  MS_RIGHT
};

struct MovementComponent
{
  MovementState state;

  MovementComponent()
    : state(MS_IDLE)
  {
  }
};
