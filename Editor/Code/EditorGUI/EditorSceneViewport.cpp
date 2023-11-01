#include "EditorSceneViewport.h"
#include "../EditorLayer.h"
#include "../Gizmos/ViewGizmo.h"
#include "../Gizmos/TranslateGizmo.h"
#include "../Gizmos/RotateGizmo.h"
#include "../Gizmos/ScaleGizmo.h"
#include "../Gizmos/GridGizmo.h"
#include "Graphics/Camera2D.h"
#include <imgui/imgui.h>
#include <IconsFontAwesome6.h>

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
    m_gizmos.push_back(std::make_unique<GridGizmo>(*this));
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

void EditorSceneViewport::RenderTools()
{
    static int location = 0;
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_AlwaysAutoResize;
    if (location >= 0)
    {
        const float PAD = 10.0f;
        const ImGuiViewport* viewport = ImGui::GetWindowViewport();

        ImVec2 vMin = ImGui::GetWindowContentRegionMin();
        ImVec2 vMax = ImGui::GetWindowContentRegionMax();

        vMin.x += ImGui::GetWindowPos().x;
        vMin.y += ImGui::GetWindowPos().y;
        vMax.x += ImGui::GetWindowPos().x;
        vMax.y += ImGui::GetWindowPos().y;

        ImVec2 work_pos = vMin;
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
        window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
        window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
        window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        //ImGui::SetNextWindowViewport(viewport->ID);
        window_flags |= ImGuiWindowFlags_NoMove;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 8));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 2));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);
    ImGui::PushStyleColor(ImGuiCol_Border, (ImVec4)ImColor(0, 0, 0, 255));

    if (ImGui::Begin("##gizmo_modes", NULL, window_flags))
    {
        std::string iconStrings[] = { ICON_FA_HAND, ICON_FA_ARROWS_UP_DOWN_LEFT_RIGHT, ICON_FA_ROTATE, ICON_FA_EXPAND, ICON_FA_TABLE_CELLS };
        ImColor normalColor{ 85, 85, 85, 255 };

        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)normalColor);

        EditorMode currentMode = m_editor.GetCurrentMode();
        int modes = m_editor.IsGridModeAvailable() ? 5 : 4;

        for (int i = 0; i < modes; i++)
        {
            if (currentMode == i)
                ImGui::PopStyleColor();

            if (ImGui::Button(iconStrings[i].c_str()))
                m_editor.SetCurrentMode((EditorMode)i);

            if (currentMode == i)
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)normalColor);
        }
        ImGui::PopStyleColor();

        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Custom", NULL, location == -1)) location = -1;
            if (ImGui::MenuItem("Center", NULL, location == -2)) location = -2;
            if (ImGui::MenuItem("Top-left", NULL, location == 0)) location = 0;
            if (ImGui::MenuItem("Top-right", NULL, location == 1)) location = 1;
            if (ImGui::MenuItem("Bottom-left", NULL, location == 2)) location = 2;
            if (ImGui::MenuItem("Bottom-right", NULL, location == 3)) location = 3;

            ImGui::EndPopup();
        }
    }
    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(5);
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

    if (!m_editor.IsScenePlaying())
        RenderTools();

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
        if (m_editor.GetCurrentMode() == EditorMode::GridMode)
        {
            m_gizmos[m_editor.GetCurrentMode()]->OnPicking2D(worldCursorPosition);
        }
        else
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
}

void EditorSceneViewport::OnMouseButtonPressed(MouseButtonPressedEvent& event)
{
    if (!m_viewportHovered)
        return;

    m_editor.SetMouseActive(true);
    glm::vec4 worldCursorPosition = m_camera->ScreenToWorldPosition(m_cursorPosition);

    if (m_editor.GetCurrentMode() == EditorMode::GridMode)
    {
        GridGizmoMode mode = GridGizmoMode::None;
        GridGizmo* gridGizmo = (GridGizmo*)m_gizmos[m_editor.GetCurrentMode()].get();

        if (event.GetMouseButton() == Mouse::ButtonLeft)
            mode = GridGizmoMode::Draw;
        else if (event.GetMouseButton() == Mouse::ButtonRight)
            mode = GridGizmoMode::Erase;

        gridGizmo->SetMode(mode);
        gridGizmo->OnPicking2D(worldCursorPosition);
    }
    else if (event.GetMouseButton() == Mouse::ButtonLeft)
    {
        if (!m_gizmos[m_editor.GetCurrentMode()]->OnPicking2D(worldCursorPosition))
        {
            if (!m_editor.CheckPicking2D(worldCursorPosition))
                m_editor.UnselectObject();
        }
    }
}

void EditorSceneViewport::OnMouseButtonReleased(MouseButtonReleasedEvent& event)
{
    m_editor.SetMouseActive(false);

    if (m_gizmoStatus.dragging)
    {
        m_gizmos[m_editor.GetCurrentMode()]->OnDraggingEnd();
        m_gizmoStatus.dragging = false;
    }
}
