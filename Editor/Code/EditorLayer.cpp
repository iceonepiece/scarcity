#include "EditorLayer.h"
#include "Platforms/OpenGL/OpenGLWindow.h"
#include "Core/Camera2D.h"
#include "Math/Math.h"
#include "Gizmos/ViewGizmo.h"
#include "Gizmos/TranslateGizmo.h"
#include "Gizmos/RotateGizmo.h"
#include "Gizmos/ScaleGizmo.h"
#include <iostream>

EditorLayer::EditorLayer(EditorApplication& app)
	: m_app(app)
{
	std::cout << "EditorLayer Constructor()\n\n";
}

void EditorLayer::Initialize()
{
	std::cout << "EditorLayer Initialize()\n\n";

	GLFWwindow* window = static_cast<GLFWwindow*>(m_app.GetWindow().GetNativeWindow());
	m_imgui = std::make_unique<ImGuiMain>(*this, window, "#version 330");

    m_gizmos.push_back(std::make_unique<ViewGizmo>(*this));
    m_gizmos.push_back(std::make_unique<TranslateGizmo>(*this));
    m_gizmos.push_back(std::make_unique<RotateGizmo>(*this));
    m_gizmos.push_back(std::make_unique<ScaleGizmo>(*this));

    m_camera = std::make_unique<Camera2D>(
        glm::vec3 { 0.0f, 0.0f, -1.0f },
        glm::vec2 { 1.0f, 1.0f },
        glm::vec2 { 1280, 720 }
    );

    m_camera->SetCameraType(CameraType::Orthographic);
}


void EditorLayer::CalculateWorldCursorPosition()
{
    glm::vec2 ndcPosition = Math::ConvertToNDC(m_cursorPosition, m_camera->GetScreenSize());
    glm::mat4 ivProjection = glm::inverse(m_camera->GetProjectionMatrix());
    glm::mat4 ivView = glm::inverse(m_camera->GetViewMatrix());
    m_worldCursorPosition = Math::ConvertToWorldSpace(ndcPosition, ivProjection, ivView);
}

void EditorLayer::OnMouseMoved(MouseMovedEvent& event)
{
    m_cursorPosition.x = event.GetX();
    m_cursorPosition.y = event.GetY();

    CalculateWorldCursorPosition();

    if (m_mouseActive)
        m_gizmos[m_currentMode]->OnDragging(m_worldCursorPosition.x, m_worldCursorPosition.y);
}

void EditorLayer::OnKeyPressed(KeyPressedEvent& event)
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantTextInput)
        return;

    std::cout << "On Key PRessed" << std::endl;

    switch (event.GetKeyCode())
    {
        case Key::Q:
        {
            m_currentMode = EditorMode::ViewMode;
        }
        break;

        case Key::W:
        {
            m_currentMode = EditorMode::TranslateMode;
        }
        break;

        case Key::E:
        {
            m_currentMode = EditorMode::RotateMode;
        }
        break;

        case Key::R:
        {
            m_currentMode = EditorMode::ScaleMode;
        }
        break;
    }
}


void EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& event)
{
    std::cout << "OnMouseButtonPressed" << std::endl;

    ImGuiIO& io = ImGui::GetIO();

    if (io.WantCaptureMouse)
        event.handled = true;

    if (event.GetMouseButton() == Mouse::ButtonLeft)
    {
        m_mouseActive = true;

        CalculateWorldCursorPosition();

        if (!m_gizmos[m_currentMode]->OnPicking2D(m_worldCursorPosition))
            m_entityPicked = CheckPicking2D();
    }
}

void EditorLayer::OnMouseButtonReleased(MouseButtonReleasedEvent& event)
{
    std::cout << "OnMouseButtonReleased" << std::endl;

    if (event.GetMouseButton() == Mouse::ButtonLeft)
    {
        m_mouseActive = false;
    }
}

bool EditorLayer::CheckPicking2D()
{
    auto transforms = m_app.GetActiveScene()->GetEntityManager().m_registry.view<TransformComponent>();
    for (auto [entity, transform] : transforms.each())
    {
        if (!Shape2D::IsPointOnRectangle(m_worldCursorPosition, transform.position, transform.scale, transform.rotation.z))
            continue;

        std::cout << "PICKED" << std::endl;
        m_pickedEntity = entity;

        return true;
    }

    return false;
}

