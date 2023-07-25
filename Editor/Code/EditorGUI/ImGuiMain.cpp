#include "ImGuiMain.h"
#include "Components/Components.h"
#include "../Editor2D.h"

ImGuiMain::ImGuiMain(Editor2D& editor, GLFWwindow* window, const char* glslVersion)
    : m_editor(editor)
    , m_entityProperties(editor)
    , m_hierarchy(editor)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    io.Fonts->AddFontFromFileTTF("Fonts/Xolonium-Regular.ttf", 18);
}

ImGuiMain::~ImGuiMain()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiMain::Render()
{
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Writing your code here
    m_entityProperties.Render();
    m_hierarchy.Render();

    //ImGui::ShowDemoWindow();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}