#pragma once

#include "Actionable.h"

class ActionableRect : public Actionable
{
public:
	ActionableRect(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color)
		: Actionable(position, color)
		, m_scale(scale)
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

		renderer.DrawQuad2D(m_position + offset, m_scale, 0.0f, m_color);
	}

	virtual bool IsCursorOn(float x, float y, const glm::vec2& entityPosition) override
	{
		glm::vec2 offset {0.0f};

		if (m_transform != nullptr)
			offset = m_transform->position;

		glm::vec2 realPosition = m_position + offset;

		if (x < realPosition.x - m_scale.x / 2 ||
			x > realPosition.x + m_scale.x / 2 ||
			y < realPosition.y - m_scale.y / 2 ||
			y > realPosition.y + m_scale.y / 2)
			return false;

		return true;
	}

protected:
	glm::vec2 m_scale;
};