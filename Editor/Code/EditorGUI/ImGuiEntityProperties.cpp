#include <entt/entt.hpp>
#include "ImGuiEntityProperties.h"
#include "Components/Components.h"
#include "../Editor2D.h"
#include "imgui/imgui_stdlib.h"
#include <string.h>

ImGuiEntityProperties::ImGuiEntityProperties(Editor2D& editor)
    : m_editor(editor)
{}

void RenderInputVec3(const std::string& name, glm::vec3& values, int width = 100)
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
    ImGui::Begin("Properties", NULL, ImGuiWindowFlags_NoCollapse);

    if (m_editor.GetScene() != nullptr && m_editor.IsEntityPicked())
    {
        Scene* scene = m_editor.GetScene();
        entt::entity entity = m_editor.GetPickedEntity();

        ImGui::PushID((int)entity);

        BaseComponent* base = scene->GetEntityManager().m_registry.try_get<BaseComponent>(entity);
        if (base != nullptr)
        {
            ImGui::Text("Name: "); ImGui::SameLine();
            ImGui::InputText("##name", &base->name);
        }

        TransformComponent* transform = scene->GetEntityManager().m_registry.try_get<TransformComponent>(entity);

        if (transform != nullptr)
        {
            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
            {
                RenderInputVec3("Position", transform->position);
                RenderInputVec3("Rotation", transform->rotation);
                RenderInputVec3("Scale", transform->scale);
            }
        }

        ImGui::PopID();
    }

    ImGui::End();
}