#include "ImGuiSelectAnimatorControllerWindow.h"
#include "Core/Application.h"

ImGuiSelectAnimatorControllerWindow::ImGuiSelectAnimatorControllerWindow(EditorLayer& editor, const std::filesystem::path& path)
	: ImGuiWindow(editor)
	, m_baseDirectory(path)
	, m_currentDirectory(path)
{
}

void ImGuiSelectAnimatorControllerWindow::Render()
{
	auto& animControllerMap = Application::Get().GetAssetManager().GetAnimatorControllerAssets();

	for (auto& animController : animControllerMap)
	{
		if (ImGui::Selectable(animController.second->GetName().c_str(), m_selectedControllerName == animController.first, ImGuiSelectableFlags_DontClosePopups))
			m_selectedControllerName = animController.first;
	}

}