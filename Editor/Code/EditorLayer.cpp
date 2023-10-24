#include "EditorLayer.h"
#include "Graphics/Camera2D.h"
#include "Gizmos/ViewGizmo.h"
#include "Gizmos/TranslateGizmo.h"
#include "Gizmos/RotateGizmo.h"
#include "Gizmos/ScaleGizmo.h"
#include "File/FileSystem.h"
#include "Utils/FileDialog.h"
#include <iostream>
#include "Input/Input.h"
#include "Lua/LuaEngine.h"
#include "File/FileSystem.h"
#include "File/MetaSerializer.h"
#include "EditorGUI/Windows/ImGuiSelectSpriteWindow.h"
#include "EditorGUI/Windows/ImGuiSelectAnimatorControllerWindow.h"
#include "Scene/SceneSerializer.h"
#include "Scene/SceneManager.h"
#include "Asset/AssetManager.h"
#include "Project/ProjectSerializer.h"

EditorLayer* EditorLayer::s_instance = nullptr;

EditorLayer::EditorLayer(EditorApplication& app, std::unique_ptr<Project> project)
    : m_app(app)
    , m_activeProject(std::move(project))
    , m_entityProperties(*this)
    , m_hierarchy(*this)
    , m_mainMenuBar(*this)
    , m_assetPanel(*this)
    , m_gameLayer(app)
{
    s_instance = this;

    m_fileWatcher = std::make_unique<filewatch::FileWatch<std::string>>(
        (m_activeProject->GetDirectory()).string(),
        [&](const std::string& path, const filewatch::Event change_type)
        {
            std::filesystem::path absolutePath = m_activeProject->GetDirectory() / path;
            std::lock_guard<std::mutex> lock(m_fileEventMutex);
            m_fileEvents.push_back({ absolutePath, change_type });
        }
    );

    std::filesystem::path luaFilePath = m_activeProject->GetDirectory() / (m_activeProject->GetName() + ".lua");

    if (FileSystem::FileExists(luaFilePath))
        m_app.GetLuaEngine().ReadScript(luaFilePath.string());

    m_app.GetAssetManager().InitializeAssets(m_activeProject->GetDirectory());

    m_imGuiWindowMap[ImGuiWindowType::SelectSprite] = std::make_unique<ImGuiSelectSpriteWindow>(*this, m_activeProject->GetDirectory());
    m_imGuiWindowMap[ImGuiWindowType::SelectAnimatorController] = std::make_unique<ImGuiSelectAnimatorControllerWindow>(*this, m_activeProject->GetDirectory());

    OpenScene(m_activeProject->GetStartScene());

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void EditorLayer::OnFileEvent(const FileEvent& event)
{
    switch (event.type)
    {
        case filewatch::Event::added:
        {
            std::cout << "File Added at: " << event.path << std::endl;

            if (FileSystem::IsImageFile(event.path))
            {
                std::cout << "Handle Image File!!!\n";
                FileSystem::GenerateImageMetaFile(event.path);

                m_app.GetAssetManager().LoadTexture(event.path.string(), event.path.string().c_str(), true);
            }
        }
        break;

        case filewatch::Event::removed:
        {
            std::cout << "File Removed at: " << event.path << std::endl;
            m_app.GetAssetManager().RemoveTexture(event.path.string());
        }
        break;
    }
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
        glm::vec2 { m_app.GetWindow().GetWidth(), m_app.GetWindow().GetHeight() }
    );

    m_camera->SetCameraType(CameraType::Orthographic);
    
    if (m_activeProject != nullptr)
        m_assetPanel.SetProjectDirectory(m_activeProject->GetDirectory());
}

bool EditorLayer::OpenScene(std::filesystem::path absolutePath)
{
    std::cout << "OpenScene: " << absolutePath << std::endl;
    bool success = true;

    m_activeScene = m_activeProject->LoadScene(absolutePath);

    if (m_activeScene == nullptr)
    {
        m_activeScene = SceneManager::CreateDefaultScene(m_activeProject->GetDirectory() / "Scenes");
        success = false;
    }

    SceneManager::ResolveUniqueIDs(*m_activeScene);
    ReloadNativeScripts();

    m_activeScene->SetApplication(&m_app);
    m_activeScene->Initialize();

    OnSceneUpdate();

    return success;
}

