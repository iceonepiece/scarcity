#include "ImGuiMainMenuBar.h"
#include "../EditorLayer.h"
#include "Core/Timer.h"
#include <imgui/imgui_internal.h>
#include <IconsFontAwesome6.h>
#include "../EditorGUI/Windows/ImGui_WindowManager.h"

ImGuiMainMenuBar::ImGuiMainMenuBar(EditorLayer& editor)
	: m_editor(editor)
{

}

void ImGuiMainMenuBar::Render()
{
    ImVec4 menuBarColor{ 0.098f, 0.098f, 0.098f, 1.0f };
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, menuBarColor);

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

        if (ImGui::MenuItem("Save Project"))
        {
            m_editor.SaveProject();
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Exit"))
        {
            WindowCloseEvent event;
            m_editor.OnEvent(event);
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Edit"))
    {
        if (ImGui::MenuItem("Project Settings"))
        {
            ImGui_WindowManager::GetWindow(ImGuiWindowType::ProjectSettings)->SetOpen(true);
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Window"))
    {
        if (ImGui::MenuItem("Animator"))
        {
            ImGui_WindowManager::GetWindow(ImGuiWindowType::Animator)->SetOpen(true);
        }

        ImGui::EndMenu();
    }

    if (ImGui::MenuItem("Reload Native Scripts"))
    {
        m_editor.ReloadNativeScripts();
    }

    bool vsyncEnabled = Application::Get().GetWindow().IsVSync();
    if (ImGui::Checkbox("VSync", &vsyncEnabled))
    {
        Application::Get().GetWindow().SetVSync(vsyncEnabled);
    }

    ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2);

    if (m_editor.IsScenePlaying() && ImGui::MenuItem(ICON_FA_STOP))
        m_editor.StopScene();

    else if (!m_editor.IsScenePlaying() && ImGui::MenuItem(ICON_FA_PLAY))
        m_editor.PlayScene();

    if (ImGui::MenuItem(ICON_FA_PAUSE)) {}

    if (ImGui::MenuItem(ICON_FA_FORWARD_STEP)) {}


    std::stringstream fpsText;
    fpsText << "FPS: " << std::fixed << std::setprecision(1) << ImGui::GetIO().Framerate;

    ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 120);
    ImGui::Text(fpsText.str().c_str());

    ImGui::EndMainMenuBar();

    /*
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
    float height = ImGui::GetFrameHeight();

    if (ImGui::BeginViewportSideBar("##SecondaryMenuBar", viewport, ImGuiDir_Up, height, window_flags))
    {
        if (ImGui::BeginMenuBar())
        {
            ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2);

            if (m_editor.IsScenePlaying() && ImGui::MenuItem(ICON_FA_STOP " "))
                m_editor.StopScene();

            else if (!m_editor.IsScenePlaying() && ImGui::MenuItem(ICON_FA_PLAY " "))
                m_editor.PlayScene();

            ImGui::EndMenuBar();
        }

        ImGui::End();
    }
    */

    ImGui::PopStyleColor();
}