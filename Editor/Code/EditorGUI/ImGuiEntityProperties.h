#pragma once

class EditorLayer;

class ImGuiEntityProperties
{
public:
	ImGuiEntityProperties(EditorLayer& editor);
	void Render();

private:
	EditorLayer& m_editor;
};