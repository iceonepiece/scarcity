#pragma once

#include "Gizmo.h"
#include "ActionableRect.h"

class ScaleGizmo : public Gizmo
{
public:
	ScaleGizmo(Editor2D& editor)
		: Gizmo(editor)
	{
		m_actionables.push_back(std::make_unique<ActionableRect>(
			glm::vec2 { m_length / 2, 0.0f },
			glm::vec2 { m_length, 5.0f },
			glm::vec4 { 0.8, 0.2, 0.2, 1.0 }
		));

		m_actionables.push_back(std::make_unique<ActionableRect>(
			glm::vec2 { 0.0f, m_length / 2 },
			glm::vec2 { 5.0f, m_length },
			glm::vec4 { 0.2, 0.8, 0.2, 1.0 }
		));

		m_actionables.push_back(std::make_unique<ActionableRect>(
			glm::vec2 { 0.0f },
			glm::vec2 { 20.0f, 20.0f },
			glm::vec4 { 1.0f }
		));

	}

protected:
	float m_length = 150.0f;
};