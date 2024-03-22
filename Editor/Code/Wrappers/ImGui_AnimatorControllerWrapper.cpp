#include "ImGui_AnimatorControllerWrapper.h"
#include "../EditorLayer.h"

void ImGui_AnimatorControllerWrapper::RenderBrowser(EditorLayer& editor)
{
	std::string useIcon = (ICON_FA_CIRCLE_NODES " ");

	ImGuiTreeNodeFlags flags = (editor.GetSelectedPath() == m_animController->GetPath() ? ImGuiTreeNodeFlags_Selected : 0);
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	flags |= ImGuiTreeNodeFlags_Leaf;

	bool opened = ImGui::TreeNodeEx(m_animController->GetPath().string().c_str(), flags, (useIcon + m_animController->GetName()).c_str());

	if (ImGui::IsItemClicked())
	{
		editor.SetSelectedObject(EditorObjectType::Asset, m_animController);
		editor.SetAnimatorController(*m_animController);
		ImGui_AnimatorWindow::Get()->SetOpen(true);
	}

	if (opened)
	{
		ImGui::TreePop();
	}

}