void EditorLayer::Shutdown()
{
	std::cout << "EditorLayer Shutdown()\n\n";
}

TransformComponent* EditorLayer::GetEntityTransform()
{
    if (!m_entityPicked)
        return nullptr;

    return &m_app.GetActiveScene()->GetEntityManager().m_registry.get<TransformComponent>(m_pickedEntity);
}


void EditorLayer::Update(float deltaTime)
{
    Renderer& renderer = m_app.GetRenderer();

    if (m_scenePlaying)
    {
        m_playingScene->Update(deltaTime);
        m_playingScene->SetCamera(nullptr);
        m_playingScene->Render();
    }
    else
    {
        m_gizmos[m_currentMode]->Update(deltaTime);

        Scene* activeScene = m_app.GetActiveScene();

        if (activeScene != nullptr)
        {
            activeScene->SetCamera(m_camera.get());
            activeScene->Render();

            if (m_currentMode != EditorMode::ViewMode && m_entityPicked)
            {
                auto& transform = activeScene->GetEntityManager().m_registry.get<TransformComponent>(m_pickedEntity);
                m_gizmos.at(m_currentMode)->Render(renderer, transform.position);
            }
        }
    }

    m_imgui->Render();
}

void EditorLayer::OnWindowResize(WindowResizeEvent& event)
{
    m_camera->SetScreenSize({ event.GetWidth() , event.GetHeight() });
}

void EditorLayer::PlayScene()
{
    Scene* activeScene = m_app.GetActiveScene();
    
    if (activeScene != nullptr)
    {
        m_playingScene = Scene::Copy(*activeScene);
        m_playingScene->SetApplication(&m_app);
        m_playingScene->Start();
        m_scenePlaying = true;
    }
}

void EditorLayer::StopScene()
{
    if (m_playingScene != nullptr)
    {
        m_playingScene->Stop();
        m_playingScene.reset();
        m_scenePlaying = false;
    }
}

void EditorLayer::OnMouseScrolled(MouseScrolledEvent& event)
{
    if (m_camera->GetCameraType() == CameraType::Orthographic)
    {
        m_camera->SetZoom(m_camera->GetZoom() + (event.GetY() * 0.04f));
    }
    else if (m_camera->GetCameraType() == CameraType::Perspective)
    {
        glm::vec3 newPosition = m_camera->GetPosition();
        newPosition.z += event.GetY() * 0.5f;
        m_camera->SetPosition(newPosition);
    }
}

void EditorLayer::OnEvent(Event& event)
{
    EventType evenType = event.GetType();

    ImGuiIO& io = ImGui::GetIO();

    if (io.WantCaptureMouse &&
        (
            evenType == EventType::MouseButtonPressed ||
            evenType == EventType::MouseButtonReleased ||
            evenType == EventType::MouseMoved ||
            evenType == EventType::MouseScrolled
        )
    )
    {
        event.handled = true;
        return;
    }

    if (io.WantCaptureKeyboard &&
        (
            evenType == EventType::KeyPressed ||
            evenType == EventType::KeyReleased
        )
    )
    {
        event.handled = true;
        return;
    }

    switch (evenType)
    {
        case EventType::WindowResize:
            OnWindowResize(static_cast<WindowResizeEvent&>(event));
            break;

        case EventType::MouseMoved:
            OnMouseMoved(static_cast<MouseMovedEvent&>(event));
            break;

        case EventType::MouseScrolled:
            OnMouseScrolled(static_cast<MouseScrolledEvent&>(event));
            break;

        case EventType::MouseButtonPressed:
            OnMouseButtonPressed(static_cast<MouseButtonPressedEvent&>(event));
            break;

        case EventType::MouseButtonReleased:
            OnMouseButtonReleased(static_cast<MouseButtonReleasedEvent&>(event));
            break;

        case EventType::KeyPressed:
            OnKeyPressed(static_cast<KeyPressedEvent&>(event));
            break;
    }


}