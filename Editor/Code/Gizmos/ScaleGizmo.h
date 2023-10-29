#pragma once

#include "Gizmo.h"
#include "ActionableRect.h"

class ScaleGizmo : public Gizmo
{
public:
	ScaleGizmo(EditorViewport& viewport)
		: Gizmo(viewport)
	{
		m_actionables.push_back(std::make_unique<ActionableRect>(
			glm::vec2 { m_length / 2, 0.0f },
			glm::vec2 { m_length, m_thickness },
			glm::vec4 { 0.8, 0.2, 0.2, 1.0 }
		));

		m_actionables.push_back(std::make_unique<ActionableRect>(
			glm::vec2 { 0.0f, m_length / 2 },
			glm::vec2 { m_thickness, m_length },
			glm::vec4 { 0.2, 0.8, 0.2, 1.0 }
		));

		m_actionables.push_back(std::make_unique<ActionableRect>(
			glm::vec2 { 0.0f },
			glm::vec2 { 0.25f, 0.25f },
			glm::vec4 { 1.0f }
		));

		m_actionables[2]->SetDraggingCallback([](Actionable& actor, float x, float y)
			{
				glm::vec2& startCursor = actor.GetStartCursorPosition();
				float diffX = x - startCursor.x;
				float diffY = y - startCursor.y;
				actor.SetStartCursorPosition(glm::vec2 {x, y});

				TransformComponent* transform = actor.GetTransformComponent();
				transform->scale.x += diffX;
				transform->scale.y += diffY;

				return true;
			});

		m_actionables[0]->SetDraggingCallback([](Actionable& actor, float x, float y)
			{
				glm::vec2& startCursor = actor.GetStartCursorPosition();
				float diffX = x - startCursor.x;
				actor.SetStartCursorPosition(glm::vec2 {x, y});

				TransformComponent* transform = actor.GetTransformComponent();
				transform->scale.x += diffX;

				return true;
			});

		m_actionables[1]->SetDraggingCallback([](Actionable& actor, float x, float y)
			{
				glm::vec2& startCursor = actor.GetStartCursorPosition();
				float diffY = y - startCursor.y;
				actor.SetStartCursorPosition(glm::vec2 {x, y});

				TransformComponent* transform = actor.GetTransformComponent();
				transform->scale.y += diffY;

				return true;
			});

		m_actionables[0]->SetDependingFlags(DependOnPosition | DependOnRotation);
		m_actionables[1]->SetDependingFlags(DependOnPosition | DependOnRotation);
		m_actionables[2]->SetDependingFlags(DependOnPosition | DependOnRotation);
	}

protected:
	float m_length = 1.4f;
	float m_thickness = 0.07f;
};