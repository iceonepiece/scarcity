#include "EditorApplication.h"
#include "Platforms/OpenGL/OpenGLWindow.h"
#include "Platforms/OpenGL/OpenGLRenderer.h"
#include "Core/Timer.h"
#include "Components/EntityListWindow.h"
#include "Components/InspectorWindow.h"
#include "HubLayer.h"
#include "EditorLayer.h"
#include "Core/Camera2D.h"
#include "ImGui/ImGuiManager.h"
#include "Utils/FileDialog.h"
#include "Project/ProjectSerializer.h"
#include "Scene/SceneSerializer.h"
#include "ScriptManager.h"

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

    m_imguiManager = std::make_unique<ImGuiManager>(*this);

    AddLayer(std::make_unique<HubLayer>(*this));
    //AddLayer(std::make_unique<EditorLayer>(*this));
}

bool EditorApplication::NewProject(const std::string& name, std::filesystem::path location)
{
    std::filesystem::path directory = location / name;

    if (FileUtils::CreateFolder(directory))
    {
        Project project(name, directory);
        FileUtils::CreateFolder(directory / "Scenes");

        ScriptManager::CreateVisualStudioFiles(directory, name);

        std::unique_ptr<Scene> defaultScene = Scene::CreateDefaultScene(directory);
        defaultScene->SetApplication(this);
        defaultScene->Initialize();

        std::filesystem::path scenePath = directory / "Scenes" / (defaultScene->m_name + SCENE_FILE_EXTENSION);
        project.SetStartScene(scenePath);

        ProjectSerializer::Serialize(project, directory / (name + PROJECT_FILE_EXTENSION));
        SceneSerializer::Serialize(*defaultScene, scenePath);

        return true;
    }

    return false;
}

void EditorApplication::OpenProject(std::filesystem::path path)
{
    std::cout << "Open Project: " << path << std::endl;
   
    std::unique_ptr<Project> project = std::make_unique<Project>();

    if (ProjectSerializer::Deserialize(*project, path))
    {
        m_window->SetTitle(project->GetName() + " - " + ENGINE_NAME);

        PopLayer();
        AddLayer(std::make_unique<EditorLayer>(*this, std::move(project)));
    }
}

void EditorApplication::SaveProject()
{
    
}

void EditorApplication::NewScene(const std::string& name, std::filesystem::path directory)
{

}

void EditorApplication::OpenScene(std::filesystem::path path)
{

}

void EditorApplication::SaveScene()
{

}
