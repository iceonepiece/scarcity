#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include "Components/UIComponents.h"

static void RenderImGui(ButtonComponent& button)
{
	float color[4] = { button.color.r, button.color.g, button.color.b, button.color.a };
	ImGui::InputText("Text", &button.text);

	if (ImGui::ColorEdit4("Color", color))
	{
		button.color = { color[0], color[1], color[2], color[3] };
		button.instance.SetBackgroundColor(button.color);
	}

	std::string entityName = "";
	if (button.targetEntity.IsValid())
		entityName = button.targetEntity.GetName();

	ImGui::Button(entityName.c_str(), ImVec2(200, 0));

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY"))
		{
			Entity payloadEntity = *(Entity*)payload->Data;
			button.targetEntity = payloadEntity;
			button.targetID = payloadEntity.GetID();

			std::cout << "Drag Drop: " << payloadEntity.GetID() << std::endl;
		}
		ImGui::EndDragDropTarget();
	}
}