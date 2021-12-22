#pragma once

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Game;

struct InputCommand
{
  int keyCode;
  std::string name;
  bool isActive;

  InputCommand(int keyCode, std::string name)
    : keyCode(keyCode)
    , name(name)
    , isActive(false)
  {
  }
};

class Input
{
public:
  Input(Game* game);
  bool IsKeyPressed(int keyCode);

private:
  Game* m_game;
};
