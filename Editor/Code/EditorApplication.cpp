#include "EditorApplication.h"
#include "Platforms/OpenGL/OpenGLWindow.h"
#include "Platforms/OpenGL/OpenGLRenderer.h"
#include "Core/Timer.h"
#include "HubLayer.h"
#include "EditorLayer.h"
#include "Graphics/Camera2D.h"
#include "ImGui/ImGuiManager.h"
#include "Utils/FileDialog.h"
#include "File/FileSystem.h"
#include "Project/ProjectSerializer.h"
#include "Scene/SceneSerializer.h"
#include "Scene/SceneManager.h"
#include "ScriptManager.h"
#include "Platforms/GLFW/GLFWInput.h"
#include "Core/ResourceAPI.h"
#include "Platforms/OpenGL/OpenGLResourceManager.h"

EditorApplication::EditorApplication(const ApplicationConfigs& configs)
    : Application(configs)
{
}

EditorApplication::~EditorApplication()
{
}

void EditorApplication::Initialize()
{
    m_input = std::make_unique<GLFWInput>(*((GLFWwindow*)m_window->GetNativeWindow()));

    ResourceAPI::Initialize(new OpenGLResourceManager());

    m_imguiManager = std::make_unique<ImGuiManager>(*this);

    FileDialog::SetNativeWindow(m_window->GetNativeWindow());

    AddLayer(std::make_unique<HubLayer>(*this));
    //AddLayer(std::make_unique<EditorLayer>(*this));
}

void EditorApplication::ChangeScene(std::string name)
{
    for (auto& layer : m_layers)
    {
        if (EditorLayer* editorLayer = dynamic_cast<EditorLayer*>(layer.get()))
        {
            editorLayer->ChangeScene(name);
        }
    }

}

bool EditorApplication::NewProject(const std::string& name, std::filesystem::path location)
{
    std::filesystem::path directory = location / name;

    if (FileSystem::CreateFolder(directory))
    {
        Project project(name, directory);
        FileSystem::CreateFolder(directory / "Scenes");
        FileSystem::CreateFolder(directory / "Scripts");

        ScriptManager::CreateVisualStudioFiles(directory, name);

        std::unique_ptr<Scene> defaultScene = SceneManager::CreateDefaultScene(directory);
        defaultScene->SetApplication(this);
        defaultScene->Initialize();

        std::filesystem::path scenePath = "Scenes";
        scenePath /= (defaultScene->m_name + SCENE_FILE_EXTENSION);

        project.SetStartScene(scenePath);

        ProjectSerializer::Serialize(project, directory / (name + PROJECT_FILE_EXTENSION));
        SceneSerializer::Serialize(*defaultScene, directory / scenePath);

        return true;
    }

    return false;
}

void EditorApplication::OpenProject(std::filesystem::path path)
{
    std::cout << "Open Project: " << path << std::endl;
    std::cout << "Relative: " << path.parent_path().filename() << std::endl;
   
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