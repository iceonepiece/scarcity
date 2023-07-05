#pragma once

#include "../Events/MouseEvent.h"

enum EditorMouseMode
{
	Move,
	Position,
	Scale,
	Rotation
};

class EditorMouse
{
public:
	EditorMouse(Scene *scene)
		: m_scene(scene)
	{

	}

	void OnEvent(Event *e)
	{
		if (e->GetType() == EventType::MouseMoved)
		{
			HandleMouseMoved(e);
		}
		else if (e->GetType() == EventType::MouseScrolled)
		{
			HandleMouseScrolled(e);
		}
	}

	void HandleMouseMoved(Event *e)
	{
		glm::vec2 pos = Input::GetCursorPosition();

		if (Input::IsMouseButtonHeld(Mouse::ButtonLeft))
		{
			float xDiff = m_previousPosition.x - pos.x;
			float yDiff = m_previousPosition.y - pos.y;

			glm::vec3 camPos = m_scene->m_camera->GetPosition();
			camPos.x += xDiff / 20;
			camPos.y -= yDiff / 20;

			m_scene->m_camera->SetPosition(camPos);
		}

		m_previousPosition = pos;
	}

	void HandleMouseScrolled(Event* e)
	{
		glm::vec3 camPos = m_scene->m_camera->GetPosition();

		MouseScrolledEvent* event = (MouseScrolledEvent*)e;

		camPos.z += event->GetY() * 1;

		m_scene->m_camera->SetPosition(camPos);
	}

	float DragCamera(glm::vec2 currentPosition)
	{
		return glm::distance(currentPosition, m_previousPosition);
	}

private:
	Scene *m_scene;
	glm::vec2 m_previousPosition;
};