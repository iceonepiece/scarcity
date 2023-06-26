#pragma once

#include "ImGuiWindow.h"
#include "Core/Scene.h"
#include "Components/PlayerComponent.h"
#include "Components/AIComponent.h"

class EntityListWindow;

class InspectorWindow : public ImGuiWindow
{
public:
    InspectorWindow(EntityListWindow* entityListWindow);
    virtual void Draw() override;

private:
    Scene* m_scene;
    EntityListWindow* m_entityListWindow;
};