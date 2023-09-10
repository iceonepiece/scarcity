#include "ImGuiSelectAnimatorControllerWindow.h"

ImGuiSelectAnimatorControllerWindow::ImGuiSelectAnimatorControllerWindow(EditorLayer& editor, const std::filesystem::path& path)
	: ImGuiWindow(editor)
	, m_baseDirectory(path)
	, m_currentDirectory(path)
{
}

void ImGuiSelectAnimatorControllerWindow::Render()
{

}