Entity EditorLayer::GetSelectedEntity()
{
    return Entity { &m_activeScene->m_manager, m_selectedObject.entity };
}

void EditorLayer::CreatePrefab(entt::entity entity, const std::filesystem::path& path)
{
    if (m_activeScene)
    {
        SceneSerializer::SerializeEntity(*m_activeScene, entity, path);
    }
}

void EditorLayer::SetPickedEntity(entt::entity picked)
{
    m_selectedObject.type = EditorObjectType::Entity;
    m_selectedObject.entity = picked;
    m_selectedObject.asset = nullptr;
}

void EditorLayer::SetSelectedAsset(Asset* asset, const std::string& note)
{
    m_selectedObject.type = EditorObjectType::Path;
    m_selectedObject.asset = asset;
    m_selectedObject.entity = entt::null;
    m_selectedObject.note = note;
}

void EditorLayer::SetSelectedPath(const std::filesystem::path& path, const std::string& note)
{
    m_selectedObject.type = EditorObjectType::Path;
    m_selectedObject.asset = GetAsset(path);
    m_selectedObject.entity = entt::null;
    m_selectedObject.note = note;
}

Asset* EditorLayer::GetAsset(const std::filesystem::path& path)
{
    FileSystem::HandleMetaFile(path);

    AssetManager& assetManager = s_instance->m_app.GetAssetManager();
    
    Asset* asset = assetManager.GetAsset(path);

    if (asset == nullptr)
        asset = assetManager.LoadAsset(path);

    return asset;
}

void EditorLayer::UnselectObject()
{
    m_selectedObject.type = EditorObjectType::None;
    m_selectedObject.entity = entt::null;
    m_selectedObject.asset = nullptr;
    m_selectedObject.note = "";
}

void EditorLayer::OnMouseMoved(MouseMovedEvent& event)
{
    m_cursorPosition.x = event.GetX();
    m_cursorPosition.y = event.GetY();

    m_worldCursorPosition = m_camera->ScreenToWorldPosition(m_viewportCursorPosition);

    if (m_mouseActive)
    {
        bool dragging = m_gizmos[m_currentMode]->OnDragging(m_worldCursorPosition.x, m_worldCursorPosition.y);

        if (dragging && !m_gizmoStatus.dragging)
        {
            m_gizmos[m_currentMode]->OnDraggingStart();
            m_gizmoStatus.dragging = true;
            m_gizmoStatus.mode = m_currentMode;
        }
    }
}

void EditorLayer::OnKeyPressed(KeyPressedEvent& event)
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantTextInput)
        return;

    std::cout << "On Key PRessed" << std::endl;

    bool control = m_app.GetInput().GetKey(Key::LeftControl) || m_app.GetInput().GetKey(Key::RightControl);

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

        case Key::D:
        {
            if (control)
                DuplicateEntity();
        }
        break;

        case Key::Z:
        {
            if (control)
                UndoCommand();
        }
        break;

        case Key::Y:
        {
            if (control)
                RedoCommand();
        }
        break;

        case Key::Delete:
        {
            if (m_selectedObject.type == EditorObjectType::Entity)
                DeleteEntity(m_selectedObject.entity);
        }
        break;
    }
}

void EditorLayer::AddCommand(EditorCommand* command)
{
    if (command)
    {
        int k = m_editorCommands.size() - 1;
        if (m_currentCommandIndex < k)
            m_editorCommands.erase(m_editorCommands.begin() + (m_currentCommandIndex + 1), m_editorCommands.begin() + m_editorCommands.size());

        m_editorCommands.emplace_back(std::unique_ptr<EditorCommand>(command));
        m_currentCommandIndex = m_editorCommands.size() - 1;

        std::cout << "Add Command: index: " << m_currentCommandIndex << ", size: " << m_editorCommands.size() << std::endl;
    }
}

