#include "GameApplication.h"
#include "Timer.h"
#include "../Audio/Audio.h"
#include "../Graphics/Renderer.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "../Scenes/MenuScene.h"
#include "../Scenes/Level1.h"
#include "../Scenes/LuaScene.h"
#include "OpenGLWindow.h"
#include "../Graphics/OpenGLRenderer.h"

GameApplication::GameApplication()
{
}

GameApplication::~GameApplication()
{
    for (auto& scene : m_scenes)
        delete scene.second;
    m_scenes.clear();

    RendererAPI::Terminate();

    delete m_window;
}

void GameApplication::Initialize()
{
    m_window = new OpenGLWindow(1280, 720);

    Renderer::Init();
    RendererAPI::Initialize(new OpenGLRenderer());
    ParticleSystem::Init();
    FontSystem::Init();
    Input::Init();
    Audio::Init();

    ResourceManager::LoadParticles("./Code/Scripts/particles.lua");

    LoadScenes();
}

void GameApplication::LoadScenes()
{
    //m_scenes.insert({ "menu", new MenuScene(this) });
    m_scenes.insert({ "level1", new Level1(this) });
    //m_scenes.insert({ "lua", new LuaScene(this) });
    ChangeScene("level1");

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

    if (m_window->WindowShouldClose())
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