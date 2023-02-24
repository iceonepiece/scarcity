#include "ImGuiComponent.h"
#include "ImGuiLayer.h"
#include <iostream>

ImGuiComponent::~ImGuiComponent()
{
    std::cout << "ImGuiComponent Destructor" << std::endl;
    for (auto child : m_children)
    {
        delete child;
    }
    m_children.clear();
}

void ImGuiComponent::SetImGuiLayer(ImGuiLayer* layer)
{
    m_gui = layer;
}

void ImGuiComponent::AddChild(ImGuiComponent* component)
{
    m_children.push_back(component);
}