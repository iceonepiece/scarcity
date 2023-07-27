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

        /*
        Entity rect = m_manager.CreateEntity();
        rect.AddComponent<TransformComponent>(glm::vec3 {0.0f, 0.0f, 0.0f}, glm::vec3 {0.0f}, glm::vec3 {200.0f, 200.0f, 1.0f});
        rect.AddComponent<SpriteRendererComponent>(Shape_Square);

        Entity rect2 = m_manager.CreateEntity();
        rect2.AddComponent<TransformComponent>(glm::vec3 {-300.0f, 0.0f, 0.0f}, glm::vec3 {0.0f}, glm::vec3 {50.0f, 100.0f, 1.0f});
        rect2.AddComponent<SpriteRendererComponent>(Shape_Square, glm::vec4 {0.5f});
	    */
    }

};