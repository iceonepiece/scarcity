#pragma once

#include "Components/Rigidbody2DComponent.h"
#include "../ImGuiUtils.h"

static void RenderImGui(Rigidbody2DComponent& rb2d)
{
    const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
    const char* currentBodyTypeString = bodyTypeStrings[(int)rb2d.type];
    if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
    {
        for (int i = 0; i < 2; i++)
        {
            bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
            if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
            {
                currentBodyTypeString = bodyTypeStrings[i];
                rb2d.type = (BodyType)i;
            }

            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }

    ImGui::Checkbox("Fixed Rotation", &rb2d.fixedRotation);
    ImGui::InputFloat("Gravity Scale", &rb2d.gravityScale);
}