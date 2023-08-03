#include "HubLayer.h"
#include "Core/Camera2D.h"
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

}

void HubLayer::OnEvent(Event& event)
{

}

void HubLayer::RenderImGui()
{
    bool showModal = false;

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
        static std::string projectName;
        static std::string location;
        ImGui::Text("Project Setting");

        ImGui::Text("Project name: "); ImGui::SameLine();
        ImGui::InputText("###projectName", &projectName);

        ImGui::Text("Location: "); ImGui::SameLine();
        ImGui::InputText("###location", &directory); ImGui::SameLine();
        
        if (ImGui::Button("Browse"))
        {
            directory = FileDialog::OpenFolder(m_editor.GetWindow().GetNativeWindow());
        }

        if (ImGui::Button("Cancel"))
            ImGui::CloseCurrentPopup();

        if (ImGui::Button("Create Project"))
        {
            m_editor.NewProject(std::make_unique<Project>(projectName, directory));
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }


    ImGui::ShowDemoWindow();
}