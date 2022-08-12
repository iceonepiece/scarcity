#include "GUIComponent.h"
#include "GUI.h"
#include <iostream>

GUIComponent::GUIComponent(GUI* gui)
  : m_gui(gui)
{
}

GUIComponent::~GUIComponent()
{
  std::cout << "GUIComponent Destructor" << std::endl;
  for (auto child : m_children)
  {
    delete child;
  }
  m_children.clear();
}

void GUIComponent::AddChild(GUIComponent* component)
{
  m_children.push_back(component);
}
