#include "Editor2D.h"
#include "Platforms/OpenGL/OpenGLWindow.h"
#include "Platforms/OpenGL/OpenGLRenderer.h"
#include "Gizmos/ViewGizmo.h"
#include "Gizmos/TranslateGizmo.h"
#include "Gizmos/RotateGizmo.h"
#include "Gizmos/ScaleGizmo.h"
#include "Core/Camera2D.h"
#include "Events/KeyEvent.h"
#include "Core/Entity.h"
#include "Components/Components.h"
#include "SampleScene.h"
#include "Core/Timer.h"
#include "Project/Project.h"

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
	m_window = std::make_unique<OpenGLWindow>(this, title, width, height);

    m_window->SetEventCallback([this](Event* event) {
        //this->OnEvent(event);
    });

    m_camera = std::make_unique<Camera2D>(
        glm::vec3 { 0.0f, 0.0f, -1.0f },
        glm::vec2 { 1.0f, 1.0f },
        glm::vec2 { 1280, 720 }
    );



    m_renderer = std::make_unique<OpenGLRenderer>();
    m_renderer->Initialize();

    m_gizmos.push_back(std::make_unique<ViewGizmo>(*this));
    m_gizmos.push_back(std::make_unique<TranslateGizmo>(*this));
    m_gizmos.push_back(std::make_unique<RotateGizmo>(*this));
    m_gizmos.push_back(std::make_unique<ScaleGizmo>(*this));

    if (OpenGLWindow* openGLWindow = dynamic_cast<OpenGLWindow*>(m_window.get()))
        m_imgui = std::make_unique<ImGuiMain>(*this, openGLWindow->GetGLFWwindow(), "#version 330");

	Input::Init();
    
    m_camera->SetCameraType(CameraType::Orthographic);
    
    
    //m_scene = std::make_unique<SampleScene>();
    m_scene = std::make_unique<Scene>();
    m_scene->SetInitializeFunction([](Scene* scene)
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


    m_scene->SetApplication(this);
    m_scene->Initialize();
}

void Editor2D::OnEvent(Event& event)
{
    //m_events.push_back(std::unique_ptr<Event>(event));
}

void Editor2D::ProcessInput()
{
    m_window->ProcessInput();

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
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantTextInput)
        return;

    switch (event.GetKeyCode())
    {
        case Key::Q:
        {
            m_window->SetCursorShape(CursorShape::Hand);
            m_currentMode = EditorMode::ViewMode;
        }
        break;

        case Key::W:
        {
            m_window->SetCursorShape(CursorShape::Arrow);
            m_currentMode = EditorMode::TranslateMode;
        }
        break;

        case Key::E:
        {
            m_window->SetCursorShape(CursorShape::Arrow);
            m_currentMode = EditorMode::RotateMode;
        }
        break;

        case Key::R:
        {
            m_window->SetCursorShape(CursorShape::Arrow);
            m_currentMode = EditorMode::ScaleMode;
        }
        break;

        case Key::S: SaveScene(); break;
        case Key::O: OpenScene(); break;
    }
}

void Editor2D::OpenScene()
{
    std::cout << "Opening the Scene..." << std::endl;
    SceneSerializer serializer(*m_scene);
    serializer.Deserialize("Untitled.scene.json");
}

void Editor2D::SaveScene()
{
    std::cout << "Saving the Scene..." << std::endl;
    SceneSerializer serializer(*m_scene);
    serializer.Serialize("Untitled.scene.json");
    /*
    Scene scene;
    SceneSerializer serializer(scene);
    serializer.Serialize("Untitled.scene.json");
    */
}

void Editor2D::OnWindowResize(int width, int height)
{
    std::cout << "OnWindowResize: " << width << "," << height << std::endl;
    m_camera->SetScreenSize({ width, height });
}

void Editor2D::OnWindowClose()
{
    m_running = false;
}

void Editor2D::OnMouseMoved(float x, float y)
{
    m_cursorPosition.x = x;
    m_cursorPosition.y = y;

    CalculateWorldCursorPosition();

    if (m_mouseActive)
        m_gizmos[m_currentMode]->OnDragging(m_worldCursorPosition.x, m_worldCursorPosition.y);
}

void Editor2D::OnMouseButtonPressed(int button)
{
    std::cout << "OnMouseButtonPressed" << std::endl;

    ImGuiIO& io = ImGui::GetIO();

    if (io.WantCaptureMouse)
        return;

    if (button == Mouse::ButtonLeft)
    {
        m_mouseActive = true;

        CalculateWorldCursorPosition();

        if (!m_gizmos[m_currentMode]->OnPicking2D(m_worldCursorPosition))
            m_entityPicked = CheckPicking2D();
    }
}

void Editor2D::OnMouseButtonReleased(int button)
{
    std::cout << "OnMouseButtonReleased" << std::endl;

    if (button == Mouse::ButtonLeft)
    {
        m_mouseActive = false;
    }
}


void Editor2D::CalculateWorldCursorPosition()
{
    glm::vec2 ndcPosition = ConvertToNDC(m_cursorPosition, m_camera->GetScreenSize());
    glm::mat4 ivProjection = glm::inverse(m_camera->GetProjectionMatrix());
    glm::mat4 ivView = glm::inverse(m_camera->GetViewMatrix());
    m_worldCursorPosition = ConvertToWorldSpace(ndcPosition, ivProjection, ivView);
}

void Editor2D::Update()
{
    float dt = Timer::GetDeltaTime();

    if (m_scenePlaying)
        m_playingScene->Update(dt);
    else
        m_gizmos[m_currentMode]->Update(dt);

}

