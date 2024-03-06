#pragma once

#include <imgui/imgui.h>
#include <string>

class EditorLayer;

enum class ImGuiWindowType
{
	None,
	SelectSprite,
	Tags,
	SelectAnimatorController,
	AnimationClip,
	Animator,
	ProjectSettings,
	LuaEditor
};

class ImGui_Window
{
public:
	ImGui_Window(EditorLayer& editor, const std::string& name = "");
	virtual ~ImGui_Window() = default;
	virtual void Render() = 0;

	inline void SetOpen(bool isOpen) { m_isOpen = isOpen; }

protected:
	EditorLayer& m_editor;

	std::string m_windowName;
	bool m_isOpen = false;
};