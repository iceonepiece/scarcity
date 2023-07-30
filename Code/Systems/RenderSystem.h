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

	glm::mat4 GetViewMatrix(const CameraComponent& camera, const TransformComponent& transform)
	{
		if (camera.projection == ProjectionType::Orthographic)
		{
			glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
			return glm::lookAt(transform.position, transform.position + front, glm::vec3(0.0f, 1.0f, 0.0f));
		}

		return glm::translate(glm::mat4(1.0f), transform.position);
	}

	glm::mat4 GetProjectionMatrix(const CameraComponent& camera)
	{
		glm::vec2 screenSize = { 1280.0f, 720.0f };

		if (camera.projection == ProjectionType::Perspective)
			return glm::perspective(glm::radians(45.0f), screenSize.x / screenSize.y, camera.near, camera.far);

		if (camera.projection == ProjectionType::Orthographic)
			return glm::ortho(-(screenSize.x / 2), (screenSize.x / 2), -(screenSize.y / 2), (screenSize.y / 2));

		return glm::mat4(1.0f);
	}

	virtual void Update(float deltaTime) override
	{
		Renderer& renderer = m_scene->m_app->GetRenderer();

		auto view = m_registry.view<TransformComponent, CameraComponent>();
		for (auto [entity, transform, camera] : view.each())
		{
			renderer.m_viewMatrix = GetViewMatrix(camera, transform);
			renderer.m_projectionMatrix = GetProjectionMatrix(camera);
		}
	}

	virtual void Render() override
	{

	}
};