bool Editor2D::CheckPicking2D()
{
    auto transforms = m_scene->GetEntityManager().m_registry.view<TransformComponent>();
    for (auto [entity, transform] : transforms.each())
    {
        /*
        if (m_worldCursorPosition.x < transform.position.x - transform.scale.x / 2 ||
            m_worldCursorPosition.x > transform.position.x + transform.scale.x / 2 ||
            m_worldCursorPosition.y < transform.position.y - transform.scale.y / 2 ||
            m_worldCursorPosition.y > transform.position.y + transform.scale.y / 2)
        {
            continue;
        }
        */

        if (!Shape2D::IsPointOnRectangle(m_worldCursorPosition, transform.position, transform.scale, transform.rotation.z))
            continue;

        std::cout << "PICKED" << std::endl;
        m_pickedEntity = entity;

        return true;
    }

    return false;
}

void Editor2D::OnMouseScroll(float x, float y)
{
    std::cout << "Scroll: " << x << ", " << y << std::endl;

    ImGuiIO& io = ImGui::GetIO();

    if (io.WantCaptureMouse)
        return;

    if (m_camera->GetCameraType() == CameraType::Orthographic)
    {
        m_camera->SetZoom(m_camera->GetZoom() + (y * 0.04f));
    }
    else if (m_camera->GetCameraType() == CameraType::Perspective)
    {
        glm::vec3 newPosition = m_camera->GetPosition();
        newPosition.z += y * 0.5f;
        m_camera->SetPosition(newPosition);
    }
}

TransformComponent* Editor2D::GetEntityTransform()
{
    if (!m_entityPicked)
        return nullptr;

    return &m_scene->GetEntityManager().m_registry.get<TransformComponent>(m_pickedEntity);
}

void Editor2D::Render()
{
    m_window->PreRender();

    //m_window->GetWindowData();
    if (!m_scenePlaying)
    {
        glm::vec2 screenSize = m_camera->GetScreenSize();
        m_renderer->SetCamera(m_camera.get());

        m_renderer->SetViewMatrix(m_camera->GetViewMatrix());
        m_renderer->SetProjectionMatrix(m_camera->GetProjectionMatrix());

        auto transforms = m_scene->GetEntityManager().m_registry.view<TransformComponent, SpriteRendererComponent>();

        for (auto [entity, transform, sprite] : transforms.each())
        {
            glm::vec2 position = transform.position;
            glm::vec2 scale = transform.scale;
            float angle = transform.rotation.z;

            if (sprite.shape == SpriteShape::Shape_Square)
                m_renderer->DrawQuad2D(position, scale, angle, sprite.color);
            else if (sprite.shape == SpriteShape::Shape_Circle)
            {
                Circle2D circle;
                circle.color = sprite.color;
                circle.position = transform.position;
                circle.scale = transform.scale;

                m_renderer->DrawCircle2D(circle);
            }
        }

        if (m_currentMode != EditorMode::ViewMode && m_entityPicked)
        {
            auto& transform = m_scene->GetEntityManager().m_registry.get<TransformComponent>(m_pickedEntity);
            m_gizmos.at(m_currentMode)->Render(*m_renderer, transform.position);
        }

        m_imgui->Render();
    }
    else
    {
        auto view = m_playingScene->GetEntityManager().m_registry.view<TransformComponent, CameraComponent>();
        for (auto [entity, transform, camera] : view.each())
        {   
            WindowData window = m_playingScene->m_app->GetWindow().GetWindowData();
            float ratio = window.width / (float)window.height;
            float width = camera.size * ratio;

            m_renderer->SetViewMatrix(glm::inverse(glm::translate(glm::mat4(1.0f), transform.position)));
            m_renderer->SetProjectionMatrix(glm::ortho(-width, width, -camera.size, camera.size));
        }

        auto transforms = m_playingScene->GetEntityManager().m_registry.view<TransformComponent, SpriteRendererComponent>();

        for (auto [entity, transform, sprite] : transforms.each())
        {
            /*
            glm::vec2 position = transform.position;
            glm::vec2 scale = transform.scale;
            float angle = transform.rotation.z;

            m_renderer->DrawQuad2D(position, scale, angle, sprite.color);
            */

            glm::vec2 position = transform.position;
            glm::vec2 scale = transform.scale;
            float angle = transform.rotation.z;

            if (sprite.shape == SpriteShape::Shape_Square)
                m_renderer->DrawQuad2D(position, scale, angle, sprite.color);
            else if (sprite.shape == SpriteShape::Shape_Circle)
            {
                Circle2D circle;
                circle.color = sprite.color;
                circle.position = transform.position;
                circle.scale = transform.scale;

                m_renderer->DrawCircle2D(circle);
            }
        }
        //m_scene->Render();
        m_imgui->Render();
    }


    m_window->Render();
}

void Editor2D::PlayScene()
{
    std::cout << "Play Scene\n";

    m_playingScene = Scene::Copy(*m_scene);
    m_playingScene->SetApplication(this);
    m_playingScene->Start();
    m_scenePlaying = true;
}

void Editor2D::StopScene()
{
    std::cout << "Stop Scene\n";
    m_playingScene->Stop();
    m_playingScene.reset();
    m_scenePlaying = false;
}

void Editor2D::NewProject()
{
    Project::New();
}

bool Editor2D::OpenProject()
{
    /*
    std::string filepath = FileDialogs::OpenFile("Hazel Project (*.hproj)\0*.hproj\0");
    if (filepath.empty())
        return false;

    OpenProject(filepath);
    */

    return true;
}

void Editor2D::SaveProject()
{
    Project::SaveActive();
}