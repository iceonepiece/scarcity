#pragma once

class Editor2D;

class ImGuiHierarchy
{
public:
	ImGuiHierarchy(Editor2D& editor);
	void Render();

private:
	Editor2D& m_editor;
};