#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "ImGuiEntityProperties.h"
#include "ImGuiHierarchy.h"

class Editor2D;

class ImGuiMain
{
public:
	ImGuiMain(Editor2D& editor, GLFWwindow* window, const char* glslVersion);
	~ImGuiMain();
	void Render();

private:
	Editor2D& m_editor;
	ImGuiEntityProperties m_entityProperties;
	ImGuiHierarchy m_hierarchy;
};