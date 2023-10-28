#pragma once

#include <memory>
#include "Graphics/Framebuffer.h"
#include "Graphics/Camera.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvent.h"

class EditorLayer;

class EditorViewport
{
public:
    EditorViewport(EditorLayer& editor);
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;

    virtual void OnMouseScrolled(MouseScrolledEvent& event) {}
    virtual void OnWindowResize(WindowResizeEvent& event) {}
    virtual void OnMouseMoved(MouseMovedEvent& event) {}
    virtual void OnMouseButtonPressed(MouseButtonPressedEvent& event) {}
    virtual void OnMouseButtonReleased(MouseButtonReleasedEvent& event) {}

    void SetViewportSize(unsigned int width, unsigned int height);

    inline EditorLayer& GetEditorLayer() { return m_editor; }
    inline Camera& GetCamera() { return *m_camera; }

protected:
    EditorLayer& m_editor;

    unsigned int m_width;
    unsigned int m_height;

    bool m_viewportFocused;
    bool m_viewportHovered;

    glm::vec2 m_cursorPosition;

    std::unique_ptr<Framebuffer> m_framebuffer;
    std::unique_ptr<Camera> m_camera;
};