#include "EditorApplication.h"
#include "Platforms/OpenGL/OpenGLWindow.h"
#include "Platforms/OpenGL/OpenGLRenderer.h"
#include "Core/Timer.h"
#include "Components/EntityListWindow.h"
#include "Components/InspectorWindow.h"
#include "GameLayer.h"
#include "EditorLayer.h"
#include "Core/Camera2D.h"

EditorApplication::EditorApplication()
{
}

EditorApplication::~EditorApplication()
{
}

void EditorApplication::Initialize(std::string title, int width, int height)
{
    m_window = std::make_unique<OpenGLWindow>(this, title, width, height);

    m_renderer = std::make_unique<OpenGLRenderer>();
    m_renderer->Initialize();

    Input::Init();

    m_activeScene = std::make_unique<Scene>();
    m_activeScene->SetInitializeFunction([](Scene* scene)
    {
        scene->m_camera = std::make_unique<Camera2D>(
            glm::vec3 { 0.0f, 0.0f, -1.0f },
            glm::vec2 { 1.0f, 1.0f },
            glm::vec2 { 1280, 720 }
        );

        Renderer& renderer = scene->m_app->GetRenderer();
        renderer.SetCamera(scene->m_camera.get());

        Entity camera = scene->m_manager.CreateEntity();
        camera.AddComponent<BaseComponent>("Main Camera");
        camera.AddComponent<TransformComponent>(glm::vec3 { 0.0f, 0.0f, -1.0f }, glm::vec3 {0.0f}, glm::vec3 {1.0f});
        camera.AddComponent<CameraComponent>();

        Entity rect = scene->m_manager.CreateEntity();
        rect.AddComponent<BaseComponent>("Rect");
        rect.AddComponent<TransformComponent>(glm::vec3 {0.0f, 0.0f, 0.0f}, glm::vec3 {0.0f}, glm::vec3 {1.0f, 1.0f, 1.0f});
        rect.AddComponent<SpriteRendererComponent>(Shape_Square);
    });

    m_activeScene->SetApplication(this);
    m_activeScene->Initialize();

    AddLayer(std::make_unique<EditorLayer>(*this));
}

