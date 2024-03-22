#pragma once

#include "ImGui_Window.h"
#include <memory>

namespace ImGuiWindowType
{
	enum Type
	{
		SelectSprite,
		Tags,
		SelectAnimatorController,
		AnimationClip,
		Animator,
		ProjectSettings,
		LuaEditor,
		TilemapEditor,
		Count,
	};
}

class ImGui_WindowManager
{
public:
	static void Initialize(EditorLayer& editor);
	static void Render();
	static ImGui_Window* GetWindow(ImGuiWindowType::Type type);

private:
	inline static std::unique_ptr<ImGui_Window> s_imGuiWindows[ImGuiWindowType::Count];
};