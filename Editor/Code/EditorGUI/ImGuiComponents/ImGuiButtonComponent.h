#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include "Components/UIComponents.h"

static void RenderImGui(ButtonComponent& button)
{
	float color[4] = { button.color.r, button.color.g, button.color.b, button.color.a };
	ImGui::InputText("Text", &button.text);
	ImGui::ColorEdit4("Color", color);

	button.color = { color[0], color[1], color[2], color[3] };
}