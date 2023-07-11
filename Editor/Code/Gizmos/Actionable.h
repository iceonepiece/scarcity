#pragma once

#include <glm/glm.hpp>
#include "Graphics/Renderer.h"
#include "Components/TransformComponent.h"

class Actionable
{
public:
	Actionable(const glm::vec2& position, const glm::vec4 color)
		: m_position(position)
		, m_color(color)
	{}

	virtual bool OnMouseHover(const glm::vec2& cursor) = 0;
	virtual bool OnMouseButtonPressed(const glm::vec2& cursor) = 0;
	virtual void Render(Renderer& renderer) = 0;
	virtual bool IsCursorOn(float x, float y, const glm::vec2& entityPosition) = 0;

	inline void SetTransformComponent(TransformComponent* transform) { m_transform = transform; }
	inline void SetPosition(const glm::vec2& position) { m_position = position; }
	inline void SetColor(const glm::vec4& color) { m_color = color; }
	inline void SetStartCursorPosition(const glm::vec2& position) { m_startCursorPosition = position; }

protected:
	glm::vec2 m_startCursorPosition;
	TransformComponent* m_transform;
	glm::vec2 m_position;
	glm::vec4 m_color;
};