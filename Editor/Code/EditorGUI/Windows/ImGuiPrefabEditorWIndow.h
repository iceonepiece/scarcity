#pragma once

#include "ImGui_Window.h"

class ImGuiPrefabEditorWindow : public ImGui_Window
{
public:
	ImGuiPrefabEditorWindow(EditorLayer& editor);
	virtual void Render() override;
};