#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "Graphics/Renderer.h"
#include "Actionable.h"
#include "Events/MouseEvent.h"

class EditorViewport;

class Gizmo
{
public:
	Gizmo(EditorViewport& viewport);
	virtual void Update(float dt);
	void Render(Renderer& renderer, const glm::vec2& position);

	virtual bool OnPicking2D(const glm::vec2& cursorPosition);
	
	virtual bool OnDragging(float x, float y)
	{
		if (m_actor != nullptr)
		{
			return m_actor->OnDragging(x, y);
		}

		return false;
	}

	virtual void OnDraggingStart() {}
	virtual void OnDraggingEnd() {}

	virtual void OnMouseButtonPressed(MouseButtonPressedEvent& event) {}
	virtual void OnMouseButtonReleased(MouseButtonReleasedEvent& event) {}

protected:
	EditorViewport& m_viewport;
	glm::vec2 m_position;
	Actionable* m_actor = nullptr;
	std::vector<std::unique_ptr<Actionable>> m_actionables;
};
