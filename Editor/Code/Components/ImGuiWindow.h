#pragma once

#include <string>
#include "../ImGuiComponent.h"

class ImGuiWindow : public ImGuiComponent
{
public:
    ImGuiWindow(std::string name)
        : m_name(name)
    {
    }

    virtual void Draw()
    {
        ImGui::Begin(m_name.c_str());

        for (auto child : m_children)
            child->Draw();

        ImGui::End();
    }

protected:
    std::string m_name;
};
