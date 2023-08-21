#include "../EditorLayer.h"
#include "ImGuiHierarchy.h"
#include "Components/Components.h"
#include "Components/Relationship.h"

ImGuiHierarchy::ImGuiHierarchy(EditorLayer& editor)
    : m_editor(editor)
{

}

void ImGuiHierarchy::Render()
{
    ImGui::Begin("Hierarchy");
    EntityManager& manager = m_editor.GetScene()->GetEntityManager();
    auto& registry = manager.m_registry;

    std::string sceneHeader = "Scene: " + m_editor.GetScene()->m_name;

    if (ImGui::TreeNode(sceneHeader.c_str()))
    {
        registry.each([&](entt::entity entity) {
            BaseComponent& base = registry.get<BaseComponent>(entity);

            std::string name = base.name + "###ID" + std::to_string((int)entity);

            ImGuiTreeNodeFlags flags = ((m_editor.GetPickedEntity() == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
            flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
            bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, base.name.c_str());

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