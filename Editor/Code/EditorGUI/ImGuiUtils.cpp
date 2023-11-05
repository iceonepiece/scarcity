#include "ImGuiUtils.h"

void ImGuiUtils::RenderTags(const std::vector<std::string>& tags, std::string& str)
{
    for (int i = 0; i < tags.size(); i++)
    {
        bool isSelected = str == tags[i];

        ImGui::PushID(tags[i].c_str());

        if (ImGui::Selectable(tags[i].c_str(), isSelected))
            str = tags[i];

        if (isSelected)
            ImGui::SetItemDefaultFocus();

        ImGui::PopID();
    }
}

void ImGuiUtils::RenderInputVec3(const std::string& name, glm::vec3& values, float width)
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

void ImGuiUtils::RenderDropdownList(const std::string& label, const std::vector<std::string>& keys, unsigned int& selectedIndex)
{
    if (keys.size() == 0)
        return;

    if (ImGui::BeginCombo(label.c_str(), keys[selectedIndex].c_str()))
    {
        for (int i = 0; i < keys.size(); i++)
        {
            bool isSelected = selectedIndex == i;

            if (ImGui::Selectable(keys[i].c_str(), isSelected))
                selectedIndex = i;

            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }
}