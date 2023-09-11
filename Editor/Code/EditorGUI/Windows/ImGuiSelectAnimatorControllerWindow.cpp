#include "ImGuiSelectAnimatorControllerWindow.h"
#include "Core/ResourceAPI.h"
#include "Core/ResourceManager.h"

ImGuiSelectAnimatorControllerWindow::ImGuiSelectAnimatorControllerWindow(EditorLayer& editor, const std::filesystem::path& path)
	: ImGuiWindow(editor)
	, m_baseDirectory(path)
	, m_currentDirectory(path)
{
}

void ImGuiSelectAnimatorControllerWindow::Render()
{
	auto& animControllerMap = ResourceAPI::GetResourceManager()->GetAnimatorControllerAssets();

	for (auto& animController : animControllerMap)
	{
		if (ImGui::Selectable(animController.second->GetName().c_str(), m_selectedControllerName == animController.first, ImGuiSelectableFlags_DontClosePopups))
			m_selectedControllerName = animController.first;
	}

}