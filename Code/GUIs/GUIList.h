#pragma once

#include <string>
#include <vector>
#include "../Core/GUIComponent.h"

class GUIList : public GUIComponent
{
public:
  GUIList(GUI* gui)
    : GUIComponent(gui)
  {
  }

  void Draw()
  {
    if (ImGui::Button("Add name"))
      m_names.push_back("XXX");

    ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

    int nodeClicked = -1;

    for (int i = 0; i < m_names.size(); i++)
    {
      ImGuiTreeNodeFlags node_flags = base_flags;

      if (m_selectedIndex == i)
          node_flags |= ImGuiTreeNodeFlags_Selected;

      bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, m_names[i].c_str(), i);

      if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        nodeClicked = i;

      if (node_open)
      {
        ImGui::BulletText("Blah blah\nBlah Blah");
        ImGui::TreePop();
      }
    }

    if (nodeClicked != -1)
      m_selectedIndex = nodeClicked;
  }

  void AddMember(std::string name)
  {
    m_names.push_back(name);
  }

private:
  std::vector<std::string> m_names;
  unsigned int m_selectedIndex = -1;
};
