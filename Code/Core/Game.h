#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <sol/sol.hpp>

#include "Entity.h"
#include "EntityManager.h"
#include "Physics.h"
#include "Input.h"
#include "Renderer.h"
#include "Camera.h"
#include "ParticleSystem.h"
#include "GUI.h"
#include "GUIComponent.h"

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
  ParticleSystem m_particleSystem;
  std::unordered_map<std::string, ParticleProps> m_particles;

  GUI m_gui;

  sol::state m_lua;

  void LoadParticles();

  void ProcessInput(float deltaTime);
  void Update(float deltaTime);
  void Render();

  friend class GUI;
  friend class GUIComponent;
};
