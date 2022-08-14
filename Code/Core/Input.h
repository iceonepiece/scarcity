#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Game;

struct InputCommand
{
  int keyCode;
  std::string name;
  int previousState;
  int currentState;
  bool isActive;

  InputCommand(int keyCode, std::string name)
    : keyCode(keyCode)
    , name(name)
    , isActive(false)
    , previousState(GLFW_RELEASE)
    , currentState(GLFW_RELEASE)
  {
  }
};

class Input
{
public:
  Input(Game* game);
  bool IsKeyPressed(int keyCode);
  bool IsKeyHeld(int keyCode);
  void PollInputs();
  void AddInputCommand(int keyCode, std::string name);

  Game* m_game;
  std::vector<InputCommand> m_inputCommands;
};
