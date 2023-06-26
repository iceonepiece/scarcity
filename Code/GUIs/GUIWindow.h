#pragma once

#include <string>
#include "GUIComponent.h"

class GUIWindow : public GUIComponent
{
public:
    GUIWindow(GUI* gui, std::string name)
        : GUIComponent(gui)
        , m_name(name)
    {
    }

    virtual void Draw()
    {
        ImGui::Begin(m_name.c_str());

        for (auto child : m_children)
        {
            child->Draw();
        }

        ImGui::End();
    }

protected:
    std::string m_name;
};
