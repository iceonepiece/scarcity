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

void ImGui_LuaEditorPanel::LoadScript(const std::filesystem::path & filepath)
{
	FileSystem::ReadFile(filepath, [&](std::fstream& fs)
	{
		std::stringstream ss;
		ss << fs.rdbuf();
		m_textEditor.SetText(ss.str());
	});
}

void ImGui_LuaEditorPanel::Render()
{
	ImGui::Begin("Lua Editor");

	if (ImGui::Button("Save"))
	{
		Asset* asset = m_editor.GetSelectedAsset();

		if (asset->GetType() == AssetType::LuaScript)
		{
			FileSystem::WriteFile(asset->GetPath(), [&](std::fstream& fs)
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

	ImGui::End();
}