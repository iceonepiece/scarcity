#include <entt/entt.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ImGuiEntityProperties.h"
#include "Components/Components.h"
#include "../Editor2D.h"
#include "imgui/imgui_stdlib.h"
#include <string.h>

std::string EditorComponentNames[] = {
    "Box Collider 2D",
    "Camera",
    "Circle Collider 2D",
    "Rigidbody 2D",
    "Sprite Renderer"
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

template<typename T, typename UIFunction>
static void RenderComponent(const std::string& name, entt::registry& registry, entt::entity entity, UIFunction uiFunction)
{
    const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

    if (registry.all_of<T>(entity))
    {
        auto& component = registry.get<T>(entity);

        bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());

        ImGui::SameLine();
        if (ImGui::Button("+"))
        {
            ImGui::OpenPopup("ComponentSettings");
        }

        bool removeComponent = false;
        if (ImGui::BeginPopup("ComponentSettings"))
        {
            if (ImGui::MenuItem("Remove component"))
                removeComponent = true;

            ImGui::EndPopup();
        }

        if (open)
        {
            uiFunction(component);
            ImGui::TreePop();
        }

        if (removeComponent)
            registry.remove<T>(entity);
    }
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

        RenderComponent<SpriteRendererComponent>("Sprite Renderer", registry, entity, [](auto& component)
        {
            const char* shapeTypeStrings[] = { "None", "Square", "Circle" };
            const char* currentShapeTypeString = shapeTypeStrings[(int)component.shape];
            if (ImGui::BeginCombo("Shape Type", currentShapeTypeString))
            {
                for (int i = 0; i < 3; i++)
                {
                    bool isSelected = currentShapeTypeString == shapeTypeStrings[i];
                    if (ImGui::Selectable(shapeTypeStrings[i], isSelected))
                    {
                        currentShapeTypeString = shapeTypeStrings[i];
                        component.shape = (SpriteShape)i;
                    }

                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }
        });

        RenderComponent<CameraComponent>("Camera", registry, entity, [](auto& component)
        {
            ImGui::InputFloat("Size", &component.size);
            ImGui::InputFloat("Near", &component.near);
            ImGui::InputFloat("Far", &component.far);
        });

        RenderComponent<Rigidbody2DComponent>("Rigidbody 2D", registry, entity, [](auto& component)
        {
            const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
            const char* currentBodyTypeString = bodyTypeStrings[(int)component.type];
            if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
            {
                for (int i = 0; i < 2; i++)
                {
                    bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
                    if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
                    {
                        currentBodyTypeString = bodyTypeStrings[i];
                        component.type = (BodyType)i;
                    }

                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }

            ImGui::Checkbox("Fixed Rotation", &component.fixedRotation);
        });

        RenderComponent<BoxCollider2DComponent>("Box Collider 2D", registry, entity, [](auto& component)
        {
            ImGui::DragFloat2("Offset", glm::value_ptr(component.offset));
            ImGui::DragFloat2("Size", glm::value_ptr(component.size));
        });

        RenderComponent<CircleCollider2DComponent>("Circle Collider 2D", registry, entity, [](auto& component)
        {
            ImGui::DragFloat2("Offset", glm::value_ptr(component.offset));
            ImGui::InputFloat("Radius", &component.radius);
        });

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
                    else if (EditorComponentNames[i] == "Sprite Renderer")
                    {
                        if (registry.try_get<SpriteRendererComponent>(entity) == nullptr)
                            registry.emplace<SpriteRendererComponent>(entity);
                    }
                }
            }
            ImGui::EndPopup();
        }

        ImGui::PopID();
    }

    ImGui::End();
}