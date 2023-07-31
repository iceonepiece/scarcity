#include "Scene.h"
#include "Core/System.h"
#include "Core/GameState.h"
#include "Systems/ScriptableSystem.h"
#include "Systems/RenderSystem.h"
#include "Core/Camera2D.h"
#include "Components/Components.h"

Scene::Scene()
	: m_camera(
        new Camera2D({ 0.0f, 0.0f, -14.0f }, { 0.5f, 0.25f }, { 1280, 720 })
    )
    , m_ui(this)
{
    m_systems.emplace_back(new ScriptableSystem(this));
    m_systems.emplace_back(new RenderSystem(this));
}

Scene::~Scene()
{
	for (auto system : m_systems)
	{
		delete system;
	}
	m_systems.clear();

    for (auto gs : m_gameStates)
    {
        delete gs.second;
    }
    m_gameStates.clear();
}

void Scene::OnEvent(Event* e)
{

}

void Scene::Initialize()
{
}

void Scene::Start()
{
    StartPhysics();
}

void Scene::Stop()
{
    StopPhysics();
}

void Scene::StartPhysics()
{
    //m_physics = std::make_unique<Physics>();
    m_physics = new b2World({ 0.0f, -9.8f });

	auto view = m_manager.m_registry.view<TransformComponent, Rigidbody2DComponent>();
	for (auto [entity, transform, rb2d] : view.each())
	{
		b2BodyDef bodyDef;
		bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d.type);
		bodyDef.position.Set(transform.position.x, transform.position.y);
		bodyDef.angle = transform.rotation.z;

		b2Body* body = m_physics->CreateBody(&bodyDef);
		body->SetFixedRotation(rb2d.fixedRotation);
		rb2d.body = body;

		if (m_manager.m_registry.all_of<BoxCollider2DComponent>(entity))
		{
			auto& bc2d = m_manager.m_registry.get<BoxCollider2DComponent>(entity);

			b2PolygonShape boxShape;
			//boxShape.SetAsBox(bc2d.size.x * transform.scale.x, bc2d.size.y * transform.scale.y, b2Vec2(bc2d.offset.x, bc2d.offset.y), 0.0f);
            boxShape.SetAsBox(transform.scale.x / 2.0f, transform.scale.y / 2.0f, b2Vec2(bc2d.offset.x, bc2d.offset.y), 0.0f);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &boxShape;
            fixtureDef.density = 1.0f;
            fixtureDef.friction = 0.0f;

			body->CreateFixture(&fixtureDef);
		}

        if (m_manager.m_registry.all_of<CircleCollider2DComponent>(entity))
        {
            auto& cc2d = m_manager.m_registry.get<CircleCollider2DComponent>(entity);

            b2CircleShape circleShape;
            circleShape.m_p.Set(cc2d.offset.x, cc2d.offset.y);
            circleShape.m_radius = transform.scale.x * cc2d.radius;

            b2FixtureDef fixtureDef;
            fixtureDef.shape = &circleShape;
            fixtureDef.density = 1.0f;
            fixtureDef.friction = 0.0f;

            body->CreateFixture(&fixtureDef);
        }
	}
}

void Scene::StopPhysics()
{
    //m_physics.reset();

    delete m_physics;
    m_physics = nullptr;
}

void Scene::Update(float deltaTime)
{
    for (auto system : m_systems)
        system->Update(deltaTime);

    if (m_physics != nullptr && physicsActive)
    {
        //m_physics->Update(deltaTime);

        const int32_t velocityIterations = 6;
        const int32_t positionIterations = 2;
        m_physics->Step(deltaTime, velocityIterations, positionIterations);

        auto view = m_manager.m_registry.view<TransformComponent, Rigidbody2DComponent>();
        for (auto [entity, transform, rb2d] : view.each())
        {
            b2Body* body = (b2Body*)rb2d.body;

            const auto& position = body->GetPosition();
            transform.position.x = position.x;
            transform.position.y = position.y;
            transform.rotation.z = body->GetAngle();

            std::cout << transform.position.x << " : " << transform.position.y << std::endl;
        }
    }

    m_camera->Update();
}

void Scene::Enter()
{
    auto view = m_manager.m_registry.view<ScriptableComponent>();

    for (auto [entity, scriptable] : view.each())
        scriptable.scriptable->Start();

    m_app->GetRenderer().SetCamera(m_camera.get());
}

void Scene::Exit()
{

}

void Scene::Render()
{
    //WindowData windowData = m_app->GetWindow().GetWindowData();
    //m_camera->SetScreenSize({ windowData.width, windowData.height });
    Renderer& renderer = m_app->GetRenderer();
    //renderer.SetCamera(m_camera.get());

    for (auto system : m_systems)
        system->Render();

    auto view = m_manager.m_registry.view<TransformComponent, SpriteRendererComponent>();

    for (auto [entity, transform, sprite] : view.each())
    {
        glm::vec2 position = transform.position;
        glm::vec2 scale = transform.scale;
        float angle = transform.rotation.z;

        renderer.DrawQuad2D(position, scale, angle, sprite.color);
    }

}

void Scene::RenderUI()
{
    m_ui.Render();
}

EntityManager& Scene::GetEntityManager()
{
    return m_manager;
}

void Scene::ChangeGameState(std::string gameStateName)
{
    std::cout << "Change from " << m_currentGameStateName << " to " << gameStateName << std::endl;


    if (m_gameStates[m_currentGameStateName] != nullptr)
        m_gameStates[m_currentGameStateName]->OnExit();

    m_currentGameStateName = gameStateName;

    if (m_gameStates[m_currentGameStateName] != nullptr)
        m_gameStates[m_currentGameStateName]->OnEnter();

}