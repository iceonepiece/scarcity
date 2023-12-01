#include "Scene.h"
#include "Core/Application.h"
#include "Events/Event.h"
#include "Graphics/Renderer.h"
#include "Graphics/Camera2D.h"
#include "Components/Components.h"
#include "File/FileSystem.h"
#include "NativeScript/NativeScriptEngine.h"
#include "Animations/AnimationSerializer.h"
#include "Physics/NullFixtureData.h"
#include "Physics/EntityFixtureData.h"
#include "Audio/Audio.h"
#include "UI/UIManager.h"

Scene::Scene(const std::string& name, const std::filesystem::path& path)
    : Asset(path, AssetType::Scene)
    , m_name(name)
	, m_camera(
        new Camera2D({ 0.0f, 0.0f, -14.0f }, { 0.5f, 0.25f }, { 1280, 720 })
    )
{
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

    registry.emplace<IDComponent>(newEntity);

    std::apply([&](auto... componentTypes) {
        (DuplicateComponent<decltype(componentTypes)>(registry, entity, newEntity), ...);
    }, ComponentList{});

    return newEntity;
}

void Scene::SpawnCollision2D(Collision2D* collision)
{
    m_spawnCommands.push_back({ collision });
}

void Scene::OnDestroyEntity(entt::entity entity)
{
    if (m_manager.IsEntityValid(entity))
        m_toDestroyEntities.push_back(entity);
}


