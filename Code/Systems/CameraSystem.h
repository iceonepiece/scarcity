#pragma once

#include "Components/Components.h"
#include "Core/System.h"

class CameraSystem : public System
{
public:
	CameraSystem(Scene* scene)
		: System(scene)
	{
	}

	virtual void Update(float deltaTime) override
	{
		/*
		Renderer& renderer = m_scene->m_app->GetRenderer();

		auto view = m_registry.view<TransformComponent, CameraComponent>();
		for (auto [entity, transform, camera] : view.each())
		{
			renderer.Set
		}
		*/
	}
};