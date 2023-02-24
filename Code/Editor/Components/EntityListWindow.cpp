#include "EntityListWindow.h"
#include "../ImGuiLayer.h"

EntityListWindow::EntityListWindow()
    : ImGuiWindow("EntityList")
    , m_selectedEntity{ entt::null }
{

}

void EntityListWindow::Draw()
{
    ImGui::Begin(m_name.c_str());
    EntityManager& manager = m_gui->GetEntityManager();

    auto view = manager.m_registry.view<Collider2DComponent>();

    int counter = 0;

    for (auto [entity, collider] : view.each())
    {
        std::string x = "Platform: " + std::to_string(counter);

        if (ImGui::Selectable(x.c_str(), m_selected == counter))
        {
            m_selected = counter;
            m_selectedEntity = entity;
        }
        counter++;
    }

    ImGui::End();
}