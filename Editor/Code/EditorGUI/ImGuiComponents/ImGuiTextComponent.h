#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include "Components/UIComponents.h"

static void RenderImGui(TextComponent& text)
{
	ImGui::InputText("Text", &text.text);
	ImGui::InputFloat("Size", &text.size);
}