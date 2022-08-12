#include "GUI.h"
#include "Game.h"

GUI::GUI(Game* game)
  : m_game(game)
{
}

GUI::~GUI()
{
}

void GUI::Destroy()
{
  std::cout << "GUI Destroy" << std::endl;
  for (auto component : m_components)
  {
    delete component;
  }
  m_components.clear();

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

void GUI::AddComponent(GUIComponent* component)
{
  m_components.push_back(component);
}

void GUI::NewFrame()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  //ImGui::ShowDemoWindow();

  ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.


  ImGui::End();

  /*
  for (auto component : m_components)
  {
    component->Draw();
  }
  */

  /*
  ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.


  ImGui::End();
  */

  ImGui::Render();
}

void GUI::Draw()
{
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
