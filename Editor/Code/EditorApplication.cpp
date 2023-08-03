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

void EditorApplication::NewProject(std::unique_ptr<Project> project)
{
    m_activeProject = std::move(project);

    PopLayer();
    AddLayer(std::make_unique<EditorLayer>(*this));
}
