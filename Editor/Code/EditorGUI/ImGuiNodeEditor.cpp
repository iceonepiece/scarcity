#include "ImGuiNodeEditor.h"

ImGuiNodeEditor::ImGuiNodeEditor()
{
    ed::Config config;
    config.SettingsFile = "NodeEditor.json";
    m_context = ed::CreateEditor(&config);
}

ImGuiNodeEditor::~ImGuiNodeEditor()
{
    ed::DestroyEditor(m_context);
}

void ImGuiNodeEditor::Render()
{
    auto& io = ImGui::GetIO();

    ImGui::Begin("Node Editor");

    ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

    ImGui::Separator();

    ed::SetCurrentEditor(m_context);


    ed::Begin("My Editor", ImVec2(0.0, 0.0f));
    /*
    int uniqueId = 1;
    // Start drawing nodes.

    ed::BeginNode(uniqueId++);

    ImGui::Text("Node A");
    ed::BeginPin(uniqueId++, ed::PinKind::Input);
    ImGui::Text("-> In");
    ed::EndPin();

    ImGui::SameLine();
    ed::BeginPin(uniqueId++, ed::PinKind::Output);
    ImGui::Text("Out ->");
    ed::EndPin();

    ed::EndNode();
                        */

    ed::End();

    ed::SetCurrentEditor(nullptr);

    ImGui::End();
}