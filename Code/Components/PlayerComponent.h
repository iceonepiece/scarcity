#pragma once

enum MovementState {
  MS_IDLE,
  MS_LEFT,
  MS_RIGHT
};

struct PlayerComponent
{
  MovementState movementState;

};
