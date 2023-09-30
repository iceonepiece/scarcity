#pragma once

#include <glm/glm.hpp>
#include <functional>
#include "Graphics/Renderer.h"
#include "Components/TransformComponent.h"
#include "Core/Entity.h"

enum TransformDepending
{
	DependOnNone = 0,
	DependOnPosition = 1 << 0,
	DependOnRotation = 1 << 1
};

class Actionable
{
public:
	//using DraggingFunction = std::function<bool(TransformComponent& transform, float x, float y)>;
	using DraggingFunction = std::function<bool(Actionable& actor, float x, float y)>;

	Actionable(const glm::vec2& position, const glm::vec4 color)
		: m_position(position)
		, m_angle(0.0f)
		, m_color(color)
		, m_draggingCallback([](Actionable& actor, float x, float y) { return false; })
	{
	}

	inline void SetDraggingCallback(DraggingFunction callback)
	{
		m_draggingCallback = callback;
	}

	virtual bool OnMouseHover(const glm::vec2& cursor) = 0;
	virtual bool OnMouseButtonPressed(const glm::vec2& cursor) = 0;
	virtual bool OnDragging(float x, float y)
	{
		return m_draggingCallback(*this, x, y);
	}

	virtual void UpdateTransform(float zoom) {}

	virtual void Render(Renderer& renderer) = 0;
	virtual bool IsCursorOn(float x, float y, const glm::vec2& entityPosition) = 0;

	inline Entity GetEntity() { return m_entity; }
	inline void SetEntity(Entity entity) { m_entity = entity; }

	inline void SetTransformComponent(TransformComponent* transform) { m_transform = transform; }

	inline TransformComponent* GetTransformComponent() { return m_transform; }

	inline void SetStartCursorPosition(const glm::vec2& position) { m_startCursorPosition = position; }
	inline glm::vec2& GetStartCursorPosition() { return m_startCursorPosition; }

	inline void SetPosition(const glm::vec2& position) { m_position = position; }
	inline void SetColor(const glm::vec4& color) { m_color = color; }

	inline TransformComponent GetStartTransformComponent() { return m_startTransform; }
	
	inline void SetDependingFlags(int flags)
	{
		m_dependingFlags = flags;
	}

protected:
	int m_dependingFlags = DependOnNone;
	
	DraggingFunction m_draggingCallback;

	Entity m_entity;

	glm::vec2 m_startCursorPosition;
	TransformComponent m_startTransform;
	TransformComponent* m_transform;
	glm::vec2 m_position;
	glm::vec4 m_color;

	float m_angle;

	friend class Gizmo;
};