#pragma once

#include <imgui/imgui.h>
#include "Components/CircleCollider2DComponent.h"

static void RenderImGui(CircleCollider2DComponent& circle)
{
    ImGui::Checkbox("Is Trigger", &circle.isTrigger);
    ImGui::DragFloat2("Offset", glm::value_ptr(circle.offset));
    ImGui::InputFloat("Radius", &circle.radius);
}