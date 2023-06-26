#pragma once

#include <vector>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "ImGuiComponent.h"
#include "Events/Event.h"
#include "Core/EntityManager.h"

class ImGuiLayer
{
public:
	~ImGuiLayer();
	void Initialize(GLFWwindow* window, const char* glslVersion);

	void Destroy();

	void NewFrame();
	void Draw();

	void AddComponent(ImGuiComponent* component);

	bool BlockEvent(Event* e);
	EntityManager& GetEntityManager();

protected:
	std::vector<ImGuiComponent*> m_components;
	EntityManager m_manager;
};