#pragma once

#include <imgui/imgui.h>

class EditorLayer;

enum class ImGuiWindowType
{
	None,
	SelectSprite,
	Tags,
	SelectAnimatorController,
	AnimationClip,
};

class ImGui_Window
{
public:
	ImGui_Window(EditorLayer& editor);
	virtual ~ImGui_Window() = default;
	virtual void Render() = 0;

	inline void SetOpen(bool isOpen) { m_isOpen = isOpen; }

protected:
	EditorLayer& m_editor;
	bool m_isOpen = false;
};