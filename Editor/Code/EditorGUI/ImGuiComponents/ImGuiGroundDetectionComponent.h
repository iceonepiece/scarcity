#pragma once

#include "Components/GroundDetectionComponent.h"

static void RenderImGui(GroundDetectionComponent& groundDetection)
{
	auto& tags = Application::Get().GetTags();

	ImGui::Text("Ground Tag: "); ImGui::SameLine();

	if (ImGui::BeginCombo("##Tag", groundDetection.groundTag > 0 ? tags[groundDetection.groundTag - 1].c_str() : "Untagged"))
	{
		for (int i = 0; i <= TAG_SIZE; i++)
		{
			bool isSelected = groundDetection.groundTag == i;

			ImGui::PushID(("TAG_" + std::to_string(i)).c_str());
			if (i == 0)
			{
				if (ImGui::Selectable("Untagged", isSelected))
					groundDetection.groundTag = i;

				ImGui::Separator();
			}
			else
			{
				if (ImGui::Selectable(tags[i - 1].c_str(), isSelected))
					groundDetection.groundTag = i;
			}

			if (isSelected)
				ImGui::SetItemDefaultFocus();

			ImGui::PopID();
		}

		ImGui::EndCombo();
	}
}