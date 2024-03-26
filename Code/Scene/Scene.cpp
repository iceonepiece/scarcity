#include "Scene.h"
#include "Core/Application.h"
#include "Events/Event.h"
#include "Graphics/Renderer.h"
#include "Graphics/Camera2D.h"
#include "Components/Components.h"
#include "File/FileSystem.h"
#include "NativeScript/NativeScriptEngine.h"
#include "Lua/LuaEngine.h"
#include "Audio/Audio.h"
#include "UI/UIManager.h"
#include "Input/Input.h"
#include "Project/Project.h"
#include "Entity/ScriptableEntity.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include "System/NativeScriptSystem.h"
#include "System/LuaScriptSystem.h"
#include "System/PhysicsSystem.h"
#include "System/HierarchySystem.h"
#include "System/AnimatorSystem.h"
#include "System/TilemapSystem.h"

struct RenderCommand
{
    Sprite* sprite;
    TransformComponent transform;
    int order;
};

Scene::Scene(const std::string& name, const std::filesystem::path& path)
    : Asset(path, AssetType::Scene)
    , m_name(name)
	, m_camera(
        new Camera2D({ 0.0f, 0.0f, -14.0f }, { 0.5f, 0.25f }, { 1280, 720 })
    )
{
    m_systems.push_back(std::make_unique<NativeScriptSystem>(*this));
    m_systems.push_back(std::make_unique<LuaScriptSystem>(*this));
    m_systems.push_back(std::make_unique<PhysicsSystem>(*this));
    m_systems.push_back(std::make_unique<AnimatorSystem>(*this));
    //m_systems.push_back(std::make_unique<TilemapSystem>(*this));

    m_luaEngine = std::make_unique<LuaEngine>();
}

void Scene::SetProject(Project* project)
{
    m_project = project;
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
        m_physics.DestroyPhysicsEntity(m_manager.m_registry.get<Rigidbody2DComponent>(entity));
    }

    if (NativeScriptComponent* nativeScript = m_manager.m_registry.try_get<NativeScriptComponent>(entity))
    {
        delete nativeScript->instance;
    }

    m_manager.m_registry.destroy(entity);
}

