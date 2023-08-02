#include "ImGuiMainMenuBar.h"
#include "../EditorLayer.h"

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

    if (ImGui::MenuItem("New")) {}

    if (m_editor.IsScenePlaying() && ImGui::MenuItem("Stop"))
        m_editor.StopScene();

    else if (!m_editor.IsScenePlaying() && ImGui::MenuItem("Play"))
        m_editor.PlayScene();

    ImGui::EndMainMenuBar();
    
    ImGui::PopStyleVar();
}