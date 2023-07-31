#pragma once

#include "Gizmo.h"
#include "Actionable.h"
#include "ActionableRect.h"
#include "ActionableCircle.h"

class RotateGizmo : public Gizmo
{
public:
	RotateGizmo(Editor2D& editor)
		: Gizmo(editor)
	{
		m_actionables.push_back(std::make_unique<ActionableRect>(
			glm::vec2 { 0.0f },
			glm::vec2 { m_thickness, m_length },
			glm::vec4 { 0.2, 0.8, 0.2, 1.0 }
		));

		m_actionables.push_back(std::make_unique<ActionableRect>(
			glm::vec2 { 0.0f },
			glm::vec2 { m_length, m_thickness },
			glm::vec4 { 0.8, 0.2, 0.2, 1.0 }
		));

		m_actionables.push_back(std::make_unique<ActionableCircle>(
			glm::vec2 { 0.0f },
			m_length / 2,
			glm::vec4 { 1.0f }
		));

		m_actionables[2]->SetDraggingCallback([](Actionable& actor, float x, float y)
		{
			glm::vec2& previousCursor = actor.GetStartCursorPosition();
			TransformComponent* transform = actor.GetTransformComponent();
			glm::vec2 centerPosition = transform->position;

			glm::vec2 startVector = previousCursor - centerPosition;
			glm::vec2 currentVector { x - centerPosition.x, y - centerPosition.y };

			float theta = glm::acos(
				glm::dot(
					glm::normalize(startVector),
					glm::normalize(currentVector)
				)
			);

			glm::vec3 crossProduct = glm::cross(
				glm::vec3 { startVector.x, startVector.y, 0.0f },
				glm::vec3 { currentVector.x, currentVector.y, 0.0f }
			);

			int direction = crossProduct.z > 0 ? 1 : -1;

			TransformComponent startTransform = actor.GetStartTransformComponent();
			transform->rotation.z = startTransform.rotation.z + theta * direction;

			return true;
		});
	}

private:
	float m_length = 2.8f;
	float m_thickness = 0.07f;
};