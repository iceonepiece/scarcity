#include "../EditorLayer.h"
#include "ImGuiHierarchy.h"
#include "Components/Components.h"
#include "Components/Relationship.h"
#include <IconsFontAwesome6.h>



ImGuiHierarchy::ImGuiHierarchy(EditorLayer& editor)
    : m_editor(editor)
{

}

void ImGuiHierarchy::Render()
{
    std::string useIcon = (ICON_FA_CUBE " ");

    ImGui::Begin("Hierarchy");
    EntityManager& manager = m_editor.GetScene()->GetEntityManager();
    auto& registry = manager.m_registry;

    std::string sceneHeader = "Scene: " + m_editor.GetScene()->m_name;

    if (ImGui::TreeNodeEx(sceneHeader.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
    {
        registry.each([&](entt::entity entity) {
            BaseComponent& base = registry.get<BaseComponent>(entity);

            ImGuiTreeNodeFlags flags = ((m_editor.GetPickedEntity() == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
            flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
            flags |= ImGuiTreeNodeFlags_SpanFullWidth;

            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(140, 200, 255, 255));
            bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, (useIcon + base.name).c_str());
            ImGui::PopStyleColor();

            if (ImGui::BeginDragDropSource())
            {
                ImGui::SetDragDropPayload("HIERARCHY_ENTITY", &entity, sizeof(entt::entity));
                ImGui::Text(base.name.c_str());
                ImGui::EndDragDropSource();
            }

            if (ImGui::IsItemClicked())
            {
                m_editor.SetPickedEntity(entity);
            }

            bool entityDeleted = false;
            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::MenuItem("Delete Entity"))
                    entityDeleted = true;

                ImGui::EndPopup();
            }

            if (opened)
            {
                ImGui::TreePop();
            }

            if (entityDeleted)
                m_editor.DeleteEntity(entity);
        });

        if (ImGui::BeginPopupContextWindow(NULL, ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight))
        {
            if (ImGui::MenuItem("Create Empty Entity"))
            {
                auto entity = manager.CreateEntity();
                entity.AddComponent<Relationship>();
                entity.AddComponent<BaseComponent>("Untitled");
                entity.AddComponent<TransformComponent>();
            }

            ImGui::EndPopup();
        }

        ImGui::TreePop();
    }

    ImGui::End();
}