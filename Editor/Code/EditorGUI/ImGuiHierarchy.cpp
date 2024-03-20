#include "../EditorLayer.h"
#include "ImGuiHierarchy.h"
#include "Components/Components.h"
#include "Components/Relationship.h"
#include <IconsFontAwesome6.h>

ImGuiHierarchy::ImGuiHierarchy(EditorLayer& editor)
    : m_editor(editor)
{

}

void ImGuiHierarchy::RenderEntity(EntityManager& manager, entt::entity entity)
{
    BaseComponent& base = manager.m_registry.get<BaseComponent>(entity);

    std::string useIcon = (ICON_FA_CUBE " ");

    ImGuiTreeNodeFlags flags = ((m_editor.GetPickedEntity() == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
    flags |= ImGuiTreeNodeFlags_SpanFullWidth;

    if (!manager.m_registry.try_get<ChildrenComponent>(entity))
        flags |= ImGuiTreeNodeFlags_Leaf;

    //ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(140, 200, 255, 255));
    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, (useIcon + base.name).c_str());
    //ImGui::PopStyleColor();

    if (ImGui::BeginDragDropSource())
    {
        Entity sourceEntity{ &manager, entity };
        ImGui::SetDragDropPayload("HIERARCHY_ENTITY", &sourceEntity, sizeof(Entity));
        ImGui::Text(base.name.c_str());
        ImGui::EndDragDropSource();
    }

    if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
    {
        if (IDComponent* id = manager.m_registry.try_get<IDComponent>(entity))
        {
            std::cout << "ID Component: " << id->ID << std::endl;
        }

        m_editor.SetPickedEntity(entity);
    }

    bool entityDeleted = false;
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::MenuItem("Create Empty Entity"))
        {
            manager.CreateEmptyEntity(entity);
        }

        if (ImGui::MenuItem("Delete Entity"))
            entityDeleted = true;

        ImGui::EndPopup();
    }

    if (opened)
    {
        if (ChildrenComponent* children = manager.m_registry.try_get<ChildrenComponent>(entity))
        {
            for (int i = 0; i < children->size; i++)
            {
                RenderEntity(manager, children->entities[i]);
            }
        }

        ImGui::TreePop();
    }


    if (entityDeleted)
        m_editor.DeleteEntity(entity);
}

void ImGuiHierarchy::Render()
{
    ImGui::Begin("Hierarchy");
    EntityManager& manager = m_editor.GetScene()->GetEntityManager();
    auto& registry = manager.m_registry;

    std::string sceneHeader = "Scene: " + m_editor.GetScene()->m_name;

    ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0.0f);
    bool opened = ImGui::TreeNodeEx(sceneHeader.c_str(), ImGuiTreeNodeFlags_DefaultOpen);
    ImGui::PopStyleVar();

    if (opened)
    {
        registry.each([&](entt::entity entity) {
            if (!registry.try_get<ParentComponent>(entity))
            {
                RenderEntity(manager, entity);
            }
        });

        if (ImGui::BeginPopupContextWindow("##popup_empty", ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight))
        {
            if (ImGui::MenuItem("Create Empty Entity"))
            {
                manager.CreateEmptyEntity();
            }

            ImGui::EndPopup();
        }

        ImGui::TreePop();
    }

    ImGui::End();
}