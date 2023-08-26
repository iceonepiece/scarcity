#pragma once

#include <imgui-node-editor/imgui_node_editor.h>

namespace ed = ax::NodeEditor;

class ImGuiNodeEditor
{
public:
	struct LinkInfo
	{
		ed::LinkId Id;
		ed::PinId  InputId;
		ed::PinId  OutputId;
	};

	ImGuiNodeEditor();
	~ImGuiNodeEditor();

	void Render();

private:
	bool m_firstFrame = true;
	ed::EditorContext* m_context = nullptr;
	ImVector<LinkInfo>   m_links;
	int                  m_nextLinkId = 100;
};