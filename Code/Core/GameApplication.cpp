#include "GameApplication.h"
#include "Timer.h"
#include "Audio/Audio.h"
#include "Graphics/Renderer.h"
#include "Scene.h"
#include "Graphics/RendererAPI.h"
#include "Graphics/FontSystem.h"
#include "Platforms/OpenGL/OpenGLWindow.h"
#include "Platforms/OpenGL/OpenGLResourceManager.h"
#include "Platforms/OpenGL/OpenGLRenderer.h"
#include "ResourceAPI.h"

GameApplication::GameApplication()
{
}

GameApplication::~GameApplication()
{
    //RendererAPI::Terminate();
}

void GameApplication::OnEvent(Event* event)
{
    //std::cout << event->ToString() << std::endl;
    //delete event;
}

void GameApplication::Initialize(std::string title, int width, int height)
{
    m_window = std::make_unique<OpenGLWindow>(this, title, width, height);

    m_window->SetEventCallback([this](Event* event) {
        this->OnEvent(event);
    });

   /*
    m_camera = std::make_unique<Camera2D>(
        glm::vec3 { 0.0f, 0.0f, -1.0f },
        glm::vec2 { 1.0f, 1.0f },
        glm::vec2 { 1280, 720 }
    );
    */

    m_renderer = std::make_unique<OpenGLRenderer>();
    m_renderer->Initialize();
    //m_renderer->SetCamera(m_camera.get());


    ResourceAPI::Initialize(new OpenGLResourceManager());
    //ParticleSystem::Init();
    //FontSystem::Init();
    Input::Init();
    //Audio::Init();

    //ResourceAPI::LoadParticles("Scripts/particles.lua");
}

void GameApplication::AddScene(std::string name, Scene *scene)
{
    scene->m_app = this;
    m_scenes.insert({ name, std::unique_ptr<Scene>(scene) });
}

void GameApplication::LoadScenes()
{
    for (auto& [name, scene] : m_scenes)
        scene->Initialize();
}

void GameApplication::ChangeScene(std::string sceneName)
{
    if (m_scenes.find(sceneName) != m_scenes.end())
    {
        m_currentSceneName = sceneName;
        m_scenes[m_currentSceneName]->Enter();
    }
}

void GameApplication::Run()
{
    while (m_running)
    {
        Timer::Tick();

        ProcessInput();
        Update();
        Render();

        Timer::DisplayFPS();
    }
}

void GameApplication::ProcessInput()
{
    m_window->ProcessInput();

    if (m_window->WindowShouldClose())
        m_running = false;
}

void GameApplication::Update()
{
    if (m_scenes.find(m_currentSceneName) != m_scenes.end())
    {
        m_scenes[m_currentSceneName]->Update(Timer::GetDeltaTime());
    }
}


void GameApplication::Render()
{
    m_window->PreRender();

   // m_renderer->DrawQuadUI(glm::vec2(0, 0), glm::vec2(2000, 2000), glm::vec4(0.25, 0.25, 0.25, 1), UIAlignment::CENTER);

    if (m_scenes.find(m_currentSceneName) != m_scenes.end())
    {
        m_scenes[m_currentSceneName]->Render();
        //m_scenes[m_currentSceneName]->RenderUI();
    }

    m_window->Render();
}