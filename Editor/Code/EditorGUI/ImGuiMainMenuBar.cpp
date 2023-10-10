#include "ImGuiMainMenuBar.h"
#include "../EditorLayer.h"
#include "Core/Timer.h"

ImGuiMainMenuBar::ImGuiMainMenuBar(EditorLayer& editor)
	: m_editor(editor)
{

}

void ImGuiMainMenuBar::Render()
{
    ImGui::BeginMenuBar();

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

    std::stringstream fpsText;
    fpsText << "FPS: " << std::fixed << std::setprecision(1) << ImGui::GetIO().Framerate;

    ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 120);
    ImGui::Text(fpsText.str().c_str());

    ImGui::EndMenuBar();
}