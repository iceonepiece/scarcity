#include "Input.h"
#include "Game.h"

Input::Input(Game* game)
  : m_game(game)
{
}

void Input::AddInputCommand(int keyCode, std::string name)
{
  m_inputCommands.push_back(InputCommand(keyCode, name));
}

void Input::PollInputs()
{
  for (auto& inputCommand : m_inputCommands)
  {
    inputCommand.previousState = inputCommand.currentState;
    inputCommand.currentState = glfwGetKey(m_game->GetWindow(), inputCommand.keyCode);
  }
}

bool Input::IsKeyPressed(int keyCode)
{
  for (auto inputCommand : m_inputCommands)
  {
    if (
      inputCommand.keyCode == keyCode &&
      inputCommand.previousState == GLFW_RELEASE &&
      inputCommand.currentState == GLFW_PRESS
    ) return true;
  }

  return false;
}

bool Input::IsKeyHeld(int keyCode)
{
  for (auto inputCommand : m_inputCommands)
  {
    if (
      inputCommand.keyCode == keyCode &&
      inputCommand.previousState == GLFW_PRESS &&
      inputCommand.currentState == GLFW_PRESS
    ) return true;
  }

  return false;
}
