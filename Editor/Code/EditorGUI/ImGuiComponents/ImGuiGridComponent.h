#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include "Components/GridComponent.h"

static void RenderImGui(GridComponent& grid)
{
	ImGui::InputFloat("Size", &grid.size);

	float color[4] = { grid.color.r, grid.color.g, grid.color.b, grid.color.a };

	if (ImGui::ColorEdit4("Color", color))
	{
		grid.color = { color[0], color[1], color[2], color[3] };
	}
}