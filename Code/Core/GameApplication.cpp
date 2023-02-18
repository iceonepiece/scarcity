#include "GameApplication.h"
#include "Timer.h"
#include "../Audio/Audio.h"
#include "../Rendering/Renderer.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "../Scenes/MenuScene.h"
#include "../Scenes/Level1.h"
#include "OpenGLWindow.h"


GameApplication::GameApplication()
{
}

GameApplication::~GameApplication()
{
    for (auto& scene : m_scenes)
        delete scene.second;
    m_scenes.clear();

    delete m_window;
}

void GameApplication::Initialize()
{
    m_window = new OpenGLWindow(1280, 720);

    Renderer::Init();
    ParticleSystem::Init();
    FontSystem::Init();
    Input::Init();
    Audio::Init();

    ResourceManager::LoadParticles("./Code/Scripts/particles.lua");

    LoadScenes();
}

void GameApplication::LoadScenes()
{
    m_scenes.insert({ "menu", new MenuScene(this) });
    m_scenes.insert({ "level1", new Level1(this) });
    ChangeScene("menu");

    for (auto& [name, scene] : m_scenes)
        scene->Init();
}

void GameApplication::ChangeScene(std::string sceneName)
{
    if (m_scenes.find(sceneName) != m_scenes.end())
    {
        m_currentSceneName = sceneName;
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
    }
}

void GameApplication::ProcessInput()
{
    m_window->ProcessInput();

    if (m_window->WindowShouldClose() || Input::IsKeyPressed(Key::Escape))
        m_running = false;

    m_scenes[m_currentSceneName]->ProcessInput();
}

void GameApplication::Update()
{
    m_scenes[m_currentSceneName]->Update(Timer::GetDeltaTime());
}

void GameApplication::Render()
{
    m_window->PreRender();

    Renderer::DrawQuadUI(glm::vec2(0, 0), glm::vec2(2000, 2000), glm::vec4(0.15, 0.15, 0.15, 1), UIAlignment::CENTER);

    m_scenes[m_currentSceneName]->Render();
    m_scenes[m_currentSceneName]->RenderUI();

    m_window->Render();
}