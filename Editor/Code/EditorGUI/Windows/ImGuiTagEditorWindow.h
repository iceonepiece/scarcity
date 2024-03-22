#pragma once

#include "ImGui_Window.h"

class ImGuiTagEditorWindow : public ImGui_Window
{
public:
	ImGuiTagEditorWindow(EditorLayer& editor);
	virtual void Render() override;

	static ImGuiTagEditorWindow* Get()
	{
		return s_instance;
	}

private:
	inline static ImGuiTagEditorWindow* s_instance = nullptr;
};