#pragma once

#include <vector>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "GUIComponent.h"
#include "../Events/Event.h"

class Game;

class GUI
{
public:
  GUI(Game* game);
  ~GUI();

  void Init(GLFWwindow* window, const char* glslVersion);
  void Destroy();

  void NewFrame();
  void Draw();

  void AddComponent(GUIComponent* component);

  bool BlockEvent(Event* e);

private:
  Game* m_game;
  std::vector<GUIComponent*> m_components;

  friend class GUIComponent;
};
