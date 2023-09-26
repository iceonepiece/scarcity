#pragma once

#include "Gizmo.h"
#include "ActionableRect.h"
#include "../Commands/TranslateCommand.h"

class TranslateGizmo : public Gizmo
{
public:
	TranslateGizmo(EditorLayer& editor)
		: Gizmo(editor)
	{
		m_actionables.push_back(std::make_unique<ActionableRect>(
			glm::vec2 { 0.13f },
			glm::vec2 { 0.3f },
			glm::vec4 { 0.2, 0.2, 0.7, 0.5 }
		));

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

		m_actionables[0]->SetDraggingCallback([](Actionable& actor, float x, float y)
		{
			glm::vec2& previousCursor = actor.GetStartCursorPosition();
			float diffX = x - previousCursor.x;
			float diffY = y - previousCursor.y;
			actor.SetStartCursorPosition(glm::vec2 {x, y});

			TransformComponent* transform = actor.GetTransformComponent();
			transform->position.x += diffX;
			transform->position.y += diffY;
			return true;
		});

		m_actionables[1]->SetDraggingCallback([&](Actionable& actor, float x, float y)
		{
			glm::vec2& previousCursor = actor.GetStartCursorPosition();
			float diffX = x - previousCursor.x;
			actor.SetStartCursorPosition(glm::vec2 {x, y});

			TranslateCommand* translateCommand = new TranslateCommand(
				*actor.GetTransformComponent(),
				glm::vec3{ diffX, 0.0f, 0.0f }
			);

			m_editor.AddCommand(translateCommand);

			//TransformComponent* transform = actor.GetTransformComponent();
			//transform->position.x += diffX;
			return true;
		});

		m_actionables[2]->SetDraggingCallback([](Actionable& actor, float x, float y)
		{
			glm::vec2& previousCursor = actor.GetStartCursorPosition();
			float diffY = y - previousCursor.y;
			actor.SetStartCursorPosition(glm::vec2 {x, y});

			TransformComponent* transform = actor.GetTransformComponent();
			transform->position.y += diffY;
			return true;
		});
	}

protected:
	float m_length = 1.4f;
	float m_thickness = 0.07f;


};