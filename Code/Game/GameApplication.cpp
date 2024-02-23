#include "GameApplication.h"
#include "Platforms/OpenGL/OpenGLWindow.h"
#include "Platforms/OpenGL/OpenGLRenderer.h"
#include "Core/Timer.h"
#include "Graphics/Camera2D.h"
#include "ImGui/ImGuiManager.h"
#include "Utils/FileDialog.h"
#include "File/FileSystem.h"
#include "Project/ProjectSerializer.h"
#include "Scene/SceneSerializer.h"
#include "Scene/SceneManager.h"
#include "Platforms/GLFW/GLFWInput.h"
#include "Platforms/OpenGL/OpenGLAssetManager.h"
#include "GameLayer.h"

GameApplication::GameApplication(const ApplicationConfigs& configs)
	: Application(configs)
{
}

void GameApplication::Initialize()
{
    m_input = std::make_unique<GLFWInput>(*((GLFWwindow*)m_window->GetNativeWindow()));
    m_assetManager = std::make_unique<OpenGLAssetManager>();
    m_imguiManager = std::make_unique<ImGuiManager>(*this);
}

void GameApplication::OpenProject(std::filesystem::path path)
{
    std::cout << "Open Project: " << path << std::endl;
    std::cout << "Relative: " << path.parent_path().filename() << std::endl;

    std::unique_ptr<Project> project = std::make_unique<Project>();

    if (ProjectSerializer::Deserialize(*project, path))
    {
        m_window->SetTitle(project->GetName());
        AddLayer(std::make_unique<GameLayer>(*this, std::move(project)));
    }
}
