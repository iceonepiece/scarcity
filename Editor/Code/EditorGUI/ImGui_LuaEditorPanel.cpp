#include "ImGui_LuaEditorPanel.h"
#include "../EditorLayer.h"
#include <imgui/imgui.h>

ImGui_LuaEditorPanel::ImGui_LuaEditorPanel(EditorLayer& editor)
	: m_editor(editor)
{
	m_textEditor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
	m_font = ImGui::GetIO().Fonts->Fonts[1];
}

ImGui_LuaEditorPanel::~ImGui_LuaEditorPanel()
{
}

void ImGui_LuaEditorPanel::Render()
{
	ImGui::PushFont(m_font);
	ImGui::Begin("Lua Editor");

	auto cpos = m_textEditor.GetCursorPosition();
	ImGui::Text("Cursor at %d,%d", cpos.mLine + 1, cpos.mColumn + 1);

	m_textEditor.Render("Lua Editor");

	ImGui::End();
	ImGui::PopFont();
}