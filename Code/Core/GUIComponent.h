#pragma once

#include <vector>

class GUI;

class GUIComponent
{
public:
  GUIComponent(GUI* gui);
  virtual ~GUIComponent();
  virtual void Draw() = 0;

  void AddChild(GUIComponent* component);

protected:
  GUI* m_gui;
  std::vector<GUIComponent*> m_children;
};
