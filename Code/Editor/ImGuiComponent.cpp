#include "ImGuiComponent.h"
#include "ImGuiLayer.h"
#include <iostream>

ImGuiComponent::ImGuiComponent(ImGuiLayer* gui)
    : m_gui(gui)
{
}

ImGuiComponent::~ImGuiComponent()
{
    std::cout << "ImGuiComponent Destructor" << std::endl;
    for (auto child : m_children)
    {
        delete child;
    }
    m_children.clear();
}

void ImGuiComponent::AddChild(ImGuiComponent* component)
{
    m_children.push_back(component);
}