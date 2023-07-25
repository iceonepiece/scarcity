#pragma once

#include <imgui/imgui.h>
#include "../Editor2D.h"

class ImGuiWindow
{
public:
	ImGuiWindow(Editor2D& editor)
		: m_editor(editor)
	{}

	virtual void Render() = 0;

protected:
	Editor2D& m_editor;
};