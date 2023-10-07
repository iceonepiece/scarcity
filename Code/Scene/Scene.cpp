#include "Scene.h"
#include "Systems/ScriptableSystem.h"
#include "Systems/RenderSystem.h"
#include "Graphics/Renderer.h"
#include "Core/Camera2D.h"
#include "Components/Components.h"
#include "File/FileSystem.h"
#include "NativeScript/NativeScriptEngine.h"
#include "Animations/AnimationSerializer.h"
#include "Systems/AnimationSystem.h"
#include "Physics/NullFixtureData.h"
#include "Physics/EntityFixtureData.h"
#include "Audio/Audio.h"

Scene::Scene(const std::string& name, const std::filesystem::path& path)
    : m_name(name)
    , m_path(path)
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

entt::entity Scene::DuplicateEntity(entt::entity entity)
{
    entt::registry& registry = m_manager.m_registry;
    auto newEntity = registry.create();

    std::apply([&](auto... componentTypes) {
        (DuplicateComponent<decltype(componentTypes)>(registry, entity, newEntity), ...);
    }, ComponentList{});

    return newEntity;
}

void Scene::SpawnCollision2D(Collision2D* collision)
{
    m_spawnCommands.push_back({ collision });
}

void Scene::DestroyEntity(entt::entity entity)
{
    if (m_manager.m_registry.all_of<Rigidbody2DComponent>(entity))
    {
        DestroyPhysicsEntity(m_manager.m_registry.get<Rigidbody2DComponent>(entity));
    }

    m_manager.m_registry.destroy(entity);
}

void Scene::Start()
{
    StartPhysics();

    std::cout << "Scene::Start()\n";
    auto view = m_manager.m_registry.view<SpriteAnimatorComponent>();

    for (auto [entity, animator] : view.each())
    {
        Entity myEntity{ &m_manager, entity };

        AnimatorController* baseAnimator = ResourceAPI::GetResourceManager()->GetAnimatorController(animator.controllerName);
        animator.controller = baseAnimator != nullptr ? new AnimatorController(*baseAnimator) : nullptr;
    }

    auto audioView = m_manager.m_registry.view<AudioSourceComponent>();

    for (auto [entity, audioSource] : audioView.each())
    {
        audioSource.audioSource = m_app->GetAudio().CreateAudioSource();
    }

    auto cameraView = m_manager.m_registry.view<CameraComponent>();

    for (auto [entity, camera] : cameraView.each())
    {
        uint64_t id = camera.targetID;
        entt::entity target = entt::null;

        if (m_manager.m_uniqueIDToEntity.find(id) != m_manager.m_uniqueIDToEntity.end())
        {
            target = m_manager.m_uniqueIDToEntity[id];
        }

        camera.targetEntity = Entity{ &m_manager, target };
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
        delete animator.controller;
    }

    auto audioView = m_manager.m_registry.view<AudioSourceComponent>();
    for (auto [entity, audioSource] : audioView.each())
    {
        delete audioSource.audioSource;
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
            script.instance->m_scene = this;
            script.instance->m_entity = Entity { &m_manager, entity };
            script.instance->Start();
        }
    }
}

void Scene::RenderTexts()
{
    Renderer& renderer = Application::Get().GetRenderer();

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto textView = m_manager.m_registry.view<TransformComponent, CanvasComponent, TextComponent>();
    for (auto [entity, transform, canvas, text] : textView.each())
    {
        renderer.DrawText(text.text, canvas.position, text.size, text.color);
    }

    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
}

