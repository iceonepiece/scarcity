#include "ImGui_AnimationClipWrapper.h"
#include "../EditorLayer.h"
#include "../EditorGUI/Windows/ImGui_AnimationClipWindow.h"

void ImGui_AnimationClipWrapper::RenderBrowser(EditorLayer& editor)
{
	std::string useIcon = (ICON_FA_PERSON_RUNNING " ");

	ImGuiTreeNodeFlags flags = (editor.GetSelectedPath() == m_animationClip->GetPath() ? ImGuiTreeNodeFlags_Selected : 0);
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	flags |= ImGuiTreeNodeFlags_Leaf;

	bool opened = ImGui::TreeNodeEx(m_animationClip->GetPath().string().c_str(), flags, (useIcon + m_animationClip->GetName()).c_str());

	if (ImGui::IsItemClicked())
		editor.SetSelectedObject(EditorObjectType::Asset, m_animationClip);

	if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
	{
		ImGui_AnimationClipWindow* animClipWindow = static_cast<ImGui_AnimationClipWindow*>(editor.GetImGuiWindow(ImGuiWindowType::AnimationClip));
		animClipWindow->SetAnimationClip(m_animationClip);
		animClipWindow->SetOpen(true);
	}

	if (opened)
	{
		ImGui::TreePop();
	}
}