#pragma once

#include "ImGuiWindow.h"

class ImGuiPrefabEditorWindow : public ImGuiWindow
{
public:
	ImGuiPrefabEditorWindow(EditorLayer& editor);
	virtual void Render() override;
};