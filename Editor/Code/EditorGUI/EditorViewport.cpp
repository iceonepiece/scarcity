#include "EditorViewport.h"

EditorViewport::EditorViewport(EditorLayer& editor)
    : m_editor(editor)
    , m_viewportFocused(false)
    , m_viewportHovered(false)
    , m_width(1)
    , m_height(1)
    , m_cursorPosition(0, 0)
{
}

void EditorViewport::SetViewportSize(unsigned int width, unsigned int height)
{
    m_width = width;
    m_height = height;
}
