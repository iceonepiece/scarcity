#include "HubLayer.h"
#include "Core/Camera2D.h"
#include "File/FileSystem.h"
#include "Utils/FileDialog.h"
#include <imgui/imgui_stdlib.h>

HubLayer::HubLayer(EditorApplication& editor)
	: m_editor(editor)
{
}

void HubLayer::Initialize()
{
}

void HubLayer::Shutdown()
{

}

void HubLayer::Update(float deltaTime)
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void HubLayer::OnEvent(Event& event)
{

}

void HubLayer::RenderImGui()
{
    bool showModal = false;
    bool projectCreated = false;
    bool projectOpened = false;

    static std::string projectName;
    static std::string location;
    static std::string directory;

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 10)); // Increase vertical padding
    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("New Project"))
        {
            showModal = true;
        }
        
        if (ImGui::MenuItem("Open Project"))
        {
            directory = FileDialog::OpenFile(PROJECT_FILE_FILTER);
            projectOpened = true;
        }
        
        ImGui::Separator();
        
        if (ImGui::MenuItem("Exit"))
        {
            WindowCloseEvent event;
            m_editor.OnEvent(event);
        }

        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();

    ImGui::PopStyleVar();

    if (showModal)
        ImGui::OpenPopup("New Project");

    if (ImGui::BeginPopupModal("New Project", NULL, NULL))
    {
        ImGui::Text("Project Setting");

        ImGui::Text("Project name: "); ImGui::SameLine();
        ImGui::InputText("###projectName", &projectName);

        ImGui::Text("Location: "); ImGui::SameLine();
        ImGui::InputText("###location", &directory); ImGui::SameLine();
        
        if (ImGui::Button("Browse"))
        {
            directory = FileDialog::OpenFolder();
        }

        if (ImGui::Button("Cancel"))
            ImGui::CloseCurrentPopup();

        if (ImGui::Button("Create Project"))
        {
            projectCreated = true;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }


    ImGui::ShowDemoWindow();

    if (projectCreated)
    {
        std::filesystem::path createdDirectory = directory;
        
        if (m_editor.NewProject(projectName, createdDirectory))
        {
            m_editor.OpenProject(createdDirectory / projectName / (projectName + PROJECT_FILE_EXTENSION));
        }
    }
    else if (projectOpened)
    {
        m_editor.OpenProject(directory);
    }
}