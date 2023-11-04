#pragma once

#include "ImGuiWindow.h"

class ImGuiTagEditorWindow : public ImGuiWindow_
{
public:
	ImGuiTagEditorWindow(EditorLayer& editor);
	virtual void Render() override;
};