#include "../GUIs/GUIWindow.h"
#include "../Core/Scene.h"
#include "../Components/PlayerComponent.h"
#include "../Components/AIComponent.h"

std::string componentNames[] = { "Collider2D", "Player", "AI" };

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
            ImGui::NewLine();

            Collider2DComponent* a = manager.m_registry.try_get<Collider2DComponent>(m_entitiesWindow->m_selectedEntity);

            if (a != nullptr)
            {
                if (ImGui::CollapsingHeader("Collider2D", ImGuiTreeNodeFlags_None))
                {
                    const b2Vec2& position = a->body->GetPosition();
                    b2Vec2 newPosition{ position.x, position.y };

                    ImGui::Text("X");
                    ImGui::SameLine();
                    ImGui::InputFloat("##x", &newPosition.x);

                    ImGui::Text("Y");
                    ImGui::SameLine();
                    ImGui::InputFloat("##y", &newPosition.y);

                    a->body->SetTransform(newPosition, a->body->GetAngle());
                }
            }

            ImGui::NewLine();

            PlayerComponent* b = manager.m_registry.try_get<PlayerComponent>(m_entitiesWindow->m_selectedEntity);
            if (b != nullptr)
            {
                if (ImGui::CollapsingHeader("Player", ImGuiTreeNodeFlags_None))
                {
                    ImGui::Text("PLAYER_MOVE_SPEED");
                    ImGui::SameLine();
                    ImGui::InputFloat("##PLAYER_MOVE_SPEED", &b->PLAYER_MOVE_SPEED);

                    ImGui::Text("PLAYER_JUMP_FORCE");
                    ImGui::SameLine();
                    ImGui::InputFloat("##PLAYER_JUMP_FORCE", &b->PLAYER_JUMP_FORCE);
                }
            }

            if (ImGui::Button("Add Component"))
                ImGui::OpenPopup("add_component");

            ImGui::SameLine();

            if (ImGui::BeginPopup("add_component"))
            {
                ImGui::Text("Component");
                ImGui::Separator();
                for (int i = 0; i < IM_ARRAYSIZE(componentNames); i++)
                {
                    if (ImGui::Selectable(componentNames[i].c_str()))
                    {
                        std::cout << "Selected: " << i << std::endl;

                        if (componentNames[i] == "Player")
                        {
                            manager.m_registry.emplace<PlayerComponent>(m_entitiesWindow->m_selectedEntity);
                        }
                        else if (componentNames[i] == "AI")
                        {
                            //manager.m_registry.emplace<AIComponent>(m_entitiesWindow->m_selectedEntity);
                        }
                    }
                }
                ImGui::EndPopup();
            }
        }
        
        ImGui::End();
    }

private:

    Scene* m_scene;
    EntitiesWindow* m_entitiesWindow;
};