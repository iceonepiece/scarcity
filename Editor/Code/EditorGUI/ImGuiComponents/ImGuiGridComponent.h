#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include "Components/GridComponent.h"

static void RenderImGui(GridComponent& grid)
{
	ImGui::InputFloat("Size", &grid.size);
}