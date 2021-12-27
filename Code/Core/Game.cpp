#include "Game.h"

#include "../Components/Collider2DComponent.h"
#include "../Components/PlayerComponent.h"
#include "PlayerFixtureData.h"

const float PLAYER_MOVE_SPEED = 7.0f;
const float PLAYER_JUMP_FORCE = 4.2f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

Game::Game(unsigned int width, unsigned int height)
  : m_width(width)
  , m_height(height)
  , m_input(this)
  , m_camera(glm::vec3(0.0f, 0.0f, -16.0f), glm::vec2(0.8f, 1.0f), glm::vec2(width, height))
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
  m_particleSystem.Init();
}

Game::~Game()
{
  glfwTerminate();
}

void Game::Init()
{
  auto player = m_manager.CreateEntity();
  b2Body* playerBody = m_physics.CreateBodyWithFixture(b2Vec2 {0, 8}, b2Vec2 {0.5, 1.5}, new PlayerFixtureData(player), true);
  player.AddComponent<Collider2DComponent>(playerBody);
  player.AddComponent<PlayerComponent>();

  auto platform = m_manager.CreateEntity();
  platform.AddComponent<Collider2DComponent>(m_physics.CreateBoxBody(0, 0, 15, 0.5));

  auto platform2 = m_manager.CreateEntity();
  platform2.AddComponent<Collider2DComponent>(m_physics.CreateBoxBody(-10, -3, 10, 0.5));

  m_camera.SetBody(player.GetComponent<Collider2DComponent>()->body);

  m_particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_particle.SizeBegin = 0.5f, m_particle.SizeVariation = 0.3f, m_particle.SizeEnd = 0.0f;
	m_particle.LifeTime = 1.0f;
	m_particle.Velocity = { 0.0f, 0.0f };
	m_particle.VelocityVariation = { 3.0f, 1.0f };
	m_particle.Position = { 0.0f, 0.0f };
}

void Game::Run()
{
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  m_camera.Update();

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

  auto view = m_manager.m_registry.view<PlayerComponent>();
  for (auto [entity, player]: view.each())
  {
    player.movementState = MS_IDLE;
    player.jump = false;

    if (m_input.IsKeyPressed(GLFW_KEY_LEFT))
      player.movementState = MS_LEFT;

    if (m_input.IsKeyPressed(GLFW_KEY_RIGHT))
      player.movementState = MS_RIGHT;

    if (m_input.IsKeyPressed(GLFW_KEY_SPACE))
      player.jump = true;
  }

  if (m_input.IsKeyPressed(GLFW_KEY_F))
  {
    m_particle.Position = { 0, 0 };
    for (int i = 0; i < 5; i++)
      m_particleSystem.Emit(m_particle);
  }
}

void Game::Update(float deltaTime)
{
  auto view = m_manager.m_registry.view<PlayerComponent, Collider2DComponent>();

  for (auto [entity, player, collider]: view.each())
  {
    b2Body* body = collider.body;
    if (body)
    {
      b2Vec2 velocity = body->GetLinearVelocity();
      float desiredVelocity = 0;

      switch (player.movementState)
      {
        case MS_LEFT:
          desiredVelocity = -PLAYER_MOVE_SPEED; break;
        case MS_IDLE:
          desiredVelocity = 0; break;
        case MS_RIGHT:
          desiredVelocity = PLAYER_MOVE_SPEED; break;
      }

      body->SetLinearVelocity(b2Vec2(desiredVelocity, velocity.y));

      if (player.jump && player.numGrounds > 0)
        body->ApplyLinearImpulse(b2Vec2(0, PLAYER_JUMP_FORCE), body->GetWorldCenter(), true);
    }
  }

  m_particleSystem.Update(deltaTime);
  m_physics.Update(deltaTime);
  m_camera.Update();
}

void Game::Render()
{
  glClearColor(0.133f, 0.157f, 0.192f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  auto view = m_manager.m_registry.view<Collider2DComponent>();
  for (auto [entity, collider]: view.each())
  {
    Renderer::DrawQuad(collider.body, m_camera);
  }

  m_particleSystem.Render(m_camera);

  glfwSwapBuffers(m_window);
  glfwPollEvents();
}
