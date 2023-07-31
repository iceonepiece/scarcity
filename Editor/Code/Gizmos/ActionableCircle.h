#pragma once

#include "Actionable.h"
#include "Shapes/Shape2D.h"

class ActionableCircle : public Actionable
{
public:
	ActionableCircle(const glm::vec2& position, float radius, const glm::vec4& color)
		: Actionable(position, color)
		, m_radius(radius)
	{
		m_dependingFlags |= DependOnPosition;
		m_circle.position = position;
		m_circle.radius = radius;
		m_circle.color = color;
	}

	virtual bool OnMouseHover(const glm::vec2& cursor) override
	{
		return false;
	}

	virtual bool OnMouseButtonPressed(const glm::vec2& cursor) override
	{
		return false;
	}

	virtual void UpdateTransform(float zoom) override
	{
		if (m_transform == nullptr)
			return;

		m_circle.radius = m_radius / zoom;

		if (m_dependingFlags & DependOnPosition)
		{
			m_circle.position = m_transform->position;
			m_circle.position += m_position / zoom;
		}
	}

	virtual void Render(Renderer& renderer) override
	{
		/*
		glm::vec2 offset {0.0f};

		if (m_transform != nullptr)
			offset = m_transform->position;

		renderer.DrawCircle(m_position + offset, m_radius);
		*/
		renderer.DrawCircle2D(m_circle, 0.06f);
	}

	virtual bool IsCursorOn(float x, float y, const glm::vec2& entityPosition) override
	{
		/*
		glm::vec2 offset {0.0f};

		if (m_transform != nullptr)
			offset = m_transform->position;

		glm::vec2 realPosition = m_position + offset;

		float distance = glm::distance(glm::vec2 {x, y}, realPosition);
		return distance >= m_radius * 0.92 && distance <= m_radius;
		*/

		float distance = glm::distance(glm::vec2 {x, y}, m_circle.position);
		return distance >= m_circle.radius * 0.93 && distance <= m_circle.radius;
	}


protected:
	Circle2D m_circle;

	float m_radius;
};