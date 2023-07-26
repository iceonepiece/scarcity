#include <entt/entt.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ImGuiEntityProperties.h"
#include "Components/Components.h"
#include "../Editor2D.h"
#include "imgui/imgui_stdlib.h"
#include <string.h>

std::string EditorComponentNames[] = {
    "Sprite Renderer",
    "Box Collider 2D",
    "Rigidbody 2D"
};

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
        auto& registry = scene->GetEntityManager().m_registry;
       
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

        Rigidbody2DComponent* rb2d = scene->GetEntityManager().m_registry.try_get<Rigidbody2DComponent>(entity);

        if (rb2d != nullptr)
        {
            if (ImGui::CollapsingHeader("Rigidbody 2D", ImGuiTreeNodeFlags_DefaultOpen))
            {
                const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
                const char* currentBodyTypeString = bodyTypeStrings[(int)rb2d->type];
                if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
                {
                    for (int i = 0; i < 2; i++)
                    {
                        bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
                        if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
                        {
                            currentBodyTypeString = bodyTypeStrings[i];
                            rb2d->type = (BodyType)i;
                        }

                        if (isSelected)
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }

                ImGui::Checkbox("Fixed Rotation", &rb2d->fixedRotation);
            }
        }

        BoxCollider2DComponent* boxCollider2D = scene->GetEntityManager().m_registry.try_get<BoxCollider2DComponent>(entity);

        if (boxCollider2D != nullptr)
        {
            if (ImGui::CollapsingHeader("Box Collider 2D", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::DragFloat2("Offset", glm::value_ptr(boxCollider2D->offset));
                ImGui::DragFloat2("Size", glm::value_ptr(boxCollider2D->size));
            }
        }

        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("add_component");

        ImGui::SameLine();

        if (ImGui::BeginPopup("add_component"))
        {
            ImGui::Text("Component");
            ImGui::Separator();
            for (int i = 0; i < IM_ARRAYSIZE(EditorComponentNames); i++)
            {
                if (ImGui::Selectable(EditorComponentNames[i].c_str()))
                {
                    if (EditorComponentNames[i] == "Rigidbody 2D")
                    {
                        if (registry.try_get<Rigidbody2DComponent>(entity) == nullptr)
                            registry.emplace<Rigidbody2DComponent>(entity);
                    }
                    else if (EditorComponentNames[i] == "Box Collider 2D")
                    {
                        if (registry.try_get<BoxCollider2DComponent>(entity) == nullptr)
                            registry.emplace<BoxCollider2DComponent>(entity);
                    }
                }
            }
            ImGui::EndPopup();
        }

        ImGui::PopID();
    }

    ImGui::End();
}