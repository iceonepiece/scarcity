#include "Editor2D.h"
#include "Platforms/OpenGL/OpenGLWindow.h"
#include "Platforms/OpenGL/OpenGLRenderer.h"
#include "Gizmos/TranslateGizmo.h"
#include "Gizmos/RotateGizmo.h"
#include "Gizmos/ScaleGizmo.h"
#include "Core/Camera2D.h"
#include "Events/KeyEvent.h"
#include "Core/Entity.h"
#include "Components/Components.h"

glm::vec2 ConvertToNDC(const glm::vec2& screenPos, const glm::vec2& screenSize)
{
    float normalizedX = screenPos.x / screenSize.x;
    float normalizedY = screenPos.y / screenSize.y;

    float ndcX = (normalizedX * 2.0f) - 1.0f;
    float ndcY = (normalizedY * -2.0f) + 1.0f;

    return { ndcX, ndcY };
}


glm::vec2 ConvertToWorldSpace(const glm::vec2& ndcPos, const glm::mat4& inverseProjectionMatrix, const glm::mat4& inverseViewMatrix) {
    glm::vec4 ndcPosition = glm::vec4(ndcPos.x, ndcPos.y, 0.0f, 1.0f);

    glm::vec4 clipSpacePosition = inverseProjectionMatrix * ndcPosition;
    clipSpacePosition /= clipSpacePosition.w;

    glm::vec4 worldSpacePosition = inverseViewMatrix * clipSpacePosition;

    return { worldSpacePosition.x, worldSpacePosition.y };
}


void Editor2D::Initialize(std::string title, int width, int height)
{
	m_window = std::make_unique<OpenGLWindow>(title, width, height);

    m_window->SetEventCallback([this](Event* event) {
        this->OnEvent(event);
    });

    m_camera = std::make_unique<Camera2D>(
        glm::vec3 { 0.0f, 0.0f, -1.0f },
        glm::vec2 { 1.0f, 1.0f },
        glm::vec2 { 1280, 720 }
    );

    m_renderer = std::make_unique<OpenGLRenderer>();
    m_renderer->Initialize();
    m_renderer->SetCamera(m_camera.get());

    m_gizmos.push_back(std::make_unique<TranslateGizmo>());
    m_gizmos.push_back(std::make_unique<RotateGizmo>());
    m_gizmos.push_back(std::make_unique<ScaleGizmo>());

	Input::Init();

    m_camera->SetCameraType(CameraType::Orthographic);


    Entity rect = m_entityManager.CreateEntity();
    rect.AddComponent<TransformComponent>(glm::vec3 {0.0f, 0.0f, 0.0f}, glm::vec3 {0.0f}, glm::vec3 {200.0f, 200.0f, 1.0f});
    
    Entity rect2 = m_entityManager.CreateEntity();
    rect2.AddComponent<TransformComponent>(glm::vec3 {-300.0f, 0.0f, 0.0f}, glm::vec3 {0.0f}, glm::vec3 {50.0f, 100.0f, 1.0f});

    //Entity rect2 = m_entityManager.CreateEntity();
    //rect2.AddComponent<TransformComponent>(glm::vec3 {3.0f, 2.0f, 0.0f}, glm::vec3 {0.0f}, glm::vec3 {0.5f, 0.5f, 1.0f});
}

void Editor2D::OnEvent(Event* event)
{
    m_events.push_back(std::unique_ptr<Event>(event));
}

void Editor2D::Run()
{
    while (m_running)
    {
        ProcessInput();
        Update();
        Render();
    }
}

void Editor2D::ProcessInput()
{
    m_window->ProcessInput();

    if (m_window->WindowShouldClose())
        m_running = false;


    for (auto& gizmo : m_gizmos)
    {
        //gizmo->Proc
    }


    for (auto& e : m_events)
        e->Process(*this);

    m_events.clear();
}

void Editor2D::OnKeyPressed(KeyPressedEvent& event)
{
    switch (event.GetKeyCode())
    {
        case Key::Q: m_currentMode = EditorMode::ViewMode;      break;
        case Key::W: m_currentMode = EditorMode::TranslateMode; break;
        case Key::E: m_currentMode = EditorMode::RotateMode;    break;
        case Key::R: m_currentMode = EditorMode::ScaleMode;     break;
    }
}

void Editor2D::OnMouseMoved(MouseMovedEvent& event)
{
    m_cursorPosition.x = event.GetX();
    m_cursorPosition.y = event.GetY();
}

void Editor2D::OnMouseButtonPressed(MouseButtonPressedEvent& event)
{
    if (m_currentMode != EditorMode::ViewMode && event.GetMouseButton() == Mouse::ButtonLeft)
    {
        std::cout << "Check2DPicking at (" << m_cursorPosition.x << ", " << m_cursorPosition.y << ")" << std::endl;
        
        glm::vec2 ndcPosition = ConvertToNDC(m_cursorPosition, m_camera->GetScreenSize());

        glm::mat4 ivProjection = glm::inverse(m_camera->GetProjectionMatrix());
        glm::mat4 ivView = glm::inverse(m_camera->GetViewMatrix());

        glm::vec2 worldCursor = ConvertToWorldSpace(ndcPosition, ivProjection, ivView);
        std::cout << "worldCursor: " << worldCursor.x << ", " << worldCursor.y << std::endl;

        auto transforms = m_entityManager.m_registry.view<TransformComponent>();
        for (auto [entity, transform] : transforms.each())
        {
            if (worldCursor.x < transform.position.x - transform.scale.x / 2 ||
                worldCursor.x > transform.position.x + transform.scale.x / 2 ||
                worldCursor.y < transform.position.y - transform.scale.y / 2 ||
                worldCursor.y > transform.position.y + transform.scale.y / 2 )
            {
                m_pickedEntity = entt::entity(1000);
                continue;
            }

            std::cout << "PICKED" << std::endl;
            m_pickedEntity = entity;
            break;
        }


    }
}

void Editor2D::Update()
{
}

void Editor2D::Render()
{
    m_window->PreRender();

    auto transforms = m_entityManager.m_registry.view<TransformComponent>();

    for (auto [entity, transform] : transforms.each())
    {
        glm::vec2 position = transform.position;
        glm::vec2 scale = transform.scale;
        if (entity == m_pickedEntity)
            m_renderer->DrawQuad2D(position, scale, 0.0f, {0.8f, 0.8f, 0.2f, 1.0f});
        else
            m_renderer->DrawQuad2D(position, scale);
    }
    

    if (m_currentMode != EditorMode::ViewMode)
    {
        m_gizmos.at(m_currentMode - 1)->Render(*m_renderer);
    }

    m_window->Render();
}