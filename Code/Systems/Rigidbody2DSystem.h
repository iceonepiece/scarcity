#pragma once

#include "Core/System.h"
#include "Graphics/RendererAPI.h"
#include "Components/Components.h"
#include <box2d/box2d.h>

class Rigidbody2DSystem : public System
{
public:
	Rigidbody2DSystem(Scene* scene)
		: System(scene)
	{
	}

	virtual void Update(float deltaTime) override
	{

	}

	virtual void Render() override
	{
		Renderer& renderer = m_scene->m_app->GetRenderer();
		Camera& camera = *m_scene->m_camera;

		auto view = m_registry.view<Rigidbody2DComponent>();
		for (auto [entity, rb2d] : view.each())
		{
			b2Body* body = (b2Body*)rb2d.body;
			renderer.DrawRect((b2Body*)rb2d.body, camera);
		}
	}
};