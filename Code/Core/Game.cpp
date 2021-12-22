#include "Game.h"

#include "../Components/Collider2DComponent.h"
#include "../Components/ControllerComponent.h"
#include "../Components/MovementComponent.h"

const float PLAYER_MOVE_SPEED = 7.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

Game::Game(unsigned int width, unsigned int height)
  : m_width(width)
  , m_height(height)
  , m_input(this)
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
  player.AddComponent<Collider2DComponent>(m_physics.CreateBoxBody(2, 4, 1, 2, true));
  player.AddComponent<ControllerComponent>(
    std::vector<InputCommand> {
      { GLFW_KEY_LEFT, "MOVE_LEFT" },
      { GLFW_KEY_RIGHT, "MOVE_RIGHT" },
    }
  );
  player.AddComponent<MovementComponent>();

  auto platform = m_manager.CreateEntity();
  platform.AddComponent<Collider2DComponent>(m_physics.CreateBoxBody(0, -5, 15, 0.5));
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
  if (m_input.IsKeyPressed(GLFW_KEY_ESCAPE))
    glfwSetWindowShouldClose(m_window, true);

  auto view = m_manager.m_registry.view<ControllerComponent>();
  for (auto [entity, controller]: view.each())
  {
    for (auto& inputCommand : controller.inputCommands)
    {
      if (m_input.IsKeyPressed(inputCommand.keyCode))
        inputCommand.isActive = true;
      else
        inputCommand.isActive = false;
    }
  }

  auto xview = m_manager.m_registry.view<ControllerComponent, MovementComponent>();
  for (auto [entity, controller, movement]: xview.each())
  {
    movement.state = MS_IDLE;

    for (auto inputCommand : controller.inputCommands)
    {
      if (!inputCommand.isActive)
        continue;

      if (inputCommand.name == "MOVE_LEFT")
        movement.state = MS_LEFT;
      else if (inputCommand.name == "MOVE_RIGHT")
        movement.state = MS_RIGHT;
    }
  }
}

void Game::Update(float deltaTime)
{
  auto view = m_manager.m_registry.view<MovementComponent, Collider2DComponent>();

  for (auto [entity, movement, collider]: view.each())
  {
    b2Body* body = collider.body;
    if (body)
    {
      b2Vec2 velocity = body->GetLinearVelocity();
      float desiredVelocity = 0;

      switch (movement.state)
      {
        case MS_LEFT:
          desiredVelocity = -PLAYER_MOVE_SPEED; break;
        case MS_IDLE:
          desiredVelocity = 0; break;
        case MS_RIGHT:
          desiredVelocity = PLAYER_MOVE_SPEED; break;
      }

      body->SetLinearVelocity(b2Vec2(desiredVelocity, velocity.y));
    }
  }

  m_physics.Update(deltaTime);
}

void Game::Render()
{
  glClearColor(0.133f, 0.157f, 0.192f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  auto view = m_manager.m_registry.view<Collider2DComponent>();
  for (auto [entity, collider]: view.each())
  {
    Renderer::DrawQuad(collider.body);
  }

  glfwSwapBuffers(m_window);
  glfwPollEvents();
}
