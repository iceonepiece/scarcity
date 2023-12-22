#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <IconsFontAwesome6.h>
#include "ImGuiCircleCollider2DComponent.h"
#include "ImGuiBoxCollider2DComponent.h"
#include "Components/Collider2DGroupComponent.h"
#include "Components/BoxCollider2DComponent.h"
#include "Components/CircleCollider2DComponent.h"
#include "../ImGuiUtils.h"

static void RenderImGui(Collider2DGroupComponent& group)
{
	if (ImGui::Button("Add" ICON_FA_CARET_DOWN))
	{
		ImGui::OpenPopup("Add Collider");
	}

	if (ImGui::BeginPopup("Add Collider"))
	{
		if (ImGui::Selectable("Box"))
			group.colliders.push_back({ ColliderType::Box });

		if (ImGui::Selectable("Circle"))
			group.colliders.push_back({ ColliderType::Circle });

		ImGui::EndPopup();
	}

	int deleteIndex = -1;

	auto& allLayers = Physics::GetLayers();
	std::vector<uint16_t> availableLayers;

	static std::vector<std::string> stringTypes{ "Box", "Circle" };

	for (int i = 0; i < allLayers.size(); i++)
	{
		if (allLayers[i].size() > 0)
			availableLayers.push_back(i);
	}

	for (int i = 0; i < group.colliders.size(); i++)
	{
		auto& collider = group.colliders[i];

		ImGui::PushID(i);

		ImGui::SeparatorText(std::to_string(i).c_str());

		if (ImGui::Button("Remove"))
			deleteIndex = i;

		ImGui::PushItemWidth(150);
		if (ImGui::BeginCombo("Layer: ", allLayers[collider.layer].c_str()))
		{
			for (int i = 0; i < availableLayers.size(); i++)
			{
				bool isSelected = collider.layer == availableLayers[i];
				if (ImGui::Selectable(allLayers[availableLayers[i]].c_str(), isSelected))
				{
					collider.layer = availableLayers[i];
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		ImGui::SameLine();

		ImGui::PopItemWidth();

		ImGui::Checkbox("Trigger", &collider.isTrigger);
		ImGui::InputFloat2("Offset", glm::value_ptr(collider.offset));

		if (collider.type == ColliderType::Box)
			ImGui::InputFloat2("Size", glm::value_ptr(collider.size));
		else if (collider.type == ColliderType::Circle)
			ImGui::InputFloat("Radius", &collider.size.x);

		ImGui::PopID();
	}

	if (deleteIndex >= 0)
	{
		group.colliders.erase(group.colliders.begin() + deleteIndex);
	}
}