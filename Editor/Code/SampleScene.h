#pragma once

#include "Scene/Scene.h"
#include "Systems/Rigidbody2DSystem.h"

class SampleScene : public Scene
{
public:
	virtual void Initialize() override
	{
        m_camera = std::make_unique<Camera2D>(
            glm::vec3 { 0.0f, 0.0f, -1.0f },
            glm::vec2 { 1.0f, 1.0f },
            glm::vec2 { 1280, 720 }
        );

        Renderer& renderer = m_app->GetRenderer();
        renderer.SetCamera(m_camera.get());
 
        Entity camera = m_manager.CreateEntity();
        camera.AddComponent<BaseComponent>("Main Camera");
        camera.AddComponent<TransformComponent>(glm::vec3 { 0.0f, 0.0f, -14.0f }, glm::vec3 {0.0f}, glm::vec3 {1.0f});
        camera.AddComponent<CameraComponent>();

        Entity rect = m_manager.CreateEntity();
        rect.AddComponent<BaseComponent>("Rect");
        rect.AddComponent<TransformComponent>(glm::vec3 {0.0f, 0.0f, 0.0f}, glm::vec3 {0.0f}, glm::vec3 {5.0f, 10.0f, 1.0f});
        rect.AddComponent<SpriteRendererComponent>(Shape_Square);
    }

};