void EditorLayer::RedoCommand()
{
    int k = m_editorCommands.size() - 1;
    if (m_currentCommandIndex < k)
    {
        m_editorCommands[++m_currentCommandIndex]->Redo();
        std::cout << "Redo Command: index: " << m_currentCommandIndex << ", size: " << m_editorCommands.size() << std::endl;
    }
}

void EditorLayer::UndoCommand()
{
    if (m_currentCommandIndex >= 0 && m_editorCommands.size() > m_currentCommandIndex)
    {
        m_editorCommands[m_currentCommandIndex--]->Undo();

        std::cout << "Undo Command: index: " << m_currentCommandIndex << ", size: " << m_editorCommands.size() << std::endl;
    }
}


void EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& event)
{
    std::cout << "OnMouseButtonPressed" << std::endl;

    if (event.GetMouseButton() == Mouse::ButtonLeft)
    {
        m_mouseActive = true;
        m_worldCursorPosition = m_camera->ScreenToWorldPosition(m_viewportCursorPosition);

        if (!m_gizmos[m_currentMode]->OnPicking2D(m_worldCursorPosition))
        {
            if (!CheckPicking2D())
                UnselectObject();
        }
    }
}

void EditorLayer::OnMouseButtonReleased(MouseButtonReleasedEvent& event)
{
    std::cout << "OnMouseButtonReleased" << std::endl;

    if (event.GetMouseButton() == Mouse::ButtonLeft)
    {
        m_mouseActive = false;

        if (m_gizmoStatus.dragging)
        {
            m_gizmos[m_currentMode]->OnDraggingEnd();
            m_gizmoStatus.dragging = false;
        }
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

        SetPickedEntity(entity);

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
    if (m_selectedObject.type != EditorObjectType::Entity)
        return nullptr;

    return m_activeScene->GetEntityManager().m_registry.try_get<TransformComponent>(m_selectedObject.entity);
}


void EditorLayer::Update(float deltaTime)
{
    Renderer& renderer = m_app.GetRenderer();

    std::lock_guard<std::mutex> lock(m_fileEventMutex);

    for (auto& e : m_fileEvents)
        m_fileHandler.OnFileEvent(e);
    m_fileEvents.clear();

    if (m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f)
    {
        renderer.RescaleFramebuffer(m_viewportSize.x, m_viewportSize.y);
        m_camera->SetScreenSize(m_viewportSize);
    }

    renderer.BindFramebuffer();
    renderer.Clear({ 0.2f, 0.2f, 0.2f, 1.0f });

    if (m_scenePlaying)
    {
        if (Scene* playingScene = m_gameLayer.GetCurrentScene())
        {
            //playingScene->UpdateUI(deltaTime);
            playingScene->Update(deltaTime);
            playingScene->SetViewportSize(m_viewportSize.x, m_viewportSize.y);
            playingScene->OnViewportResize();
            playingScene->Render();
        }
    }
    else
    {
        m_gizmos[m_currentMode]->Update(deltaTime);

        if (m_activeScene != nullptr)
        {
            //m_activeScene->UpdateUI(deltaTime);
            m_activeScene->SetCamera(*m_camera);
            m_activeScene->SetViewportSize(m_viewportSize.x, m_viewportSize.y);
            m_activeScene->RenderEditor();

            if (m_currentMode != EditorMode::ViewMode && m_selectedObject.type == EditorObjectType::Entity)
            {
                auto& transform = m_activeScene->GetEntityManager().m_registry.get<TransformComponent>(m_selectedObject.entity);
                m_gizmos.at(m_currentMode)->Render(renderer, transform.position);
            }
        }
    }

    renderer.UnbindFramebuffer();
}


void EditorLayer::RenderImGui()
{
    // Note: Switch this to true to enable dockspace
    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 370.0f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    style.WindowMinSize.x = minWinSizeX;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("Scene");

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

    m_viewportFocused = ImGui::IsWindowFocused();
    m_viewportHovered = ImGui::IsWindowHovered();

    ImVec2 mousePos = ImGui::GetMousePos();
    ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();
    m_viewportCursorPosition = { mousePos.x - cursorScreenPos.x, mousePos.y - cursorScreenPos.y };
    auto& input = m_app.GetInput();
    input.SetCursorPosition(m_viewportCursorPosition.x, m_viewportCursorPosition.y);

    uint64_t textureID = m_app.GetRenderer().GetFramebufferTextureID();
    ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_viewportSize.x, m_viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

    ImGui::End();
    ImGui::PopStyleVar();

    if (!m_scenePlaying)
    {
        m_entityProperties.Render();
        m_hierarchy.Render();
        m_assetPanel.Render();
    }

    m_mainMenuBar.Render();
    //m_nodeEditor.Render();

    ImGui::End();
}

void EditorLayer::OnWindowResize(WindowResizeEvent& event)
{
    m_camera->SetScreenSize({ event.GetWidth() , event.GetHeight() });
}

void EditorLayer::PlayScene()
{
    if (m_activeScene != nullptr)
    {
        Scene* playingScene = SceneManager::Copy(*m_activeScene);
        playingScene->SetApplication(&m_app);
        playingScene->StartNativeScripts(m_app.GetNativeScriptEngine());
        playingScene->Start();

        m_gameLayer.AddScene(m_activeScene->m_name, playingScene);
        m_gameLayer.ChangeScene(m_activeScene->m_name);

        m_scenePlaying = true;
    }
}

void EditorLayer::StopScene()
{
    m_gameLayer.DestroyAllScenes();
    m_scenePlaying = false;
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
    {
        if (m_scenePlaying)
            StopScene();

        m_app.Close();
    }

    EventType evenType = event.GetType();

    ImGuiIO& io = ImGui::GetIO();

    if (!m_viewportHovered && io.WantCaptureMouse &&
        (
            evenType == EventType::MouseButtonPressed ||
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

void EditorLayer::DuplicateEntity()
{
    if (m_scenePlaying)
        return;

    if (m_selectedObject.type != EditorObjectType::Entity || m_selectedObject.entity == entt::null)
        return;

    m_activeScene->DuplicateEntity(m_selectedObject.entity);
}

void EditorLayer::DeleteEntity(entt::entity entity)
{
    if (m_activeScene->m_manager.DeleteEntity(entity) &&
        !m_activeScene->m_manager.IsEntityValid(m_selectedObject.entity)
    )
        UnselectObject();
}

bool EditorLayer::NewScene()
{
    std::cout << "New Scene\n";

    m_activeScene = SceneManager::CreateDefaultScene("");
    m_activeScene->SetApplication(&m_app);
    m_activeScene->Initialize();

    return true;
}

void EditorLayer::OnSceneUpdate()
{
    std::cout << "OnSceneUpdate: " << m_activeScene->m_name << std::endl;
    std::string windowTitle = m_activeProject->GetName() + " - " + m_activeScene->m_name + " - BossFight Engine";
    m_app.GetWindow().SetTitle(windowTitle);

    std::cout << "m_activeScene->m_path: " << m_activeScene->m_path << std::endl;
    m_activeProject->SetStartScene(m_activeScene->m_path);

    std::cout << "m_activeProject->GetDirectory(): " << m_activeProject->GetDirectory() << std::endl;
    ProjectSerializer::Serialize(*m_activeProject, m_activeProject->GetDirectory() / (m_activeProject->GetName() + PROJECT_FILE_EXTENSION));

}

bool EditorLayer::SaveScene()
{
    if (m_activeScene->HasSaved())
    {
        std::cout << "Saving the Scene..." << std::endl;
        SceneSerializer::Serialize(*m_activeScene, m_activeScene->GetAbsolutePath());
    }
    else
    {
        std::cout << "Has not Saved Yet!!!\n";
        return SaveSceneAs();
    }

    m_activeProject->SetStartScene(m_activeScene->m_path);

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

    NativeScriptEngine& nativeScriptEngine = m_app.GetNativeScriptEngine();

    nativeScriptEngine.SetClassNames(scriptClassNames);

    if (nativeScriptEngine.LoadNativeScripts(projectPath / "bin" / "Debug" / "Native-Script.dll", m_nativeClassNames))
    {
        std::cout << "Success: Load native scripts\n";
        nativeScriptEngine.InitializeScriptableEntities();
    }
    else
    {
        std::cout << "Error: Load native scripts\n";
    }


}
