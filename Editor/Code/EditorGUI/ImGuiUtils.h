#pragma once

#include <string>
#include <imgui/imgui.h>
#include <glm/glm.hpp>

class ImGuiUtils
{
public:
    static void RenderInputVec3(const std::string& name, glm::vec3& values, int width = 100)
    {
        std::string xValue = "##X" + name;
        std::string yValue = "##Y" + name;
        std::string zValue = "##Z" + name;

        ImGui::PushItemWidth(width);

        ImGui::Text(name.c_str()); ImGui::SameLine();

        ImGui::Text("X"); ImGui::SameLine();
        ImGui::InputFloat(xValue.c_str(), &values.x); ImGui::SameLine();

        ImGui::Text("Y"); ImGui::SameLine();
        ImGui::InputFloat(yValue.c_str(), &values.y); ImGui::SameLine();

        ImGui::Text("Z"); ImGui::SameLine();
        ImGui::InputFloat(zValue.c_str(), &values.z);

        ImGui::PopItemWidth();
    }
};