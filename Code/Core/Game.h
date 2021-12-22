#pragma once

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Entity.h"
#include "EntityManager.h"
#include "Physics.h"
#include "Input.h"
#include "Renderer.h"
#include "Camera.h"

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

  EntityManager m_manager;
  Physics m_physics;
  Input m_input;
  Camera m_camera;

  void ProcessInput(float deltaTime);
  void Update(float deltaTime);
  void Render();
};
