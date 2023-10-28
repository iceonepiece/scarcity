#include "EditorViewport.h"

EditorViewport::EditorViewport(EditorLayer& editor)
    : m_editor(editor)
{
}

void EditorViewport::SetViewportSize(unsigned int width, unsigned int height)
{
    m_width = width;
    m_height = height;
}
