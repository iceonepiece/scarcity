#pragma once

#include <imgui/imgui.h>
#include "Components/BoxCollider2DComponent.h"

static void RenderImGui(BoxCollider2DComponent& box)
{
	ImGui::DragFloat2("Offset", glm::value_ptr(box.offset));
	ImGui::DragFloat2("Size", glm::value_ptr(box.size));
}