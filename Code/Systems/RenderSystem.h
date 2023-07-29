#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include "Components/Components.h"
#include "Core/System.h"

class RenderSystem : public System
{
public:
	RenderSystem(Scene* scene)
		: System(scene)
	{
	}

	glm::mat4 GetViewMatrix(const CameraComponent& camera)
	{
		return glm::mat4(1.0f);
	}

	glm::mat4 GetProjectionMatrix(const CameraComponent& camera)
	{
		/*
		if (camera.projection == ProjectionType::Perspective)
			return glm::perspective(glm::radians(45.0f), m_screenSize.x / m_screenSize.y, 0.1f, 100.0f);

		if (camera.projection == CameraType::Orthographic)
			return glm::ortho(-(m_screenSize.x / m_zoom / 2), (m_screenSize.x / m_zoom / 2), -(m_screenSize.y / m_zoom / 2), (m_screenSize.y / m_zoom / 2));
		*/

		return glm::mat4(1.0f);
	}

	virtual void Update(float deltaTime) override
	{
		Renderer& renderer = m_scene->m_app->GetRenderer();

		auto view = m_registry.view<TransformComponent, CameraComponent>();
		for (auto [entity, transform, camera] : view.each())
		{
			renderer.m_viewMatrix = GetViewMatrix(camera);
			renderer.m_projectionMatrix = GetProjectionMatrix(camera);
		}
	}

	virtual void Render() override
	{

	}
};