#include "ImGuiMainMenuBar.h"
#include "../EditorLayer.h"
#include "Core/Timer.h"

ImGuiMainMenuBar::ImGuiMainMenuBar(EditorLayer& editor)
	: m_editor(editor)
{

}

void ImGuiMainMenuBar::Render()
{
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 10)); // Increase vertical padding
    //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 10));  // Increase vertical spacing
    //ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 10)); // Increase vertical padding of the main menu bar
    
    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("New Scene"))
        {
            m_editor.NewScene();
        }

        if (ImGui::MenuItem("Open Scene"))
        {
        }

        if (ImGui::MenuItem("Save Scene"))
        {
            m_editor.SaveScene();
        }


        ImGui::Separator();

        if (ImGui::MenuItem("Exit"))
        {
            WindowCloseEvent event;
            m_editor.OnEvent(event);
        }

        ImGui::EndMenu();
    }

    if (m_editor.IsScenePlaying() && ImGui::MenuItem("Stop"))
        m_editor.StopScene();

    else if (!m_editor.IsScenePlaying() && ImGui::MenuItem("Play"))
        m_editor.PlayScene();

    if (ImGui::MenuItem("Reload Native Scripts"))
    {
        m_editor.ReloadNativeScripts();
    }

    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

    ImGui::EndMainMenuBar();
    
    ImGui::PopStyleVar();
}