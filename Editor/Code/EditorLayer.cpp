#include "EditorLayer.h"
#include "Platforms/OpenGL/OpenGLWindow.h"
#include "Core/Camera2D.h"
#include "Math/Math.h"
#include "Gizmos/ViewGizmo.h"
#include "Gizmos/TranslateGizmo.h"
#include "Gizmos/RotateGizmo.h"
#include "Gizmos/ScaleGizmo.h"
#include "Utils/FileUtils.h"
#include "Utils/FileDialog.h"
#include <iostream>
#include "Input/NewInput.h"
#include "Core/ResourceAPI.h"
#include "Lua/LuaEngine.h"

EditorLayer::EditorLayer(EditorApplication& app, std::unique_ptr<Project> project)
    : m_app(app)
    , m_activeProject(std::move(project))
    , m_entityProperties(*this)
    , m_hierarchy(*this)
    , m_mainMenuBar(*this)
    , m_assetPanel(*this)
{
    std::cout << "EditorLayer Constructor()\n\n";

    LuaEngine& luaEngine = m_app.GetLuaEngine();

    std::filesystem::path path = m_activeProject->GetDirectory() / (m_activeProject->GetName() + ".lua");
    luaEngine.ReadScript(path.string());
    /*
    m_activeScene = std::make_unique<Scene>();
    m_activeScene->SetApplication((Application*)&m_app);
    m_activeScene->Initialize();
    */
    std::cout << "Start Scene: " << m_activeProject->GetStartScene() << std::endl;
    OpenScene(m_activeProject->GetStartScene());
}

EditorLayer::EditorLayer(EditorApplication& app, std::unique_ptr<Scene> scene)
	: m_app(app)
    , m_activeScene(std::move(scene))
    , m_entityProperties(*this)
    , m_hierarchy(*this)
    , m_mainMenuBar(*this)
    , m_assetPanel(*this)
{
	std::cout << "EditorLayer Constructor()\n\n";

}

void EditorLayer::Initialize()
{
	std::cout << "EditorLayer Initialize()\n\n";

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
    
    if (m_activeProject != nullptr)
    {
        m_assetPanel.SetProjectDirectory(m_activeProject->GetDirectory());
    }

    /*
    m_activeScene = std::make_unique<Scene>();
    m_activeScene->SetInitializeFunction([](Scene& scene)
    {
        scene.m_camera = std::make_unique<Camera2D>(
            glm::vec3 { 0.0f, 0.0f, -1.0f },
            glm::vec2 { 1.0f, 1.0f },
            glm::vec2 { 1280, 720 }
        );

        Renderer& renderer = scene.m_app->GetRenderer();
        renderer.SetCamera(scene.m_camera.get());

        Entity camera = scene.m_manager.CreateEntity();
        camera.AddComponent<BaseComponent>("Main Camera");
        camera.AddComponent<TransformComponent>(glm::vec3 { 0.0f, 0.0f, -1.0f }, glm::vec3 {0.0f}, glm::vec3 {1.0f});
        camera.AddComponent<CameraComponent>();

        Entity rect = scene.m_manager.CreateEntity();
        rect.AddComponent<BaseComponent>("Rect");
        rect.AddComponent<TransformComponent>(glm::vec3 {0.0f, 0.0f, 0.0f}, glm::vec3 {0.0f}, glm::vec3 {1.0f, 1.0f, 1.0f});
        rect.AddComponent<SpriteRendererComponent>(Shape_Square);
    });
    */

    //m_imgui = std::make_unique<ImGuiMain>(*this);
}

bool EditorLayer::OpenScene(std::filesystem::path path)
{
    std::cout << "OpenScene: " << path << std::endl;
    bool success = true;

    m_activeScene = std::make_unique<Scene>();

    if (!SceneSerializer::Deserialize(*m_activeScene, path))
    {
        m_activeScene = Scene::CreateDefaultScene(m_activeProject->GetDirectory() / "Scenes");
        success = false;
    }

    m_activeScene->SetApplication(&m_app);
    m_activeScene->Initialize();

    OnSceneUpdate();

    return success;
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

        case Key::Delete:
        {
            if (m_entityPicked)
                DeleteEntity(m_pickedEntity);
        }
        break;
    }
}


void EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& event)
{
    std::cout << "OnMouseButtonPressed" << std::endl;

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
    auto transforms = m_activeScene->GetEntityManager().m_registry.view<TransformComponent>();
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
    m_nativeScriptEngine.ShutdownScriptableEntities();
}

