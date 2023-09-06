#pragma once

#include <entt/entt.hpp>
#include <imgui/imgui.h>
#include "File/FileSystem.h"
#include "ImGuiAssetProperties.h"

class EditorLayer;

class ImGuiEntityProperties
{
public:
	ImGuiEntityProperties(EditorLayer& editor);
	void Render();

    template<typename T>
    void RenderComponent(const std::string& name, entt::registry& registry, entt::entity entity)
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
                RenderImGui(component);
                ImGui::TreePop();
            }

            if (removeComponent)
                registry.remove<T>(entity);
        }
    }

private:
	EditorLayer& m_editor;
	ImGuiAssetProperties m_imguiAssetProperties;
};