void Scene::Start()
{
    //StartPhysics();
    s_activeScene = this;

    for (auto& system : m_systems)
    {
        system->OnStart();
    }

    std::cout << "Scene::Start()\n";
    auto view = m_manager.m_registry.view<SpriteAnimatorComponent>();


    for (auto [entity, animator] : view.each())
    {
        Entity myEntity{ &m_manager, entity };

        if (animator.prototypeController != nullptr)
        {
            animator.controller = new AnimatorController(animator.prototypeController->GetPath());
            animator.controller->DoDeserialize(true);
            animator.controller->Initialize();
        }
    }

    
    auto luaView = m_manager.m_registry.view<LuaScriptComponent>();

    for (auto [entity, luaScript] : luaView.each())
    {
        luaScript.engine = new LuaEngine();

        if (luaScript.script != nullptr && FileSystem::FileExists(luaScript.script->GetPath()))
        {
            luaScript.engine->ReadScript(luaScript.script->GetPath().string());
        }
    }

    m_app->GetUIManager().ClearStack();

    auto audioView = m_manager.m_registry.view<AudioSourceComponent>();

    for (auto [entity, audioSource] : audioView.each())
    {
        audioSource.audioSource = Audio::Get()->CreateAudioSource();
        audioSource.audioSource->SetVolume(audioSource.volume);
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
    //StopPhysics();
    for (auto& system : m_systems)
    {
        system->OnStop();
    }

    auto luaView = m_manager.m_registry.view<LuaScriptComponent>();

    for (auto [entity, luaScript] : luaView.each())
    {
        delete luaScript.engine;
    }

    auto view = m_manager.m_registry.view<NativeScriptComponent>();
    for (auto [entity, script] : view.each())
    {
        delete script.instance;
    }
    m_scriptableEntities.clear();

    auto animators = m_manager.m_registry.view<SpriteAnimatorComponent>();
    for (auto [entity, animator] : animators.each())
    {
        delete animator.controller;
    }

    auto audioView = m_manager.m_registry.view<AudioSourceComponent>();
    for (auto [entity, audioSource] : audioView.each())
    {
        audioSource.audioSource->Destroy();
        delete audioSource.audioSource;
    }
}

void Scene::StartNativeScripts(NativeScriptEngine& scriptEngine)
{
    auto view = m_manager.m_registry.view<BaseComponent, NativeScriptComponent>();
    for (auto [entity, base, script] : view.each())
    {
        std::cout << "Get Native Class: " << script.className << std::endl;
        std::unique_ptr<ScriptableEntity> uniqueInstance = scriptEngine.CopyInstance(script.className);
        script.instance = uniqueInstance.release();
        
        if (script.instance != nullptr)
        {
            std::cout << "Start Native Class: " << script.className << std::endl;
            script.instance->m_app = m_app;
            script.instance->m_project = Project::GetActive();
            script.instance->m_scene = this;
            script.instance->m_entity = Entity { &m_manager, entity };
            m_scriptableEntities.insert({ base.name, script.instance });
        }
    }

    /*
    auto nativeScriptView = m_manager.m_registry.view<NativeScriptComponent>();
    for (auto [entity, script] : nativeScriptView.each())
    {
        if (script.instance != nullptr)
			script.instance->Start();
    }
    */
}

void Scene::RenderCollisionComponents()
{
    Renderer& renderer = Application::Get().GetRenderer();

    auto rb2dView = m_manager.m_registry.view<TransformComponent, Rigidbody2DComponent>();

    for (auto [entity, transform, rb2d] : rb2dView.each())
    {
        b2Body* body = (b2Body*)rb2d.body;

        if (!body)
            continue;

        b2Fixture* fixture = body->GetFixtureList();

        while (fixture)
        {
            if (fixture->GetFilterData().categoryBits > 0 && fixture->GetShape()->GetType() == b2Shape::e_polygon)
            {
				b2PolygonShape* shape = (b2PolygonShape*)fixture->GetShape();

                if (shape->m_count == 4)
                {
                    float width = shape->m_vertices[1].x - shape->m_vertices[0].x;
                    float height = shape->m_vertices[2].y - shape->m_vertices[0].y;

					float posX = transform.position.x + shape->m_centroid.x;
                    float posY = transform.position.y + shape->m_centroid.y;

                    renderer.DrawRect({ posX, posY }, {width, height}, transform.rotation.z, {0.0f, 1.0f, 0.0f, 1.0f}, 0.02f);
                }
            }

            fixture = fixture->GetNext();
        }


        if (rb2d.flippedBody != nullptr)
        {
            b2Body* body = (b2Body*)rb2d.flippedBody;

            b2Fixture* fixture = body->GetFixtureList();

            while (fixture)
            {
                if (fixture->GetShape()->GetType() == b2Shape::e_polygon)
                {
                    b2PolygonShape* shape = (b2PolygonShape*)fixture->GetShape();

                    if (shape->m_count == 4)
                    {
                        float width = shape->m_vertices[1].x - shape->m_vertices[0].x;
                        float height = shape->m_vertices[2].y - shape->m_vertices[0].y;

                        float posX = transform.position.x + shape->m_centroid.x;
                        float posY = transform.position.y + shape->m_centroid.y;

                        renderer.DrawRect({ posX, posY }, { width, height }, transform.rotation.z, { 0.0f, 1.0f, 0.0f, 1.0f }, 0.02f);
                    }
                }

                fixture = fixture->GetNext();
            }
        }
    }

    /*
    auto groupColliders = m_manager.m_registry.view<TransformComponent, Collider2DGroupComponent>();
    for (auto [entity, transform, group] : groupColliders.each())
    {
        for (auto& collider : group.colliders)
        {
            glm::vec2 drawPosition{
                transform.position.x + transform.scale.x * collider.offset.x,
                transform.position.y + transform.scale.y * collider.offset.y
            };

            if (collider.type == ColliderType::Box)
            {
                glm::vec2 drawSize{ collider.size.x * transform.scale.x, collider.size.y * transform.scale.y };
                renderer.DrawRect(drawPosition, drawSize, transform.rotation.z, { 0.0f, 1.0f, 0.0f, 1.0f }, 0.02f);
            }
            else if (collider.type == ColliderType::Circle)
            {
                Circle2D renderCircle;
                renderCircle.position = drawPosition;
                renderCircle.scale = transform.scale;
                renderCircle.radius = collider.size.x;
                renderCircle.color = { 0, 1, 0, 1 };

                renderer.DrawCircle2D(renderCircle, 0.01f);
            }
        }
    }

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
    */

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

void Scene::StartPhysics()
{
    auto view = m_manager.m_registry.view<TransformComponent, Rigidbody2DComponent>();
    for (auto [entity, transform, rb2d] : view.each())
    {
        Entity _entity { &m_manager, entity };
        m_physics.InitializePhysicsEntity(_entity, transform, rb2d);
    }

}

void Scene::StopPhysics()
{
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
    Renderer& renderer = m_app->GetRenderer();
    glm::vec2 screenSize = renderer.GetScreenSize();

    for (auto& system : m_systems)
    {
        if (system->IsActive())
            system->Update(deltaTime);
    }

    auto luaScriptView = m_manager.m_registry.view<LuaScriptComponent>();
    for (auto [entity, luaScript] : luaScriptView.each())
    {
        if (luaScript.script != nullptr && FileSystem::FileExists(luaScript.script->GetPath()))
        {
            //m_luaEngine->ReadScript(luaScript.script->GetPath().string());
            //sol::function updateFn = m_luaEngine->GetFunction("Update");
            //luaScript.engine->ReadScript(luaScript.script->GetPath().string());
            sol::function updateFn = luaScript.engine->GetFunction("Update");
            updateFn(deltaTime);
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

    auto view = m_manager.m_registry.view<TransformComponent, CameraComponent>();
    for (auto [entity, transform, camera] : view.each())
    {
        float ratio = screenSize.x / (float)screenSize.y;
        float width = camera.size * ratio;

        glm::mat4 viewMatrix = glm::inverse(glm::translate(glm::mat4(1.0f), transform.position));
        glm::mat4 projectionMatirx = glm::ortho(-width, width, -camera.size, camera.size);

        renderer.SetViewProjectionMatrix(projectionMatirx * viewMatrix);
    }
}

void Scene::Enter()
{
    auto nativeScriptView = m_manager.m_registry.view<NativeScriptComponent>();
    for (auto [entity, script] : nativeScriptView.each())
    {
        if (script.instance != nullptr)
            script.instance->Start();
    }
}

void Scene::Exit()
{

}

/*
void Scene::OnViewportResize()
{
    Renderer& renderer = m_app->GetRenderer();
    glm::vec2 screenSize  = renderer.GetScreenSize();

    auto view = m_manager.m_registry.view<TransformComponent, CameraComponent>();
    for (auto [entity, transform, camera] : view.each())
    {
        float ratio = screenSize.x / (float)screenSize.y;
        float width = camera.size * ratio;

        glm::mat4 viewMatrix = glm::inverse(glm::translate(glm::mat4(1.0f), transform.position));
        glm::mat4 projectionMatirx = glm::ortho(-width, width, -camera.size, camera.size);

        renderer.SetViewProjectionMatrix(projectionMatirx * viewMatrix);
    }
}
*/

void Scene::SetCamera(Camera& camera)
{
    Application::Get().GetRenderer().SetCamera(camera);
}

void Scene::Render(RenderOptions renderOptions)
{
    Renderer& renderer = Application::Get().GetRenderer();
    renderer.PreRender();
    renderer.BeginFrame();

    auto transforms = m_manager.m_registry.view<TransformComponent, SpriteRendererComponent>();

    std::vector<RenderCommand> renderCommands;

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
            if (AnimatorState* state = spriteAnimator->controller->GetCurrentState())
            {
                if (Sprite* targetSprite = state->GetSprite())
                    renderCommands.push_back({ targetSprite, transform, sprite.order });
            }
        }
        else if (sprite.sprite != nullptr)
        {
            Sprite& targetSprite = sprite.image->GetSprites()[sprite.spriteIndex];
            renderCommands.push_back({ &targetSprite, transform, sprite.order });
        }
    }

    auto tilemapView = m_manager.m_registry.view<TilemapComponent>();
    for (auto [entity, tilemap] : tilemapView.each())
    {
        if (tilemap.originalImage == nullptr)
            continue;

        if (Texture* texture = tilemap.originalImage->GetTexture())
        {
            for (auto& [pos, value] : tilemap.data)
            {
                int row = tilemap.rows - value.first - 1;

                int spriteIndex = row * tilemap.cols;
                spriteIndex += value.second;

                if (spriteIndex < tilemap.tilemapSprites.size())
                {
                    Sprite& targetSprite = tilemap.tilemapSprites[spriteIndex];
                    TransformComponent tileTransform;
                    tileTransform.position = { pos.first + 0.5f, pos.second + 0.5f, 0.0f };
                    tileTransform.scale = { 1.0f, 1.0f, 0.0f };
                    tileTransform.rotation = { 0.0f, 0.0f, 0.0f };

                    renderCommands.push_back({ &targetSprite, tileTransform, tilemap.order });
                }
            }
        }
    }

    std::sort(renderCommands.begin(), renderCommands.end(),
    [](const RenderCommand& a, const RenderCommand& b)
    {
        return a.order < b.order;
    });

    for (auto& command : renderCommands)
    {
        renderer.DrawSprite(*command.sprite, command.transform.position, command.transform.scale, command.transform.rotation.z);
    }

    for (auto& system : m_systems)
        system->Render();

    renderer.EndFrame();
    renderer.PostRender();

    if (renderOptions.collisionVisible)
        RenderCollisionComponents();

    renderer.PreRender();
    renderer.BeginFrame();

    glm::vec2 screenSize = renderer.GetScreenSize();
    glm::mat4 viewProj = glm::ortho(0.0f, screenSize.x, 0.0f, screenSize.y);
    renderer.SetViewProjectionMatrix(viewProj);

    UIManager& uiManager = m_app->GetUIManager();
    uiManager.SetScreenSize(renderer.GetScreenSize());

    for (auto& uiObject : uiManager.GetObjects())
    {
        uiObject->HandleInput(Application::Get().GetInput());

        if (uiObject->Type() == UIType_Text)
            renderer.DrawText(uiObject->text, uiObject->position, uiObject->fontSize, uiObject->fontColor, uiManager.GetFontName(), uiObject->parentRect, uiObject->flags);
        else if (uiObject->Type() == UIType_Button)
        {
            renderer.DrawQuadUI(uiObject->position, uiObject->scale, uiObject->color);
            renderer.DrawText(uiObject->text, uiObject->position, uiObject->fontSize, uiObject->fontColor, uiManager.GetFontName(), uiObject->parentRect, uiManager.GetFlag());
        }
        else if (uiObject->Type() == UIType_Image)
        {
            uiObject->Render(renderer);
        }
        else
            renderer.DrawQuadUI(uiObject->position, uiObject->scale, uiObject->color);
    }

    uiManager.Clear();

    renderer.EndFrame();
    renderer.PostRender();
}

void Scene::RenderEditor(RenderOptions renderOptions)
{
    Renderer& renderer = Application::Get().GetRenderer();
    renderer.PreRender();
    renderer.BeginFrame();

    auto transforms = m_manager.m_registry.view<TransformComponent, SpriteRendererComponent>();

    for (auto [entity, transform, sprite] : transforms.each())
    {
        glm::vec2 position = transform.position;
        glm::vec2 scale = transform.scale;
        float angle = transform.rotation.z;

        /*
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
        */

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

    auto tilemapView = m_manager.m_registry.view<TilemapComponent>();

    for (auto [entity, tilemap] : tilemapView.each())
    {
        if (tilemap.originalImage == nullptr)
            continue;

        if (Texture* texture = tilemap.originalImage->GetTexture())
        {
            for (auto& [pos, value] : tilemap.data)
            {
                int row = tilemap.rows - value.first - 1;

                int spriteIndex = row * tilemap.cols;
                spriteIndex += value.second;

                if (spriteIndex < tilemap.tilemapSprites.size())
                {
                    renderer.DrawSprite(tilemap.tilemapSprites[spriteIndex], { pos.first + 0.5f, pos.second + 0.5f }, { 1, 1 }, 0.0f);
                }
            }
        }
    }

    for (auto& system : m_systems)
        system->Render();

    renderer.EndFrame();
    renderer.PostRender();

    if (renderOptions.collisionVisible)
        RenderCollisionComponents();

    renderer.EndFrame();
    renderer.PostRender();
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

        if (Rigidbody2DComponent* rb2d = returnEntity.GetComponent<Rigidbody2DComponent>())
            m_physics.InitializePhysicsEntity(returnEntity, *transform, *rb2d);
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