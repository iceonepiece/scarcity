#pragma once

#include "../ImGuiUtils.h"
#include "Components/UIComponents.h"

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
}