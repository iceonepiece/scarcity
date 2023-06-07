#include "EditorApplication.h"
#include "../Core/OpenGLWindow.h"
#include "../Core/Timer.h"
#include "Components/EntityListWindow.h"
#include "Components/InspectorWindow.h"
#include "../Scenes/LevelEditorScene.h"

EditorApplication::EditorApplication()
    : m_sceneLayer{ new LevelEditorScene(this) }
{
}

EditorApplication::~EditorApplication()
{
    m_imGuiLayer.Destroy();
}

void EditorApplication::Initialize(std::string title, int width, int height)
{
    m_window = std::make_unique<OpenGLWindow>(title, width, height);

    if (OpenGLWindow *openGLWindow = dynamic_cast<OpenGLWindow*>(m_window.get()))
        m_imGuiLayer.Initialize(openGLWindow->GetGLFWwindow(), "#version 330");

    EntityListWindow* entityListWindow = new EntityListWindow();
    m_imGuiLayer.AddComponent(entityListWindow);
    m_imGuiLayer.AddComponent(new InspectorWindow(entityListWindow));

    Input::Init();
    Renderer::Init();
}

void EditorApplication::Run()
{
    while (m_running)
    {
        Timer::Tick();

        ProcessInput();
        Update();
        Render();
    }
}

void EditorApplication::ProcessInput()
{
    m_window->ProcessInput();

    if (m_window->WindowShouldClose())
        m_running = false;
}

void EditorApplication::Update()
{
}

void EditorApplication::Render()
{
    m_window->PreRender();

    m_sceneLayer.Render();

    m_imGuiLayer.NewFrame();
    m_imGuiLayer.Draw();

    m_window->Render();
}