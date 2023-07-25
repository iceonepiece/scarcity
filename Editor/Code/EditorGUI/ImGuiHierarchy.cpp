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
    auto& registry = m_editor.GetScene()->GetEntityManager().m_registry;


    registry.each([&](entt::entity entity) {
        BaseComponent& base = registry.get<BaseComponent>(entity);

        if (ImGui::Selectable(base.name.c_str(), m_editor.GetPickedEntity() == entity))
        {
            m_editor.SetPickedEntity(entity);
        }
    });
  
    ImGui::End();
}