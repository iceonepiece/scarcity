#pragma once

#include "Actionable.h"

class ActionableCircle : public Actionable
{
public:
	ActionableCircle(const glm::vec2& position, float radius, const glm::vec4& color)
		: Actionable(position, color)
		, m_radius(radius)
	{}

	virtual bool OnMouseHover(const glm::vec2& cursor) override
	{
		return false;
	}

	virtual bool OnMouseButtonPressed(const glm::vec2& cursor) override
	{
		return false;
	}

	virtual void Render(Renderer& renderer) override
	{
		glm::vec2 offset {0.0f};

		if (m_transform != nullptr)
			offset = m_transform->position;

		renderer.DrawCircle(m_position + offset, m_radius, false);
	}

	virtual bool IsCursorOn(float x, float y, const glm::vec2& entityPosition) override
	{
		return false;
	}


protected:
	float m_radius;
};