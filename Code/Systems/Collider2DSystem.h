#pragma once

#include "../Core/System.h"
#include "../Graphics/RendererAPI.h"
#include "../Components/Collider2DComponent.h"

class Collider2DSystem : public System
{
public:
	Collider2DSystem(Scene* scene)
		: System(scene)
	{

	}

	virtual void Update(float deltaTime) override
	{

	}

	virtual void Render() override
	{
		Renderer& renderer = m_scene->m_game->GetRenderer();
		Camera& camera = *m_scene->m_camera;
		renderer.SetCamera(&camera);

		auto view = m_registry.view<Collider2DComponent>();
		for (auto [entity, collider] : view.each())
		{
			renderer.DrawRect(collider.body, camera);
		}
	}
};