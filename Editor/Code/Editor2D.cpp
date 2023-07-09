#include "Editor2D.h"
#include "Platforms/OpenGL/OpenGLWindow.h"
#include "Platforms/OpenGL/OpenGLRenderer.h"
#include "Gizmos/TranslateGizmo.h"
#include "Gizmos/RotateGizmo.h"
#include "Gizmos/ScaleGizmo.h"
#include "Core/Camera2D.h"
#include "Events/KeyEvent.h"

void Editor2D::Initialize(std::string title, int width, int height)
{
	m_window = std::make_unique<OpenGLWindow>(title, width, height);

    m_window->SetEventCallback([this](Event& event) {
        this->OnEvent(event);
    });

    m_camera = std::make_unique<Camera2D>(
        glm::vec3 { 0.0f, 0.0f, -14.0f },
        glm::vec2 { 0.5f, 0.25f },
        glm::vec2 { 1280, 720 }
    );

    m_renderer = std::make_unique<OpenGLRenderer>();
    m_renderer->Initialize();
    m_renderer->SetCamera(m_camera.get());

    m_gizmos.push_back(std::make_unique<TranslateGizmo>());
    m_gizmos.push_back(std::make_unique<RotateGizmo>());
    m_gizmos.push_back(std::make_unique<ScaleGizmo>());

	Input::Init();
}

void Editor2D::OnEvent(Event& event)
{
    std::cout << "New Event: " << event.ToString() << std::endl;


    if (event.GetType() == EventType::KeyPressed)
    {
        std::cout << "KeyPressed Event" << std::endl;

        KeyPressedEvent* keyPressed = dynamic_cast<KeyPressedEvent*>(&event);

        switch (keyPressed->GetKeyCode())
        {
            case Key::Q: m_currentMode = EditorMode::ViewMode;      break;
            case Key::W: m_currentMode = EditorMode::TranslateMode; break;
            case Key::E: m_currentMode = EditorMode::RotateMode;    break;
            case Key::R: m_currentMode = EditorMode::ScaleMode;     break;
        }
    }

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

}

void Editor2D::Update()
{
}

void Editor2D::Render()
{
    m_window->PreRender();


    if (m_currentMode != EditorMode::ViewMode)
    {
        m_gizmos.at(m_currentMode - 1)->Render(*m_renderer);
    }


    m_window->Render();
}