void Scene::RenderCollisionComponents()
{
    Renderer& renderer = Application::Get().GetRenderer();

    auto box2dColliders = m_manager.m_registry.view<TransformComponent, BoxCollider2DComponent>();

    for (auto [entity, transform, box] : box2dColliders.each())
    {
        glm::vec2 drawPosition {
            transform.position.x + transform.scale.x * box.offset.x,
            transform.position.y + transform.scale.y * box.offset.y
        };
        glm::vec2 drawSize { box.size.x * transform.scale.x, box.size.y * transform.scale.y };
        renderer.DrawRect(drawPosition, drawSize, transform.rotation.z, { 0.0f, 1.0f, 0.0f, 1.0f }, 0.02f);
    }

    auto circle2dColliders = m_manager.m_registry.view<TransformComponent, CircleCollider2DComponent>();

    for (auto [entity, transform, circle] : circle2dColliders.each())
    {
        glm::vec2 drawPosition {
            transform.position.x + transform.scale.x * circle.offset.x,
            transform.position.y + transform.scale.y * circle.offset.y
        };

        Circle2D renderCircle;
        renderCircle.position = drawPosition;
        renderCircle.scale = transform.scale;
        renderCircle.radius = circle.radius;
        renderCircle.color = { 0, 1, 0, 1 };

        renderer.DrawCircle2D(renderCircle, 0.01f);
    }
}

void Scene::InitializePhysicsEntity(entt::entity entity, TransformComponent& transform, Rigidbody2DComponent& rb2d)
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

        glm::vec2 absoluteScale { std::abs(transform.scale.x), std::abs(transform.scale.y) };

        b2PolygonShape boxShape;
        boxShape.SetAsBox(
            bc2d.size.x * absoluteScale.x / 2,
            bc2d.size.y * absoluteScale.y / 2,
            b2Vec2(bc2d.offset.x * absoluteScale.x, bc2d.offset.y * absoluteScale.y),
            0.0f
        );

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &boxShape;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.0f;
        fixtureDef.isSensor = bc2d.isTrigger;

        FixtureData* fixtureData = new EntityFixtureData(Entity{ &m_manager, entity });
        fixtureData->tag = m_manager.m_registry.get<BaseComponent>(entity).tag;
        fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(fixtureData);
        rb2d.fixtureData = fixtureData;

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
        fixtureDef.isSensor = cc2d.isTrigger;

        FixtureData* fixtureData = new EntityFixtureData(Entity{ &m_manager, entity });
        fixtureData->tag = m_manager.m_registry.get<BaseComponent>(entity).tag;
        fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(fixtureData);
        rb2d.fixtureData = fixtureData;

        body->CreateFixture(&fixtureDef);
    }
}

void Scene::DestroyPhysicsEntity(Rigidbody2DComponent& rb2d)
{
    std::vector<FixtureData*> pendingList;

    if (b2Body* body = (b2Body*)rb2d.body)
    {
        auto fixture = body->GetFixtureList();

        while (fixture)
        {
            if (FixtureData* fixtureData = (FixtureData*)fixture->GetUserData().pointer)
            {
                pendingList.push_back((FixtureData*)fixture->GetUserData().pointer);
            }

            fixture = fixture->GetNext();
        }

        m_physics->DestroyBody(body);
    }

    for (auto fixtureData : pendingList)
    {
        delete fixtureData;
    }
}

void Scene::StartPhysics()
{
    m_physics = std::make_unique<b2World>(b2Vec2 { 0.0f, -9.8f });
    m_contactListener = std::make_unique<ContactListener>();
    m_physics->SetContactListener(m_contactListener.get());

	auto view = m_manager.m_registry.view<TransformComponent, Rigidbody2DComponent>();
	for (auto [entity, transform, rb2d] : view.each())
	{
        InitializePhysicsEntity(entity, transform, rb2d);
	}
}

void Scene::StopPhysics()
{
    auto view = m_manager.m_registry.view<Rigidbody2DComponent>();
    for (auto [entity, rb2d] : view.each())
    {
        DestroyPhysicsEntity(rb2d);
    }

    m_physics.reset();
}

