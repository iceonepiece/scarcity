#include "SceneManager.h"
#include "Scene/SceneSerializer.h"
#include "Components/Components.h"
#include "Core/Camera2D.h"

Scene* SceneManager::Copy(Scene& sourceScene)
{
    Scene* newScene = new Scene();

    entt::registry& srcRegistry = sourceScene.m_manager.m_registry;
    entt::registry& destRegistry = newScene->m_manager.m_registry;

    srcRegistry.each([&](auto srcEntity) {
        auto destEntity = destRegistry.create();

        std::apply([&](auto... componentTypes) {
            (CopyComponent<decltype(componentTypes)>(srcRegistry, destRegistry, srcEntity, destEntity), ...);
            }, ComponentList{});
        });

    return newScene;
}

std::unique_ptr<Scene> SceneManager::LoadScene(const std::filesystem::path& filePath)
{
    std::unique_ptr<Scene> scene = std::make_unique<Scene>();

	if (SceneSerializer::Deserialize(*scene, filePath))
		return scene;

	return nullptr;
}

std::unique_ptr<Scene> SceneManager::CreateDefaultScene(const std::filesystem::path& directory)
{
    std::unique_ptr<Scene> defaultScene = std::make_unique<Scene>();
    //defaultScene->m_path = directory / (defaultScene->m_name + ".scene.json");
    defaultScene->m_location = directory;

    defaultScene->SetInitializeFunction([](Scene& scene)
    {
        scene.m_camera = std::make_unique<Camera2D>(
            glm::vec3{ 0.0f, 0.0f, -1.0f },
            glm::vec2{ 1.0f, 1.0f },
            glm::vec2{ 1280, 720 }
        );

        //Renderer& renderer = scene.m_app->GetRenderer();
        //renderer.SetCamera(scene.m_camera.get());

        Entity camera = scene.m_manager.CreateEntity();
        camera.AddComponent<BaseComponent>("Main Camera");
        camera.AddComponent<TransformComponent>(glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec3{ 0.0f }, glm::vec3{ 1.0f });
        camera.AddComponent<CameraComponent>();

        Entity rect = scene.m_manager.CreateEntity();
        rect.AddComponent<BaseComponent>("Rect");
        rect.AddComponent<TransformComponent>(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f });
        rect.AddComponent<SpriteRendererComponent>(Shape_Square);
    });

    return defaultScene;
}