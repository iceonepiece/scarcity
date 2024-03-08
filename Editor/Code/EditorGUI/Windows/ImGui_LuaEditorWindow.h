#pragma once

#include "ImGui_Window.h"
#include <filesystem>
#include <ImGuiColorTextEdit/TextEditor.h>

class ImGui_LuaEditorWindow : public ImGui_Window
{
public:
	ImGui_LuaEditorWindow(EditorLayer& editor);
	virtual void Render() override;

	void LoadScript(const std::filesystem::path& filepath);

private:
	TextEditor m_textEditor;
	ImFont* m_font;
	std::filesystem::path m_currentLuaScriptPath;
};