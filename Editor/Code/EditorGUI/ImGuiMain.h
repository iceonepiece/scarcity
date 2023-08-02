#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "ImGuiMainMenuBar.h"
#include "ImGuiEntityProperties.h"
#include "ImGuiHierarchy.h"
#include "ImGuiAssetPanel.h"

class EditorLayer;

class ImGuiMain
{
public:
	ImGuiMain(EditorLayer& editor, GLFWwindow* window, const char* glslVersion);
	~ImGuiMain();
	void Render();

private:
	EditorLayer& m_editor;

	ImGuiMainMenuBar m_mainMenuBar;
	ImGuiEntityProperties m_entityProperties;
	ImGuiHierarchy m_hierarchy;
	ImGuiAssetPanel m_assetPanel;
};