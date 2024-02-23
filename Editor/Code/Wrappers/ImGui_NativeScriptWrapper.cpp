#include "ImGui_NativeScriptWrapper.h"
#include "../EditorLayer.h"

void ImGui_NativeScriptWrapper::RenderBrowser(EditorLayer& editor)
{
	std::string useIcon = (ICON_FA_CODE " ");

	ImGuiTreeNodeFlags flags = (editor.GetSelectedPath() == m_nativeScript->GetPath() ? ImGuiTreeNodeFlags_Selected : 0);
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	flags |= ImGuiTreeNodeFlags_Leaf;

	bool opened = ImGui::TreeNodeEx(m_nativeScript->GetPath().string().c_str(), flags, (useIcon + m_nativeScript->GetName()).c_str());

	if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		editor.SetSelectedObject(EditorObjectType::Asset, m_nativeScript);

	if (opened)
	{
		ImGui::TreePop();
	}

	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		ImGui::SetDragDropPayload("NATIVE_SCRIPT_FILE", m_nativeScript, sizeof(NativeScript));

		ImGui::EndDragDropSource();
	}
}

void ImGui_NativeScriptWrapper::RenderInspector()
{
	std::string name = m_nativeScript->GetPath().stem().string() + " (Native Script)";

	ImGui::Text(name.c_str());
}
