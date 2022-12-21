#include "Game.h"

#include "../Components/Collider2DComponent.h"
#include "../Components/PlayerComponent.h"
#include "../GUIs/GUIWindow.h"
#include "../GUIs/GUIList.h"
#include "../Systems/PlayerSystem.h"
#include "../Scenes/Level1.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

Game::Game(unsigned int width, unsigned int height)
  : m_width(width)
  , m_height(height)
  , m_input(this)
  , m_gui(this)
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  m_window = glfwCreateWindow(m_width, m_height, "SCARCITY", NULL, NULL);
  if (m_window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
  }
  glfwMakeContextCurrent(m_window);
  glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
  }

  m_gui.Init(m_window, "#version 330");
  Renderer::Init();
  ParticleSystem::Init();
}

Game::~Game()
{
  m_gui.Destroy();
  glfwDestroyWindow(m_window);
  glfwTerminate();
}

void Game::Init()
{
    m_scenes.push_back(new Level1(this));
    
    for (int i = 0; i < m_scenes.size(); i++)
    {
        m_scenes[i]->Init();
    }

    m_input.AddInputCommand(GLFW_KEY_ESCAPE, "ESCAPE");
    m_input.AddInputCommand(GLFW_KEY_LEFT, "LEFT");
    m_input.AddInputCommand(GLFW_KEY_RIGHT, "RIGHT");
    m_input.AddInputCommand(GLFW_KEY_SPACE, "SPACE");
    m_input.AddInputCommand(GLFW_KEY_F, "F");
    m_input.AddInputCommand(GLFW_KEY_Z, "Z");
    m_input.AddInputCommand(GLFW_KEY_X, "X");
    m_input.AddInputCommand(GLFW_KEY_Q, "Q");

    ResourceManager::LoadParticles("./Code/Scripts/particles.lua");

    GUIWindow* guiWindow = new GUIWindow(&m_gui, "Hello World");
    guiWindow->AddChild(new GUIList(&m_gui));

    m_gui.AddComponent(guiWindow);
}

void Game::Run()
{
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  while (!glfwWindowShouldClose(m_window))
  {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    ProcessInput(deltaTime);
    Update(deltaTime);
    Render();
  }
}

void Game::ProcessInput(float deltaTime)
{
    glfwPollEvents();
    m_input.PollInputs();

    if (m_input.IsKeyPressed(GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(m_window, true);

    m_scenes[m_currentSceneIndex]->ProcessInput(m_input);
}

void Game::Update(float deltaTime)
{
    m_scenes[m_currentSceneIndex]->Update(deltaTime);
}

void Game::Render()
{
    m_gui.NewFrame();

    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);

    glClearColor(0.133f, 0.157f, 0.192f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_scenes[m_currentSceneIndex]->Render();

    m_gui.Draw();

    glfwSwapBuffers(m_window);
}