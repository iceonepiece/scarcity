#pragma once

#include "../ImGuiUtils.h"
#include "Components/TransformComponent.h"

void RenderImGui(TransformComponent& transform)
{
    ImGuiUtils::RenderInputVec3("Position", transform.position);
    ImGuiUtils::RenderInputVec3("Rotation", transform.rotation);
    ImGuiUtils::RenderInputVec3("Scale", transform.scale);
}