#pragma once

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
	void Update(Camera& camera)
	{
		glm::vec2 pos = Input::GetCursorPosition();
		
		if (Input::IsMouseButtonHeld(Mouse::ButtonLeft))
		{
			float xDiff = m_previousPosition.x - pos.x;
			float yDiff = m_previousPosition.y - pos.y;

			glm::vec3 camPos = camera.GetPosition();
			camPos.x += xDiff / 20;
			camPos.y -= yDiff / 20;

			camera.SetPosition(camPos);
		}

		m_previousPosition = pos;
	}

	float DragCamera(glm::vec2 currentPosition)
	{
		return glm::distance(currentPosition, m_previousPosition);
	}

private:
	glm::vec2 m_previousPosition;


};