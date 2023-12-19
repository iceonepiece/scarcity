#include "ImGui_Window.h"
#include "../../EditorLayer.h"

ImGui_Window::ImGui_Window(EditorLayer& editor, const std::string& name)
	: m_editor(editor)
	, m_windowName(name)
{}