TransformComponent* EditorLayer::GetEntityTransform()
{
    if (!m_entityPicked)
        return nullptr;

    return &m_activeScene->GetEntityManager().m_registry.get<TransformComponent>(m_pickedEntity);
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

        if (m_activeScene != nullptr)
        {
            m_activeScene->SetCamera(m_camera.get());
            m_activeScene->Render();

            if (m_currentMode != EditorMode::ViewMode && m_entityPicked)
            {
                auto& transform = m_activeScene->GetEntityManager().m_registry.get<TransformComponent>(m_pickedEntity);
                m_gizmos.at(m_currentMode)->Render(renderer, transform.position);
            }
        }
    }
}

void EditorLayer::RenderImGui()
{
    m_entityProperties.Render();
    m_hierarchy.Render();
    m_assetPanel.Render();
    m_mainMenuBar.Render();
    //m_nodeEditor.Render();
}

void EditorLayer::OnWindowResize(WindowResizeEvent& event)
{
    m_camera->SetScreenSize({ event.GetWidth() , event.GetHeight() });
}

void EditorLayer::PlayScene()
{    
    if (m_activeScene != nullptr)
    {
        m_playingScene = Scene::Copy(*m_activeScene);
        m_playingScene->SetApplication(&m_app);
        m_playingScene->StartNativeScripts(m_nativeScriptEngine);
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
        m_camera->SetZoom(m_camera->GetZoom() * (1.0f + event.GetY() * 0.1f));
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
    if (event.GetType() == EventType::WindowClose)
        m_app.OnEvent(event);

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

        case EventType::KeyReleased:
        {
            std::cout << "Key Released\n";
            break;
        }
    }
}

void EditorLayer::DeleteEntity(entt::entity entity)
{
    if (m_activeScene->m_manager.m_registry.valid(entity))
    {
        std::cout << "Delete entity: " << (int)entity << std::endl;

        if (m_pickedEntity == entity)
        {
            m_pickedEntity = entt::null;
            m_entityPicked = false;
        }

        m_activeScene->m_manager.m_registry.destroy(entity);
    }
}

bool EditorLayer::NewScene()
{
    std::cout << "New Scene\n";

    m_activeScene = Scene::CreateDefaultScene("");
    m_activeScene->SetApplication(&m_app);
    m_activeScene->Initialize();

    return true;
}

void EditorLayer::OnSceneUpdate()
{
    std::cout << "OnSceneUpdate: " << m_activeScene->m_name << std::endl;
    std::string windowTitle = m_activeProject->GetName() + " - " + m_activeScene->m_name + " - BossFight Engine";
    m_app.GetWindow().SetTitle(windowTitle);
}

bool EditorLayer::SaveScene()
{
    if (m_activeScene->HasSaved())
    {
        std::cout << "Saving the Scene..." << std::endl;
        SceneSerializer::Serialize(*m_activeScene, m_activeScene->m_path);
    }
    else
    {
        std::cout << "Has not Saved Yet!!!\n";
        return SaveSceneAs();
    }

    return true;
}

bool EditorLayer::SaveSceneAs()
{
    std::string saveDirectory = FileDialog::SaveFileDialog("BossFight Scene Files (.bfs)\0.bfs\0");

    if (saveDirectory.size() == 0)
        return false;

    std::filesystem::path savePath = saveDirectory;
    std::string sceneName = savePath.filename().string();
    savePath += SCENE_FILE_EXTENSION;

    std::cout << "Save Scene as: " << savePath << std::endl;
    
    if (!SceneSerializer::Serialize(*m_activeScene, savePath))
        return false;

    m_activeScene->m_name = sceneName;
    m_activeScene->m_path = savePath;

    OnSceneUpdate();

    return true;
}

void EditorLayer::ReloadNativeScripts()
{
    std::filesystem::path projectPath = m_activeProject->GetDirectory();

    if (!std::filesystem::exists(projectPath / "Scripts"))
        return;

    std::vector<std::string> scriptClassNames;

    for (auto& directoryEntry : std::filesystem::directory_iterator(m_activeProject->GetDirectory() / "Scripts"))
    {
        const auto& path = directoryEntry.path();
        std::string filenameString = path.filename().string();

        if (!directoryEntry.is_directory() && path.extension() == ".h")
        {
            std::cout << "Reloaded class: " << path.stem().string() << std::endl;
            scriptClassNames.push_back(path.stem().string());
        }
    }

    m_nativeScriptEngine.SetClassNames(scriptClassNames);

    if (m_nativeScriptEngine.LoadNativeScripts(projectPath / "bin" / "Debug" / "Native-Script.dll", m_nativeClassNames))
    {
        std::cout << "Success: Load native scripts\n";
        m_nativeScriptEngine.InitializeScriptableEntities();
    }
    else
    {
        std::cout << "Error: Load native scripts\n";
    }


}
