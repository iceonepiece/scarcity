#pragma once

#include <string>
#include <entt/entt.hpp>
#include "ImGuiBaseComponent.h"
#include "ImGuiBoxCollider2DComponent.h"
#include "ImGuiSpriteRendererComponent.h"
#include "ImGuiSpriteAnimatorComponent.h"
#include "ImGuiTransformComponent.h"
#include "ImGuiNativeScriptComponent.h"
#include "ImGuiGroundDetectionComponent.h"
#include "ImGuiCircleCollider2DComponent.h"

class ImGuiComponentRenderer
{
public:
    template<typename T>
    static void RenderComponent(entt::registry& registry, entt::entity entity)
    {
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

        if (registry.all_of<T>(entity))
        {
            auto& component = registry.get<T>(entity);

            bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, T::Name().c_str());

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
                RenderImGui(component);
                ImGui::TreePop();
            }

            if (removeComponent)
                registry.remove<T>(entity);
        }
    }

};