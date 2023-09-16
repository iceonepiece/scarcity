#pragma once

#include <imgui/imgui.h>
#include "imgui/imgui_stdlib.h"
#include "Components/BaseComponent.h"

static void RenderImGui(BaseComponent& base)
{
	ImGui::Text("Name: "); ImGui::SameLine();
	ImGui::InputText("##name", &(base.name));
}