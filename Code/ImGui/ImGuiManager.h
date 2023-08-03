#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Core/Application.h"

class ImGuiManager
{
public:
	ImGuiManager(Application& app);
	~ImGuiManager();

	void Begin();
	void End();

private:
	Application& m_app;
};