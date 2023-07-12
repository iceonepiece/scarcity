#pragma once

#include <glm/glm.hpp>
#include <functional>
#include "Graphics/Renderer.h"
#include "Components/TransformComponent.h"

class Actionable
{
public:
	using DraggingFunction = std::function<bool(TransformComponent& transform, float x, float y)>;

	Actionable(const glm::vec2& position, const glm::vec4 color)
		: m_position(position)
		, m_color(color)
		, m_draggingCallback([](TransformComponent& transform, float x, float y) { return false; })
	{}

	inline void SetDraggingCallback(DraggingFunction callback)
	{
		m_draggingCallback = callback;
	}

	virtual bool OnMouseHover(const glm::vec2& cursor) = 0;
	virtual bool OnMouseButtonPressed(const glm::vec2& cursor) = 0;
	virtual bool OnDragging(float x, float y)
	{
		float diffX = x - m_startCursorPosition.x;
		float diffY = y - m_startCursorPosition.y;

		m_startCursorPosition.x = x;
		m_startCursorPosition.y = y;

		return m_draggingCallback(*m_transform, diffX, diffY);
	}
	
	virtual void Render(Renderer& renderer) = 0;
	virtual bool IsCursorOn(float x, float y, const glm::vec2& entityPosition) = 0;

	inline void SetTransformComponent(TransformComponent* transform) { m_transform = transform; }
	inline void SetPosition(const glm::vec2& position) { m_position = position; }
	inline void SetColor(const glm::vec4& color) { m_color = color; }
	inline void SetStartCursorPosition(const glm::vec2& position) { m_startCursorPosition = position; }

protected:
	DraggingFunction m_draggingCallback;

	glm::vec2 m_startCursorPosition;
	TransformComponent* m_transform;
	glm::vec2 m_position;
	glm::vec4 m_color;
};