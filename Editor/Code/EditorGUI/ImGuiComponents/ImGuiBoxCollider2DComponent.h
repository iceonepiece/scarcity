#pragma once

#include <imgui/imgui.h>
#include "Components/BoxCollider2DComponent.h"

static void RenderImGui(BoxCollider2DComponent& box)
{
	auto& allLayers = Physics::GetLayers();
	std::vector<uint16_t> availableLayers;

	for (int i = 0; i < allLayers.size(); i++)
	{
		if (allLayers[i].size() > 0)
			availableLayers.push_back(i);
	}

	ImGui::PushItemWidth(150);
	if (ImGui::BeginCombo("Layer: ", allLayers[box.layer].c_str()))
	{
		for (int i = 0; i < availableLayers.size(); i++)
		{
			bool isSelected = box.layer == availableLayers[i];
			if (ImGui::Selectable(allLayers[availableLayers[i]].c_str(), isSelected))
			{
				box.layer = availableLayers[i];
			}

			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	ImGui::SameLine();
	ImGui::PopItemWidth();

	ImGui::Checkbox("Is Trigger", &box.isTrigger);
	ImGui::DragFloat2("Offset", glm::value_ptr(box.offset));
	ImGui::DragFloat2("Size", glm::value_ptr(box.size));
}