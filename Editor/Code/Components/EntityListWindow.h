#pragma once

#include "ImGuiWindow.h"
#include "Scene/Scene.h"
#include "Components/Collider2DComponent.h"
#include "InspectorWindow.h"

class EntityListWindow : public ImGuiWindow
{
public:
    EntityListWindow();
    virtual void Draw() override;

private:
    entt::entity m_selectedEntity;
    int m_selected = -1;

    friend class InspectorWindow;
};