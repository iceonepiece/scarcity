#pragma once

#include "../ImGuiUtils.h"
#include "Components/TransformComponent.h"

static void RenderImGui(TransformComponent& transform)
{
    ImGuiUtils::RenderInputVec3("Position", transform.position, 90);
    ImGuiUtils::RenderInputVec3("Rotation", transform.rotation, 90);
    ImGuiUtils::RenderInputVec3("Scale", transform.scale, 90);
}