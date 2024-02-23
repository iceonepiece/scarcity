#include "ImGui_LuaScriptWrapper.h"
#include "../EditorLayer.h"

void ImGui_LuaScriptWrapper::RenderBrowser(EditorLayer& editor)
{
	std::string useIcon = (ICON_FA_CODE " ");

	ImGuiTreeNodeFlags flags = (editor.GetSelectedPath() == m_luaScript->GetPath() ? ImGuiTreeNodeFlags_Selected : 0);
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	flags |= ImGuiTreeNodeFlags_Leaf;

	bool opened = ImGui::TreeNodeEx(m_luaScript->GetPath().string().c_str(), flags, (useIcon + m_luaScript->GetName()).c_str());

	if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		editor.SetSelectedObject(EditorObjectType::Asset, m_luaScript);

	if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
	{
		auto& luaEditor = editor.GetLuaEditorPanel();
		luaEditor.LoadScript(m_luaScript->GetPath().string());
	}

	if (opened)
	{
		ImGui::TreePop();
	}

	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		ImGui::SetDragDropPayload("LUA_SCRIPT_FILE", m_luaScript, sizeof(LuaScript));

		ImGui::EndDragDropSource();
	}
}