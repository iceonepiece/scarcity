#include "ImGui_SceneWrapper.h"
#include "../EditorLayer.h"

void ImGui_SceneWrapper::RenderBrowser(EditorLayer& editor)
{
	std::string useIcon = (ICON_FA_WINDOW_MAXIMIZE " ");

	ImGuiTreeNodeFlags flags = (editor.GetSelectedPath() == m_scene->GetPath() ? ImGuiTreeNodeFlags_Selected : 0);
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	flags |= ImGuiTreeNodeFlags_Leaf;

	bool opened = ImGui::TreeNodeEx(m_scene->GetPath().string().c_str(), flags, (useIcon + m_scene->GetPath().filename().string()).c_str());

	if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
		editor.OpenScene(m_scene->GetPath());

	if (opened)
		ImGui::TreePop();
}