#include "ImGui_FontWrapper.h"
#include "../EditorLayer.h"

void ImGui_FontWrapper::RenderBrowser(EditorLayer& editor)
{
	std::string useIcon = (ICON_FA_FONT " ");

	ImGuiTreeNodeFlags flags = (editor.GetSelectedPath() == m_font->GetPath() ? ImGuiTreeNodeFlags_Selected : 0);
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	flags |= ImGuiTreeNodeFlags_Leaf;

	bool opened = ImGui::TreeNodeEx(m_font->GetPath().string().c_str(), flags, (useIcon + m_font->GetName()).c_str());

	if (ImGui::IsItemClicked())
		editor.SetSelectedObject(EditorObjectType::Asset, m_font);

	if (opened)
	{
		ImGui::TreePop();
	}
}