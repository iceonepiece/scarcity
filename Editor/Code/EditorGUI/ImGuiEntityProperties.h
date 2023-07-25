#pragma once

#include <entt/entt.hpp>

class Editor2D;

class ImGuiEntityProperties
{
public:
	ImGuiEntityProperties(Editor2D& editor);
	void Render();

private:
	Editor2D& m_editor;
};