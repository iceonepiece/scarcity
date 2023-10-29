#include "EditorSceneViewport.h"
#include "../EditorLayer.h"
#include "../Gizmos/ViewGizmo.h"
#include "../Gizmos/TranslateGizmo.h"
#include "../Gizmos/RotateGizmo.h"
#include "../Gizmos/ScaleGizmo.h"
#include "Graphics/Camera2D.h"
#include <imgui/imgui.h>

EditorSceneViewport::EditorSceneViewport(EditorLayer& editor)
    : EditorViewport(editor)
{
    Application& app = Application::Get();
    m_framebuffer = app.GetRenderer().CreateFramebuffer();

    m_camera = std::make_unique<Camera2D>(
        glm::vec3{ 0.0f, 0.0f, -1.0f },
        glm::vec2{ 1.0f, 1.0f },
        glm::vec2{ app.GetWindow().GetWidth(), app.GetWindow().GetHeight() }
    );

    m_camera->SetCameraType(CameraType::Orthographic);

    m_gizmos.push_back(std::make_unique<ViewGizmo>(*this));
    m_gizmos.push_back(std::make_unique<TranslateGizmo>(*this));
    m_gizmos.push_back(std::make_unique<RotateGizmo>(*this));
    m_gizmos.push_back(std::make_unique<ScaleGizmo>(*this));
}


void EditorSceneViewport::Update(float deltaTime)
{
    Application& app = Application::Get();

    if (m_width > 0.0f && m_height > 0.0f)
    {
        m_framebuffer->Rescale(m_width, m_height);
        m_camera->SetScreenSize({ m_width, m_height });
    }

    m_framebuffer->Bind();

    app.GetRenderer().Clear({0.2f, 0.2f, 0.2f, 1.0f});

    if (m_editor.IsScenePlaying())
    {
        if (Scene* playingScene = m_editor.GetGameLayer().GetCurrentScene())
        {
            playingScene->Update(deltaTime);
            playingScene->SetViewportSize(m_width, m_height);
            playingScene->OnViewportResize();
            playingScene->Render();
        }
    }
    else
    {
        m_gizmos[m_editor.GetCurrentMode()]->Update(deltaTime);

        if (Scene* activeScene = m_editor.GetScene())
        {
            activeScene->SetCamera(*m_camera);
            activeScene->SetViewportSize(m_width, m_height);
            activeScene->RenderEditor();

            if (m_editor.GetCurrentMode() != EditorMode::ViewMode && m_editor.GetSelectedObject().type == EditorObjectType::Entity)
            {
                auto& transform = activeScene->GetEntityManager().m_registry.get<TransformComponent>(m_editor.GetSelectedObject().entity);
                m_gizmos.at(m_editor.GetCurrentMode())->Render(app.GetRenderer(), transform.position);
            }
        }
    }

    m_framebuffer->Unbind();
}

void EditorSceneViewport::Render()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("Scene");

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    m_width = static_cast<unsigned int>(viewportPanelSize.x);
    m_height = static_cast<unsigned int>(viewportPanelSize.y);

    m_viewportFocused = ImGui::IsWindowFocused();
    m_viewportHovered = ImGui::IsWindowHovered();

    ImVec2 mousePos = ImGui::GetMousePos();
    ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();
    m_cursorPosition = { mousePos.x - cursorScreenPos.x, mousePos.y - cursorScreenPos.y };
    
    auto& input = Application::Get().GetInput();
    input.SetCursorPosition(m_cursorPosition.x, m_cursorPosition.y);

    uint64_t textureID = m_framebuffer->GetID();
    ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ (float)m_width, (float)m_height }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

    ImGui::End();
    ImGui::PopStyleVar();
}

void EditorSceneViewport::OnMouseScrolled(MouseScrolledEvent& event)
{
    if (!m_viewportHovered)
        return;

    if (m_camera->GetCameraType() == CameraType::Orthographic)
    {
        m_camera->SetZoom(m_camera->GetZoom() * (1.0f + event.GetY() * 0.1f));
    }
    else if (m_camera->GetCameraType() == CameraType::Perspective)
    {
        glm::vec3 newPosition = m_camera->GetPosition();
        newPosition.z += event.GetY() * 0.5f;
        m_camera->SetPosition(newPosition);
    }
}

void EditorSceneViewport::OnWindowResize(WindowResizeEvent& event)
{
    m_camera->SetScreenSize({ event.GetWidth() , event.GetHeight() });
}

void EditorSceneViewport::OnMouseMoved(MouseMovedEvent& event)
{
    if (!m_viewportHovered)
        return;

    glm::vec4 worldCursorPosition = m_camera->ScreenToWorldPosition(m_cursorPosition);

    if (m_editor.IsMouseActive())
    {
        bool dragging = m_gizmos[m_editor.GetCurrentMode()]->OnDragging(worldCursorPosition.x, worldCursorPosition.y);

        if (dragging && !m_gizmoStatus.dragging)
        {
            m_gizmos[m_editor.GetCurrentMode()]->OnDraggingStart();
            m_gizmoStatus.dragging = true;
            m_gizmoStatus.mode = m_editor.GetCurrentMode();
        }
    }
}

void EditorSceneViewport::OnMouseButtonPressed(MouseButtonPressedEvent& event)
{
    if (!m_viewportHovered)
        return;

    if (event.GetMouseButton() == Mouse::ButtonLeft)
    {
        m_editor.SetMouseActive(true);
        glm::vec4 worldCursorPosition = m_camera->ScreenToWorldPosition(m_cursorPosition);

        if (!m_gizmos[m_editor.GetCurrentMode()]->OnPicking2D(worldCursorPosition))
        {
            if (!m_editor.CheckPicking2D(worldCursorPosition))
                m_editor.UnselectObject();
        }
    }
}

void EditorSceneViewport::OnMouseButtonReleased(MouseButtonReleasedEvent& event)
{
    if (m_gizmoStatus.dragging)
    {
        m_gizmos[m_editor.GetCurrentMode()]->OnDraggingEnd();
        m_gizmoStatus.dragging = false;
    }
}
