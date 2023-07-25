#include "ImGuiEntityProperties.h"
#include "../Editor2D.h"

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
    ImGui::Begin("Properties");


    if (m_editor.GetScene() != nullptr && m_editor.IsEntityPicked())
    {
        Scene* scene = m_editor.GetScene();
        entt::entity enttiy = m_editor.GetPickedEntity();

        TransformComponent* transform = scene->GetEntityManager().m_registry.try_get<TransformComponent>(enttiy);

        if (transform != nullptr)
        {
            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_None))
            {
                RenderInputVec3("Position", transform->position);
                RenderInputVec3("Rotation", transform->rotation);
                RenderInputVec3("Scale", transform->scale);
            }
        }
    }

    ImGui::End();
}