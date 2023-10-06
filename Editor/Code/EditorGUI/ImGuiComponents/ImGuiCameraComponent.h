#pragma once

#include <imgui/imgui.h>
#include "Components/CameraComponent.h"
#include "Core/Application.h"

static void RenderImGui(CameraComponent& camera)
{
	ImGui::InputFloat("Size", &camera.size);
	ImGui::InputFloat("Near", &camera.near);
	ImGui::InputFloat("Far", &camera.far);

	ImGui::Text("Target Entity: ");
	ImGui::SameLine();

	std::string entityName = "";
	if (camera.targetEntity.IsValid())
		entityName = camera.targetEntity.GetName();

	ImGui::Button(entityName.c_str(), ImVec2(200, 0));

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY"))
		{
			Entity payloadEntity = *(Entity*)payload->Data;
			camera.targetEntity = payloadEntity;
			camera.targetID = payloadEntity.GetID();

			std::cout << "Drag Drop: " << payloadEntity.GetID() << std::endl;
		}
		ImGui::EndDragDropTarget();
	}
}