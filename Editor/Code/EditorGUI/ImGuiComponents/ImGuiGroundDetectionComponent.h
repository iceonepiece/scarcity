#pragma once

#include "Components/GroundDetectionComponent.h"

static void RenderImGui(GroundDetectionComponent& groundDetection)
{
	ImGui::InputInt("Ground Tag", &groundDetection.groundTag);
}