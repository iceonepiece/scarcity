#include <glm/gtc/type_ptr.hpp>
#include "ImGuiEntityProperties.h"
#include "ImGuiComponents/ImGuiComponentRenderer.h"
#include "Components/Components.h"
#include "../EditorLayer.h"
#include "imgui/imgui_stdlib.h"
#include <string>
#include "Core/ResourceAPI.h"

std::string EditorComponentNames[] = {
    "Box Collider 2D",
    "Camera",
    "Circle Collider 2D",
    "Mock",
    "Native Script",
    "Rigidbody 2D",
    "Sprite Animator",
    "Sprite Renderer"
};

ImGuiEntityProperties::ImGuiEntityProperties(EditorLayer& editor)
    : m_editor(editor)
{}

void RenderInputVec3(const std::string& name, glm::vec3& values, int width = 80)
{
    std::string xValue = "##X" + name;
    std::string yValue = "##Y" + name;
    std::string zValue = "##Z" + name;

    ImGui::PushItemWidth(width);

    ImGui::Text(name.c_str()); ImGui::SameLine();

    ImGui::Text("X"); ImGui::SameLine();
    ImGui::InputFloat(xValue.c_str(), &values.x); ImGui::SameLine();

    ImGui::Text("Y"); ImGui::SameLine();
    ImGui::InputFloat(yValue.c_str(), &values.y); ImGui::SameLine();

    ImGui::Text("Z"); ImGui::SameLine();
    ImGui::InputFloat(zValue.c_str(), &values.z);

    ImGui::PopItemWidth();
}

void ImGuiEntityProperties::Render()
{
    ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 4.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));
    ImGui::Begin("Properties", NULL, ImGuiWindowFlags_NoCollapse);

    if (ImGui::IsWindowAppearing())
    {
        ImGui::SetWindowPos(ImVec2(800, 100));
    }

    if (m_editor.GetScene() != nullptr && m_editor.GetSelectedObject().type == EditorObjectType::Path)
    {
        Asset* asset = m_editor.GetSelectedAsset();

        if (asset != nullptr)
        {
            m_imguiAssetProperties.Render(asset);
        }
        //Resource resource = FileSystem::GetResource(m_editor.GetSelectedObject().path);
    }
    else if (m_editor.GetScene() != nullptr && m_editor.GetSelectedObject().type == EditorObjectType::Entity)
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
            }, ComponentList{});

            ImGui::EndPopup();
        }

        ImGui::PopID();
    }

    ImGui::End();
    ImGui::PopStyleVar(2);
}