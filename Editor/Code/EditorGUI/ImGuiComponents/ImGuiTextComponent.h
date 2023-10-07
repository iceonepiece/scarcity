#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include "Components/UIComponents.h"

static void RenderImGui(TextComponent& text)
{
	float color[4] = { text.color.r, text.color.g, text.color.b, text.color.a };
	ImGui::InputText("Text", &text.text);
	ImGui::InputFloat("Size", &text.size);
	ImGui::ColorEdit4("Color", color);

	text.color = { color[0], color[1], color[2], color[3] };
}