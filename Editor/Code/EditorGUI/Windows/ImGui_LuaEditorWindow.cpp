#include "ImGui_LuaEditorWindow.h"
#include "Lua/LuaScript.h"
#include "../../EditorLayer.h"

ImGui_LuaEditorWindow::ImGui_LuaEditorWindow(EditorLayer& editor)
	: ImGui_Window(editor, "Lua Editor")
{
	m_textEditor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
	m_textEditor.SetShowWhitespaces(false);
	m_font = ImGui::GetIO().Fonts->Fonts[1];
}

void ImGui_LuaEditorWindow::LoadScript(const std::filesystem::path& filepath)
{
	bool success = FileSystem::ReadFile(filepath, [&](std::fstream& fs)
	{
		std::stringstream ss;
		ss << fs.rdbuf();
		m_textEditor.SetText(ss.str());
	});

	if (success)
		m_currentLuaScriptPath = filepath;
}

void ImGui_LuaEditorWindow::Render()
{
	if (!m_isOpen)
		return;

	if (ImGui::Begin("Lua Editor", &m_isOpen, ImGuiWindowFlags_NoScrollbar))
	{
		if (ImGui::Button("Save"))
		{
			if (FileSystem::FileExists(m_currentLuaScriptPath))
			{
				FileSystem::WriteFile(m_currentLuaScriptPath, [&](std::fstream& fs)
				{
					fs << m_textEditor.GetText();
				});
			}
		}

		ImGui::PushFont(m_font);
		m_textEditor.Render("Lua Editor");
		ImGui::PopFont();

		auto cpos = m_textEditor.GetCursorPosition();
		ImGui::Text("Cursor at %d,%d", cpos.mLine + 1, cpos.mColumn + 1);
	}

	ImGui::End();
}