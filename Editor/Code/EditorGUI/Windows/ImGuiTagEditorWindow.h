#pragma once

#include "ImGui_Window.h"

class ImGuiTagEditorWindow : public ImGui_Window
{
public:
	ImGuiTagEditorWindow(EditorLayer& editor);
	virtual void Render() override;
};