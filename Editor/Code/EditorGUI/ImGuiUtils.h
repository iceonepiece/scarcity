#pragma once

#include <vector>
#include <string>
#include <imgui/imgui.h>
#include <glm/glm.hpp>

class ImGuiUtils
{
public:
    static void RenderTags(const std::vector<std::string>& tags, std::string& str);
    static void RenderInputVec3(const std::string& name, glm::vec3& values, float width = 100.0f);
    static void RenderDropdownList(const std::string& label, const std::vector<std::string>& keys, size_t& selectedIndex);  
};