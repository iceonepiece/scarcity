#pragma once

#include "Gizmo.h"

class ViewGizmo : public Gizmo
{
public:
	ViewGizmo(Editor2D& editor)
		: Gizmo(editor)
	{}

	virtual bool OnPicking2D(const glm::vec2& cursorPosition) override
	{
		m_startCursorPosition = cursorPosition;
		return true;
	}

	virtual bool OnDragging(float x, float y) override
	{
		Camera& camera = m_editor.GetCamera();

		glm::vec3 cameraPosition = camera.GetPosition();

		cameraPosition.x += m_startCursorPosition.x - x;
		cameraPosition.y += m_startCursorPosition.y - y;

		camera.SetPosition(cameraPosition);

		return true;
	}

private:
	glm::vec2 m_startCursorPosition;
};