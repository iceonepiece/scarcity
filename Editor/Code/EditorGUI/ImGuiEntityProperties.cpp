#include <glm/gtc/type_ptr.hpp>
#include "ImGuiEntityProperties.h"
#include "Components/Components.h"
#include "ImGuiComponents/ImGuiComponents.h"
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

        RenderComponent<SpriteRendererComponent>("Sprite Renderer", registry, entity);
        RenderComponent<CameraComponent>("Camera", registry, entity);
        RenderComponent<Rigidbody2DComponent>("Rigidbody 2D", registry, entity);
        RenderComponent<BoxCollider2DComponent>("Box Collider 2D", registry, entity);
        RenderComponent<CircleCollider2DComponent>("Circle Collider 2D", registry, entity);
        RenderComponent<NativeScriptComponent>("Native Script", registry, entity);
        RenderComponent<MockComponent>("Mock", registry, entity);
        RenderComponent<SpriteAnimatorComponent>("Sprite Animator", registry, entity);

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
                    else if (EditorComponentNames[i] == "Circle Collider 2D")
                    {
                        if (registry.try_get<CircleCollider2DComponent>(entity) == nullptr)
                            registry.emplace<CircleCollider2DComponent>(entity);
                    }
                    else if (EditorComponentNames[i] == "Sprite Animator")
                    {
                        if (registry.try_get<SpriteAnimatorComponent>(entity) == nullptr)
                            registry.emplace<SpriteAnimatorComponent>(entity);
                    }
                    else if (EditorComponentNames[i] == "Sprite Renderer")
                    {
                        if (registry.try_get<SpriteRendererComponent>(entity) == nullptr)
                            registry.emplace<SpriteRendererComponent>(entity);
                    }
                    else if (EditorComponentNames[i] == "Native Script")
                    {
                        if (registry.try_get<NativeScriptComponent>(entity) == nullptr)
                            registry.emplace<NativeScriptComponent>(entity);
                    }
                    else if (EditorComponentNames[i] == "Mock")
                    {
                        if (registry.try_get<MockComponent>(entity) == nullptr)
                            registry.emplace<MockComponent>(entity);
                    }

                }
            }
            ImGui::EndPopup();
        }

        ImGui::PopID();
    }

    ImGui::End();
}