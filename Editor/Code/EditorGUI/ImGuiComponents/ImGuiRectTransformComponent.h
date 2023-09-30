#pragma once

#include "../ImGuiUtils.h"
#include "Components/UIComponents.h"

static void RenderImGui(CanvasComponent& canvas)
{    
    ImGui::PushItemWidth(120);

    ImGui::Text("X"); ImGui::SameLine();
    ImGui::InputFloat("##X", &canvas.x); ImGui::SameLine();
    ImGui::Text("Y"); ImGui::SameLine();
    ImGui::InputFloat("##Y", &canvas.y);

    ImGui::Text("Width"); ImGui::SameLine();
    ImGui::InputFloat("##Width", &canvas.width); ImGui::SameLine();
    ImGui::Text("Height"); ImGui::SameLine();
    ImGui::InputFloat("##Height", &canvas.height);

    ImGui::PopItemWidth();
}