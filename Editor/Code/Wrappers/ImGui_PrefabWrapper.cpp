#include "ImGui_PrefabWrapper.h"

void ImGui_PrefabWrapper::RenderBrowser(EditorLayer& editor)
{
	std::string useIcon = ICON_FA_CUBE;

	ImGuiTreeNodeFlags flags = (editor.GetSelectedPath() == m_prefab->GetPath() ? ImGuiTreeNodeFlags_Selected : 0);
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	flags |= ImGuiTreeNodeFlags_Leaf;

	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(140, 200, 255, 255));
	bool opened = ImGui::TreeNodeEx(m_prefab->GetPath().string().c_str(), flags, useIcon.c_str());
	ImGui::PopStyleColor();

	if (ImGui::IsItemClicked())
		editor.SetSelectedObject(EditorObjectType::Asset, m_prefab);

	if (ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload("PREFAB_ASSET", m_prefab, sizeof(m_prefab));

		ImGui::EndDragDropSource();
	}

	ImGui::SameLine();
	ImGui::Text(m_prefab->GetPath().filename().string().c_str());

	//RightClickMenu(prefab);

	if (opened)
		ImGui::TreePop();
}