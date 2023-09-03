#include "Scene.h"
#include "Systems/ScriptableSystem.h"
#include "Systems/RenderSystem.h"
#include "Graphics/Renderer.h"
#include "Core/Camera2D.h"
#include "Components/Components.h"
#include "Utils/FileUtils.h"
#include "NativeScript/NativeScriptEngine.h"
#include "Animations/AnimationSerializer.h"
#include "Systems/AnimationSystem.h"

Scene::Scene(const std::string& name)
    : m_name(name)
	, m_camera(
        new Camera2D({ 0.0f, 0.0f, -14.0f }, { 0.5f, 0.25f }, { 1280, 720 })
    )
    , m_ui(this)
{
    m_systems.push_back(std::make_unique<ScriptableSystem>(this));
    m_systems.push_back(std::make_unique<RenderSystem>(this));
    m_systems.push_back(std::make_unique<AnimationSystem>(this));
}


void Scene::OnEvent(Event* e)
{

}

void Scene::Initialize()
{
    if (m_initializeFunction != nullptr)
        m_initializeFunction(*this);
}

std::unique_ptr<Scene> Scene::CreateDefaultScene(std::filesystem::path directory)
{
    std::unique_ptr<Scene> defaultScene = std::make_unique<Scene>();
    //defaultScene->m_path = directory / (defaultScene->m_name + ".scene.json");
    defaultScene->m_location = directory;

    defaultScene->SetInitializeFunction([](Scene& scene)
    {
        scene.m_camera = std::make_unique<Camera2D>(
            glm::vec3 { 0.0f, 0.0f, -1.0f },
            glm::vec2 { 1.0f, 1.0f },
            glm::vec2 { 1280, 720 }
        );

        Renderer& renderer = scene.m_app->GetRenderer();
        renderer.SetCamera(scene.m_camera.get());

        Entity camera = scene.m_manager.CreateEntity();
        camera.AddComponent<BaseComponent>("Main Camera");
        camera.AddComponent<TransformComponent>(glm::vec3 { 0.0f, 0.0f, -1.0f }, glm::vec3 {0.0f}, glm::vec3 {1.0f});
        camera.AddComponent<CameraComponent>();

        Entity rect = scene.m_manager.CreateEntity();
        rect.AddComponent<BaseComponent>("Rect");
        rect.AddComponent<TransformComponent>(glm::vec3 {0.0f, 0.0f, 0.0f}, glm::vec3 {0.0f}, glm::vec3 {1.0f, 1.0f, 1.0f});
        rect.AddComponent<SpriteRendererComponent>(Shape_Square);
    });

    return defaultScene;
}

std::unique_ptr<Scene> Scene::Copy(Scene& sourceScene)
{
    std::unique_ptr<Scene> newScene = std::make_unique<Scene>();

    entt::registry& srcRegistry = sourceScene.m_manager.m_registry;
    entt::registry& destRegistry = newScene->m_manager.m_registry;

    srcRegistry.each([&](auto srcEntity) {
        auto destEntity = destRegistry.create();

        CopyComponent<BaseComponent>(srcRegistry, destRegistry, srcEntity, destEntity);
        CopyComponent<TransformComponent>(srcRegistry, destRegistry, srcEntity, destEntity);
        CopyComponent<SpriteAnimatorComponent>(srcRegistry, destRegistry, srcEntity, destEntity);
        CopyComponent<SpriteRendererComponent>(srcRegistry, destRegistry, srcEntity, destEntity);
        CopyComponent<Rigidbody2DComponent>(srcRegistry, destRegistry, srcEntity, destEntity);
        CopyComponent<BoxCollider2DComponent>(srcRegistry, destRegistry, srcEntity, destEntity);
        CopyComponent<CircleCollider2DComponent>(srcRegistry, destRegistry, srcEntity, destEntity);
        CopyComponent<CameraComponent>(srcRegistry, destRegistry, srcEntity, destEntity);
        CopyComponent<NativeScriptComponent>(srcRegistry, destRegistry, srcEntity, destEntity);
    });

    return newScene;
}

void Scene::Start()
{
    StartPhysics();

    std::cout << "Scene::Start()\n";
    auto view = m_manager.m_registry.view<SpriteAnimatorComponent>();
    for (auto [entity, animator] : view.each())
    {
        Entity myEntity { &m_manager, entity };
        animator.fsm = AnimationSerializer::DeserializeFSM(animator.fileName, myEntity);
    }
}

void Scene::Stop()
{
    StopPhysics();

    auto view = m_manager.m_registry.view<NativeScriptComponent>();
    for (auto [entity, script] : view.each())
    {
        delete script.instance;
    }

    auto animators = m_manager.m_registry.view<SpriteAnimatorComponent>();
    for (auto [entity, animator] : animators.each())
    {
        delete animator.fsm;
    }
}

