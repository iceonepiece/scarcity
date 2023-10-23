#pragma once

#include "../ImGuiUtils.h"
#include "Components/UIComponents.h"

static std::vector<std::string> verticalAlignments { "Top", "Middle", "Bottom" };
static std::vector<std::string> horizontalAlignments { "Left", "Center", "Right" };


static void RenderImGui(CanvasComponent& canvas)
{    
    ImGui::PushItemWidth(120);

    ImGui::Text("X"); ImGui::SameLine();
    ImGui::InputFloat("##X", &canvas.position.x); ImGui::SameLine();
    ImGui::Text("Y"); ImGui::SameLine();
    ImGui::InputFloat("##Y", &canvas.position.y);

    ImGui::Text("Width"); ImGui::SameLine();
    ImGui::InputFloat("##Width", &canvas.size.x); ImGui::SameLine();
    ImGui::Text("Height"); ImGui::SameLine();
    ImGui::InputFloat("##Height", &canvas.size.y);

    ImGui::PopItemWidth();

    ImGui::Text("Alignment:");
    ImGui::SameLine();

    ImGui::PushItemWidth(100);

    unsigned int verticalIndex = canvas.verticalAlignment;
    ImGuiUtils::RenderDropdownList("##Vertical", verticalAlignments, verticalIndex);
    canvas.verticalAlignment = (VerticalAlignment)verticalIndex;

    ImGui::SameLine();

    unsigned int horizontalIndex = canvas.horizontalAligment;
    ImGuiUtils::RenderDropdownList("##Horizontal", horizontalAlignments, horizontalIndex);
    canvas.horizontalAligment = (HorizontalAlignment)horizontalIndex;

    ImGui::PopItemWidth();
}