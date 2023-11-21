#include "ImGui_InspectorPanel.h"
#include "Inspectors/ImGui_AssetInspector.h"
#include "ImGuiComponents/ImGuiComponentRenderer.h"
#include "Inspectors/ImGui_AnimatorInspector.h"
#include "Components/Components.h"
#include "../EditorLayer.h"
#include "imgui/imgui_stdlib.h"
#include "ImGui_AnimatorPanel.h"
#include <string>

ImGui_InspectorPanel::ImGui_InspectorPanel(EditorLayer& editor)
    : m_editor(editor)
{

}

void ImGui_InspectorPanel::Render()
{
    ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 4.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));
    ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_NoCollapse);

    if (m_editor.GetSelectedObject().type == EditorObjectType::Asset)
    {
        if (Asset* asset = m_editor.GetSelectedAsset())
            ImGui_AssetInspector::Render(asset);
    }
    else if (m_editor.GetSelectedObject().type == EditorObjectType::AnimatorState)
    {
        if (Node* animatorState = reinterpret_cast<Node*>(m_editor.GetSelectedObject().objectPtr))
        {
            ImGui_AnimatorInspector::Render(*animatorState);
        }
    }

    else if (m_editor.GetSelectedObject().type == EditorObjectType::Entity)
    {
        Scene* scene = m_editor.GetScene();
        entt::entity entity = m_editor.GetSelectedObject().entity;
        auto& registry = scene->GetEntityManager().m_registry;

        ImGui::PushID((int)entity);

        std::apply([&](auto... componentTypes) {
            (ImGuiComponentRenderer::RenderComponent<decltype(componentTypes)>(registry, entity), ...);
            }, ComponentList{});

        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("add_component");

        ImGui::SameLine();

        if (ImGui::BeginPopup("add_component"))
        {
            ImGui::Text("Component");
            ImGui::Separator();

            std::apply([&](auto... componentTypes) {
                (RenderAddComponent<decltype(componentTypes)>(registry, entity), ...);
                }, ToAddComponents{});

            ImGui::Separator();

            std::apply([&](auto... componentTypes) {
                (RenderAddUIComponent<decltype(componentTypes)>(registry, entity), ...);
                }, ToAddUIComponents{});

            if (registry.try_get<GridComponent>(entity))
                m_editor.SetPickedEntity(entity);

            ImGui::EndPopup();
        }

        ImGui::PopID();
    }

    ImGui::End();
    ImGui::PopStyleVar(2);
}