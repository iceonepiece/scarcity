#include "Game.h"

#include "../Components/Collider2DComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Systems/PlayerSystem.h"
#include "../Scenes/Level1.h"
#include "../Scenes/MenuScene.h"
#include "../Scenes/IntroScene.h"
#include "../Scenes/EditorScene.h"
#include "../UIs/UIText.h"
#include "../Input/KeyCodes.h"
#include "../Audio/Audio.h"
#include "../Events/MouseEvent.h"
#include "Timer.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

Game::Game(int width, int height)
    : m_width(width)
    , m_height(height)
    , m_currentSceneName("")
    , m_screenRatio(width / (float)height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_window = glfwCreateWindow(m_width, m_height, "SCARCITY", NULL, NULL);
    if (m_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(m_window);

    glfwSetWindowUserPointer(m_window, &m_data);

    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double x, double y)
    {
        MouseMovedEvent *e = new MouseMovedEvent(x, y);
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.events.push_back(e);
    });

    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double x, double y)
    {
        MouseScrolledEvent* e = new MouseScrolledEvent(x, y);
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.events.push_back(e);
    });

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    Renderer::Init();
    ParticleSystem::Init();
    FontSystem::Init();
    Input::Init();
    Audio::Init();
}

Game::~Game()
{
    for (auto scene : m_scenes)
    {
        delete scene.second;
    }
    m_scenes.clear();

    Audio::Destroy();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Game::OnEvent(Event* e)
{
    //m_scenes[m_currentSceneName]->OnEvent(e);
}

void Game::ChangeScene(std::string sceneName)
{
    if (m_scenes.find(sceneName) != m_scenes.end())
    {
        m_currentSceneName = sceneName;
    }
}

void Game::Init()
{
    /*
    m_scenes.insert({ "editor", new EditorScene(this) });
    m_scenes.insert({ "menu", new MenuScene(this) });
    m_scenes.insert({ "intro", new Level1(this)});
    ChangeScene("editor");
    */
    
    for (auto& [name, scene] : m_scenes)
    {
        scene->Init();
    }

    ResourceManager::LoadParticles("./Code/Scripts/particles.lua");
}

void Game::Run()
{
    while (!glfwWindowShouldClose(m_window))
    {
        Timer::Tick();

        ProcessInput();
        Update();
        Render();
    }
}

void Game::ProcessInput()
{
    glfwPollEvents();
    Input::PollInputs(m_window);

    for (int i = 0; i < m_data.events.size(); i++)
    {
        Event* e = m_data.events[i];
        std::cout << e->ToString() << std::endl;
        m_scenes[m_currentSceneName]->OnEvent(e);
        delete e;
    }
    m_data.events.clear();

    double xPos;
    double yPos;
    glfwGetCursorPos(m_window, &xPos, &yPos);

    Input::SetCursorPosition(xPos, yPos);

    if (Input::IsKeyPressed(Key::Escape))
        glfwSetWindowShouldClose(m_window, true);

    m_scenes[m_currentSceneName]->ProcessInput();
}

void Game::Update()
{
    m_scenes[m_currentSceneName]->Update(Timer::GetDeltaTime());
}

void Game::Render()
{
    glfwGetFramebufferSize(m_window, &m_width, &m_height);
   
    float windowRatio = m_width / (float)m_height;

    int width = windowRatio > m_screenRatio ? m_height * m_screenRatio : m_width;
    int height = windowRatio < m_screenRatio ? m_width / m_screenRatio : m_height;

    int xOffset = (m_width - width) / 2;
    int yOffset = (m_height - height) / 2;

    Renderer::SetScreenSize(width, height, xOffset, yOffset);
    
    glViewport(xOffset, yOffset, width, height);

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    Renderer::DrawQuadUI(glm::vec2(0, 0), glm::vec2(2000, 2000), glm::vec4(0.15, 0.15, 0.15, 1), UIAlignment::CENTER);

    m_scenes[m_currentSceneName]->Render();
    m_scenes[m_currentSceneName]->RenderUI();

    glfwSwapBuffers(m_window);
}