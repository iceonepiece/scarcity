#pragma once

#include <imgui/imgui.h>

class EditorLayer;

enum class ImGuiWindowType
{
	None,
	SelectSprite,
	Tags,
	SelectAnimatorController,
};

class ImGuiWindow_
{
public:
	ImGuiWindow_(EditorLayer& editor);
	virtual ~ImGuiWindow_() = default;
	virtual void Render() = 0;

protected:
	EditorLayer& m_editor;
};