#include "Game.h"

#include "Components/Collider2D.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

Game::Game(unsigned int width, unsigned int height)
  : m_width(width)
  , m_height(height)
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
  glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
  }

  Renderer::Init();
}

Game::~Game()
{
  glfwTerminate();
}

void Game::Init()
{
  auto player = m_manager.CreateEntity();
  player.AddComponent<Collider2D>(m_physics.CreateBoxBody(2, 8, 2.0, 0.5, true));

  auto water = m_manager.CreateEntity();
  water.AddComponent<Collider2D>(m_physics.CreateBoxBody(0, 0, 10, 12, false, true));

  auto platform = m_manager.CreateEntity();
  platform.AddComponent<Collider2D>(m_physics.CreateBoxBody(0, -6, 15, 0.5));
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

    m_physics.Update(deltaTime);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    auto view = m_manager.m_registry.view<Collider2D>();
    for (auto [entity, collider]: view.each())
    {
      Renderer::DrawQuad(collider.body);
    }

    glfwSwapBuffers(m_window);
    glfwPollEvents();
  }
}

void Game::ProcessInput(float deltaTime)
{
  if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(m_window, true);
}
