#pragma once

#include "Scene/Scene.h"

class SampleScene : public Scene
{
public:
	virtual void Initialize() override
	{
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