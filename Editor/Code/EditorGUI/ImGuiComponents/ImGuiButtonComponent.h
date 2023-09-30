#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include "Components/UIComponents.h"

static void RenderImGui(ButtonComponent& button)
{
	ImGui::InputText("Text", &button.text);
}