#include "../GUIs/GUIWindow.h"
#include "../Core/Scene.h"

class InspectorWindow : public GUIWindow
{
public:
    InspectorWindow(GUI* gui, Scene* scene, EntitiesWindow* entitiesWindow)
        : GUIWindow(gui, "Inspector")
        , m_scene(scene)
        , m_entitiesWindow(entitiesWindow)
    {

    }

    virtual void Draw() override
    {
        ImGui::Begin(m_name.c_str());
        
        EntityManager& manager = m_scene->GetEntityManager();

        if (m_entitiesWindow->m_selected != -1)
        {
            Collider2DComponent* a = manager.m_registry.try_get<Collider2DComponent>(m_entitiesWindow->m_selectedEntity);

            if (a != nullptr)
            {
                const b2Vec2& position = a->body->GetPosition();
                b2Vec2 newPosition{ position.x, position.y };
                ImGui::InputFloat("X", &newPosition.x);
                ImGui::InputFloat("Y", &newPosition.y);

                a->body->SetTransform(newPosition, a->body->GetAngle());
            }
        }
        
        
        ImGui::End();
    }

private:
    Scene* m_scene;
    EntitiesWindow* m_entitiesWindow;
};