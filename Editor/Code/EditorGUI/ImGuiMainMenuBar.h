#pragma once

class EditorLayer;

class ImGuiMainMenuBar
{
public:
	ImGuiMainMenuBar(EditorLayer& editor);
	void Render();

private:
	EditorLayer& m_editor;
};