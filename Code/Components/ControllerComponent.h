#pragma once

#include <vector>
#include "../Core/Input.h"

struct ControllerComponent
{
  std::vector<InputCommand> inputCommands;
  ControllerComponent(std::vector<InputCommand> inputCommands) : inputCommands(inputCommands) {}
};
