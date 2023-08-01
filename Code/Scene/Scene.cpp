#include "Scene.h"
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
    m_systems.push_back(std::make_unique<ScriptableSystem>(this));
    m_systems.push_back(std::make_unique<RenderSystem>(this));
}

void Scene::OnEvent(Event* e)
{

}

void Scene::Initialize()
{
    if (m_initializeFunction != nullptr)
        m_initializeFunction(this);
}

std::unique_ptr<Scene> Scene::Copy(Scene& sourceScene)
{
    std::unique_ptr<Scene> newScene = std::make_unique<Scene>();

    entt::registry& sourceRegistry = sourceScene.m_manager.m_registry;
    entt::registry& destinationRegistry = newScene->m_manager.m_registry;

    sourceRegistry.each([&](auto entity) {
        auto destinationEntity = destinationRegistry.create();

        if (sourceRegistry.all_of<BaseComponent>(entity)) {
            auto& component = sourceRegistry.get<BaseComponent>(entity);
            destinationRegistry.emplace_or_replace<BaseComponent>(destinationEntity, component);
        }

        if (sourceRegistry.all_of<TransformComponent>(entity)) {
            auto& component = sourceRegistry.get<TransformComponent>(entity);
            destinationRegistry.emplace_or_replace<TransformComponent>(destinationEntity, component);
        }

        if (sourceRegistry.all_of<SpriteRendererComponent>(entity)) {
            auto& component = sourceRegistry.get<SpriteRendererComponent>(entity);
            destinationRegistry.emplace_or_replace<SpriteRendererComponent>(destinationEntity, component);
        }

        if (sourceRegistry.all_of<Rigidbody2DComponent>(entity)) {
            auto& component = sourceRegistry.get<Rigidbody2DComponent>(entity);
            destinationRegistry.emplace_or_replace<Rigidbody2DComponent>(destinationEntity, component);
        }

        if (sourceRegistry.all_of<BoxCollider2DComponent>(entity)) {
            auto& component = sourceRegistry.get<BoxCollider2DComponent>(entity);
            destinationRegistry.emplace_or_replace<BoxCollider2DComponent>(destinationEntity, component);
        }

        if (sourceRegistry.all_of<CameraComponent>(entity)) {
            auto& component = sourceRegistry.get<CameraComponent>(entity);
            destinationRegistry.emplace_or_replace<CameraComponent>(destinationEntity, component);
        }
    });

    return newScene;
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
    m_physics = std::make_unique<b2World>(b2Vec2 { 0.0f, -9.8f });

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
    m_physics.reset();
}

void Scene::Update(float deltaTime)
{
    for (auto& system : m_systems)
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

    for (auto& system : m_systems)
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