bool Scene::HasSaved()
{
    return FileSystem::FileExists(m_path);
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

    for (auto& command : m_spawnCommands)
    {
        Entity entity = m_manager.CreateEntity();
        auto& base = entity.AddComponent<BaseComponent>();
        base.name = command.collision->name;
        base.tag = command.collision->tag;

        auto& transform = entity.AddComponent<TransformComponent>(glm::vec3 { command.collision->x, command.collision->y, 0.0f }, glm::vec3 {0}, glm::vec3 {1, 1, 0});
        auto& rb2d = entity.AddComponent<Rigidbody2DComponent>();

        if (BoxCollision2D* collision = (BoxCollision2D*)(command.collision))
        {
            entity.AddComponent<BoxCollider2DComponent>();
        }
        else if (CircleCollision2D* collision = (CircleCollision2D*)(command.collision))
        {
            auto& circle = entity.AddComponent<CircleCollider2DComponent>();
            circle.radius = collision->radius;
        }

        if (command.collision->lifetime > 0.0f)
        {
            entity.AddComponent<TimerComponent>(0.0f, command.collision->lifetime);
        }

        InitializePhysicsEntity(entity.GetEntity(), transform, rb2d);

        delete command.collision;
    }
    m_spawnCommands.clear();

    auto timerView = m_manager.m_registry.view<TimerComponent>();

    for (auto [entity, timer] : timerView.each())
    {
        timer.timer += deltaTime;

        if (timer.timer >= timer.lifeTime)
            DestroyEntity(entity);
    }
}

void Scene::Enter()
{
    auto view = m_manager.m_registry.view<ScriptableComponent>();

    for (auto [entity, scriptable] : view.each())
        scriptable.scriptable->Start();

   Application::Get().GetRenderer().SetCamera(m_camera.get());
}

void Scene::Exit()
{

}

void Scene::SetCamera(Camera* camera)
{
    Application& app = Application::Get();
    Renderer& renderer = app.GetRenderer();

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
            WindowData window = app.GetWindow().GetWindowData();
            float ratio = window.width / (float)window.height;
            float width = camera.size * ratio;

            renderer.SetViewMatrix(glm::inverse(glm::translate(glm::mat4(1.0f), transform.position)));
            renderer.SetProjectionMatrix(glm::ortho(-width, width, -camera.size, camera.size));
        }
    }
}

void Scene::Render()
{
    Renderer& renderer = Application::Get().GetRenderer();

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

        SpriteAnimatorComponent* spriteAnimator = m_manager.m_registry.try_get<SpriteAnimatorComponent>(entity);
        if (spriteAnimator != nullptr && spriteAnimator->controller != nullptr)
        {
            AnimatorState* animState = spriteAnimator->controller->GetCurrentState();
            SpriteAnimation& spriteAnim = animState->GetSpriteAnimation();
            Sprite sprite = spriteAnim.GetSprite();
            renderer.DrawSprite(sprite, transform.position, transform.scale, transform.rotation.z);
        }
        else if (sprite.sprite != nullptr)
            renderer.DrawSprite(*(sprite.sprite), transform.position, transform.scale, transform.rotation.z);
    }

    glDisable(GL_BLEND);

    for (auto& system : m_systems)
        system->Render();

    RenderUI();
    RenderCollisionComponents();
    RenderTexts();
}

void Scene::RenderEditor()
{
    Renderer& renderer = Application::Get().GetRenderer();

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
            renderer.DrawSprite(*sprite.sprite, transform.position, transform.scale, transform.rotation.z);
        }
    }

    glDisable(GL_BLEND);

    auto buttonView = m_manager.m_registry.view<TransformComponent, CanvasComponent, ButtonComponent>();
    for (auto [entity, transform, canvas, button] : buttonView.each())
    {
        renderer.DrawQuad2D(transform.position, canvas.size, transform.rotation.z);
    }

    RenderCollisionComponents();
    RenderTexts();
}

void Scene::RenderUI()
{
    Renderer& renderer = Application::Get().GetRenderer();

    auto canvas = m_manager.m_registry.view<TransformComponent, CanvasComponent, ButtonComponent>();
    for (auto [entity, transform, canvas, button] : canvas.each())
    {
        glm::vec4 color{ 1 };
        renderer.DrawQuadUI(transform.position, canvas.size, color, UIAlignment::CENTER);
    }
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