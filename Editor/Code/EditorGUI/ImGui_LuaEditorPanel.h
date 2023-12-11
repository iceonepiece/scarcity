#pragma once

#include <ImGuiColorTextEdit/TextEditor.h>

class EditorLayer;

class ImGui_LuaEditorPanel
{
public:
	ImGui_LuaEditorPanel(EditorLayer& editor);
	~ImGui_LuaEditorPanel();

	void Render();

private:
	EditorLayer& m_editor;
	TextEditor m_textEditor;

	ImFont* m_font;
};