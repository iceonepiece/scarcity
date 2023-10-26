#pragma once

#include "ImGuiWindow.h"

class ImGuiPrefabEditorWindow : public ImGuiWindow_
{
public:
	ImGuiPrefabEditorWindow(EditorLayer& editor);
	virtual void Render() override;
};