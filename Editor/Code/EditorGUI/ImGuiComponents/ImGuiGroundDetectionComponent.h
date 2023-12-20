#pragma once

#include "Components/GroundDetectionComponent.h"
#include "../ImGuiUtils.h"
#include "../../EditorLayer.h"

static void RenderImGui(GroundDetectionComponent& groundDetection)
{
	ImGui::Text("Ground Tag: "); ImGui::SameLine();

	auto& tagManager = EditorLayer::GetProject()->GetTagManager();

	if (ImGui::BeginCombo("##Tag", groundDetection.groundTag.c_str()))
	{
		ImGuiUtils::RenderTags(tagManager.GetDefaultTags(), groundDetection.groundTag);
		ImGuiUtils::RenderTags(tagManager.GetTags(), groundDetection.groundTag);

		ImGui::EndCombo();
	}
}