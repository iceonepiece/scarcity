#include "ImGui_AudioClipWrapper.h"
#include "../EditorLayer.h"

void ImGui_AudioClipWrapper::RenderBrowser(EditorLayer& editor)
{
	std::string useIcon = (ICON_FA_MUSIC " ");

	ImGuiTreeNodeFlags flags = (editor.GetSelectedPath() == m_audioClip->GetPath() ? ImGuiTreeNodeFlags_Selected : 0);
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	flags |= ImGuiTreeNodeFlags_Leaf;

	bool opened = ImGui::TreeNodeEx(m_audioClip->GetPath().string().c_str(), flags, (useIcon + m_audioClip->GetName()).c_str());

	if (ImGui::IsItemClicked())
		editor.SetSelectedObject(EditorObjectType::Asset, m_audioClip);

	if (opened)
	{
		ImGui::TreePop();
	}
}