#include "GUI.h"
#include "Core/GameApplication.h"
#include <iostream>

GUI::GUI(EditorApplication* editor)
    : m_editor(editor)
{
}

GUI::~GUI()
{
}

bool GUI::BlockEvent(Event* e)
{
    ImGuiIO& io = ImGui::GetIO();

    if (io.WantCaptureMouse)
        std::cout << "WantCaptureMouse" << std::endl;

    if (io.WantCaptureMouse &&
        (
            e->GetType() == EventType::MouseMoved ||
            e->GetType() == EventType::MouseButtonPressed ||
            e->GetType() == EventType::MouseScrolled
        )
    )
        return true;


    return false;
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

    //io.Fonts->AddFontFromFileTTF("fonts/ProggyClean.ttf", 20);
    //io.Fonts->AddFontFromFileTTF("fonts/DroidSans.ttf", 20);
    //io.Fonts->AddFontFromFileTTF("Fonts/Xolonium-Regular.ttf", 20);
    //io.Fonts->AddFontFromFileTTF("fonts/Roboto-Medium.ttf", 18);
}

void GUI::AddComponent(GUIComponent* component)
{
    m_components.push_back(component);
}

void GUI::NewFrame()
{
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    for (auto component : m_components)
    {
        component->Draw();
    }

    ImGui::ShowDemoWindow();

    ImGui::Render();
}

void GUI::Draw()
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
