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

	static ImGui_LuaEditorWindow* Get()
	{
		return s_instance;
	}

private:
	inline static ImGui_LuaEditorWindow* s_instance = nullptr;

	TextEditor m_textEditor;
	ImFont* m_font;
	std::filesystem::path m_currentLuaScriptPath;
};