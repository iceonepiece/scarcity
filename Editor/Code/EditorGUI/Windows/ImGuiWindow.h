#pragma once

#include <imgui/imgui.h>

class EditorLayer;

enum class ImGuiWindowType
{
	None,
	SelectSprite,
	SelectAnimatorController
};

class ImGuiWindow
{
public:
	ImGuiWindow(EditorLayer& editor);
	virtual ~ImGuiWindow() = default;
	virtual void Render() = 0;

protected:
	EditorLayer& m_editor;
};