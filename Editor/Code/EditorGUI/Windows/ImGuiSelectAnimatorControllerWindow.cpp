#include "ImGuiSelectAnimatorControllerWindow.h"
#include "Core/Application.h"
#include "Project/Project.h"

ImGuiSelectAnimatorControllerWindow::ImGuiSelectAnimatorControllerWindow(EditorLayer& editor, const std::filesystem::path& path)
	: ImGui_Window(editor)
	, m_baseDirectory(path)
	, m_currentDirectory(path)
	, m_selectedController(nullptr)
{
	s_instance = this;
}

void ImGuiSelectAnimatorControllerWindow::Render()
{
	auto& animControllers = Project::GetActive()->GetAssetManager().GetAnimatorControllers();

	for (auto& controller : animControllers)
	{
		if (ImGui::Selectable(controller->GetName().c_str(), false, ImGuiSelectableFlags_DontClosePopups))
		{
			m_selectedController = controller;
		}
	}
}