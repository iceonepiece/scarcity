#pragma once

class EditorLayer;

class ImGuiHierarchy
{
public:
	ImGuiHierarchy(EditorLayer& editor);
	void Render();

private:
	EditorLayer& m_editor;
};