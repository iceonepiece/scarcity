#pragma once

#include <imgui/imgui.h>
#include <vector>

class ImGuiLayer;

class ImGuiComponent
{
public:
	virtual ~ImGuiComponent();
	virtual void Draw() = 0;

	void SetImGuiLayer(ImGuiLayer* layer);
	void AddChild(ImGuiComponent* component);

protected:
	ImGuiLayer* m_gui = nullptr;
	std::vector<ImGuiComponent*> m_children;
};