void Scene::DestroyEntity(entt::entity entity)
{
    if (m_manager.m_registry.all_of<Rigidbody2DComponent>(entity))
    {
        //DestroyPhysicsEntity(m_manager.m_registry.get<Rigidbody2DComponent>(entity));
    }

    if (NativeScriptComponent* nativeScript = m_manager.m_registry.try_get<NativeScriptComponent>(entity))
    {
        delete nativeScript->instance;
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

        if (animator.prototypeController != nullptr)
        {
            animator.controller = new AnimatorController(animator.prototypeController->GetPath());
            animator.controller->Initialize();
        }
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

    auto canvasView = m_manager.m_registry.view<CanvasComponent>();
    for (auto [entity, canvas] : canvasView.each())
    {
        if (ButtonComponent* button = m_manager.m_registry.try_get<ButtonComponent>(entity))
        {
            uint64_t id = button->targetID;
            entt::entity target = entt::null;

            if (m_manager.m_uniqueIDToEntity.find(id) != m_manager.m_uniqueIDToEntity.end())
            {
                target = m_manager.m_uniqueIDToEntity[id];
            }

            button->targetEntity = Entity{ &m_manager, target };

            if (NativeScriptComponent* nativeScript = button->targetEntity.GetComponent<NativeScriptComponent>())
            {
                if (ScriptableEntity* scriptable = nativeScript->instance)
                {
                    scriptable->ExportFunctions();
                    std::string functionName = button->functionName;

                    button->mousePressedHandler += [functionName, scriptable](void* context, UIComponent& uiComponent)
                    {
                        scriptable->CallFunction(functionName);
                    };
                }
            }
        }
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
    renderer.SetScreenSize(m_viewportWidth, m_viewportHeight);
    renderer.PreRender(true);

    auto textView = m_manager.m_registry.view<TransformComponent, CanvasComponent, TextComponent>();
    for (auto [entity, transform, canvas, text] : textView.each())
    {
        renderer.DrawText(text.text, canvas.position, text.size, text.color);
    }

    renderer.PostRender(true);
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

    auto gridColliders = m_manager.m_registry.view<TransformComponent, GridComponent, Rigidbody2DComponent>();

    for (auto [entity, transform, grid, rigid] : gridColliders.each())
    {
        b2Body* body = (b2Body*)rigid.body;

        if (!body)
            continue;

        b2Fixture* fixture = body->GetFixtureList();

        while (fixture)
        {
            if (b2ChainShape* chain = dynamic_cast<b2ChainShape*>(fixture->GetShape()))
            {
                for (int32 i = 0; i < chain->GetChildCount(); ++i)
                {
                    b2EdgeShape edge;
                    chain->GetChildEdge(&edge, i);
                    
                    glm::vec3 v1{ edge.m_vertex1.x, edge.m_vertex1.y, 0 };
                    glm::vec3 v2{ edge.m_vertex2.x, edge.m_vertex2.y, 0 };

                    renderer.DrawLine(v1, v2, { 0.0f, 1.0f, 0.0f, 1.0f });
                }
            }

            fixture = fixture->GetNext();
        }
    }
}

/*
void Scene::InitializePhysicsEntity(entt::entity entity, TransformComponent& transform, Rigidbody2DComponent& rb2d)
{
    b2BodyDef bodyDef;
    bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d.type);
    bodyDef.position.Set(transform.position.x, transform.position.y);
    bodyDef.angle = transform.rotation.z;
    bodyDef.gravityScale = rb2d.gravityScale;

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
        fixtureData->m_tag = m_manager.m_registry.get<BaseComponent>(entity).tag;
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
        fixtureData->m_tag = m_manager.m_registry.get<BaseComponent>(entity).tag;
        fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(fixtureData);
        rb2d.fixtureData = fixtureData;

        body->CreateFixture(&fixtureDef);
    }

    if (m_manager.m_registry.all_of<GridComponent>(entity))
    {
        auto& grid = m_manager.m_registry.get<GridComponent>(entity);

        b2ChainShape chain;

        for (int i = 0; i < 1; i++)
        {
            int edgeSize = grid.polygons[i].size();

            b2Vec2* vs = new b2Vec2[edgeSize];

            for (int j = 0; j < edgeSize; j ++)
            {
                int x = grid.polygons[i][j].startCell.first;
                int y = grid.polygons[i][j].startCell.second;
                EdgeOnCell onCell = grid.polygons[i][j].onCell;

                float pointX = x;
                float pointY = y;

                if (onCell == EdgeOnCell::Left)
                {
                    pointY++;
                }
                else if (onCell == EdgeOnCell::Right)
                {
                    pointX++;
                }
                else if (onCell == EdgeOnCell::Top)
                {
                    pointX++;
                    pointY++;
                }

                //vs[j].Set(pointX, pointY);
                std::cout << pointX << "," << pointY << '\n';
            }

            b2Vec2 ss[4];
            ss[0].Set(2.0f, 0.0f);
            ss[1].Set(1.0f, 0.25f);
            ss[2].Set(0.0f, 0.0f);
            ss[3].Set(-2.0f, 0.4f);
            chain.CreateLoop(ss, 4);
            
            delete [] vs;
        }


        b2FixtureDef fixtureDef;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.0f;
        fixtureDef.isSensor = false;
        fixtureDef.shape = &chain;
        
        FixtureData* fixtureData = new EntityFixtureData(Entity{ &m_manager, entity });
        fixtureData->m_tag = m_manager.m_registry.get<BaseComponent>(entity).tag;
        fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(fixtureData);
        rb2d.fixtureData = fixtureData;

        body->CreateFixture(&fixtureDef);

    }
}
*/

/*
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
*/

void Scene::StartPhysics()
{
    /*
    m_physics = std::make_unique<b2World>(b2Vec2 { 0.0f, -9.8f });
    m_contactListener = std::make_unique<ContactListener>();
    m_physics->SetContactListener(m_contactListener.get());

	auto view = m_manager.m_registry.view<TransformComponent, Rigidbody2DComponent>();
	for (auto [entity, transform, rb2d] : view.each())
	{
        InitializePhysicsEntity(entity, transform, rb2d);
	}
    */

    auto view = m_manager.m_registry.view<TransformComponent, Rigidbody2DComponent>();
    for (auto [entity, transform, rb2d] : view.each())
    {
        Entity _entity { &m_manager, entity };
        m_physics.InitializePhysicsEntity(_entity, transform, rb2d);
    }

}

void Scene::StopPhysics()
{
    /*
    auto view = m_manager.m_registry.view<Rigidbody2DComponent>();
    for (auto [entity, rb2d] : view.each())
    {
        DestroyPhysicsEntity(rb2d);
    }

    m_physics.reset();
    */

    auto view = m_manager.m_registry.view<Rigidbody2DComponent>();
    for (auto [entity, rb2d] : view.each())
        m_physics.DestroyPhysicsEntity(rb2d);
}

std::filesystem::path Scene::GetAbsolutePath()
{
    return m_project->GetAbsolutePath() / m_path;
}

bool Scene::HasSaved()
{
    return FileSystem::FileExists(GetAbsolutePath());
}

void Scene::Update(float deltaTime)
{
    auto nativeScriptUpdateView = m_manager.m_registry.view<NativeScriptComponent>();

    for (auto [entity, nativeScript] : nativeScriptUpdateView.each())
    {
        if (nativeScript.instance != nullptr)
            nativeScript.instance->Update(deltaTime);
    }

    auto canvasAdjustView = m_manager.m_registry.view<TransformComponent, CanvasComponent>();
    for (auto [entity, transform, canvas] : canvasAdjustView.each())
    {
        float x = transform.position.x;
        float y = transform.position.y;

        switch (canvas.horizontalAligment)
        {
            case Center: x += m_viewportWidth / 2.0f; break;
            case Right: x += m_viewportWidth; break;
        }

        switch (canvas.verticalAlignment)
        {
            case Middle: y += m_viewportHeight / 2.0f; break;
            case Bottom: y += m_viewportHeight; break;
        }

        canvas.position.x = x;
        canvas.position.y = y;
    }

    Input& input = m_app->GetInput();

    auto canvasHandleInputView = m_manager.m_registry.view<CanvasComponent>();
    for (auto [entity, canvas] : canvasHandleInputView.each())
    {
        if (ButtonComponent* button = m_manager.m_registry.try_get<ButtonComponent>(entity))
        {
            UIManager::HandleInput(canvas, *button, input);
        }
    }

    for (auto& system : m_systems)
        system->Update(deltaTime);

    auto animatorView = m_manager.m_registry.view<SpriteAnimatorComponent>();
    for (auto [entity, animator] : animatorView.each())
    {
        animator.controller->Process();
    }

    //if (m_physics != nullptr && physicsActive)
    if (physicsActive)
    {
        /*
        const int32_t velocityIterations = 6;
        const int32_t positionIterations = 2;
        m_physics->Step(deltaTime, velocityIterations, positionIterations);
        */
        m_physics.Update(deltaTime);

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

        //InitializePhysicsEntity(entity.GetEntity(), transform, rb2d);

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

    for (auto entity : m_toDestroyEntities)
    {
        DestroyEntity(entity);
    }
    m_toDestroyEntities.clear();

    auto nativeScriptLateUpdateView = m_manager.m_registry.view<NativeScriptComponent>();

    for (auto [entity, nativeScript] : nativeScriptLateUpdateView.each())
    {
        if (nativeScript.instance != nullptr)
            nativeScript.instance->LateUpdate(deltaTime);
    }
}

void Scene::Enter()
{
   Application::Get().GetRenderer().SetCamera(m_camera.get());
}

void Scene::Exit()
{

}

void Scene::UpdateUI(float deltaTime)
{
    /*
    auto canvasUpdateView = m_manager.m_registry.view<CanvasComponent>();
    for (auto [entity, canvas] : canvasUpdateView.each())
    {
        if (ButtonComponent* button = m_manager.m_registry.try_get<ButtonComponent>(entity))
        {
            button->instance.SetPosition(canvas.position);
            button->instance.SetSize(canvas.size);
            button->instance.SetBackgroundColor(button->color);
        }
    }
    */
}

void Scene::SetViewportSize(unsigned int width, unsigned int height)
{
    m_viewportWidth = width;
    m_viewportHeight = height;
}

void Scene::OnViewportResize()
{
    Renderer& renderer = m_app->GetRenderer();

    auto view = m_manager.m_registry.view<TransformComponent, CameraComponent>();
    for (auto [entity, transform, camera] : view.each())
    {
        float ratio = m_viewportWidth / (float)m_viewportHeight;
        float width = camera.size * ratio;

        renderer.SetViewMatrix(glm::inverse(glm::translate(glm::mat4(1.0f), transform.position)));
        renderer.SetProjectionMatrix(glm::ortho(-width, width, -camera.size, camera.size));
    }
}

void Scene::SetCamera(Camera& camera)
{
    Application& app = Application::Get();
    Renderer& renderer = app.GetRenderer();

    renderer.SetViewMatrix(camera.GetViewMatrix());
    renderer.SetProjectionMatrix(camera.GetProjectionMatrix());
    renderer.SetCamera(&camera);
}

void Scene::Render(RenderOptions renderOptions)
{
    Renderer& renderer = Application::Get().GetRenderer();
    renderer.PreRender();

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
            /*
            AnimatorState* animState = spriteAnimator->controller->GetCurrentState();
            SpriteAnimation spriteAnim = animState->GetSpriteAnimation();
            Sprite sprite = spriteAnim.GetSprite();
            renderer.DrawSprite(sprite, transform.position, transform.scale, transform.rotation.z);
            */
        }
        else if (sprite.sprite != nullptr)
        {
            Sprite& targetSprite = sprite.image->GetSprites()[sprite.spriteIndex];
            renderer.DrawSprite(targetSprite, transform.position, transform.scale, transform.rotation.z);
        }
    }

    renderer.PostRender();

    for (auto& system : m_systems)
        system->Render();

    RenderUI();

    if (renderOptions.collisionVisible)
        RenderCollisionComponents();

    RenderTexts();
}

void Scene::RenderEditor(RenderOptions renderOptions)
{
    Renderer& renderer = Application::Get().GetRenderer();
    renderer.PreRender();

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

    auto gridView = m_manager.m_registry.view<GridComponent>();

    for (auto [entity, grid] : gridView.each())
    {
        float halfSize = grid.size / 2.0f;
        glm::vec2 cellSize { grid.size, grid.size };

        for (auto& [pos, second] : grid.cellMap)
        {
            renderer.DrawQuad2D({ pos.first * grid.size + halfSize, pos.second * grid.size + halfSize }, cellSize, 0.0f, grid.color);
        }
    }

    renderer.PostRender();

    RenderUI();

    if (renderOptions.collisionVisible)
        RenderCollisionComponents();

    RenderTexts();
}

void Scene::RenderUI()
{
    Renderer& renderer = Application::Get().GetRenderer();
    renderer.SetScreenSize(m_viewportWidth, m_viewportHeight);

    /*
    auto canvas = m_manager.m_registry.view<TransformComponent, CanvasComponent, ButtonComponent>();
    for (auto [entity, transform, canvas, button] : canvas.each())
    {
        renderer.DrawQuadUI(canvas.position, canvas.size, button.color, UIAlignment::NONE);
    }
    */

    auto canvasView = m_manager.m_registry.view<TransformComponent, CanvasComponent>();
    for (auto [entity, transform, canvas] : canvasView.each())
    {
        if (ButtonComponent* button = m_manager.m_registry.try_get<ButtonComponent>(entity))
        {
            renderer.DrawQuadUI(canvas.position, canvas.size, button->color);
        }
    }
}

Entity Scene::InstantiateEntity(Entity entity, const glm::vec3& position, bool playing)
{
    entt::registry& srcRegistry = entity.GetRegistry();
    entt::registry& registry = m_manager.m_registry;

    auto newEntity = registry.create();
    registry.emplace<IDComponent>(newEntity);

    std::apply([&](auto... componentTypes) {
        (CopyComponent<decltype(componentTypes)>(srcRegistry, registry, entity.GetEntity(), newEntity), ...);
    }, ComponentList{});

    Entity returnEntity { &m_manager, newEntity };

    if (TransformComponent* transform = returnEntity.GetComponent<TransformComponent>())
    {
        transform->position = position;

        //if (Rigidbody2DComponent* rb2d = returnEntity.GetComponent<Rigidbody2DComponent>())
          //  InitializePhysicsEntity(newEntity, *transform, *rb2d);
    }

    if (playing)
    {
        if (NativeScriptComponent* nativeScript = returnEntity.GetComponent<NativeScriptComponent>())
        {
            std::unique_ptr<ScriptableEntity> uniqueInstance = m_app->GetNativeScriptEngine().CopyInstance(nativeScript->className);
            nativeScript->instance = uniqueInstance.release();

            if (nativeScript->instance != nullptr)
            {
                nativeScript->instance->m_app = m_app;
                nativeScript->instance->m_scene = this;
                nativeScript->instance->m_entity = returnEntity;
                nativeScript->instance->Start();
            }
        }
    }

    return returnEntity;
}

EntityManager& Scene::GetEntityManager()
{
    return m_manager;
}