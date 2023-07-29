#include "../Editor2D.h"
#include "ImGuiHierarchy.h"
#include "Components/Components.h"

ImGuiHierarchy::ImGuiHierarchy(Editor2D& editor)
    : m_editor(editor)
{

}

void ImGuiHierarchy::Render()
{
    ImGui::Begin("Hierarchy");
    EntityManager& manager = m_editor.GetScene()->GetEntityManager();
    auto& registry = manager.m_registry;

    registry.each([&](entt::entity entity) {
        BaseComponent& base = registry.get<BaseComponent>(entity);

        std::string name = base.name + "###ID" + std::to_string((int)entity);
        if (ImGui::Selectable(name.c_str(), m_editor.GetPickedEntity() == entity))
        {
            m_editor.SetPickedEntity(entity);
        }
    });

    if (ImGui::BeginPopupContextWindow(0, 1))
    {
        if (ImGui::MenuItem("Create Empty Entity"))
        {
            auto entity = manager.CreateEntity();
            entity.AddComponent<BaseComponent>("Untitled");
            entity.AddComponent<TransformComponent>();
        }

        ImGui::EndPopup();
    }
  
    ImGui::End();
}