void Scene::StartNativeScripts(NativeScriptEngine& scriptEngine)
{

    auto view = m_manager.m_registry.view<NativeScriptComponent>();
    for (auto [entity, script] : view.each())
    {
        std::cout << "Get Native Class: " << script.className << std::endl;
        std::unique_ptr<ScriptableEntity> uniqueInstance = scriptEngine.CopyInstance(script.className);
        script.instance = uniqueInstance.release();
        
        if (script.instance != nullptr)
        {
            std::cout << "Start Native Class: " << script.className << std::endl;
            script.instance->m_app = m_app;
            script.instance->m_entity = Entity { &m_manager, entity };
            script.instance->Start();
        }
    }
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

bool Scene::HasSaved()
{
    return FileUtils::FileExists(m_path);
}

void Scene::Update(float deltaTime)
{
    auto scriptable = m_manager.m_registry.view<NativeScriptComponent>();

    for (auto [entity, nativeScript] : scriptable.each())
    {
        if (nativeScript.instance != nullptr)
            nativeScript.instance->Update(deltaTime);
    }

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

void Scene::SetCamera(Camera* camera)
{
    Renderer& renderer = m_app->GetRenderer();


    if (camera != nullptr)
    {
        renderer.SetViewMatrix(camera->GetViewMatrix());
        renderer.SetProjectionMatrix(camera->GetProjectionMatrix());
        renderer.SetCamera(camera);
    }
    else
    {
        auto view = m_manager.m_registry.view<TransformComponent, CameraComponent>();
        for (auto [entity, transform, camera] : view.each())
        {
            WindowData window = m_app->GetWindow().GetWindowData();
            float ratio = window.width / (float)window.height;
            float width = camera.size * ratio;

            renderer.SetViewMatrix(glm::inverse(glm::translate(glm::mat4(1.0f), transform.position)));
            renderer.SetProjectionMatrix(glm::ortho(-width, width, -camera.size, camera.size));
        }
    }
}

void Scene::Render()
{
    Renderer& renderer = m_app->GetRenderer();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto transforms = m_manager.m_registry.view<TransformComponent, SpriteRendererComponent>();

    for (auto [entity, transform, sprite] : transforms.each())
    {
        glm::vec2 position = transform.position;
        glm::vec2 scale = transform.scale;
        float angle = transform.rotation.z;

        if (sprite.shape == SpriteShape::Shape_Square)
            renderer.DrawQuad2D(position, scale, angle, sprite.color);
        else if (sprite.shape == SpriteShape::Shape_Circle)
        {
            Circle2D circle;
            circle.color = sprite.color;
            circle.position = transform.position;
            circle.scale = transform.scale;

            renderer.DrawCircle2D(circle);
        }

        if (sprite.sprite != nullptr)
        {
            glm::mat4 model{ 1 };
            model = glm::translate(model, glm::vec3{ transform.position.x, transform.position.y, 0 });

            glm::vec2 ratio = sprite.sprite->GetRatio();
            model = glm::scale(model, glm::vec3{ ratio.x* transform.scale.x, ratio.y* transform.scale.y, 1 });

            renderer.Draw(*sprite.sprite, model);
        }
    }

    glDisable(GL_BLEND);

    for (auto& system : m_systems)
        system->Render();
}

void Scene::RenderEditor()
{
    Renderer& renderer = m_app->GetRenderer();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto transforms = m_manager.m_registry.view<TransformComponent, SpriteRendererComponent>();

    for (auto [entity, transform, sprite] : transforms.each())
    {
        glm::vec2 position = transform.position;
        glm::vec2 scale = transform.scale;
        float angle = transform.rotation.z;

        if (sprite.shape == SpriteShape::Shape_Square)
            renderer.DrawQuad2D(position, scale, angle, sprite.color);
        else if (sprite.shape == SpriteShape::Shape_Circle)
        {
            Circle2D circle;
            circle.color = sprite.color;
            circle.position = transform.position;
            circle.scale = transform.scale;

            renderer.DrawCircle2D(circle);
        }

        if (sprite.sprite != nullptr)
        {
            glm::mat4 model{ 1 };
            model = glm::translate(model, glm::vec3{ transform.position.x, transform.position.y, 0 });

            glm::vec2 ratio = sprite.sprite->GetRatio();
            model = glm::scale(model, glm::vec3{ ratio.x* transform.scale.x, ratio.y* transform.scale.y, 1 });

            renderer.Draw(*sprite.sprite, model);
        }
    }

    glDisable(GL_BLEND);

    auto box2dColliders = m_manager.m_registry.view<TransformComponent, BoxCollider2DComponent>();

    for (auto [entity, transform, box] : box2dColliders.each())
    {
        renderer.DrawRect(transform.position, transform.scale, transform.rotation.z, {0.0f, 1.0f, 0.0f, 1.0f}, 0.02f);
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