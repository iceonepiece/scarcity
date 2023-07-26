#pragma once

#include "Actionable.h"
#include "Shapes/Shape2D.h"
#include "../Editor2D.h"

class ActionableRect : public Actionable
{
public:
	ActionableRect(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color)
		: Actionable(position, color)
		, m_scale(scale)
	{
		m_dependingFlags |= DependOnPosition;
		m_quad.position = position;
		m_quad.scale = scale;
		m_quad.color = color;
	}

	virtual bool OnMouseHover(const glm::vec2& cursor) override
	{
		return false;
	}

	virtual bool OnMouseButtonPressed(const glm::vec2& cursor) override
	{
		return false;
	}
	
	virtual void UpdateTransform() override
	{
		if (m_transform == nullptr)
			return;

		if (m_dependingFlags & DependOnPosition)
		{
			m_quad.position = m_transform->position;
			m_quad.position += m_position;
		}

		if (m_dependingFlags & DependOnRotation)
		{
			float angle = m_transform->rotation.z;

			m_quad.position -= m_position;
			glm::vec2 rotated = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 0, 1)) * glm::vec4(m_position.x, m_position.y, 0.0f, 1.0f);
			m_quad.position += rotated;
			m_quad.angle = angle;
		}
	}

	virtual void Render(Renderer& renderer) override
	{
		glm::vec2 renderPosition {0.0f};

		if (m_transform != nullptr)
		{
			renderPosition += glm::vec2{ m_transform->position.x, m_transform->position.y };

			glm::vec2 rotated = glm::rotate(glm::mat4(1.0f), m_angle, glm::vec3(0, 0, 1)) * glm::vec4(m_position.x, m_position.y, 0.0f, 1.0f);
			renderPosition += rotated;
		}

		renderer.DrawQuad2D(m_quad);
	}

	virtual bool IsCursorOn(float x, float y, const glm::vec2& entityPosition) override
	{
		return Shape2D::IsPointOnRectangle(glm::vec2 {x, y}, m_quad.position, m_quad.scale, m_quad.angle);
	}

protected:
	Quad2D m_quad;

	glm::vec2 m_scale;
};