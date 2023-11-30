#include "ImGuiSelectAnimatorControllerWindow.h"
#include "Core/Application.h"

ImGuiSelectAnimatorControllerWindow::ImGuiSelectAnimatorControllerWindow(EditorLayer& editor, const std::filesystem::path& path)
	: ImGui_Window(editor)
	, m_baseDirectory(path)
	, m_currentDirectory(path)
	, m_selectedController(nullptr)
{
}

void ImGuiSelectAnimatorControllerWindow::Render()
{
	auto& animControllers = Application::Get().GetAssetManager().GetAnimatorControllers();

	for (auto& controller : animControllers)
	{
		if (ImGui::Selectable(controller->GetName().c_str(), false, ImGuiSelectableFlags_DontClosePopups))
		{
			m_selectedController = controller;
		}
	}
}