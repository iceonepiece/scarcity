#pragma once

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Game
{
public:
  Game(unsigned int width = 1280, unsigned int height = 720);
  ~Game();

  void Init();
  void Run();

  GLFWwindow* GetWindow() { return m_window; }

private:
  GLFWwindow* m_window;

  unsigned int m_width;
  unsigned int m_height;

  void ProcessInput(float deltaTime);
};
