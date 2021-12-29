#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

class GUI
{
public:
  GUI();
  ~GUI();

  void Init(GLFWwindow* window, const char* glslVersion);
  void Destroy();

  void NewFrame();
  void Draw();
};
