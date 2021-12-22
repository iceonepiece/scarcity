#include "Input.h"
#include "Game.h"

Input::Input(Game* game)
  : m_game(game)
{
}

bool Input::IsKeyPressed(int keyCode)
{
  auto state = glfwGetKey(m_game->GetWindow(), keyCode);
  return state == GLFW_PRESS || state == GLFW_REPEAT;
}
