#pragma once

#include <imgui/imgui.h>
#include <vector>

class ImGuiLayer;

class ImGuiComponent
{
public:
	ImGuiComponent(ImGuiLayer* gui);
	virtual ~ImGuiComponent();
	virtual void Draw() = 0;

	void AddChild(ImGuiComponent* component);

protected:
	ImGuiLayer* m_gui;
	std::vector<ImGuiComponent*> m_children;
};
