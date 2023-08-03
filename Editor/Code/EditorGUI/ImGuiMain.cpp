#include "ImGuiMain.h"
#include "Components/Components.h"
#include "../Editor2D.h"
#include "../EditorLayer.h"

ImGuiMain::ImGuiMain(EditorLayer& editor)
    : m_editor(editor)
    , m_entityProperties(editor)
    , m_hierarchy(editor)
    , m_mainMenuBar(editor)
{
}

void ImGuiMain::Render()
{
    // Writing your code here
    m_entityProperties.Render();
    m_hierarchy.Render();
    m_assetPanel.Render();
    m_mainMenuBar.Render();

    ImGui::ShowDemoWindow();
}