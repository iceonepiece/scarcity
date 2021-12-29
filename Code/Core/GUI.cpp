#include "GUI.h"

GUI::GUI()
{
}

GUI::~GUI()
{
}

void GUI::Destroy()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void GUI::Init(GLFWwindow* window, const char* glslVersion)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glslVersion);
}


void GUI::NewFrame()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::Render();
}

void GUI::Draw()
{
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
