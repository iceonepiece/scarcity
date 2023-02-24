#include "ImGuiLayer.h"

ImGuiLayer::~ImGuiLayer()
{
}

void ImGuiLayer::Initialize(GLFWwindow* window, const char* glslVersion)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    io.Fonts->AddFontFromFileTTF("Assets/Fonts/Xolonium-Regular.ttf", 20);
}

bool ImGuiLayer::BlockEvent(Event* e)
{
    ImGuiIO& io = ImGui::GetIO();

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

void ImGuiLayer::Destroy()
{
    for (auto component : m_components)
        delete component;

    m_components.clear();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::AddComponent(ImGuiComponent *component)
{
    component->SetImGuiLayer(this);
    m_components.push_back(component);
}

void ImGuiLayer::NewFrame()
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

void ImGuiLayer::Draw()
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

EntityManager& ImGuiLayer::GetEntityManager()
{
    return m_manager;
}