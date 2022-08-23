#include "Game.h"

#include "../Components/Collider2DComponent.h"
#include "../Components/PlayerComponent.h"
#include "PlayerFixtureData.h"
#include "../GUIs/GUIWindow.h"
#include "../GUIs/GUIList.h"
#include "../Systems/PlayerSystem.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

Game::Game(unsigned int width, unsigned int height)
  : m_width(width)
  , m_height(height)
  , m_input(this)
  , m_gui(this)
  , m_camera(glm::vec3(0.0f, 0.0f, -16.0f), glm::vec2(0.8f, 1.0f), glm::vec2(width, height))
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

  //m_gui.Init(m_window, "#version 330");
  Renderer::Init();
  m_particleSystem.Init();
}

Game::~Game()
{
  //m_gui.Destroy();
  glfwDestroyWindow(m_window);
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

  auto enemy = m_manager.CreateEntity();
  b2Body* enemyBody = m_physics.CreateBodyWithFixture(b2Vec2 {1, 8}, b2Vec2 {0.5, 1.5}, new FixtureData(enemy, "ENEMY"), true);
  enemy.AddComponent<Collider2DComponent>(enemyBody);
  //enemy.AddComponent<AIComponent>();

  /*
  m_particle.amount = 10;
  m_particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_particle.SizeBegin = 0.5f, m_particle.SizeVariation = 0.3f, m_particle.SizeEnd = 0.0f;
	m_particle.LifeTime = 1.0f;
	m_particle.Velocity = { 0.0f, 0.0f };
	m_particle.VelocityVariation = { 3.0f, 1.0f };
	m_particle.Position = { 0.0f, 0.0f };
  */

  m_input.AddInputCommand(GLFW_KEY_ESCAPE, "ESCAPE");
  m_input.AddInputCommand(GLFW_KEY_LEFT, "LEFT");
  m_input.AddInputCommand(GLFW_KEY_RIGHT, "RIGHT");
  m_input.AddInputCommand(GLFW_KEY_SPACE, "SPACE");
  m_input.AddInputCommand(GLFW_KEY_F, "F");

  LoadParticles();

  /*
  GUIWindow* guiWindow = new GUIWindow(&m_gui, "Hello World");
  guiWindow->AddChild(new GUIList(&m_gui));
  */
  //m_gui.AddComponent(new GUIWindow(&m_gui, "Hello World"));
}

void Game::LoadParticles()
{
  m_lua.script_file("./Code/Scripts/particles.lua");

  sol::table particles = m_lua["particles"];
  for (int i = 0 ;; i++)
  {
    sol::optional<sol::table> isExist = particles[i];

    if (isExist == sol::nullopt)
      break;

    sol::table node = particles[i];

    ParticleProps particleProps;
    particleProps.amount = node["amount"];

    particleProps.colorBegin = {
      node["colorBegin"]["r"],
      node["colorBegin"]["g"],
      node["colorBegin"]["b"],
      node["colorBegin"]["a"]
    };

    particleProps.colorEnd = {
      node["colorEnd"]["r"],
      node["colorEnd"]["g"],
      node["colorEnd"]["b"],
      node["colorEnd"]["a"]
    };

  	particleProps.sizeBegin = node["sizeBegin"];
    particleProps.sizeVariation = node["sizeVariation"];
    particleProps.sizeEnd = node["sizeEnd"];
  	particleProps.lifeTime = node["lifeTime"];
  	particleProps.velocity = { node["velocity"]["x"], node["velocity"]["y"] };
  	particleProps.velocityVariation = { node["velocityVariation"]["x"], node["velocityVariation"]["y"] };
  	particleProps.position = { node["position"]["x"], node["position"]["y"] };

    m_particles.emplace(node["name"], particleProps);
  }
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
  glfwPollEvents();
  m_input.PollInputs();

  if (m_input.IsKeyPressed(GLFW_KEY_ESCAPE))
    glfwSetWindowShouldClose(m_window, true);

  PlayerSystem::ProcessInput(m_input, m_manager.m_registry);

  if (m_input.IsKeyPressed(GLFW_KEY_F))
  {
    ParticleProps props = m_particles["fire"];
    props.position = { 0, 0 };
    for (int i = 0; i < props.amount; i++)
      m_particleSystem.Emit(props);
  }
}

void Game::Update(float deltaTime)
{
  PlayerSystem::Update(deltaTime, m_manager.m_registry, this);

  m_particleSystem.Update(deltaTime);
  m_physics.Update(deltaTime);
  m_camera.Update();
}

void Game::Render()
{
  //m_gui.NewFrame();

  int width, height;
  glfwGetFramebufferSize(m_window, &width, &height);
  m_camera.SetScreenSize(glm::vec2(width, height));

  glClearColor(0.133f, 0.157f, 0.192f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  auto view = m_manager.m_registry.view<Collider2DComponent>();
  for (auto [entity, collider]: view.each())
  {
    Renderer::DrawQuad(collider.body, m_camera);
  }

  m_particleSystem.Render(m_camera);
  //m_gui.Draw();

  glfwSwapBuffers(m_window);
}
