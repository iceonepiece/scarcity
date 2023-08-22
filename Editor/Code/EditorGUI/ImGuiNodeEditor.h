#pragma once

#include <imgui-node-editor/imgui_node_editor.h>

namespace ed = ax::NodeEditor;

class ImGuiNodeEditor
{
public:
	ImGuiNodeEditor();
	~ImGuiNodeEditor();

	void Render();

private:
	ed::EditorContext* m_context = nullptr;
};