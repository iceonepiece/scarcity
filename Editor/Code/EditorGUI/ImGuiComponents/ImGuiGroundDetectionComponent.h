#pragma once

#include "Components/GroundDetectionComponent.h"
#include "../ImGuiUtils.h"

static void RenderImGui(GroundDetectionComponent& groundDetection)
{
	auto& tags = Application::Get().GetTagManager().GetTags();

	ImGui::Text("Ground Tag: "); ImGui::SameLine();

	auto& tagManager = Application::Get().GetTagManager();

	if (ImGui::BeginCombo("##Tag", groundDetection.groundTag.c_str()))
	{
		ImGuiUtils::RenderTags(tagManager.GetDefaultTags(), groundDetection.groundTag);
		ImGuiUtils::RenderTags(tagManager.GetTags(), groundDetection.groundTag);

		ImGui::EndCombo();
	}
}