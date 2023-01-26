#include "../GUIs/GUIWindow.h"
#include "../Core/Scene.h"

class EntitiesWindow : public GUIWindow
{
public:
    EntitiesWindow(GUI* gui, Scene* scene)
        : GUIWindow(gui, "Entities")
        , m_scene(scene)
    {

    }

    virtual void Draw() override
    {
        ImGui::Begin(m_name.c_str());
        EntityManager& manager = m_scene->GetEntityManager();

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

private:
    Scene* m_scene;
    entt::entity m_selectedEntity;
    int m_selected = -1;

    friend class InspectorWindow;
};